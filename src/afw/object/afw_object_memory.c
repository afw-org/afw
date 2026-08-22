// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_object interface
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_object_memory.c
 * @brief Implementation of afw_object interface for in-memory objects
 */

#include "afw_internal.h"

#define impl_afw_object_get_meta \
    afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta

/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "memory"
#define AFW_OBJECT_SELF_T afw_object_internal_memory_object_t
#include "afw_object_impl_declares.h"
#include "afw_object_setter_impl_declares.h"

static void
impl_managed_face_overlay_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx);


AFW_DEFINE(const afw_object_t *)
afw_object_create_with_options(
    int options, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_object_internal_memory_object_t *self;

    /* If managed, own pool is a child of p->managed_p. */
    if (options == AFW_OBJECT_MEMORY_OPTION_managed)
    {
        p = afw_pool_create(p->managed_p, xctx);
    }

    /* Allocate memory for self and initialize. */
    self = afw_pool_calloc_type(p, afw_object_internal_memory_object_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->unmanaged = AFW_OBJECT_MEMORY_OPTION_IS(options, unmanaged);
    /*
     * Dual face: value.internal is this instance. Inf matches object
     * lifetime — managed face when the object owns a child pool;
     * unmanaged when create options say unmanaged (pool bulk free).
     */
    self->value.inf = self->unmanaged
        ? &afw_value_unmanaged_object_inf
        : &afw_value_managed_object_inf;
    self->value.internal = (const afw_object_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    /* clone_on_set: residual field; always false (no public option). */
    self->setter.inf = &impl_afw_object_setter_inf;
    self->setter.object = (const afw_object_t *)self;
    /* self->wrapped is NULL (calloc). */

    /* Return new object. */
    return (const afw_object_t *)self;
}


/* Create memory object that looks through to a wrapped base. */
AFW_DEFINE(const afw_object_t *)
afw_object_create_wrapper_with_options(
    int options,
    const afw_object_t *wrapped,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_object_internal_memory_object_t *self;

    if (!wrapped) {
        AFW_THROW_ERROR_Z(general,
            "afw_object_create_wrapper_with_options requires a wrapped object",
            xctx);
    }

    self = (afw_object_internal_memory_object_t *)
        afw_object_create_with_options(options, p, xctx);
    self->wrapped = wrapped;
    /*
     * Managed face: one reference on wrapped for the face's life. Released
     * when the face pool is destroyed (see impl_afw_object_release). Unmanaged
     * faces borrow only; caller owns base lifetime.
     */
    if (!self->unmanaged) {
        afw_object_get_reference(wrapped, xctx);
    }
    afw_pool_register_cleanup_before(self->pub.p, self, NULL,
        impl_managed_face_overlay_cleanup, xctx);
    /*
     * Carry meta (path, objectId, reconcilable, …) onto the face so
     * meta(face) matches the adapter entity. Property gets still look
     * through to wrapped for bag content.
     */
    afw_object_meta_clone_and_set((const afw_object_t *)self, wrapped, xctx);
    return (const afw_object_t *)self;
}


/* Empty face for script-mutable creates (overlay holds, not generic set). */
AFW_DEFINE(const afw_object_t *)
afw_object_create_script_wrapper(
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *base;

    base = afw_object_create_unmanaged(p, xctx);
    return afw_object_create_wrapper_unmanaged(base, p, xctx);
}



/* True if object is a memory look-through wrapper (has wrapped base). */
AFW_DEFINE(afw_boolean_t)
afw_object_is_memory_wrapper(const afw_object_t *object)
{
    const afw_object_internal_memory_object_t *self;

    if (!object || object->inf != &impl_afw_object_inf) {
        return false;
    }
    self = (const afw_object_internal_memory_object_t *)object;
    return self->wrapped != NULL;
}



/* Base under a face, or object if not a face. */
AFW_DEFINE(const afw_object_t *)
afw_object_memory_wrapper_base(const afw_object_t *object)
{
    const afw_object_internal_memory_object_t *self;

    if (!object) {
        return NULL;
    }
    if (object->inf != &impl_afw_object_inf) {
        return object;
    }
    self = (const afw_object_internal_memory_object_t *)object;
    return self->wrapped ? self->wrapped : object;
}


AFW_DEFINE(const afw_object_t *)
afw_object_create_embedded(
    const afw_object_t *embedding_object,
    const afw_value_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    afw_object_internal_memory_object_t *self;
    afw_object_internal_memory_object_t *embedder;

    /* Embedding object must be this implementation. */
    if (embedding_object->inf != &impl_afw_object_inf) {
        AFW_THROW_ERROR_Z(general,
            "Embedding object is not afw_memory object", xctx);
    }
    embedder = (afw_object_internal_memory_object_t *)embedding_object;

    /* Use pool of embedder. */
    p = embedder->pub.p;

    /* Allocate memory for self and initialize. */
    self = afw_pool_calloc_type(p, afw_object_internal_memory_object_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->value.inf = &afw_value_managed_object_inf;
    self->value.internal = (const afw_object_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    self->pub.meta.embedding_object = embedding_object;
    self->pub.meta.id = afw_object_string_property_name_as_utf8(
        property_name, xctx);
    self->managed_by_entity = true;
    self->setter.inf = &impl_afw_object_setter_inf;
    self->setter.object = (const afw_object_t *)self;
    self->clone_on_set = embedder->clone_on_set;

    /* Set embedded object as property of embedding object. */
    afw_object_set_property_as_object(
        embedding_object, property_name, (const afw_object_t *)self, xctx);

    /* Return new object. */
    return (const afw_object_t *)self;
}



/* Insure embedded object exists creating if necessary. */
AFW_DEFINE(const afw_object_t *)
afw_object_insure_embedded_exists(
    const afw_object_t *embedding_object,
    const afw_value_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    result = afw_object_old_get_property_as_object(embedding_object,
        property_name, xctx);

    if (!result) {
        result = afw_object_create_embedded(embedding_object,
            property_name, xctx);
    }

    return result;
}



/* Release overlay values this face slot_store'd. Not look-through base. */
static void
impl_release_local_properties(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_object_internal_name_value_entry_t *e;

    for (e = self->first_property; e; e = e->next) {
        if (e->value) {
            afw_value_release(e->value, xctx);
            e->value = NULL;
        }
    }
}


/* Managed face: walk overlay while self is still in the pool. */
static void
impl_managed_face_overlay_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    AFW_OBJECT_SELF_T *self = (AFW_OBJECT_SELF_T *)data;

    (void)data2;
    (void)p;
    impl_release_local_properties(self, xctx);
}


/*
 * Implementation of method release of interface afw_object.
 */
void
impl_afw_object_release(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_object_t *entity;
    const afw_object_t *wrapped;

    /*
     * Unmanaged generic: pointer bag, zero is not an event.
     * Unmanaged face: extra holds; last extra hold walks overlay.
     */
    if (self->unmanaged) {
        if (!self->wrapped) {
            return;
        }
        if (self->reference_count <= 0) {
            return;
        }
        if (self->reference_count == 1) {
            self->reference_count = 0;
            /*
             * Overlay walk is a pool cleanup. Do not walk here: C-style
             * for clones the current scope and last-release of the
             * previous clone can drop this instance to zero while the
             * object is still in use.
             */
            wrapped = self->wrapped;
            if (wrapped) {
                afw_object_release(wrapped, xctx);
            }
            afw_pool_release(self->pub.p, xctx);
            return;
        }
        self->reference_count--;
        afw_pool_release(self->pub.p, xctx);
        return;
    }

    /*
     * If embedded object managed by parent, call parent's release and
     * return.
     */
    if (self->managed_by_entity) {
        AFW_OBJECT_GET_ENTITY(entity, &self->pub);
        afw_object_release(entity, xctx);
        return;
    }

    /*
     * Save wrapped before pool release: if this call destroys the face pool,
     * self is gone. Overlay walk is a pool cleanup for managed faces.
     * Drop the create-time pin only when the pool is destroyed
     * (afw_pool_release returns NULL).
     */
    wrapped = self->wrapped;
    if (afw_pool_release(self->pub.p, xctx) == NULL && wrapped) {
        afw_object_release(wrapped, xctx);
    }
}


/*
 * Implementation of method get_reference of interface afw_object.
 */
void
impl_afw_object_get_reference(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_object_t *entity;

    if (self->unmanaged) {
        if (!self->wrapped) {
            return;
        }
        self->reference_count++;
        if (self->reference_count == 1) {
            afw_object_get_reference(self->wrapped, xctx);
        }
        afw_pool_get_reference(self->pub.p, xctx);
        return;
    }

    /*
     * If embedded object managed by parent, call parent's get_reference
     * and return.
     */
    if (self->managed_by_entity) {
        AFW_OBJECT_GET_ENTITY(entity, &self->pub);
        afw_object_get_reference(entity, xctx);
        return;
    }

    /* Increment reference count of pool holding this managed object.  */
    afw_pool_get_reference(self->pub.p, xctx);
}

/*
 * Implementation of method get_count for interface afw_object.
 */
afw_size_t
impl_afw_object_get_count(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)&self->pub;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}


/*
 * Look up a local property only (no wrapped look-through, no promote).
 * Deleted entries (NULL value) are skipped for match purposes: a NULL value
 * marks a deleted local property that does not fall through to wrapped.
 */
static const afw_value_t *
impl_get_local_property(
    AFW_OBJECT_SELF_T *self,
    const afw_value_t *property_name,
    afw_boolean_t *found_local,
    afw_xctx_t *xctx)
{
    afw_object_internal_name_value_entry_t *e;

    *found_local = false;

    for (e = self->first_property; e; e = e->next) {
        if (afw_value_equal(e->name, property_name, xctx)) {
            *found_local = true;
            return e->value;
        }
    }
    return NULL;
}


/* True if name is present on the local list (including deleted). */
static afw_boolean_t
impl_has_local_property_name(
    AFW_OBJECT_SELF_T *self,
    const afw_value_t *property_name,
    afw_xctx_t *xctx)
{
    afw_boolean_t found_local;

    impl_get_local_property(self, property_name, &found_local, xctx);
    return found_local;
}


/*
 * If value is object or array and this wrapper is mutable, create a *new*
 * nested face over the instance as given, shadow it on self, and return it.
 * Always re-face (even if value is already a face) so callers do not share
 * nested faces across parents — issue #17 nested hard edge. Do not peel to
 * ultimate base (array faces may hold content only on the face ring).
 */
static const afw_value_t *
impl_promote_structured_from_base(
    AFW_OBJECT_SELF_T *self,
    const afw_value_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_object_t *nested_obj;
    const afw_array_t *nested_arr;
    const afw_object_t *wrap_obj;
    const afw_array_t *wrap_arr;

    if (!value || self->immutable) {
        return value;
    }

    if (afw_value_is_object(value)) {
        nested_obj = ((const afw_value_object_t *)value)->internal;
        if (!nested_obj) {
            return value;
        }
        wrap_obj = afw_object_create_wrapper_unmanaged(nested_obj,
            self->pub.p, xctx);
        afw_object_set_property((const afw_object_t *)self, property_name,
            wrap_obj->value, xctx);
        return wrap_obj->value;
    }

    if (afw_value_is_array(value)) {
        nested_arr = ((const afw_value_array_t *)value)->internal;
        if (!nested_arr) {
            return value;
        }
        wrap_arr = afw_array_create_wrapper_unmanaged(nested_arr,
            self->pub.p, xctx);
        afw_object_set_property((const afw_object_t *)self, property_name,
            wrap_arr->value, xctx);
        return wrap_arr->value;
    }

    return value;
}


/*
 * Implementation of method get_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property(
    AFW_OBJECT_SELF_T *self,
    const afw_value_t * property_name,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    afw_boolean_t found_local;

    /* Local properties shadow wrapped (including deleted → not found). */
    value = impl_get_local_property(self, property_name, &found_local, xctx);
    if (found_local) {
        return value;
    }

    /* Look through to wrapped base. */
    if (!self->wrapped) {
        return NULL;
    }

    value = afw_object_get_property(self->wrapped, property_name, xctx);
    if (!value) {
        return NULL;
    }

    /* Promote mutable nested objects/arrays onto this wrapper. */
    return impl_promote_structured_from_base(self, property_name, value,
        xctx);
}



/* Iterator when self->wrapped is set: local first, then wrapped (skip local). */
typedef struct {
    AFW_OBJECT_SELF_T *self;
    afw_boolean_t on_wrapped;
    afw_object_internal_name_value_entry_t *local_e;
    const afw_iterator_old_t *wrapped_iterator;
} impl_memory_wrapped_iterator_t;


/*
 * Implementation of method get_next_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property(
    AFW_OBJECT_SELF_T *self,
    const afw_iterator_old_t * * iterator,
    const afw_value_t * * property_name,
    afw_xctx_t *xctx)
{
    afw_object_internal_name_value_entry_t *e;
    impl_memory_wrapped_iterator_t *wit;
    const afw_value_t *value;
    const afw_value_t *name;

    /* Fast path: no wrapped base — original list walk. */
    if (!self->wrapped) {
        if (!*iterator) {
            e = self->first_property;
        }
        else {
            e = (afw_object_internal_name_value_entry_t *)*iterator;
            e = e->next;
        }

        for (; e && !e->value; e = e->next);

        *iterator = (afw_iterator_old_t *)e;

        if (!e) {
            if (property_name) {
                *property_name = NULL;
            }
            return NULL;
        }

        if (property_name) {
            *property_name = e->name;
        }
        return e->value;
    }

    /* Wrapper path: allocate iterator state on first call. */
    if (!*iterator) {
        wit = afw_pool_calloc_type(self->pub.p,
            impl_memory_wrapped_iterator_t, xctx);
        wit->self = self;
        wit->on_wrapped = false;
        wit->local_e = self->first_property;
        wit->wrapped_iterator = NULL;
        *iterator = (const afw_iterator_old_t *)wit;
    }
    else {
        wit = (impl_memory_wrapped_iterator_t *)*iterator;
    }

    /* Local properties first (skip deleted). */
    if (!wit->on_wrapped) {
        e = wit->local_e;
        for (; e && !e->value; e = e->next);
        if (e) {
            wit->local_e = e->next;
            if (property_name) {
                *property_name = e->name;
            }
            return e->value;
        }
        wit->on_wrapped = true;
        wit->wrapped_iterator = NULL;
    }

    /* Then wrapped names not present locally; promote mutable objects. */
    for (;;) {
        name = NULL;
        value = afw_object_get_next_property(self->wrapped,
            &wit->wrapped_iterator, &name, xctx);
        if (!value) {
            *iterator = NULL;
            if (property_name) {
                *property_name = NULL;
            }
            return NULL;
        }

        if (impl_has_local_property_name(self, name, xctx)) {
            continue;
        }

        value = impl_promote_structured_from_base(self, name, value, xctx);
        if (property_name) {
            *property_name = name;
        }
        return value;
    }
}



