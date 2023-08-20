// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Value Compiler Listing
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_value_compiler_listing.c
 * @brief Adaptive Framework value decompile to compiler listing
 */

#include "afw_internal.h"



#define AFW_IMPLEMENTATION_ID "afw_value_compiler_listing_writer"
#include "afw_writer_impl_declares.h"

static const afw_utf8_octet_t
impl_empty_prefix[] =
"                                          + ";

static const afw_utf8_t
impl_default_tab = AFW_UTF8_LITERAL("    ");


/* Frame symbol types. */
static const afw_utf8_t *
impl_symbol_type_names[] = {
#define XX(name, description) &afw_s_ ## name,
    AFW_VALUE_BLOCK_SYMBOL_TYPE(XX)
#undef XX
    NULL
};



static afw_size_t
impl_digits_needed(afw_size_t i)
{
    afw_size_t result;

    for (result = 1; i > 9; i /= 10) {
        result++;
    }

    return result;
}



/*
 * Implementation of method write_eol for interface afw_writer.
 */
static afw_size_t
impl_afw_writer_write_raw_cb(
    void *context,
    const void *buffer,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_t *self =
        (afw_value_compiler_listing_t *)context;
    afw_size_t i;

    for (i = 0; i < size; i++) {
        APR_ARRAY_PUSH(self->ary, char) =
            ((const afw_octet_t *)buffer)[i];
    }
    return size;
}



/*
 * Implementation of method release for interface afw_writer.
 */
void
impl_afw_writer_release(
    const afw_writer_t *instance,
    afw_xctx_t *xctx)
{
    /* Don't do anything. */
}



/*
 * Implementation of method flush for interface afw_writer.
 */
void
impl_afw_writer_flush(
    const afw_writer_t *instance,
    afw_xctx_t *xctx)
{
    /* Ignore flush. */
}



static afw_boolean_t
impl_write_source_line(
    afw_value_compiler_listing_t *self,
    afw_boolean_t line_written,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *src;
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *end;
    char buffer[sizeof(impl_empty_prefix)];
    int len, i;

    src = self->compiled_value->full_source;

    if (self->source_eof || src->len < self->cursor_written) {
        return false;
    }
    self->last_line_written++;

    if (!line_written && self->empty_line_between_switch) {
        APR_ARRAY_PUSH(self->ary, char) = '\n';
    }

    /* Write prefix */
    len = snprintf(buffer, sizeof(buffer),
        AFW_SIZE_T_FMT,
        self->last_line_written);
    len = (int)self->prefix_size - len - 3;
    len = snprintf(buffer, sizeof(buffer),
        "%.*s" AFW_SIZE_T_FMT " | ",
        len,
        &impl_empty_prefix[0],
        self->last_line_written);
    for (i = 0; i < len; i++) {
        APR_ARRAY_PUSH(self->ary, char) = buffer[i];
    }

    /* Write a line. */
    for (
        s = src->s + self->cursor_written,
        end = src->s + src->len;
        ;
        s++)
    {
        /* If not at end, push char. If it's a new line, break. */
        if (s < end) {
            APR_ARRAY_PUSH(self->ary, char) = *s;
            if (*s == '\n') {
                s++;
                if (s == end) {
                    self->source_eof = true;
                }
                break;
            }
        }

        /* If no trailing new line, add one and break. */
        else {
            APR_ARRAY_PUSH(self->ary, char) = '\n';
            self->source_eof = true;
            break;
        }

    }

    /* Set cursor written. */
    self->cursor_written = s - src->s;
    return !self->source_eof;
}


/*
 * Implementation of method write for interface afw_writer.
 */
