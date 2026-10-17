// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_array interface
 *
 * Copyright (c) 2010-2024 Clemson University
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

/* Declares and rti/inf defines for interface afw_array */
#define AFW_IMPLEMENTATION_ID "memory"
typedef struct afw_memory_internal_array_s afw_memory_internal_array_t;
#define AFW_ARRAY_SELF_T afw_memory_internal_array_t
#define impl_afw_array_for_each_reference afw_array_no_references_for_each
#define impl_afw_array_release_references afw_array_no_references_release_references
#include "afw_array_impl_declares.h"
#define impl_afw_array_setter_push_value_take \
    afw_array_setter_push_value_take_by_copy
#include "afw_array_setter_impl_declares.h"
#undef impl_afw_array_setter_push_value_take

static void
impl_afw_array_managed_for_each_reference(
    AFW_ARRAY_SELF_T *self, afw_reference_cb_t callback, void *context,
    afw_xctx_t *xctx);
static afw_size_t
impl_afw_array_managed_get_reference_count(
    AFW_ARRAY_SELF_T *self, afw_xctx_t *xctx);
static void
impl_afw_array_managed_release_references(
    AFW_ARRAY_SELF_T *self, afw_xctx_t *xctx);
static void
impl_afw_array_managed_setter_push_value_take(
    const afw_array_setter_t *setter,
    const afw_value_t *value,
    afw_xctx_t *xctx);
static void
impl_afw_array_managed_release(
    AFW_ARRAY_SELF_T *self, afw_xctx_t *xctx);
static const afw_array_t *
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
static void
impl_afw_array_managed_setter_remove_value_by_index(
    const afw_array_setter_t *self,
    afw_integer_t index,
    afw_xctx_t *xctx);
static void
impl_afw_array_managed_setter_remove_value(
    const afw_array_setter_t *self,
    const afw_value_t *value,
    afw_xctx_t *xctx);
static const afw_value_t *
impl_afw_array_managed_setter_pop_value(
    const afw_array_setter_t *self,
    afw_boolean_t *found,
    const afw_pool_t *p,
    afw_xctx_t *xctx);