/*
 * Implementation of method has_property of interface afw_object.
 */
afw_boolean_t
impl_afw_object_has_property(
    AFW_OBJECT_SELF_T *self,
    const afw_value_t * property_name,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    afw_boolean_t found_local;

    /* Do not promote on has — only test presence. */
    value = impl_get_local_property(self, property_name, &found_local, xctx);
    if (found_local) {
        return (value) ? AFW_TRUE : AFW_FALSE;
    }

    if (self->wrapped) {
        return afw_object_has_property(self->wrapped, property_name, xctx);
    }

    return AFW_FALSE;
}



/*
 * Implementation of method get_setter of interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter (
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Assign &self->pub pointer to self. */

    return (self->immutable) ? NULL : &self->setter;
}



/*
 * Implementation of method set_immutable of interface afw_object_setter.
 */
void
impl_afw_object_setter_set_immutable (
    const afw_object_setter_t * self,
    afw_xctx_t *xctx)
{
    /* Set self to object associated with setter. */
    afw_object_internal_memory_object_t * memory_object_self = 
        (afw_object_internal_memory_object_t *)self->object;

    /* Set object to immutable. */
    memory_object_self->immutable = true;
}


/*
 * Implementation of method set_property of interface afw_object_setter.
 */
void
impl_afw_object_setter_set_property(
    const afw_object_setter_t * self,
    const afw_value_t * property_name,
    const afw_value_t * value,
    afw_xctx_t *xctx)
{
    /* Set self to object associated with setter. */
    afw_object_internal_memory_object_t *memory_object_self =
        (afw_object_internal_memory_object_t *)self->object;
    afw_object_internal_name_value_entry_t *e;
    afw_object_internal_name_value_entry_t *final_e;

    do { if (memory_object_self->immutable) { AFW_OBJECT_ERROR_OBJECT_IMMUTABLE; } } while (0);

    for (e = memory_object_self->first_property,final_e = NULL; e; e = e->next) {
        final_e = e;
        if (afw_value_equal(e->name, property_name, xctx)) {
            /*
             * Overlay hold only on look-through faces. Generic memory
             * objects still store the pointer (adapter/runtime/snapshots).
             */
            if (memory_object_self->wrapped) {
                if (!value) {
                    afw_value_release(e->value, xctx);
                    e->value = NULL;
                }
                else {
                    afw_value_slot_store(&e->value, value,
                        xctx->p, xctx);
                }
            }
            else {
                e->value = value;
            }
            return;
        }
    }
    /*
     * No local entry. Delete of a missing name is a no-op on a plain memory
     * object. On a look-through face, allocate a NULL local tombstone so
     * get/has/iterate do not revive the wrapped base.
     */
    if (!value && !memory_object_self->wrapped) {
        return;
    }
    e = afw_pool_calloc_type(self->object->p,
        afw_object_internal_name_value_entry_t, xctx);
    /*
     * Unmanaged string names may be stack (AFW_VALUE_STRING_UNMANAGED) or
     * another pool. Copy the header into this object pool so get-promote
     * cannot keep a pointer below the caller's frame.
     */
    if (!property_name) {
        property_name = afw_v_a_empty_string;
    }
    else if (property_name->inf == &afw_value_unmanaged_string_inf) {
        property_name = afw_value_create_unmanaged_string(
            &((const afw_value_string_t *)property_name)->internal,
            self->object->p, xctx);
    }
    e->name = property_name;
    e->value = NULL;
    if (value) {
        if (memory_object_self->wrapped) {
            afw_value_slot_store(&e->value, value, xctx->p, xctx);
        }
        else {
            e->value = value;
        }
    }
    if (final_e) {
        final_e->next = e;
    }
    else {
        memory_object_self->first_property = e;
    }
}
