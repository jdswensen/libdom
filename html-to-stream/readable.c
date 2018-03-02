#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "readable.h"

void setup_title ( dom_node_t *node );
void setup_heading ( dom_node_t *node );
void setup_p ( dom_node_t *node );
void setup_br ( dom_node_t *node );
void setup_text ( dom_node_t *node );

void setup_title ( dom_node_t *node )
{
    if ( node != NULL )
    {
        if ( dom_node_is_title(node) == true )
        {
            dom_node_set_readable(node, "Title: ", DOM_NODE_READABLE_PREPEND);
            dom_node_set_readable(node, "\n\n", DOM_NODE_READABLE_APPEND);
        }
    }
}


void setup_heading ( dom_node_t *node )
{
    if ( node != NULL )
    {
        if ( dom_node_is_h(node) == true )
        {
            dom_node_set_readable(node, "=== ", DOM_NODE_READABLE_PREPEND);
            dom_node_set_readable(node, " ===\n\n", DOM_NODE_READABLE_APPEND);
        }
    }
}


void setup_p ( dom_node_t *node )
{
    if ( node != NULL )
    {
        if ( dom_node_is_p(node) == true )
        {
            dom_node_set_readable(node, "\n\n", DOM_NODE_READABLE_APPEND);
        }
    }
}


void setup_br ( dom_node_t *node )
{
    if ( node != NULL )
    {
        if ( dom_node_is_br(node) == true )
        {
            dom_node_set_readable(node, "\n", DOM_NODE_READABLE_SET);
        }
    }
}


void setup_text ( dom_node_t *node )
{
    char *tmp_str = NULL;
    if ( node != NULL )
    {
        if ( dom_node_is_text(node) == true )
        {
            tmp_str = (char *)malloc(2048);
            strncpy(tmp_str, node->text, 2048);
            dom_node_set_readable(node, tmp_str, DOM_NODE_READABLE_SET);
            free(tmp_str);
        }
    }
}


void setup_readable ( dom_node_t *node )
{
    dom_node_t *loop_node = NULL;
    dom_node_t *tmp_node = NULL;

    if ( node != NULL )
    {
        loop_node = dom_node_walk(node);
        while ( loop_node != NULL )
        {
            setup_text(loop_node);
            setup_title(loop_node);
            setup_heading(loop_node);
            setup_p(loop_node);
            setup_br(loop_node);

            tmp_node = dom_node_walk(loop_node);
            loop_node = tmp_node;
        }
    }
}
