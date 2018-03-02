#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <str_utils.h>
#include "dom.h"

static void dom_node_type_string (dom_node_type_t type, char *type_str, size_t len);
//static char *dom_get_heading_str (char *str, int8_t level);
static dom_node_t *dom_node_walk_up (dom_node_t *node);


/**
 ******************************************************************************
 * @fn          dom_node
 * @brief       Create a new node with name and type
 * @param[in]   name: name of the html tag
 * @param[in]   type: type of node to create
 * @retval      Newly created node
 *
 * @details
 ******************************************************************************
 */
dom_node_t *dom_node (const char *name, dom_node_type_t type)
{
    dom_node_t *new_node = NULL;

    new_node = (dom_node_t *)malloc(sizeof(dom_node_t));

    if ( new_node != NULL )
    {
        new_node->node_type = type;
        strncpy(new_node->name, name, sizeof(new_node->name));
        strncpy(new_node->text, "", sizeof(new_node->text));
        strncpy(new_node->attr_list, "", sizeof(new_node->attr_list));
        new_node->parent = NULL;
        new_node->children = NULL;
        new_node->siblings = NULL;
        new_node->readable_prepend_str = NULL;
        new_node->readable_str = NULL;
        new_node->readable_append_str = NULL;
    }
    return new_node;
}


/**
 ******************************************************************************
 * @fn          dom_node_free
 * @brief       Release DOM node resources
 * @param[in]   node: pointer to DOM node to free
 * @retval      none
 *
 * @details
 ******************************************************************************
 */
void dom_node_free (dom_node_t *node)
{
    // Avoid double free errors
    if ( node != NULL )
    {
        if ( node->children != NULL )
        {
            dom_node_free(node->children);
            node->children = NULL;
        }

        if ( node->siblings != NULL )
        {
            dom_node_free(node->siblings);
            node->siblings = NULL;
        }

        if ( node->readable_prepend_str != NULL )
        {
            free(node->readable_prepend_str);
        }

        if ( node->readable_str != NULL )
        {
            free(node->readable_str);
        }

        if ( node->readable_append_str != NULL )
        {
            free(node->readable_append_str);
        }
        free(node);
    }
}


/**
 ******************************************************************************
 * @fn          dom_node_append
 * @brief       Append a child node to a parent
 * @param[in]   root: parent node to append new node to
 * @param[in]   new_node: node to append
 * @retval      true: successful append
 *              false: problem during append
 *
 * @details
 ******************************************************************************
 */
bool dom_node_append (dom_node_t *root, dom_node_t *new_node)
{
    bool retval = false;

    if ( root != NULL && new_node != NULL )
    {
        new_node->parent = root;

        dom_node_t *tmp_node;
        new_node->siblings = NULL;

        if ( root->children != NULL )
        {
            // Parent has other children
            tmp_node = root->children;

            // Walk through siblings until null
            while ( tmp_node->siblings )
            {
                tmp_node = tmp_node->siblings;
            }

            // End of list, add new node
            tmp_node->siblings = new_node;
        }
        else
        {
            // Parent's first child, lucky you
            root->children = new_node;
        }

        retval = true;
    }

    return retval;
}


/**
 ******************************************************************************
 * @fn          dom_node_set_text
 * @brief
 * @param[in]   node: pointer to node to change
 * @param[in]   text: new text to set
 * @retval      true: successfully changed text
 *              false: problem during text change
 *
 * @details
 ******************************************************************************
 */
bool dom_node_set_text (dom_node_t *node, const char *text)
{
    bool retval = false;

    if ( node != NULL && text != NULL )
    {
        strncpy(node->text, text, sizeof(node->text));
        retval = true;
    }
    return retval;
}


/**
 ******************************************************************************
 * @fn          dom_node_set_readable
 * @brief
 * @param[in]   node: pointer to node to change
 * @param[in]   str: str to set to readable
 * @param[in]   mode: mode in which to insert text
 * @retval      true: successfully changed text
 *              false: problem during text change
 *
 * @details
 ******************************************************************************
 */