static const afw_value_t *
impl_afw_array_managed_setter_shift_value(
    const afw_array_setter_t *self,
    afw_boolean_t *found,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

#undef AFW_IMPLEMENTATION_ID
#define AFW_IMPLEMENTATION_ID "memory_managed"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_array_managed_inf
#define AFW_IMPLEMENTATION_INF_VARIABLES true
#define AFW_ARRAY_INF_ONLY
#define impl_afw_array_release impl_afw_array_managed_release
#define impl_afw_array_get_reference impl_afw_array_managed_get_reference
#undef impl_afw_array_for_each_reference
#define impl_afw_array_for_each_reference impl_afw_array_managed_for_each_reference
#define impl_afw_array_get_reference_count \
    impl_afw_array_managed_get_reference_count
#undef impl_afw_array_release_references
#define impl_afw_array_release_references \
    impl_afw_array_managed_release_references
#include "afw_array_impl_declares.h"
#undef impl_afw_array_release_references
#undef impl_afw_array_for_each_reference
#undef impl_afw_array_get_reference_count
#undef AFW_ARRAY_INF_ONLY
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_INF_VARIABLES
#undef impl_afw_array_release
#undef impl_afw_array_get_reference
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_array_managed_setter_inf
#define AFW_ARRAY_SETTER_INF_ONLY
#define impl_afw_array_setter_push_value \
    impl_afw_array_managed_setter_push_value
#define impl_afw_array_setter_push_value_take \
    impl_afw_array_managed_setter_push_value_take
#define impl_afw_array_setter_set_value \
    impl_afw_array_managed_setter_set_value
#define impl_afw_array_setter_insert_value \
    impl_afw_array_managed_setter_insert_value
#define impl_afw_array_setter_remove_all_values \
    impl_afw_array_managed_setter_remove_all_values
#define impl_afw_array_setter_remove_value_by_index \
    impl_afw_array_managed_setter_remove_value_by_index
#define impl_afw_array_setter_remove_value \
    impl_afw_array_managed_setter_remove_value
#define impl_afw_array_setter_pop_value \
    impl_afw_array_managed_setter_pop_value
#define impl_afw_array_setter_shift_value \
    impl_afw_array_managed_setter_shift_value
#include "afw_array_setter_impl_declares.h"
#undef AFW_ARRAY_SETTER_INF_ONLY
#undef impl_afw_array_setter_push_value
#undef impl_afw_array_setter_push_value_take
#undef impl_afw_array_setter_set_value
#undef impl_afw_array_setter_insert_value
#undef impl_afw_array_setter_remove_all_values
#undef impl_afw_array_setter_remove_value_by_index
#undef impl_afw_array_setter_remove_value
#undef impl_afw_array_setter_pop_value
#undef impl_afw_array_setter_shift_value
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
    /*
     * Typed (generic false): every defined element is data_type. Generic:
     * data_type is the type of data_type_count elements; the array is of
     * data_type when that is every element but undefined_count holes.
     * data_type_unknown: the last element of data_type left a mixed
     * array; get_data_type counts again.
     */
    const afw_data_type_t *data_type;
    afw_size_t data_type_count;
    afw_size_t undefined_count;
    afw_boolean_t data_type_unknown;
    afw_array_setter_t setter;
    /*
     * Dense store of value pointers. Do not hold interior
     * entries pointers across grow.
     */
    afw_memory_internal_array_values_t *values;
    /*
     * Unmanaged memory arrays: extra holds from add_reference. Zero is
     * idle. Last extra hold walks remaining slot_store'd elements.
     */
    afw_integer_t reference_count;
    /* In this xctx's possible roots (afw_reference_possible_root). */
    afw_boolean_t buffered;
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

static const afw_data_type_t *
impl_current_data_type(
    afw_memory_internal_array_t *self,
    afw_xctx_t *xctx);



AFW_DEFINE(const afw_array_t *)
afw_array_create_with_options(
    int options,
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{

    afw_memory_internal_array_t *self;

    /*
     * If new_p, own pool is a child of dest p: without a release it
     * goes with p (caller may release, #656).
     */
    if (AFW_ARRAY_MEMORY_OPTION_IS(options, new_p)) {
        p = afw_pool_tracker_create(p, xctx);
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
     * Face: pooled for a pooled array (get_reference throws); counted
     * for an array that owns its pool (references are its pool's).
     */
    self->value.inf = (self->unmanaged)
        ? &afw_value_unmanaged_array_inf
        : &afw_value_counted_array_inf;
    self->value.internal = (const afw_array_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    self->data_type = data_type;
    self->generic = data_type == NULL;
    self->values = afw_vector_create(
        afw_memory_internal_array_values_t, 0, p, xctx);
    self->setter.inf = &impl_afw_array_setter_inf;
    self->setter.array = (const afw_array_t *)self;

    return (const afw_array_t *)self;

}


/* Slot / get / pop lifetime: afw_array_create_managed in afw_array.h. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_managed(
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self;

    p = p->managed_p;
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
        obj = afw_object_to_managed(obj, to->p, xctx);
        afw_array_push_value(to, obj->value, xctx);
        afw_object_release(obj, xctx);
        return;
    }
    if (afw_value_is_array(value)) {
        arr = ((const afw_value_array_t *)value)->internal;
        if (!arr) {
            return;
        }
        arr = afw_array_to_managed(arr, to->p, xctx);
        afw_array_push_value(to, arr->value, xctx);
        afw_array_release(arr, xctx);
        return;
    }
    afw_array_push_value(to, value, xctx);
}


AFW_DEFINE(const afw_array_t *)
afw_array_to_managed(
    const afw_array_t *from,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_array_t *to;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;
    const afw_data_type_t *data_type;

    if (!from) {
        AFW_THROW_ERROR_Z(general,
            "afw_array_to_managed requires from",
            xctx);
    }
    if (from->inf == &impl_afw_array_managed_inf) {
        afw_array_get_reference(from, xctx);
        return from;
    }
    data_type = afw_array_get_declared_data_type(from, xctx);
    to = afw_array_create_managed(data_type, p, xctx);
    if (from->inf == &impl_afw_array_inf) {
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


/* Data type a copy of array is created with. */
AFW_DEFINE(const afw_data_type_t *)
afw_array_get_declared_data_type(
    const afw_array_t *array,
    afw_xctx_t *xctx)
{
    const afw_memory_internal_array_t *self;

    if (array->inf == &impl_afw_array_inf ||
        array->inf == &impl_afw_array_managed_inf)
    {
        self = (const afw_memory_internal_array_t *)array;
        return self->generic ? NULL : self->data_type;
    }
    return afw_array_get_data_type(array, xctx);
}



/* True if array is a generic memory array. */
AFW_DEFINE(afw_boolean_t)
afw_array_is_memory(const afw_array_t *array)
{
    return array && array->inf == &impl_afw_array_inf;
}




/*
 * Implementation of method release of interface afw_array.
 */
void
impl_afw_array_release(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Pooled: no reference was ever taken, so a release is a bug. */
    if (self->unmanaged) {
        AFW_THROW_ERROR_Z(general,
            "release of a pooled array", xctx);
    }

    /* Owns its pool (new_p / cede_p): a reference is the pool's. */
    afw_pool_release(self->pub.p, xctx);
}


/*
 * Implementation of method get_reference of interface afw_array.
 */
const afw_array_t *
impl_afw_array_get_reference(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Pooled: nothing to reference (see the object equivalent). */
    if (self->unmanaged) {
        AFW_THROW_ERROR_Z(general,
            "get_reference of a pooled array", xctx);
    }
    /* new_p / cede_p: pin the pool. Value inf still throws. */
    afw_pool_get_reference(self->pub.p, xctx);
    return (const afw_array_t *)self;
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
    return impl_current_data_type(self, xctx);
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
    return *slot;
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
    return entries[i];
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

    if (index < 0) {
        resolved = (afw_integer_t)count + index;
    }
    else {
        resolved = index;
    }

    /* Say the index and the length: "out of bounds" alone hides both. */
    if (resolved < 0) {
        AFW_THROW_ERROR_FZ(argument_error, xctx,
            "Index " AFW_INTEGER_FMT " is before the start of an array of"
            " length " AFW_SIZE_T_FMT, index, count);
    }
    if (resolved >= (afw_integer_t)count) {
        AFW_THROW_ERROR_FZ(argument_error, xctx,
            "Index " AFW_INTEGER_FMT " is past the end of an array of"
            " length " AFW_SIZE_T_FMT, index, count);
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



/* Count each element of a generic array again. */
static void
impl_count_data_types(
    afw_memory_internal_array_t *self,
    afw_xctx_t *xctx)
{
    const afw_value_t **entries;
    const afw_data_type_t *dt;
    afw_size_t i;

    self->data_type = NULL;
    self->data_type_count = 0;
    self->undefined_count = 0;
    self->data_type_unknown = false;
    entries = self->values->entries;
    for (i = 0; i < self->values->count; i++) {
        if (afw_value_is_undefined(entries[i])) {
            self->undefined_count++;
            continue;
        }
        dt = afw_value_get_data_type(entries[i], xctx);
        if (!self->data_type) {
            self->data_type = dt;
        }
        if (dt == self->data_type) {
            self->data_type_count++;
        }
    }
}



/* Data type of every defined element, or NULL if mixed or none. */
static const afw_data_type_t *
impl_current_data_type(
    afw_memory_internal_array_t *self,
    afw_xctx_t *xctx)
{
    if (!self->generic) {
        return self->data_type;
    }
    if (self->data_type_unknown) {
        impl_count_data_types(self, xctx);
    }
    if (self->data_type_count > 0 &&
        self->data_type_count + self->undefined_count ==
            self->values->count)
    {
        return self->data_type;
    }
    return NULL;
}



/*
 * Before value is stored. A typed array throws if value is not its data
 * type; a generic array counts it.
 */
static void
impl_note_added(
    afw_memory_internal_array_t *self,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *dt;

    if (afw_value_is_undefined(value)) {
        if (self->generic) {
            self->undefined_count++;
        }
        return;
    }
    dt = afw_value_get_data_type(value, xctx);
    if (!self->generic) {
        if (self->data_type && self->data_type != dt) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "An array of %ku can not hold a value of data type %ku",
                &self->data_type->data_type_id, &dt->data_type_id);
        }
        return;
    }
    if (self->data_type_unknown) {
        return;
    }
    if (!self->data_type) {
        self->data_type = dt;
    }
    if (dt == self->data_type) {
        self->data_type_count++;
    }
}



/* Data type of an element, or NULL for a hole. */
#define IMPL_ELEMENT_DATA_TYPE(_value, _xctx) \
    (afw_value_is_undefined(_value) \
        ? NULL : afw_value_get_data_type((_value), (_xctx)))



/*
 * After an element left a generic array. removed_dt is its
 * IMPL_ELEMENT_DATA_TYPE, taken before the element was released.
 */
static void
impl_note_removed(
    afw_memory_internal_array_t *self,
    const afw_data_type_t *removed_dt)
{
    if (!self->generic) {
        return;
    }
    if (!self->values || self->values->count == 0) {
        self->data_type = NULL;
        self->data_type_count = 0;
        self->undefined_count = 0;
        self->data_type_unknown = false;
        return;
    }
    if (self->data_type_unknown) {
        return;
    }
    if (!removed_dt) {
        self->undefined_count--;
    }
    else if (removed_dt == self->data_type) {
        self->data_type_count--;
        if (self->data_type_count == 0) {
            self->data_type = NULL;
            self->data_type_unknown =
                self->undefined_count != self->values->count;
        }
    }
}



/*
 * Unmanaged memory array: store the raw pointer, the same as an object
 * set. Compile-time array literals must not wrap nested arrays into the
 * constant.
 */
static void
impl_store_element(
    AFW_ARRAY_SELF_T *self,
    const afw_value_t **slot,
    const afw_value_t *incoming,
    afw_xctx_t *xctx)
{
    *slot = incoming;
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

    /* Count now: get_data_type of an immutable array never writes. */
    impl_current_data_type(array_self, xctx);
    array_self->immutable = true;
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

    impl_note_added(array_self, value, xctx);

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
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t *value;

    /* Unmanaged: store a raw pointer; dest p unused. */
    (void)p;

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
    impl_note_removed(array_self, IMPL_ELEMENT_DATA_TYPE(value, xctx));
    return value;
}



/*
 * Implementation of method shift_value for interface afw_array_setter.
 */
const afw_value_t *
impl_afw_array_setter_shift_value(
    const afw_array_setter_t * self,
    afw_boolean_t *found,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t *value;

    /* Unmanaged: store a raw pointer; dest p unused. */
    (void)p;

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
    impl_note_removed(array_self, IMPL_ELEMENT_DATA_TYPE(value, xctx));
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

    impl_note_added(array_self, value, xctx);

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
    const afw_data_type_t *removed_dt;

    /* Append at length: dense grow-by-one (issue #39 array semantics). */
    if (index >= 0 &&
        (afw_size_t)index == array_self->values->count)
    {
        impl_afw_array_setter_insert_value(self, index, value, xctx);
        return;
    }

    at = impl_resolve_element_index(index,
        array_self->values->count, xctx);
    slot = impl_slot_at(array_self, at);
    if (!slot) {
        AFW_THROW_ERROR_Z(general,
            "Internal error: no slot at a resolved index", xctx);
    }

    impl_note_added(array_self, value, xctx);
    removed_dt = IMPL_ELEMENT_DATA_TYPE(*slot, xctx);
    impl_store_element(array_self, slot, value, xctx);
    impl_note_removed(array_self, removed_dt);
}



/*
 * Implementation of method remove_value_by_index for interface afw_array_setter.
 *
 * Unmanaged arrays don't hold their entries, so nothing is released.
 * Managed arrays use the managed setter, which last-releases the occupant.
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
    const afw_data_type_t *removed_dt;

    at = impl_resolve_element_index(index,
        array_self->values->count, xctx);

    slot = impl_slot_at(array_self, at);
    if (!slot) {
        AFW_THROW_ERROR_Z(general,
            "Internal error: no slot at a resolved index", xctx);
    }

    removed_dt = IMPL_ELEMENT_DATA_TYPE(*slot, xctx);
    afw_vector_remove(array_self->values, at, xctx);
    impl_note_removed(array_self, removed_dt);
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
    const afw_data_type_t *removed_dt;

    count = array_self->values->count;
    entries = array_self->values->entries;
    for (i = 0; i < count; i++) {
        if (afw_value_equal(value, entries[i], xctx)) {
            removed_dt = IMPL_ELEMENT_DATA_TYPE(entries[i], xctx);
            afw_vector_remove(array_self->values, i, xctx);
            impl_note_removed(array_self, removed_dt);
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

    afw_vector_clear(array_self->values);

    impl_note_removed(array_self, NULL);
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
        afw_reference_possible_root(&self->pub.ref, self->pub.p,
            &self->buffered, xctx);
        return;
    }
    afw_reference_forget(&self->pub.ref, &self->buffered, xctx);
    /*
     * One walk: release every held element (nested object/array
     * the same as scalar), then the vector, then the header.
     */
    if (self->values) {
        count = self->values->count;
        entries = self->values->entries;
        for (i = 0; i < count; i++) {
            if (entries[i]) {
                afw_value_release_held(entries[i], xctx);
                entries[i] = NULL;
            }
        }
        afw_vector_release(self->values, xctx);
        self->values = NULL;
    }
    afw_pool_free_memory(self->pub.p, self,
        sizeof(afw_memory_internal_array_t), xctx);
}


const afw_array_t *
impl_afw_array_managed_get_reference(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;
    self->reference_count++;
    return (const afw_array_t *)self;
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

    impl_note_added(array_self, value, xctx);
    slot = impl_new_slot(array_self, array_self->values->count, xctx);
    afw_value_slot_store(slot, value, array_self->pub.p, xctx);
}


/* push_value_take of a fully managed memory array: the slot takes it. */
static void
impl_afw_array_managed_setter_push_value_take(
    const afw_array_setter_t *setter,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self;
    const afw_value_t **slot;

    self = (afw_memory_internal_array_t *)setter->array;
    if (self->immutable) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }
    impl_note_added(self, value, xctx);
    slot = impl_new_slot(self, self->values->count, xctx);
    afw_value_slot_take(slot, value, xctx);
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

    impl_note_added(array_self, value, xctx);
    at = impl_resolve_insert_index(index,
        array_self->values->count, xctx);
    slot = impl_new_slot(array_self, at, xctx);
    afw_value_slot_store(slot, value, array_self->pub.p, xctx);
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
    const afw_data_type_t *removed_dt;

    if (index >= 0 &&
        (afw_size_t)index == array_self->values->count)
    {
        impl_afw_array_managed_setter_insert_value(self, index,
            value, xctx);
        return;
    }
    at = impl_resolve_element_index(index,
        array_self->values->count, xctx);
    slot = impl_slot_at(array_self, at);
    if (!slot) {
        AFW_THROW_ERROR_Z(general,
            "Internal error: no slot at a resolved index", xctx);
    }
    impl_note_added(array_self, value, xctx);
    removed_dt = IMPL_ELEMENT_DATA_TYPE(*slot, xctx);
    afw_value_slot_store(slot, value, array_self->pub.p, xctx);
    impl_note_removed(array_self, removed_dt);
}


/* Transferred occupant last-release on dest p. Caller does not
 * release. See afw_array_create_managed. */
static const afw_value_t *
impl_register_transferred_temp(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (!value) {
        return NULL;
    }
    if (!p) {
        AFW_THROW_ERROR_Z(general, "pop/shift with no dest p", xctx);
    }
    afw_pool_release_value_at_cleanup(value, p, xctx);
    return value;
}


const afw_value_t *
impl_afw_array_managed_setter_pop_value(
    const afw_array_setter_t *self,
    afw_boolean_t *found,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t *value;

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
    impl_note_removed(array_self, IMPL_ELEMENT_DATA_TYPE(value, xctx));
    return impl_register_transferred_temp(value, p, xctx);
}


const afw_value_t *
impl_afw_array_managed_setter_shift_value(
    const afw_array_setter_t *self,
    afw_boolean_t *found,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t *value;

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
    impl_note_removed(array_self, IMPL_ELEMENT_DATA_TYPE(value, xctx));
    return impl_register_transferred_temp(value, p, xctx);
}


void
impl_afw_array_managed_setter_remove_value_by_index(
    const afw_array_setter_t *self,
    afw_integer_t index,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **slot;
    afw_size_t at;
    const afw_data_type_t *removed_dt;

    at = impl_resolve_element_index(index,
        array_self->values->count, xctx);
    slot = impl_slot_at(array_self, at);
    if (!slot) {
        AFW_THROW_ERROR_Z(general,
            "Internal error: no slot at a resolved index", xctx);
    }
    /* Managed array holds occupants; splice would leak the slot hold. */
    removed_dt = IMPL_ELEMENT_DATA_TYPE(*slot, xctx);
    afw_value_release_held(*slot, xctx);
    *slot = NULL;
    afw_vector_remove(array_self->values, at, xctx);
    impl_note_removed(array_self, removed_dt);
}


void
impl_afw_array_managed_setter_remove_value(
    const afw_array_setter_t *self,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *array_self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)self)->array;
    const afw_value_t **entries;
    afw_size_t i;
    afw_size_t count;
    const afw_data_type_t *removed_dt;

    count = array_self->values->count;
    entries = array_self->values->entries;
    for (i = 0; i < count; i++) {
        if (afw_value_equal(value, entries[i], xctx)) {
            removed_dt = IMPL_ELEMENT_DATA_TYPE(entries[i], xctx);
            afw_value_release_held(entries[i], xctx);
            entries[i] = NULL;
            afw_vector_remove(array_self->values, i, xctx);
            impl_note_removed(array_self, removed_dt);
            return;
        }
    }

    AFW_THROW_ERROR_Z(general, "Value not in array", xctx);
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
                afw_value_release_held(entries[i], xctx);
                entries[i] = NULL;
            }
        }
        afw_vector_clear(array_self->values);
    }
    impl_note_removed(array_self, NULL);
}


