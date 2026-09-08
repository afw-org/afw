// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_array interface
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */


/**
 * @file afw_array_memory.c
 * @brief Implementation of afw_array interface for in-memory objects
 */

#include "afw_internal.h"



#define impl_afw_array_get_entry_meta afw_array_impl_get_entry_meta
#define impl_afw_array_get_next_entry_meta afw_array_impl_get_next_entry_meta
#define impl_afw_array_initialize_iterator afw_array_impl_initialize_iterator

static void
impl_managed_array_elements_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx);

/* Declares and rti/inf defines for interface afw_array */
#define AFW_IMPLEMENTATION_ID "memory"
typedef struct afw_memory_internal_array_s afw_memory_internal_array_t;
#define AFW_ARRAY_SELF_T afw_memory_internal_array_t
#include "afw_array_impl_declares.h"
#include "afw_array_setter_impl_declares.h"

static void
impl_afw_array_managed_release(
    AFW_ARRAY_SELF_T *self, afw_xctx_t *xctx);
static void
impl_afw_array_managed_get_reference(
    AFW_ARRAY_SELF_T *self, afw_xctx_t *xctx);
static void
impl_afw_array_managed_setter_push_value(
    const afw_array_setter_t *self,
    const afw_value_t *value,
    afw_xctx_t *xctx);
static void
impl_afw_array_managed_setter_set_value(
    const afw_array_setter_t *self,
    afw_integer_t index,
    const afw_value_t *value,
    afw_xctx_t *xctx);
static void
impl_afw_array_managed_setter_insert_value(
    const afw_array_setter_t *self,
    afw_integer_t index,
    const afw_value_t *value,
    afw_xctx_t *xctx);
static void
impl_afw_array_managed_setter_remove_all_values(
    const afw_array_setter_t *self,
    afw_xctx_t *xctx);

#undef AFW_IMPLEMENTATION_ID
#define AFW_IMPLEMENTATION_ID "memory_managed"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_array_managed_inf
#define AFW_ARRAY_INF_ONLY
#define impl_afw_array_release impl_afw_array_managed_release
#define impl_afw_array_get_reference impl_afw_array_managed_get_reference
#include "afw_array_impl_declares.h"
#undef AFW_ARRAY_INF_ONLY
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_array_release
#undef impl_afw_array_get_reference
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_array_managed_setter_inf
#define AFW_ARRAY_SETTER_INF_ONLY
#define impl_afw_array_setter_push_value \
    impl_afw_array_managed_setter_push_value
#define impl_afw_array_setter_set_value \
    impl_afw_array_managed_setter_set_value
#define impl_afw_array_setter_insert_value \
    impl_afw_array_managed_setter_insert_value
#define impl_afw_array_setter_remove_all_values \
    impl_afw_array_managed_setter_remove_all_values
#include "afw_array_setter_impl_declares.h"
#undef AFW_ARRAY_SETTER_INF_ONLY
#undef impl_afw_array_setter_push_value
#undef impl_afw_array_setter_set_value
#undef impl_afw_array_setter_insert_value
#undef impl_afw_array_setter_remove_all_values
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_ID

typedef struct afw_memory_internal_array_s
afw_memory_internal_array_t;

AFW_VECTOR_STRUCT(afw_memory_internal_array_values_s,
    const afw_value_t *);
typedef struct afw_memory_internal_array_values_s
    afw_memory_internal_array_values_t;

struct afw_memory_internal_array_s {
    afw_array_t pub;
    afw_value_array_t value;
    const afw_data_type_t *data_type;
    afw_array_setter_t setter;
    /*
     * Dense store of value pointers. Do not hold interior
     * entries pointers across grow.
     */
    afw_memory_internal_array_values_t *values;
    /*
     * Optional base for create_wrapper_* faces (NULL for a normal memory
     * array). After materialize, entry values live on the local vector; sets
     * never write to wrapped. See afw_array_create_wrapper_with_options().
     */
    const afw_array_t *wrapped;
    /*
     * Unmanaged memory arrays: extra holds from add_reference. Zero is
     * idle. Last extra hold walks remaining slot_store'd elements.
     */
    afw_integer_t reference_count;
    afw_boolean_t immutable;
    afw_boolean_t generic;
    afw_boolean_t unmanaged;
};