bool dom_node_set_readable (dom_node_t *node, const char *str, dom_node_readable_t mode)
{
    bool retval = false;

    if ( node != NULL )
    {
        if ( node->readable_prepend_str == NULL )
        {
            node->readable_prepend_str = (char *)malloc(MAX_STR_LEN);
            strncpy(node->readable_prepend_str, "\0", MAX_STR_LEN);
        }

        if ( node->readable_str == NULL )
        {
            node->readable_str = (char *)malloc(MAX_STR_LEN);
            strncpy(node->readable_str, "\0", MAX_STR_LEN);
        }

        if ( node->readable_append_str == NULL )
        {
            node->readable_append_str = (char *)malloc(MAX_STR_LEN);
            strncpy(node->readable_append_str, "\0", MAX_STR_LEN);
        }

        switch ( mode )
        {
        case DOM_NODE_READABLE_PREPEND_EACH_CHILD:
            if ( node->children != NULL )
            {
                dom_node_set_readable(node->children, str, mode);
            }
            if ( node->siblings != NULL )
            {
                dom_node_set_readable(node->siblings, str, mode);
            }
            str_prepend(node->readable_prepend_str, str, MAX_STR_LEN);
            break;
        case DOM_NODE_READABLE_PREPEND:
            str_prepend(node->readable_prepend_str, str, MAX_STR_LEN);
            break;

        case DOM_NODE_READABLE_APPEND_EACH_CHILD:
            if ( node->children != NULL )
            {
                dom_node_set_readable(node->children, str, mode);
            }
            if ( node->siblings != NULL )
            {
                dom_node_set_readable(node->siblings, str, mode);
            }
            str_prepend(node->readable_append_str, str, MAX_STR_LEN);
            break;

        case DOM_NODE_READABLE_APPEND:
            str_prepend(node->readable_append_str, str, MAX_STR_LEN);
            break;

        case DOM_NODE_READABLE_SET:
            strncpy(node->readable_str, str, MAX_STR_LEN);
            break;

        default:
            break;
        }
    }

    return retval;
}

/**
 ******************************************************************************
 * @fn          dom_node_num_children
 * @brief       Return number of children belonging to the node
 * @param[in]
 * @retval
 *
 * @details
 ******************************************************************************
 */
int32_t dom_node_num_children (dom_node_t *root)
{
    dom_node_t *loop_node = NULL;
    int32_t num_children = 0;

    if ( root != NULL )
    {
        loop_node = root->children;
        while ( loop_node != NULL )
        {
            num_children++;
            loop_node = loop_node->siblings;
        }
    }
    else
    {
        // fall through
    }

    return num_children;
}


/**
 ******************************************************************************
 * @fn          dom_node_walk
 * @brief       Find the next node in a DOM
 * @param[in]   node: pointer to DOM node to start walking through
 * @retval      pointer to next node in the DOM
 *
 * @details
 ******************************************************************************
 */
dom_node_t *dom_node_walk (dom_node_t *node)
{
    dom_node_t *ret_ptr = NULL;

    if ( node != NULL )
    {
        if ( node->children != NULL )
        {
            // Have children!
            return node->children;
        }
        else if ( node->siblings != NULL )
        {
            // Don't have children, but have siblings
            return node->siblings;
        }
        else if ( node->parent != NULL )
        {
            // No children or siblings, find nearest unparsed relative
            return dom_node_walk_up(node->parent);
        }
        else
        {
            fprintf(stdout, "%s", "dom_node_walk: unknown walking condition\n");
        }

    }

    return ret_ptr;
}


/**
 ******************************************************************************
 * @fn          dom_node_print_readable
 * @brief       Print DOM to stream
 * @param[in]   node: pointer to DOM node to start printing
 * @param[in]   stream: stream to send DOM printing
 * @retval      none
 *
 * @details
 ******************************************************************************
 */
void dom_node_print_readable (dom_node_t *node, FILE *stream)
{
    if ( node != NULL )
    {
        // Print text before children
        if ( node->readable_prepend_str != NULL )
        {
            fprintf(stream, "%s", node->readable_prepend_str);
        }

        // Print node text
        if ( node->readable_str != NULL )
        {
            fprintf(stream, "%s", node->readable_str);
        }

        if ( node->children != NULL )
        {
            dom_node_print_readable(node->children, stream);
        }

        // Print text after children
        if ( node->readable_append_str != NULL )
        {
            fprintf(stream, "%s", node->readable_append_str);
        }

        // Finally print all the siblings
        if ( node->siblings != NULL )
        {
            dom_node_print_readable(node->siblings, stream);
        }
    }
}


/**
 ******************************************************************************
 * @fn          dom_node_print_ancestry
 * @brief       Print ancestry info from root node
 * @param[in]   root: node to start printing from
 * @param[in]   num_tabs: number of tabs to insert before printing
 * @retval      none
 *
 * @details
 ******************************************************************************
 */
