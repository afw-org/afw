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


AFW_DEFINE(const afw_object_t *)
afw_object_create_with_options(
    int options, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_object_internal_memory_object_t *self;

    /* If managed, create subpool for object. */
    if (options == AFW_OBJECT_MEMORY_OPTION_managed)
    {
        p = afw_pool_create_subpool(p, xctx);
    }

    /* Allocate memory for self and initialize. */
    self = afw_pool_calloc_type(p, afw_object_internal_memory_object_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->unmanaged = AFW_OBJECT_MEMORY_OPTION_IS(options, unmanaged);
    /*
     * Dual face: value.internal is this instance. Inf matches object
     * lifetime — managed face when object RC owns a subpool; unmanaged
     * when create options say unmanaged (pool bulk free only).
     */
    self->value.inf = self->unmanaged
        ? &afw_value_unmanaged_object_inf
        : &afw_value_managed_object_inf;
    self->value.internal = (const afw_object_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    //FIXME self->clone_on_set = AFW_OBJECT_MEMORY_OPTION_IS(options, clone_on_set);
    self->setter.inf = &impl_afw_object_setter_inf;
    self->setter.object = (const afw_object_t *)self;

    /* Return new object. */
    return (const afw_object_t *)self;
}


AFW_DEFINE(const afw_object_t *)
afw_object_create_embedded(
    const afw_object_t *embedding_object,
    const afw_utf8_t *property_name,
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
    self->pub.meta.id = property_name;
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
    const afw_utf8_t *property_name,
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



/*
 * Implementation of method release of interface afw_object.
 */
void
impl_afw_object_release(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_object_t *entity;

    /* If unmanaged, just return. */
    if (self->unmanaged) {
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

    /* Release pool holding managed object. */
    afw_pool_release(self->pub.p, xctx);
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

    /* If unmanaged, just return. */
    if (self->unmanaged) return;

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
 * Implementation of method get_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property(
    AFW_OBJECT_SELF_T *self,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    /** @fixme Add parent support. */

    const afw_value_t *value;
    afw_object_internal_name_value_entry_t *e;

    value = NULL;

    /* Search property list. */
    for (value = NULL, e = self->first_property; e; e = e->next) {
        if (afw_utf8_equal(e->name, property_name)) {
            value = e->value;
            break;
        }
    }

    /* Return value. */
    return value;
}



/*
 * Implementation of method get_next_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property(
    AFW_OBJECT_SELF_T *self,
    const afw_iterator_t * * iterator,
    const afw_utf8_t * * property_name,
    afw_xctx_t *xctx)
{
    afw_object_internal_name_value_entry_t *e;

    /* If iterator is NULL, get first. */
    if (!*iterator) {
        e = self->first_property;
    }

    /* If iterator is not NULL, get next. */
    else {
        e = (afw_object_internal_name_value_entry_t *)*iterator;
        e = e->next;
    }

    /* Skip any deleted properties. */
    for (; e && !e->value; e = e->next);

    /* Set iterator to entry. */
    *iterator = (afw_iterator_t *)e;

    /* If e NULL, return not found. */
    if (!e)
    {
        if (property_name) {
            *property_name = NULL;
        }
        return NULL;
    }

    /* Return entries property name and value. */
    if (property_name) {
        *property_name = e->name;
    }
    return e->value;
}



/*
 * Implementation of method has_property of interface afw_object.
 */
afw_boolean_t
impl_afw_object_has_property(
    AFW_OBJECT_SELF_T *self,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    /* Search for property.  If found, set exists to true. */
    value = impl_afw_object_get_property(self, property_name, xctx);

    /* Return true or false. */
    return (value) ? AFW_TRUE : AFW_FALSE;
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
    const afw_utf8_t * property_name,
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
        if (afw_utf8_equal(e->name, property_name)) {
            e->value = value;
            return;
        }
    }
    if (!value) {
        return; /* Delete property just return. */
    }
    e = afw_pool_calloc_type(self->object->p,
        afw_object_internal_name_value_entry_t, xctx);
    e->name = (property_name) ? property_name : afw_s_a_empty_string;

    /** @fixme
     * Need to think about setting a property that is an object.  Embedding
     * object and property name needs to be correct, plus path and related
     * meta needs to be clear.  Might involve clone, but
     * afw_object_create_embedded() also calls this, so if embedded
     * object is this self, no clone needed.  Also consider that clone
     * might not should be used since the one setting property might want
     * to add properties after doing the set.
     */
    e->value = (memory_object_self->clone_on_set)
        ? afw_value_clone(value, memory_object_self->pub.p, xctx)
        : value;
    if (final_e) {
        final_e->next = e;
    }
    else {
        memory_object_self->first_property = e;
    }
}