static const afw_value_t **
impl_slot_at(
    afw_memory_internal_array_t *self,
    afw_size_t at);

static const afw_value_t **
impl_new_slot(
    afw_memory_internal_array_t *self,
    afw_size_t at,
    afw_xctx_t *xctx);



AFW_DEFINE(const afw_array_t *)
afw_array_create_with_options(
    int options,
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{

    afw_memory_internal_array_t *self;

    /* If new_p, own pool is a child of p->managed_p. */
    if (AFW_ARRAY_MEMORY_OPTION_IS(options, new_p)) {
        p = afw_pool_create(p->managed_p, xctx);
    }

    /* Allocate memory for self. */
    self = afw_pool_calloc_type(p, afw_memory_internal_array_t, xctx);

    /* Initialize self. */
    self->pub.inf = &impl_afw_array_inf;
    self->pub.p = p;
    self->unmanaged =
        !AFW_ARRAY_MEMORY_OPTION_IS(options, new_p) &&
        !AFW_ARRAY_MEMORY_OPTION_IS(options, cede_p);
    /*
     * Pool-world dual face is always unmanaged (value get_reference
     * throws).
     */
    self->value.inf = &afw_value_unmanaged_array_inf;
    self->value.internal = (const afw_array_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    self->data_type = data_type;
    self->generic = data_type == NULL;
    self->values = afw_vector_create(
        afw_memory_internal_array_values_t, 0, p, xctx);
    self->setter.inf = &impl_afw_array_setter_inf;
    self->setter.array = (const afw_array_t *)self;

    if (!self->unmanaged) {
        afw_pool_register_cleanup_before(p, self, NULL,
            impl_managed_array_elements_cleanup, xctx);
    }

    return (const afw_array_t *)self;

}


AFW_DEFINE(const afw_array_t *)
afw_array_create_managed(
    const afw_data_type_t *data_type,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self;
    const afw_pool_t *p;

    p = xctx->p;
    self = afw_pool_calloc_type(p, afw_memory_internal_array_t, xctx);
    self->pub.inf = &impl_afw_array_managed_inf;
    self->pub.p = p;
    self->value.inf = &afw_value_managed_array_inf;
    self->value.internal = (const afw_array_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    self->data_type = data_type;
    self->generic = data_type == NULL;
    self->values = afw_vector_create(
        afw_memory_internal_array_values_t, 0, p, xctx);
    self->setter.inf = &impl_afw_array_managed_setter_inf;
    self->setter.array = (const afw_array_t *)self;
    self->reference_count = 1;
    return (const afw_array_t *)self;
}


static void
impl_push_cloned_into_managed(
    const afw_array_t *to,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_object_t *obj;
    const afw_array_t *arr;

    if (!value) {
        return;
    }
    if (afw_value_is_object(value)) {
        obj = ((const afw_value_object_t *)value)->internal;
        if (!obj) {
            return;
        }
        obj = afw_object_create_managed_clone(obj, xctx);
        afw_array_push_value(to, obj->value, xctx);
        afw_object_release(obj, xctx);
        return;
    }
    if (afw_value_is_array(value)) {
        arr = ((const afw_value_array_t *)value)->internal;
        if (!arr) {
            return;
        }
        arr = afw_array_create_managed_clone(arr, xctx);
        afw_array_push_value(to, arr->value, xctx);
        afw_array_release(arr, xctx);
        return;
    }
    afw_array_push_value(to, value, xctx);
}


AFW_DEFINE(const afw_array_t *)
afw_array_create_managed_clone(
    const afw_array_t *from,
    afw_xctx_t *xctx)
{
    const afw_array_t *to;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;
    const afw_data_type_t *data_type;

    if (!from) {
        AFW_THROW_ERROR_Z(general,
            "afw_array_create_managed_clone requires from",
            xctx);
    }
    if (from->inf == &impl_afw_array_managed_inf) {
        afw_array_get_reference(from, xctx);
        return from;
    }
    data_type = afw_array_get_data_type(from, xctx);
    to = afw_array_create_managed(data_type, xctx);
    if (from->inf == &impl_afw_array_inf &&
        !afw_array_is_memory_wrapper(from))
    {
        const afw_memory_internal_array_t *from_mem;
        const afw_value_t **entries;
        afw_size_t i;
        afw_size_t count;

        from_mem = (const afw_memory_internal_array_t *)from;
        count = from_mem->values->count;
        entries = from_mem->values->entries;
        for (i = 0; i < count; i++) {
            impl_push_cloned_into_managed(to, entries[i], xctx);
        }
        /* Compile `[]` is immutable memory; clone stays mutable. */
        return to;
    }
    iterator = NULL;
    for (;;) {
        value = afw_array_get_next_value(from, &iterator, xctx);
        if (!value) {
            break;
        }
        impl_push_cloned_into_managed(to, value, xctx);
    }
    if (!afw_array_is_memory(from) &&
        !afw_array_get_setter(from, xctx))
    {
        afw_array_set_immutable(to, xctx);
    }
    return to;
}


AFW_DEFINE(afw_boolean_t)
afw_array_is_memory_managed(const afw_array_t *array)
{
    return array && array->inf == &impl_afw_array_managed_inf;
}


/* Create memory array face that wraps another array (issue #17). */
AFW_DEFINE(const afw_array_t *)
afw_array_create_wrapper_with_options(
    int options,
    const afw_array_t *wrapped,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;
    const afw_data_type_t *data_type;

    if (!wrapped) {
        AFW_THROW_ERROR_Z(general,
            "afw_array_create_wrapper_with_options requires a wrapped array",
            xctx);
    }

    data_type = afw_array_get_data_type(wrapped, xctx);
    self = (afw_memory_internal_array_t *)
        afw_array_create_with_options(options, data_type, p, xctx);
    self->wrapped = wrapped;
    /* Face holds the array the same way for in_pool / and_pool / permanent. */
    afw_array_get_reference(wrapped, xctx);
    if (self->unmanaged) {
        afw_pool_register_cleanup_before(self->pub.p, self, NULL,
            impl_managed_array_elements_cleanup, xctx);
    }

    /*
     * Materialize entries onto the face so mutators only touch local
     * storage (base is not written). push_value slot_stores
     * (get_assignable_value) so nested unmanaged arrays get a face; an
     * already-assignable child is bumped, not peeled.
     */
    for (iterator = NULL;;) {
        value = afw_array_get_next_value(wrapped, &iterator, xctx);
        if (!value) {
            break;
        }
        afw_array_push_value((const afw_array_t *)self, value, xctx);
    }

    self->value.inf = &afw_value_assignable_array_inf;
    return (const afw_array_t *)self;
}



/* True if array is a generic memory array (face or not). */
AFW_DEFINE(afw_boolean_t)
afw_array_is_memory(const afw_array_t *array)
{
    return array && array->inf == &impl_afw_array_inf;
}


/* True if array is a memory look-through / materialize wrapper face. */
AFW_DEFINE(afw_boolean_t)
afw_array_is_memory_wrapper(const afw_array_t *array)
{
    const afw_memory_internal_array_t *self;

    if (!afw_array_is_memory(array)) {
        return false;
    }
    self = (const afw_memory_internal_array_t *)array;
    return self->wrapped != NULL;
}


AFW_DEFINE(const afw_array_t *)
afw_array_create_script_wrapper(
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_array_t *base;

    base = afw_array_create_unmanaged(p, xctx);
    return afw_array_create_wrapper_unmanaged(base, p, xctx);
}



/* Base under a face, or array if not a face. */
AFW_DEFINE(const afw_array_t *)
afw_array_memory_wrapper_base(const afw_array_t *array)
{
    const afw_memory_internal_array_t *self;

    if (!array) {
        return NULL;
    }
    if (array->inf != &impl_afw_array_inf) {
        return array;
    }
    self = (const afw_memory_internal_array_t *)array;
    return self->wrapped ? self->wrapped : array;
}



static void impl_store_element(
    AFW_ARRAY_SELF_T *self,
    const afw_value_t **slot,
    const afw_value_t *incoming,
    afw_xctx_t *xctx);

/* Face GET: store get_assignable_value of the child. Same pointer is a no-op. */
static const afw_value_t *
impl_promote_structured_entry(
    AFW_ARRAY_SELF_T *self,
    const afw_value_t **slot,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    if (!value || !slot || !self->wrapped || self->immutable) {
        return value;
    }
    impl_store_element(self, slot, value, xctx);
    return *slot;
}



/* Release remaining elements this array slot_store'd (not pop/shift). */
static void
impl_release_remaining_elements(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_value_t **entries;
    afw_size_t i;
    afw_size_t count;

    /* Generic memory arrays store raw pointers (compile literals, YAML). */
    if (!self->values || !self->wrapped) {
        return;
    }
    count = self->values->count;
    entries = self->values->entries;
    for (i = 0; i < count; i++) {
        if (entries[i]) {
            afw_value_release(entries[i], xctx);
            entries[i] = NULL;
        }
    }
}


static void
impl_managed_array_elements_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)data2;
    (void)p;
    impl_release_remaining_elements((AFW_ARRAY_SELF_T *)data, xctx);
}


/*
 * Implementation of method release of interface afw_array.
 */
void
impl_afw_array_release(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_array_t *wrapped;

    if (self->unmanaged) {
        if (self->reference_count <= 0) {
            return;
        }
        if (self->reference_count == 1) {
            self->reference_count = 0;
            /*
             * Overlay walk is a pool cleanup (same as managed faces).
             * Do not walk here: C-style for clones the current scope and
             * last-release of the previous clone can drop this instance
             * to zero while the array is still in use.
             */
            if (self->wrapped) {
                afw_array_release(self->wrapped, xctx);
            }
            afw_pool_release(self->pub.p, xctx);
            return;
        }
        self->reference_count--;
        afw_pool_release(self->pub.p, xctx);
        return;
    }

    wrapped = self->wrapped;
    if (afw_pool_release(self->pub.p, xctx) == NULL && wrapped) {
        afw_array_release(wrapped, xctx);
    }
}


/*
 * Implementation of method get_reference of interface afw_array.
 */
void
impl_afw_array_get_reference(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    if (self->unmanaged) {
        self->reference_count++;
        afw_pool_get_reference(self->pub.p, xctx);
        return;
    }
    /* new_p / cede_p: pin the pool. Value inf still throws. */
    afw_pool_get_reference(self->pub.p, xctx);
}



/*
 * Implementation of method get_count for interface afw_array.
 */
afw_size_t
impl_afw_array_get_count(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{

    return self->values->count;
}



/*
 * Implementation of method get_data_type for interface afw_array.
 */
const afw_data_type_t *
impl_afw_array_get_data_type(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{

    return self->data_type;
}



/*
 * Implementation of method get_entry_value for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_entry_value(
    AFW_ARRAY_SELF_T *self,
    afw_integer_t index,
    afw_xctx_t *xctx)
{
    const afw_value_t **slot;
    afw_integer_t resolved;
    afw_size_t count;

    count = self->values->count;
    if (count == 0) {
        return NULL;
    }

    /* Negative indexes count from the end (-1 is last), same as setter. */
    if (index < 0) {
        resolved = (afw_integer_t)count + index;
    }
    else {
        resolved = index;
    }

    if (resolved < 0 ||
        resolved >= (afw_integer_t)count)
    {
        return NULL;
    }

    slot = impl_slot_at(self, (afw_size_t)resolved);
    if (!slot) {
        return NULL;
    }
    return impl_promote_structured_entry(self, slot, *slot, xctx);
}



/*
 * Implementation of method get_next_value for interface afw_array.
 *
 * Cursor is the next index stored in the iterator pointer (NULL is 0).
 * Do not point at entries: grow moves them.
 */
const afw_value_t *
impl_afw_array_get_next_value(
    AFW_ARRAY_SELF_T *self,
    const afw_iterator_old_t * * iterator,
    afw_xctx_t *xctx)
{
    const afw_value_t **entries;
    afw_size_t i;
    afw_size_t count;

    count = self->values->count;
    entries = self->values->entries;
    i = *iterator ? (afw_size_t)(uintptr_t)*iterator : 0;
    if (i >= count) {
        *iterator = NULL;
        return NULL;
    }

    *iterator = (const afw_iterator_old_t *)(uintptr_t)(i + 1);
    return impl_promote_structured_entry(self, &entries[i],
        entries[i], xctx);
}



/*
 * Implementation of method get_setter for interface afw_array.
 */
const afw_array_setter_t *
impl_afw_array_get_setter(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{

    return (self->immutable) ? NULL : &self->setter;
}



/*
 * Resolve signed index for an existing element (0 .. count-1).
 * Negative indexes count from the end (-1 is last).
 */
static afw_size_t
impl_resolve_element_index(
    afw_integer_t index,
    afw_size_t count,
    afw_xctx_t *xctx)
{
    afw_integer_t resolved;

    if (count == 0) {
        AFW_THROW_ERROR_Z(general, "Index out of bounds", xctx);
    }

    if (index < 0) {
        resolved = (afw_integer_t)count + index;
    }
    else {
        resolved = index;
    }

    if (resolved < 0 ||
        resolved >= (afw_integer_t)count)
    {
        AFW_THROW_ERROR_Z(general, "Index out of bounds", xctx);
    }

    return (afw_size_t)resolved;
}



/*
 * Resolve signed index for insert (0 .. count inclusive).
 * Negative indexes count from the end; result is clamped into range.
 */
static afw_size_t
impl_resolve_insert_index(
    afw_integer_t index,
    afw_size_t count,
    afw_xctx_t *xctx)
{
    afw_integer_t resolved;

    if (index < 0) {
        resolved = (afw_integer_t)count + index;
        if (resolved < 0) {
            resolved = 0;
        }
    }
    else {
        resolved = index;
        if (resolved > (afw_integer_t)count) {
            resolved = (afw_integer_t)count;
        }
    }

    return (afw_size_t)resolved;
}



/* Update generic/typed data_type bookkeeping when storing value. */
static void
impl_note_value_data_type(
    afw_memory_internal_array_t *self,
    const afw_value_t *value,
    afw_boolean_t was_empty,
    afw_xctx_t *xctx)
{
    if (self->generic) {
        if (was_empty) {
            if (!afw_value_is_undefined(value)) {
                self->data_type = afw_value_get_data_type(value, xctx);
            }
        }
        else if (
            !afw_value_is_undefined(value) &&
            self->data_type &&
            self->data_type != afw_value_get_data_type(value, xctx))
        {
            self->data_type = NULL;
        }
    }
    else if (
        !afw_value_is_undefined(value) &&
        self->data_type &&
        self->data_type != afw_value_get_data_type(value, xctx))
    {
        AFW_THROW_ERROR_Z(general,
            "Value data_type is not array's data type.", xctx);
    }
}



/* Clear inferred data type when a generic array becomes empty. */
static void
impl_maybe_clear_generic_data_type(afw_memory_internal_array_t *self)
{
    if (self->generic && self->values->count == 0) {
        self->data_type = NULL;
    }
}



/*
 * Face overlay: slot_store (get_assignable_value). Generic memory
 * array: raw pointer, same as object set on a non-wrapper. Compile-time
 * array literals must not wrap nested arrays into the constant. Script
 * arrays are faces from get_assignable_value (self if already a face).
 */
static void
impl_store_element(
    AFW_ARRAY_SELF_T *self,
    const afw_value_t **slot,
    const afw_value_t *incoming,
    afw_xctx_t *xctx)
{
    if (self->wrapped) {
        afw_value_slot_store(slot, incoming, xctx);
    }
    else {
        *slot = incoming;
    }
}


/* Face drop releases a held occupant. Generic array never held. */
static void
impl_drop_element(
    AFW_ARRAY_SELF_T *self,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    if (self->wrapped) {
        afw_value_release(value, xctx);
    }
}



/*
 * Slot at zero-based index. NULL if at >= count.
 */
static const afw_value_t **
impl_slot_at(
    afw_memory_internal_array_t *self,
    afw_size_t at)
{
    if (at >= self->values->count) {
        return NULL;
    }
    return &self->values->entries[at];
}



/*
 * New used slot at at (at == count appends). Always NULL first: vector
 * reuse after pop/shift can leave a transferred pointer in the hole, and
 * slot_store would release it.
 */
static const afw_value_t **
impl_new_slot(
    afw_memory_internal_array_t *self,
    afw_size_t at,
    afw_xctx_t *xctx)
{
    afw_vector_insert(self->values, at, xctx) = NULL;
    return &self->values->entries[at];
}



/*
 * Implementation of method set_immutable for interface afw_array_setter.
 */
void
impl_afw_array_setter_set_immutable(
    const afw_array_setter_t * self,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;

    array_self->immutable = true;
}



/*
 * Implementation of method determine_data_type_and_set_immutable for
 * interface afw_array_setter.
 */
const afw_data_type_t *
impl_afw_array_setter_determine_data_type_and_set_immutable(
    const afw_array_setter_t * self,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **entries;
    afw_size_t i;
    afw_size_t count;

    /* Make immutable if not already. */
    if (array_self->immutable) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }
    array_self->immutable = true;

    /* If data type not known yet, try to determine it. */
    if (!array_self->data_type) {
        count = array_self->values->count;
        entries = array_self->values->entries;
        for (i = 0; i < count; i++) {
            if (!array_self->data_type) {
                array_self->data_type =
                    afw_value_get_data_type(entries[i], xctx);
            }
            else {
                if (array_self->data_type !=
                    afw_value_get_data_type(entries[i], xctx))
                {
                    array_self->data_type = NULL;
                    break;
                }
            }
        }
    }

    return array_self->data_type;
}



/*
 * Implementation of method push_value for interface afw_array_setter.
 */
void
impl_afw_array_setter_push_value(
    const afw_array_setter_t * self,
    const afw_value_t * value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **slot;
    afw_boolean_t was_empty;

    was_empty = (array_self->values->count == 0);
    impl_note_value_data_type(array_self, value, was_empty, xctx);

    slot = impl_new_slot(array_self, array_self->values->count, xctx);
    impl_store_element(array_self, slot, value, xctx);
}



/*
 * Implementation of method pop_value for interface afw_array_setter.
 */
const afw_value_t *
impl_afw_array_setter_pop_value(
    const afw_array_setter_t * self,
    afw_boolean_t *found,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t *value;

    /* Empty: NULL; optional found=false (undefined in script if ignored). */
    if (array_self->values->count == 0) {
        if (found) {
            *found = false;
        }
        return NULL;
    }

    if (found) {
        *found = true;
    }
    value = afw_vector_last(array_self->values);
    afw_vector_pop(array_self->values, xctx);
    impl_maybe_clear_generic_data_type(array_self);
    return value;
}



/*
 * Implementation of method shift_value for interface afw_array_setter.
 */
const afw_value_t *
impl_afw_array_setter_shift_value(
    const afw_array_setter_t * self,
    afw_boolean_t *found,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t *value;

    /* Empty: NULL; optional found=false (undefined in script if ignored). */
    if (array_self->values->count == 0) {
        if (found) {
            *found = false;
        }
        return NULL;
    }

    if (found) {
        *found = true;
    }
    value = array_self->values->entries[0];
    afw_vector_remove(array_self->values, 0, xctx);
    impl_maybe_clear_generic_data_type(array_self);
    return value;
}



/*
 * Implementation of method insert_value for interface afw_array_setter.
 */
void
impl_afw_array_setter_insert_value(
    const afw_array_setter_t * self,
    afw_integer_t index,
    const afw_value_t * value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **slot;
    afw_size_t at;
    afw_boolean_t was_empty;

    was_empty = (array_self->values->count == 0);
    impl_note_value_data_type(array_self, value, was_empty, xctx);

    at = impl_resolve_insert_index(index,
        array_self->values->count, xctx);
    slot = impl_new_slot(array_self, at, xctx);
    impl_store_element(array_self, slot, value, xctx);
}



/*
 * Implementation of method set_value for interface afw_array_setter.
 *
 * In-range replace, or append when index === count (grow by one). Never
 * fills gaps for index > count (no sparse assign). Negative indexes only
 * resolve into existing elements (no grow from the end).
 */
void
impl_afw_array_setter_set_value(
    const afw_array_setter_t *self,
    afw_integer_t index,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **slot;
    afw_size_t at;

    /* Append at length: dense grow-by-one (issue #39 array semantics). */
    if (index >= 0 &&
        (afw_size_t)index == array_self->values->count)
    {
        impl_afw_array_setter_insert_value(self, index, value, xctx);
        return;
    }

    at = impl_resolve_element_index(index,
        array_self->values->count, xctx);
    impl_note_value_data_type(array_self, value, false, xctx);

    slot = impl_slot_at(array_self, at);
    if (!slot) {
        AFW_THROW_ERROR_Z(general, "Index out of bounds", xctx);
    }

    impl_store_element(array_self, slot, value, xctx);
}



/*
 * Implementation of method remove_value_by_index for interface afw_array_setter.
 */
void
impl_afw_array_setter_remove_value_by_index(
    const afw_array_setter_t *self,
    afw_integer_t index,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **slot;
    afw_size_t at;

    at = impl_resolve_element_index(index,
        array_self->values->count, xctx);

    slot = impl_slot_at(array_self, at);
    if (!slot) {
        AFW_THROW_ERROR_Z(general, "Index out of bounds", xctx);
    }

    impl_drop_element(array_self, *slot, xctx);
    afw_vector_remove(array_self->values, at, xctx);
    impl_maybe_clear_generic_data_type(array_self);
}



/*
 * Implementation of method remove_value of interface afw_array_setter.
 */
void
impl_afw_array_setter_remove_value(
    const afw_array_setter_t * self,
    const afw_value_t * value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **entries;
    afw_size_t i;
    afw_size_t count;

    count = array_self->values->count;
    entries = array_self->values->entries;
    for (i = 0; i < count; i++) {
        if (afw_value_equal(value, entries[i], xctx)) {
            impl_drop_element(array_self, entries[i], xctx);
            afw_vector_remove(array_self->values, i, xctx);
            impl_maybe_clear_generic_data_type(array_self);
            return;
        }
    }

    AFW_THROW_ERROR_Z(general, "Value not in array", xctx);
}



/*
 * Implementation of method remove_all_values of interface afw_array_setter.
 */
void
impl_afw_array_setter_remove_all_values(
    const afw_array_setter_t * self,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **entries;
    afw_size_t i;
    afw_size_t count;

    count = array_self->values->count;
    entries = array_self->values->entries;
    for (i = 0; i < count; i++) {
        impl_drop_element(array_self, entries[i], xctx);
    }
    afw_vector_clear(array_self->values);

    if (array_self->generic) {
        array_self->data_type = NULL;
    }
}


void
impl_afw_array_managed_release(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_value_t **entries;
    afw_size_t i;
    afw_size_t count;

    if (self->reference_count <= 0) {
        return;
    }
    self->reference_count--;
    if (self->reference_count != 0) {
        return;
    }
    if (self->values) {
        count = self->values->count;
        entries = self->values->entries;
        for (i = 0; i < count; i++) {
            if (entries[i]) {
                afw_value_release(entries[i], xctx);
                entries[i] = NULL;
            }
        }
        afw_vector_release(self->values, xctx);
        self->values = NULL;
    }
    afw_pool_free_memory(xctx->p, self,
        sizeof(afw_memory_internal_array_t), xctx);
}


void
impl_afw_array_managed_get_reference(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;
    self->reference_count++;
}


void
impl_afw_array_managed_setter_push_value(
    const afw_array_setter_t *self,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **slot;
    afw_boolean_t was_empty;

    was_empty = (array_self->values->count == 0);
    impl_note_value_data_type(array_self, value, was_empty, xctx);
    slot = impl_new_slot(array_self, array_self->values->count, xctx);
    afw_value_slot_store(slot, value, xctx);
}


void
impl_afw_array_managed_setter_insert_value(
    const afw_array_setter_t *self,
    afw_integer_t index,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **slot;
    afw_size_t at;
    afw_boolean_t was_empty;

    was_empty = (array_self->values->count == 0);
    impl_note_value_data_type(array_self, value, was_empty, xctx);
    at = impl_resolve_insert_index(index,
        array_self->values->count, xctx);
    slot = impl_new_slot(array_self, at, xctx);
    afw_value_slot_store(slot, value, xctx);
}


void
impl_afw_array_managed_setter_set_value(
    const afw_array_setter_t *self,
    afw_integer_t index,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **slot;
    afw_size_t at;

    if (index >= 0 &&
        (afw_size_t)index == array_self->values->count)
    {
        impl_afw_array_managed_setter_insert_value(self, index,
            value, xctx);
        return;
    }
    at = impl_resolve_element_index(index,
        array_self->values->count, xctx);
    impl_note_value_data_type(array_self, value, false, xctx);
    slot = impl_slot_at(array_self, at);
    if (!slot) {
        AFW_THROW_ERROR_Z(general, "Index out of bounds", xctx);
    }
    afw_value_slot_store(slot, value, xctx);
}


void
impl_afw_array_managed_setter_remove_all_values(
    const afw_array_setter_t *self,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **entries;
    afw_size_t i;
    afw_size_t count;

    if (array_self->values) {
        count = array_self->values->count;
        entries = array_self->values->entries;
        for (i = 0; i < count; i++) {
            if (entries[i]) {
                afw_value_release(entries[i], xctx);
                entries[i] = NULL;
            }
        }
        afw_vector_clear(array_self->values);
    }
    if (array_self->generic) {
        array_self->data_type = NULL;
    }
}


/* Create or clone of an array. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_or_clone(
    const afw_array_t *array,
    const afw_data_type_t *data_type,
    afw_boolean_t clone_values,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_iterator_old_t *iterator;
    const afw_array_t *result;
    const afw_value_t *value;
    const afw_data_type_t *use_data_type;

    use_data_type = data_type;
    if (array) {
        use_data_type = afw_array_get_data_type(array, xctx);
        if (data_type && use_data_type && data_type != use_data_type)
        {
            AFW_THROW_ERROR_Z(general, "data_type does not match", xctx);
        }
    }
    result = afw_array_create_unmanaged_of(use_data_type, p, xctx);
    if (array) for (iterator = NULL;;)
    {
        value = afw_array_get_next_value(array, &iterator, xctx);
        if (!value) {
            break;
        }
        if (clone_values) {
            value = afw_value_clone(value, p, xctx);
        }
        afw_array_push_value(result, value, xctx);
    }

    return result;
}


/* Create a typed array from a value. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_unmanaged_from_value(
    const afw_data_type_t *data_type,
    const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_data_type_t *value_data_type;
    const afw_array_t *value_array;
    const afw_array_t *old_array;
    const afw_iterator_old_t *iterator;
    const afw_value_t *v;

    value_data_type = afw_value_get_data_type(value, xctx);

    if (value_data_type == afw_data_type_array) {
        value_array = ((afw_value_array_t *)value)->internal;
        value_data_type = afw_array_get_data_type(value_array, xctx);
        if (value_data_type != data_type) {
            old_array = value_array;
            value_array = afw_array_create_unmanaged_of(data_type, p, xctx);
            for (iterator = NULL;;) {
                v = afw_array_get_next_value(old_array, &iterator, xctx);
                if (!v) {
                    break;
                }
                v = afw_value_convert(v, data_type, true, p, xctx);
                afw_array_push_value(value_array, v, xctx);
            }        
        }
    }
    else {
        v = afw_value_convert(value, data_type, true, p, xctx);
        value_array = afw_array_create_unmanaged_from_c_array(
            AFW_VALUE_INTERNAL(v), false, data_type, 1, p, xctx);
    }

    return value_array;
}