void
impl_afw_writer_write(
    const afw_writer_t *instance,
    const void *buffer,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_t *self =
        (afw_value_compiler_listing_t *)instance;
    afw_size_t count, i;
    const afw_octet_t *c;
    afw_boolean_t line_written;

    if (size == 0) {
        return;
    }

    /*
     * In this is a new line and the line does not starts with '#' or '---' or
     * is an empty line, deal with prefix and tabs.
     */
    if (self->is_new_line &&
        !(
            (size >= 1 && *(char *)buffer == '#') ||
            (size >= 1 && *(char *)buffer == '\n') ||
            (size >= 3 && (memcmp(buffer, "---", 3) == 0))
        ))
    {
        if (self->include_source) {
            for (line_written = false;
                self->max_value_cursor >= self->cursor_written;
                )
            {
                if (!impl_write_source_line(self, line_written, xctx)) {
                    break;
                }
                line_written = true;
            }
            if (self->empty_line_between_switch && line_written) {
                APR_ARRAY_PUSH(self->ary, char) = '\n';
            }
        }
        if (!self->current_prefix) {
            self->current_prefix = &self->empty_prefix;
        }
        for (i = 0; i < self->current_prefix->len; i++) {
            APR_ARRAY_PUSH(self->ary, char) = self->current_prefix->s[i];
        }
        self->current_prefix = NULL;
        for (count = 0; count < self->writer.indent; count++) {
            for (i = 0; i < self->writer.tab->len; i++) {
                APR_ARRAY_PUSH(self->ary, char) = self->writer.tab->s[i];
            }
        }
    }

    /*
     * In the case that current prefix is supplied but no newline, just set to
     * NULL.
     */
    self->current_prefix = NULL;

    /* Always reset is_new_line and push buffer on to stack. */
    self->is_new_line = false;
    c = (const afw_octet_t *)buffer;
    for (count = 0; count < size; count++) {
        APR_ARRAY_PUSH(self->ary, char) = *c++;
    }
}


/*
 * Implementation of method write_eol for interface afw_writer.
 */
void
impl_afw_writer_write_eol(
    const afw_writer_t *instance,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_t *self =
        (afw_value_compiler_listing_t *)instance;

    if (self->writer.tab) {
        APR_ARRAY_PUSH(self->ary, char) = '\n';
        self->is_new_line = true;
    }
}

/*
 * Implementation of method increment_indent for interface afw_writer.
 */
void
impl_afw_writer_increment_indent(
    const afw_writer_t *instance,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_t *self =
        (afw_value_compiler_listing_t *)instance;

    self->writer.indent++;
}

/*
 * Implementation of method decrement_indent for interface afw_writer.
 */
void
impl_afw_writer_decrement_indent(
    const afw_writer_t *instance,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_t *self =
        (afw_value_compiler_listing_t *)instance;

    if (self->writer.indent == 0) {
        AFW_THROW_ERROR_Z(general,
            "decrement indent when indent already 0",
            xctx);
    }

    self->writer.indent--;
}



AFW_DEFINE_INTERNAL(const afw_utf8_t *)
afw_value_compiler_listing_symbol_type_name(
    afw_value_block_symbol_type_t type)
{
    if (type < 0 || type >= afw_value_block_symbol_type_count) {
        type = 0;
    }
    return impl_symbol_type_names[type];
}



AFW_DEFINE_INTERNAL(const afw_utf8_t *)
afw_value_compiler_listing_for_child(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_t *parent;
    afw_value_compiler_listing_t *self;

    if (writer->inf != &impl_afw_writer_inf) {
        AFW_THROW_ERROR_Z(general,
            "afw_value_compiler_listing_for_child() can only be called by"
            "afw_value_compiler_listing implementation of writer",
            xctx);
    }
    parent = (afw_value_compiler_listing_t *)writer;

    self = afw_value_compiler_listing_to_string_instance(
        instance, parent, parent->writer.tab, parent->p, xctx);

    return self->reference_id;
}



static void
impl_symbol_listing(
    const afw_writer_t *writer,
    afw_value_block_t *block,
    afw_xctx_t *xctx)
{
    afw_value_block_symbol_t *e;
    afw_value_block_t *child;

    if (!block) {
        afw_writer_write_z(writer, "\nblock=NULL", xctx);
        return;
    }

    afw_writer_write_z(writer, "\nblock=", xctx);
    afw_writer_write_size(writer, block->number, xctx);
    afw_writer_write_z(writer, " depth=", xctx);
    afw_writer_write_size(writer, block->depth, xctx);
    afw_writer_write_z(writer, " entries=", xctx);
    afw_writer_write_size(writer, block->symbol_count, xctx);
    afw_writer_write_z(writer, " : [", xctx);
    afw_writer_write_eol(writer, xctx);

    for (e = block->first_entry; e; e = e->next_entry) {
        afw_writer_write_z(writer, "    [", xctx);
        afw_writer_write_size(writer, e->index, xctx);
        afw_writer_write_z(writer, "] ", xctx);
        afw_writer_write_utf8(writer,
            afw_value_compiler_listing_symbol_type_name(e->symbol_type),
            xctx);
        afw_writer_write_z(writer, " ", xctx);
        afw_value_compiler_listing_name_and_type(writer,
            e->name, &e->type, xctx);
        afw_writer_write_eol(writer, xctx);
    }
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);

    for (child = block->first_child_block;
        child;
        child = child->next_sibling_block)
    {
        impl_symbol_listing(writer, child, xctx);
    }
}



