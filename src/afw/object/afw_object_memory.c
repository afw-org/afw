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

/* Managed bag: separate inf, no extra ifs on set/release. */
static void
impl_afw_object_managed_release(
    AFW_OBJECT_SELF_T *self, afw_xctx_t *xctx);
static void
impl_afw_object_managed_get_reference(
    AFW_OBJECT_SELF_T *self, afw_xctx_t *xctx);
static void
impl_afw_object_managed_setter_set_property(
    const afw_object_setter_t *self,
    const afw_value_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx);

#undef AFW_IMPLEMENTATION_ID
#define AFW_IMPLEMENTATION_ID "memory_managed"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_object_managed_inf
#define AFW_OBJECT_INF_ONLY
#define impl_afw_object_release impl_afw_object_managed_release
#define impl_afw_object_get_reference impl_afw_object_managed_get_reference
#include "afw_object_impl_declares.h"
#undef AFW_OBJECT_INF_ONLY
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_object_release
#undef impl_afw_object_get_reference
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_object_managed_setter_inf
#define AFW_OBJECT_SETTER_INF_ONLY
#define impl_afw_object_setter_set_property \
    impl_afw_object_managed_setter_set_property
#include "afw_object_setter_impl_declares.h"
#undef AFW_OBJECT_SETTER_INF_ONLY
#undef impl_afw_object_setter_set_property
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_ID

static void
impl_managed_face_overlay_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx);