void dom_node_print_ancestry_recursive (dom_node_t *root, int32_t num_tabs)
{
    dom_node_t *elt = NULL;
    dom_node_t *loop = NULL;
    int32_t index = 0;
    char type_str[MAX_STR_LEN] = "";
    char indent_str[MAX_STR_LEN] = "";

    (void)elt;
    (void)loop;

    if ( root != NULL )
    {
        for ( index = 0; index < num_tabs; index++ )
        {
            strncat(indent_str, "\t", sizeof(indent_str));
        }

        dom_node_type_string(root->node_type, type_str, MAX_STR_LEN);

        printf("%s%s%s\n", indent_str, "node tag: ", root->name);
        printf("%s%s%s\n", indent_str, "node type: ", type_str);
        printf("%s%s%s\n", indent_str, "node text: ", root->text);
        printf("%s%s%i\n", indent_str, "num children: ", dom_node_num_children(root));

        if ( root->readable_prepend_str != NULL )
        {
            printf("%s%s%s\n", indent_str, "readable_prepend_str: ", root->readable_prepend_str);
        }

        if ( root->readable_str != NULL )
        {
            printf("%s%s%s\n", indent_str, "readable_str: ", root->readable_str);
        }

        if ( root->readable_append_str != NULL )
        {
            printf("%s%s%s\n", indent_str, "readable_append_str: ", root->readable_append_str);
        }

        printf("\n");

        if ( root->children != NULL )
        {
            dom_node_print_ancestry_recursive(root->children, num_tabs + 1);
        }

        if ( root->siblings != NULL )
        {
            dom_node_print_ancestry_recursive(root->siblings, num_tabs);
        }
    }
}


/**
 ******************************************************************************
 * @fn          dom_node_is
 * @brief       Check node type against 'type'
 * @param[in]   node: pointer to node to check
 * @param[im]   type: node type to check agains
 * @retval      true: node type is of 'type' passed in
 *              false: otherwise
 *
 * @details
 ******************************************************************************
 */
bool dom_node_is (const dom_node_t *node, dom_node_type_t type)
{
    bool retval = false;

    if ( node != NULL )
    {
        if ( node->node_type == type )
        {
            retval = true;
        }
    }

    return retval;
}


/**
 ******************************************************************************
 * @fn          dom
 * @brief
 * @param[in]
 * @retval
 *
 * @details
 ******************************************************************************
 */
static void dom_node_type_string (dom_node_type_t type, char *type_str, size_t len)
{
    switch ( type )
    {
    case DOM_NODE_TYPE_DOCUMENT:
        strncpy(type_str, "document\0", len);
        break;
    case DOM_NODE_TYPE_TITLE:
        strncpy(type_str, "title\0", len);
        break;
    case DOM_NODE_TYPE_HEADING:
        strncpy(type_str, "heading\0", len);
        break;
    case DOM_NODE_TYPE_PARA:
        strncpy(type_str, "p\0", len);
        break;
    case DOM_NODE_TYPE_BREAK:
        strncpy(type_str, "br\0", len);
        break;
    case DOM_NODE_TYPE_STRONG:
        strncpy(type_str, "strong\0", len);
        break;
    case DOM_NODE_TYPE_BLOCKQUOTE:
        strncpy(type_str, "blockquote\0", len);
        break;
    case DOM_NODE_TYPE_ELEMENT:
        strncpy(type_str, "element\0", len);
        break;
    case DOM_NODE_TYPE_TEXT:
        strncpy(type_str, "text\0", len);
        break;
    case DOM_NODE_TYPE_COMMENT:
        strncpy(type_str, "comment\0", len);
        break;
    default:
        strncpy(type_str, "unknown\0", len);
        break;
    }
}


/**
 ******************************************************************************
 * @fn          dom_get_heading_str
 * @brief
 * @param[in]
 * @retval
 *
 * @details
 ******************************************************************************
 */
// static char *dom_get_heading_str (char *str, int8_t level)
// {
//     char *new_str = NULL;
//     int8_t index = 0;
//
//     if ( str != NULL && level <= 6 )
//     {
//         new_str = (char *)malloc(MAX_STR_LEN);
//         strncpy(new_str, "", MAX_STR_LEN);
//         for ( index = 1; index <= level; index++ )
//         {
//             strncat(new_str, "===", MAX_STR_LEN);
//         }
//         strncat(new_str, " ", MAX_STR_LEN);
//         strncat(new_str, str, MAX_STR_LEN);
//         strncat(new_str, " ", MAX_STR_LEN);
//         for ( index = 1; index <= level; index++ )
//         {
//             strncat(new_str, "===", MAX_STR_LEN);
//         }
//         strncat(new_str, "\n\0", MAX_STR_LEN);
//     }
//     else
//     {
//         // fall through
//     }
//
//     return new_str;
// }


static dom_node_t *dom_node_walk_up (dom_node_t *node)
{
    dom_node_t *ret_ptr = NULL;

    if ( node != NULL )
    {
        if ( node->siblings != NULL )
        {
            ret_ptr = node->siblings;
        }
        else if ( node->parent != NULL )
        {
            ret_ptr = dom_node_walk_up(node->parent);
        }
        else
        {
            // Oldest living ancestor
            ret_ptr = NULL;
        }
    }

    return ret_ptr;
}
