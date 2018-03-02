#ifndef __DOM_H__
#define __DOM_H__

#if defined( __cplusplus )
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define DOM_MAX_STR_LEN 2048

// ===========================================================================
// Modes used when setting up readable output
// ===========================================================================
typedef enum dom_node_readable_e
{
    DOM_NODE_READABLE_UNKNOWN = 0,
    DOM_NODE_READABLE_PREPEND,
    DOM_NODE_READABLE_PREPEND_CHILD,
    DOM_NODE_READABLE_PREPEND_EACH_CHILD,
    DOM_NODE_READABLE_SET,
    DOM_NODE_READABLE_APPEND_EACH_CHILD,
    DOM_NODE_READABLE_APPEND_CHILD,
    DOM_NODE_READABLE_APPEND,
} dom_node_readable_t;


// ===========================================================================
// Different DOM node types
// ===========================================================================
typedef enum dom_node_type_e
{
    DOM_NODE_TYPE_UNKNOWN = 0,
    DOM_NODE_TYPE_DOCUMENT,
    DOM_NODE_TYPE_TITLE,
    DOM_NODE_TYPE_HEADING,
    DOM_NODE_TYPE_PARA,
    DOM_NODE_TYPE_BREAK,
    DOM_NODE_TYPE_STRONG,
    DOM_NODE_TYPE_BLOCKQUOTE,
    DOM_NODE_TYPE_ELEMENT,
    DOM_NODE_TYPE_TEXT,
    DOM_NODE_TYPE_COMMENT,
} dom_node_type_t;


// ===========================================================================
// DOM structure
// ===========================================================================
typedef struct dom_node_t
{
    dom_node_type_t node_type;
    char name[DOM_MAX_STR_LEN];
    char text[DOM_MAX_STR_LEN];
    char attr_list[DOM_MAX_STR_LEN];
    char *readable_prepend_str;
    char *readable_str;
    char *readable_append_str;
    struct dom_node_t *parent;
    struct dom_node_t *children;
    struct dom_node_t *siblings;
} dom_node_t;


// ===========================================================================
// Functions to access and control a DOM
// ===========================================================================
dom_node_t *dom_node ( const char *name, dom_node_type_t type );
void dom_node_free ( dom_node_t *node );
bool dom_node_append ( dom_node_t *root, dom_node_t *new_node );
bool dom_node_set_attr ( dom_node_t *node, char *attr );
bool dom_node_set_text ( dom_node_t *node, const char *text );
bool dom_node_set_node_type ( dom_node_t *node, dom_node_type_t *node_type );
bool dom_node_set_readable ( dom_node_t *node, const char *str, dom_node_readable_t mode );

// ===========================================================================
// Macros to wrap new node creation for DOMs
// ===========================================================================
#define dom_node_document() dom_node("html", DOM_NODE_TYPE_DOCUMENT)
#define dom_node_title() dom_node("title", DOM_NODE_TYPE_TITLE)
#define dom_node_h(heading) dom_node(heading, DOM_NODE_TYPE_HEADING)
#define dom_node_p() dom_node("p", DOM_NODE_TYPE_PARA)
#define dom_node_br() dom_node("br", DOM_NODE_TYPE_BREAK)
#define dom_node_strong() dom_node("strong", DOM_NODE_TYPE_STRONG)
#define dom_node_blockquote() dom_node("blockquote", DOM_NODE_TYPE_BLOCKQUOTE)
#define dom_node_element(name) dom_node(name, DOM_NODE_TYPE_ELEMENT)
#define dom_node_text() dom_node("text", DOM_NODE_TYPE_TEXT)
#define dom_node_comment() dom_node("comment", DOM_NODE_TYPE_COMMENT)


// ===========================================================================
// Functions to create multiple nodes in a DOM
// ===========================================================================
dom_node_t *dom_loadf ( FILE *input );
dom_node_t *dom_loads ( const char *html_markup );


// ===========================================================================
// Functions to interact with node children
// ===========================================================================
int32_t dom_node_num_children ( dom_node_t *root );
dom_node_t *dom_node_walk ( dom_node_t *node );


// ===========================================================================
// Functions to print information in DOMs
// ===========================================================================
void dom_node_print_readable ( dom_node_t *node, FILE *stream );
#define dom_node_print_ancestry(root) dom_node_print_ancestry_recursive(root, 0)
void dom_node_print_ancestry_recursive ( dom_node_t *root, int32_t depth );


// ===========================================================================
// Functions and macros to check node type
// ===========================================================================
bool dom_node_is ( const dom_node_t *node, dom_node_type_t type );

// Macros to wrap the dom_node_is function
#define dom_node_is_document(node) dom_node_is(node, DOM_NODE_TYPE_DOCUMENT)
#define dom_node_is_title(node) dom_node_is(node, DOM_NODE_TYPE_TITLE)
#define dom_node_is_h(node) dom_node_is(node, DOM_NODE_TYPE_HEADING)
#define dom_node_is_p(node) dom_node_is(node, DOM_NODE_TYPE_PARA)
#define dom_node_is_br(node) dom_node_is(node, DOM_NODE_TYPE_BREAK)
#define dom_node_is_element(node) dom_node_is(node, DOM_NODE_TYPE_ELEMENT)
#define dom_node_is_text(node) dom_node_is(node, DOM_NODE_TYPE_TEXT)
#define dom_node_is_comment(node) dom_node_is(node, DOM_NODE_TYPE_COMMENT)

#ifdef __cplusplus
}
#endif

#endif