AFW_DEFINE(const afw_object_t *)
afw_object_create_with_options(
    int options, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_object_internal_memory_object_t *self;

    /* If new_p, own pool is a child of p->managed_p. */
    if (AFW_OBJECT_MEMORY_OPTION_IS(options, new_p))
    {
        p = afw_pool_create(p->managed_p, xctx);
    }

    /* Allocate memory for self and initialize. */
    self = afw_pool_calloc_type(p, afw_object_internal_memory_object_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    /* Live in p unless new_p or cede_p (object still owns that pool). */
    self->unmanaged =
        !AFW_OBJECT_MEMORY_OPTION_IS(options, new_p) &&
        !AFW_OBJECT_MEMORY_OPTION_IS(options, cede_p);
    /* Pool-world dual face is always unmanaged (value get_reference throws). */
    self->value.inf = &afw_value_unmanaged_object_inf;
    self->value.internal = (const afw_object_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    /* clone_on_set: residual field; always false (no public option). */
    self->setter.inf = &impl_afw_object_setter_inf;
    self->setter.object = (const afw_object_t *)self;
    /* self->wrapped is NULL (calloc). */

    /* Return new object. */
    return (const afw_object_t *)self;
}


AFW_DEFINE(const afw_object_t *)
afw_object_create_managed(
    afw_xctx_t *xctx)
{
    afw_object_internal_memory_object_t *self;
    const afw_pool_t *p;

    p = xctx->p;
    self = afw_pool_calloc_type(p,
        afw_object_internal_memory_object_t, xctx);
    self->pub.inf = &impl_afw_object_managed_inf;
    self->pub.p = p;
    self->value.inf = &afw_value_managed_object_inf;
    self->value.internal = (const afw_object_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    self->setter.inf = &impl_afw_object_managed_setter_inf;
    self->setter.object = (const afw_object_t *)self;
    self->reference_count = 1;
    return (const afw_object_t *)self;
}


static void
impl_copy_into_managed(
    const afw_object_t *to,
    const afw_object_t *from,
    afw_xctx_t *xctx);

static afw_boolean_t
impl_is_meta_object(const afw_object_t *o)
{
    if (!o) {
        return false;
    }
    if (o->meta.object_type_uri &&
        afw_utf8_equal(o->meta.object_type_uri, afw_s__AdaptiveMeta_))
    {
        return true;
    }
    if (o->meta.embedding_object && o->meta.id &&
        afw_utf8_equal(o->meta.id, afw_s_a_meta_key))
    {
        return true;
    }
    return false;
}


/* True if from is a name/value memory bag we can walk via first_property. */
static afw_boolean_t
impl_is_memory_property_list(const afw_object_t *o)
{
    if (!o || impl_is_meta_object(o) ||
        afw_object_is_memory_wrapper(o))
    {
        return false;
    }
    return o->inf == &impl_afw_object_inf ||
        o->inf == &impl_afw_object_managed_inf;
}


static const afw_object_t *
impl_meta_delta_bag(const afw_object_t *from)
{
    const afw_object_t *meta;

    meta = from->meta.meta_object;
    if (!meta) {
        return NULL;
    }
    if (meta->inf &&
        afw_utf8_equal_utf8_z(&meta->inf->rti.implementation_id,
            "object_meta"))
    {
        return ((const afw_object_meta_object_t *)meta)->delta;
    }
    return meta;
}


static void
impl_copy_meta_delta_into_managed(
    const afw_object_t *to,
    const afw_object_t *from,
    afw_xctx_t *xctx)
{
    const afw_object_t *src;
    const afw_object_t *delta;
    const afw_iterator_old_t *iterator;
    const afw_value_t *name;
    const afw_value_t *value;
    const afw_value_t *cloned;

    src = impl_meta_delta_bag(from);
    if (!src) {
        return;
    }
    delta = afw_object_meta_get_nonempty_delta(to, xctx);
    iterator = NULL;
    for (;;) {
        name = NULL;
        value = afw_object_get_next_property(src, &iterator, &name,
            xctx);
        if (!value) {
            break;
        }
        cloned = afw_value_clone_unmanaged(value, to->p, xctx);
        afw_object_set_property(delta,
            name ? afw_value_clone_unmanaged(name, to->p, xctx)
                : afw_v_a_empty_string,
            cloned, xctx);
    }
}


static void
impl_copy_property_into_managed(
    const afw_object_t *to,
    const afw_value_t *name,
    const afw_value_t *prop,
    afw_xctx_t *xctx)
{
    const afw_object_t *from_obj;
    const afw_object_t *nested;
    const afw_array_t *from_array;
    const afw_array_t *cloned_array;

    if (!prop) {
        return;
    }
    if (afw_value_is_object(prop)) {
        from_obj = ((const afw_value_object_t *)prop)->internal;
        if (!from_obj) {
            return;
        }
        if (from_obj->inf == &impl_afw_object_managed_inf) {
            afw_object_set_property(to, name, prop, xctx);
            return;
        }
        nested = afw_object_create_managed_embedded(to, name, xctx);
        impl_copy_into_managed(nested, from_obj, xctx);
        return;
    }
    if (afw_value_is_array(prop)) {
        from_array = ((const afw_value_array_t *)prop)->internal;
        if (!from_array) {
            return;
        }
        cloned_array = afw_array_create_managed_from(from_array, xctx);
        afw_object_set_property(to, name, cloned_array->value, xctx);
        afw_array_release(cloned_array, xctx);
        return;
    }
    afw_object_set_property(to, name, prop, xctx);
}


static void
impl_copy_into_managed(
    const afw_object_t *to,
    const afw_object_t *from,
    afw_xctx_t *xctx)
{
    afw_object_t *self = (afw_object_t *)to;
    const afw_pool_t *p = xctx->p;
    const afw_object_internal_memory_object_t *from_mem;
    afw_object_internal_name_value_entry_t *e;
    afw_object_internal_name_value_entry_t *next;
    const afw_iterator_old_t *iterator;
    const afw_value_t *name;
    const afw_value_t *value;

    if (!self->meta.object_uri && from->meta.object_uri &&
        !self->meta.embedding_object)
    {
        self->meta.object_uri = afw_utf8_clone(from->meta.object_uri,
            p, xctx);
    }
    if (!self->meta.id && from->meta.id) {
        self->meta.id = afw_utf8_clone(from->meta.id, p, xctx);
    }
    if (!self->meta.object_type_uri && from->meta.object_type_uri) {
        self->meta.object_type_uri = afw_utf8_clone(
            from->meta.object_type_uri, p, xctx);
    }
    impl_copy_meta_delta_into_managed(to, from, xctx);

    if (impl_is_memory_property_list(from)) {
        from_mem = (const afw_object_internal_memory_object_t *)from;
        for (e = from_mem->first_property; e; e = next) {
            next = e->next;
            /* Names are values. A non-string name ends the walk
             * (qualifier() snapshots have grown a garbage tail). */
            if (!e->name || !afw_value_is_string(e->name)) {
                break;
            }
            if (e->value) {
                impl_copy_property_into_managed(to, e->name, e->value,
                    xctx);
            }
        }
        return;
    }

    iterator = NULL;
    for (;;) {
        name = NULL;
        value = afw_object_get_next_property(from, &iterator, &name,
            xctx);
        if (!value) {
            break;
        }
        impl_copy_property_into_managed(to, name, value, xctx);
    }
}


AFW_DEFINE(const afw_object_t *)
afw_object_create_managed_from(
    const afw_object_t *from,
    afw_xctx_t *xctx)
{
    const afw_object_t *to;

    if (!from) {
        AFW_THROW_ERROR_Z(general,
            "afw_object_create_managed_from requires from",
            xctx);
    }
    if (from->inf == &impl_afw_object_managed_inf) {
        afw_object_get_reference(from, xctx);
        return from;
    }
    to = afw_object_create_managed(xctx);
    impl_copy_into_managed(to, from, xctx);
    return to;
}


AFW_DEFINE(const afw_object_t *)
afw_object_create_managed_embedded(
    const afw_object_t *embedding_object,
    const afw_value_t *property_name,
    afw_xctx_t *xctx)
{
    afw_object_internal_memory_object_t *self;
    const afw_utf8_t *id;
    const afw_object_t *pub;

    if (!embedding_object ||
        embedding_object->inf != &impl_afw_object_managed_inf)
    {
        AFW_THROW_ERROR_Z(general,
            "Embedding object is not a managed memory object",
            xctx);
    }
    self = afw_pool_calloc_type(xctx->p,
        afw_object_internal_memory_object_t, xctx);
    self->pub.inf = &impl_afw_object_managed_inf;
    self->pub.p = xctx->p;
    self->value.inf = &afw_value_managed_object_inf;
    self->value.internal = (const afw_object_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    self->setter.inf = &impl_afw_object_managed_setter_inf;
    self->setter.object = (const afw_object_t *)self;
    self->pub.meta.embedding_object = embedding_object;
    id = afw_object_string_property_name_as_utf8(property_name, xctx);
    if (id) {
        self->pub.meta.id = afw_utf8_clone(id, xctx->p, xctx);
    }
    self->reference_count = 1;
    pub = (const afw_object_t *)self;
    afw_object_set_property(embedding_object, property_name,
        pub->value, xctx);
    /* Slot holds; drop the create-time extra hold. */
    afw_object_release(pub, xctx);
    return pub;
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
    /* Face holds the bag the same way for in_pool / and_pool / permanent. */
    afw_object_get_reference(wrapped, xctx);
    afw_pool_register_cleanup_before(self->pub.p, self, NULL,
        impl_managed_face_overlay_cleanup, xctx);
    /*
     * Carry meta (path, objectId, reconcilable, …) onto the face so
     * meta(face) matches the adapter entity. Property gets still look
     * through to wrapped for bag content.
     */
    afw_object_meta_clone_and_set((const afw_object_t *)self, wrapped, xctx);
    self->value.inf = &afw_value_assignable_object_inf;
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


AFW_DEFINE(afw_boolean_t)
afw_object_is_memory_managed(const afw_object_t *object)
{
    return object && object->inf == &impl_afw_object_managed_inf;
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
    self->value.inf = &afw_value_unmanaged_object_inf;
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
     * Unmanaged generic (in_pool bag): extra holds keep the pool/frame
     * alive. Unmanaged face: extra holds; last extra hold walks overlay.
     */
    if (self->unmanaged) {
        if (!self->wrapped) {
            if (self->reference_count <= 0) {
                return;
            }
            self->reference_count--;
            if (self->pub.p) {
                afw_pool_release(self->pub.p, xctx);
            }
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
     * new_p / cede_p: instance release is pool_release of object->p.
     * If the pool dies, drop the create-time pin on wrapped.
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
            self->reference_count++;
            if (self->pub.p) {
                afw_pool_get_reference(self->pub.p, xctx);
            }
            return;
        }
        self->reference_count++;
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

    /* new_p / cede_p: pin the pool. Value inf still throws. */
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


/* Overlay store is clone_or_reference (GET-cache on this face). */
static const afw_value_t *
impl_hold_from_base(
    AFW_OBJECT_SELF_T *self,
    const afw_value_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_boolean_t found_local;
    const afw_value_t *local;

    if (!value || self->immutable) {
        return value;
    }

    /* Overlay slot_store is clone_or_reference. */
    afw_object_set_property((const afw_object_t *)self, property_name,
        value, xctx);
    local = impl_get_local_property(self, property_name, &found_local, xctx);
    return found_local && local ? local : value;
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

    /* Hold looked-up value on this face (clone_or_reference via set). */
    return impl_hold_from_base(self, property_name, value, xctx);
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

        value = impl_hold_from_base(self, name, value, xctx);
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
     * Copy name octets into this object pool. create_unmanaged_string
     * only copies the utf8 header; .s must not point at a parked
     * managed return-temp that pop_value will free.
     */
    if (!property_name) {
        property_name = afw_v_a_empty_string;
    }
    else if (afw_value_is_string(property_name) &&
        property_name != afw_v_a_empty_string)
    {
        const afw_utf8_t *copied;

        copied = afw_utf8_create(
            ((const afw_value_string_t *)property_name)->internal.s,
            ((const afw_value_string_t *)property_name)->internal.len,
            self->object->p, xctx);
        property_name = afw_value_create_unmanaged_string(
            copied, self->object->p, xctx);
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


void
impl_afw_object_managed_release(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_object_internal_name_value_entry_t *e;

    if (self->reference_count <= 0) {
        return;
    }
    self->reference_count--;
    if (self->reference_count != 0) {
        return;
    }
    for (e = self->first_property; e; e = e->next) {
        if (e->value) {
            afw_value_release(e->value, xctx);
            e->value = NULL;
        }
        if (e->name) {
            afw_value_release(e->name, xctx);
            e->name = NULL;
        }
    }
    afw_pool_free_memory(xctx->p, self,
        sizeof(afw_object_internal_memory_object_t), xctx);
}


void
impl_afw_object_managed_get_reference(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;
    self->reference_count++;
}


void
impl_afw_object_managed_setter_set_property(
    const afw_object_setter_t *self,
    const afw_value_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_object_internal_memory_object_t *memory_object_self =
        (afw_object_internal_memory_object_t *)self->object;
    afw_object_internal_name_value_entry_t *e;
    afw_object_internal_name_value_entry_t *final_e;

    do { if (memory_object_self->immutable) { AFW_OBJECT_ERROR_OBJECT_IMMUTABLE; } } while (0);

    for (e = memory_object_self->first_property, final_e = NULL;
        e; e = e->next)
    {
        final_e = e;
        if (afw_value_equal(e->name, property_name, xctx)) {
            if (!value) {
                afw_value_release(e->value, xctx);
                e->value = NULL;
            }
            else {
                afw_value_slot_store(&e->value, value,
                    xctx->p, xctx);
            }
            return;
        }
    }
    if (!value) {
        return;
    }
    e = afw_pool_calloc_type(xctx->p,
        afw_object_internal_name_value_entry_t, xctx);
    if (!property_name) {
        property_name = afw_v_a_empty_string;
    }
    /* New entry: name is isolated once. Replace never changes the name. */
    e->name = afw_value_as_assignable(property_name, xctx->p, xctx);
    afw_value_slot_store(&e->value, value, xctx->p, xctx);
    if (final_e) {
        final_e->next = e;
    }
    else {
        memory_object_self->first_property = e;
    }
}