/* Create or clone of an array. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_pooled_copy(
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
        use_data_type = afw_array_get_declared_data_type(array, xctx);
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
            value = afw_value_create_pooled_copy(value, p, xctx);
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


/* Last release releases every held element. */
static void
impl_afw_array_managed_for_each_reference(
    AFW_ARRAY_SELF_T *self,
    afw_reference_cb_t callback,
    void *context,
    afw_xctx_t *xctx)
{
    afw_size_t i;

    if (!self->values) {
        return;
    }
    for (i = 0; i < self->values->count; i++) {
        afw_value_list_reference(self->values->entries[i],
            callback, context, xctx);
    }
}


/* Pooled: not counted. Owns its pool: the pool's count. */
afw_size_t
impl_afw_array_get_reference_count(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    if (self->unmanaged) {
        return 0;
    }
    return afw_pool_get_reference_count(self->pub.p, xctx);
}


static afw_size_t
impl_afw_array_managed_get_reference_count(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;
    return (afw_size_t)self->reference_count;
}


/* Release every held element, as last release would. */
static void
impl_afw_array_managed_release_references(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_size_t i;
    const afw_value_t *value;

    if (!self->values) {
        return;
    }
    for (i = 0; i < self->values->count; i++) {
        value = self->values->entries[i];
        self->values->entries[i] = NULL;
        afw_value_release_held(value, xctx);
    }
}