/* Decompile a value to a compiler listing string. */
AFW_DEFINE_INTERNAL(afw_value_compiler_listing_t *)
afw_value_compiler_listing_to_string_instance(
    const afw_value_t *value,
    afw_value_compiler_listing_t *parent,
    const afw_utf8_t *tab,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_t *self;
    const afw_writer_t *writer;
    afw_value_info_t info;

    /* Create self and ary. */
    self = afw_pool_calloc_type(p,
        afw_value_compiler_listing_t,
        xctx);
    writer = &self->writer;
    self->writer.inf = &impl_afw_writer_inf;
    self->writer.p = p;
    self->p = p;
    self->writer.write_raw_cb = impl_afw_writer_write_raw_cb;
    self->ary = apr_array_make(afw_pool_get_apr_pool(p), 4000, 1);
    self->writer.tab = tab;
    if (tab->len == 1 && *(tab->s) == '\t') {
        self->writer.tab = &impl_default_tab;
    }
    self->tab_size = (int)tab->len;

    /* Chain in self to parent and siblings. */
    if (parent) {
        self->parent = parent;
        if (parent->final_child) {
            parent->final_child->next_sibling = self;
            parent->final_child = self;
        }
        else {
            parent->first_child = self;
            parent->final_child = self;
        }
    }

    /* Write header section. */
    afw_writer_write_z(writer, "\n", xctx);
    afw_writer_write_z(writer, "\n", xctx);
    afw_writer_write_z(writer, "#", xctx);
    afw_writer_write_eol(writer, xctx);

    /* If value is not a compiled value, just produce decompile. */
    if (afw_value_is_undefined_or_evaluated(value)) {
        afw_writer_write_z(writer,
            "#  Adaptive Value is evaluated at compile time", xctx);
        afw_writer_write_eol(writer, xctx);
        afw_writer_write_z(writer, "#", xctx);
        afw_writer_write_eol(writer, xctx);
        if (afw_value_is_undefined(value)) {
            afw_writer_write_utf8(writer, &afw_s_undefined, xctx);
        }
        else {
            afw_value_decompile(value, writer, xctx);
        }
        afw_writer_write_eol(writer, xctx);
        return self;
    }

    /* Set self->compiled_value */
    AFW_VALUE_ASSERT_IS(value, compiled_value, xctx);
    self->compiled_value = (const afw_value_compiled_value_t *)value;
    self->reference_id = self->compiled_value->reference_id;

    /** @todo make these an option */
    self->include_source = true;
    self->empty_line_between_switch = true;

    afw_writer_write_z(writer, "#  Compiled Adaptive Value Listing", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_write_z(writer, "#", xctx);
    afw_writer_write_eol(writer, xctx);
    if (self->compiled_value->source_location) {
        afw_writer_write_z(writer, "#  Source location: ", xctx);
        afw_writer_write_utf8(writer,
            self->compiled_value->source_location, xctx);
        afw_writer_write_eol(writer, xctx);
        afw_writer_write_z(writer, "#", xctx);
        afw_writer_write_eol(writer, xctx);
    }

    /* Set up prefix for --CompiledValue. */
    self->is_new_line = true;
    if (self->compiled_value->line_count == 0) {
        self->offset_only = true;
        self->prefix_format = AFW_SIZE_T_FMT "-" AFW_SIZE_T_FMT;
        self->prefix_size =
            impl_digits_needed(self->compiled_value->full_source->len) * 2 +
            4 /* '-' + space + '> '*/;
    }
    else {
        self->prefix_format = AFW_SIZE_T_FMT ":" AFW_SIZE_T_FMT
            "-" AFW_SIZE_T_FMT ":" AFW_SIZE_T_FMT;
        self->prefix_size =
            impl_digits_needed(self->compiled_value->line_count) * 2 +
            impl_digits_needed(self->compiled_value->longest_line) * 2 +
            6 /* 2*':' + '-' + space + '> '*/;
    }

    /* Make prefix size a multiple of tab size. */
    if (self->tab_size != 0) {
        self->prefix_size =
            (self->prefix_size + self->tab_size - 1) /
                self->tab_size * self->tab_size;
    }

    /*  Make sure this does not exceed limitation. */
    if (self->prefix_size > sizeof(impl_empty_prefix)) {
        AFW_THROW_ERROR_Z(general, "Limitation", xctx);
    }

    /* Make empty prefix. */
    self->empty_prefix.s = &impl_empty_prefix[
        sizeof(impl_empty_prefix) - self->prefix_size - 1];
    self->empty_prefix.len = self->prefix_size;

    /* Write --CompiledValue */
    afw_value_get_info(value, &info, writer->p, xctx);
    afw_writer_write_z(writer, "---CompiledValue ", xctx);
    afw_writer_write_utf8(writer, self->reference_id, xctx);
    afw_writer_write_eol(writer, xctx);
    afw_value_compiler_listing_begin_value(writer, value, NULL, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);

    /* Adaptive Value Decompile. */
    afw_writer_increment_indent(writer, xctx);
    afw_value_produce_compiler_listing(self->compiled_value->root_value,
        writer, xctx);
    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);

    /* Write residual source. */
    if (self->include_source && !self->source_eof) {
        afw_writer_write_eol(writer, xctx);
        while (impl_write_source_line(self, true, xctx));
    }

    /* Reset to empty prefix and no indent. */
    self->current_prefix = NULL;
    self->empty_prefix.len = 0;
    self->empty_prefix.s = NULL;

    /* Symbol table. */
    afw_writer_write_z(writer, "\n", xctx);
    afw_writer_write_z(writer, "\n", xctx);
    afw_writer_write_z(writer, "---Symbols ", xctx);
    afw_writer_write_utf8(writer, self->reference_id, xctx);
    afw_writer_write_eol(writer, xctx);
    impl_symbol_listing(writer, self->compiled_value->top_block, xctx);
    afw_writer_write_eol(writer, xctx);

    /* Literals. */
    afw_writer_write_z(writer, "\n", xctx);
    afw_writer_write_z(writer, "\n", xctx);
    afw_writer_write_z(writer, "---Literals ", xctx);
    afw_writer_write_utf8(writer, self->reference_id, xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_write_z(writer, "Literals coming soon", xctx);
    afw_writer_write_eol(writer, xctx);

    /* Write --- for end. */
    afw_writer_write_z(writer, "\n", xctx);
    afw_writer_write_z(writer, "\n", xctx);
    afw_writer_write_z(writer, "---", xctx);
    afw_writer_write_eol(writer, xctx);

    /* Release writer and return. */
    afw_writer_release(&self->writer, xctx);
    return self;
}



static afw_size_t
impl_total_buffer_needed(const afw_value_compiler_listing_t *self)
{
    afw_size_t result;
    const afw_value_compiler_listing_t *child;

    result = self->ary->nelts;
    for (child = self->first_child; child; child = child->next_sibling) {
        result += impl_total_buffer_needed(child);
    }

    return result;
}



static void
impl_move_to_buffer(const afw_value_compiler_listing_t *self,
    char **buffer)
{
    const afw_value_compiler_listing_t *child;

    memcpy(*buffer, self->ary->elts, self->ary->nelts);
    *buffer = *buffer + self->ary->nelts;
    for (child = self->first_child; child; child = child->next_sibling) {
        impl_move_to_buffer(child, buffer);
    }
}



/* Decompile a value to a compiler listing string. */
AFW_DEFINE(const afw_utf8_t *)
afw_value_compiler_listing_to_string(
    const afw_value_t *value,
    const afw_utf8_t *tab,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_t *self;
    char *buffer;
    afw_utf8_t *result;
    const afw_pool_t *temp_pool;
    afw_size_t len;

    /* Create self and ary in its own pool. */
    temp_pool = afw_pool_create(p, xctx);

    self = afw_value_compiler_listing_to_string_instance(
        value, NULL, tab, temp_pool, xctx);

    len = impl_total_buffer_needed(self);
    buffer = afw_pool_malloc(p, len, xctx);
    result = afw_pool_malloc_type(p, afw_utf8_t, xctx);
    result->len = len;
    result->s = buffer;
    impl_move_to_buffer(self, &buffer);

    afw_writer_release(&self->writer, xctx);
    afw_pool_release(temp_pool, xctx);

    return result;
}


/* Decompile a value to a compiler listing string. */
AFW_DEFINE_INTERNAL(void)
afw_value_compiler_listing_begin_value(
    const afw_writer_t *writer,
    const afw_value_t *value,
    const afw_compile_value_contextual_t *contextual,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_t *self =
        (afw_value_compiler_listing_t *)writer;
    afw_size_t line_number;
    afw_size_t column_number;
    afw_size_t end_line_number;
    afw_size_t end_column_number;
    afw_utf8_t prefix;
    afw_size_t len;
    char buffer[sizeof(impl_empty_prefix)];

    if (contextual) {
        if (contextual->value_offset > self->max_value_cursor) {
            self->max_value_cursor = contextual->value_offset;
        }
        if (self->offset_only) {
            len = snprintf(buffer, sizeof(buffer),
                self->prefix_format,
                contextual->value_offset,
                contextual->value_offset + contextual->value_size);
        }
        else {
            afw_utf8_line_column_of_offset(&line_number, &column_number,
                contextual->compiled_value->full_source, contextual->value_offset,
                self->tab_size, xctx);
            afw_utf8_line_column_of_offset(&end_line_number, &end_column_number,
                contextual->compiled_value->full_source,
                contextual->value_offset + contextual->value_size,
                self->tab_size, xctx);
            len = snprintf(buffer, sizeof(buffer),
                self->prefix_format,
                line_number, column_number,
                end_line_number, end_column_number);
        }
        prefix.s = &buffer[0];
        prefix.len = self->empty_prefix.len;
        memcpy(&buffer[len],
            &impl_empty_prefix[len + (sizeof(impl_empty_prefix) - prefix.len) - 1],
            prefix.len - len);
        self->current_prefix = &prefix;
    }
    else {
        self->current_prefix = &self->empty_prefix;
    }

    if (value) {
        afw_writer_write_utf8(&self->writer,
            &value->inf->rti.implementation_id,
            xctx);
    }
}


/* Decompile a value to a compiler listing string. */
AFW_DEFINE_INTERNAL(void)
afw_value_compiler_listing_end_value(
    const afw_writer_t *writer,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_writer_write_eol(writer, xctx);
}


/* Decompile call args. */
AFW_DEFINE_INTERNAL(void)
afw_value_compiler_listing_call_args(
    const afw_writer_t *writer,
    const afw_value_call_args_t *args,
    afw_xctx_t *xctx)
{
    afw_size_t i;

    AFW_VALUE_COMPILER_LISTING_IF_NOT_LIMIT_EXCEEDED

    for (i = 0; i <= args->argc; i++) {
        afw_value_compiler_listing_value(args->argv[i], writer, xctx);
    }
}



/* Decompile Value::. */
AFW_DEFINE_INTERNAL(void)
afw_value_compiler_listing_value(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    if (!instance) {
        afw_writer_write_utf8(writer, &afw_s_undefined, xctx);
        afw_writer_write_eol(writer, xctx);
    }
    else {
        afw_value_produce_compiler_listing(instance, writer, xctx);
    }
}


AFW_DEFINE_INTERNAL(void)
afw_value_compiler_listing_name_and_type(
    const afw_writer_t *writer,
    const afw_utf8_t *name,
    const afw_value_type_t *type,
    afw_xctx_t *xctx)
{
    if (name) {
        afw_writer_write_utf8(writer, name, xctx);
    }
    if (type) {
        if (name) {
            afw_writer_write_z(writer, ": ", xctx);
        }
        if (type->data_type) {
            afw_writer_write_utf8(writer,
                &type->data_type->data_type_id, xctx);
            if (type->data_type_parameter_contextual) {
                afw_writer_write_z(writer, "<", xctx);
                afw_writer_write(writer,
                    type->data_type_parameter_contextual->compiled_value->
                        full_source->s +
                        type->data_type_parameter_contextual->value_offset,
                    type->data_type_parameter_contextual->value_size,
                    xctx);
                afw_writer_write_z(writer, ">", xctx);
            }
        }
        else {
            afw_writer_write_z(writer, "any", xctx);
        }
    }
}



