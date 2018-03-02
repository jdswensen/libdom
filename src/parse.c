
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dom.h"
#include "str_utils.h"

static bool dom_tag_is_empty (char *str);

// ===========================================================================
// Functions and definitions to parse different types of elements
// ===========================================================================
typedef bool (*element_type_parse_cb_t)(dom_node_t **root, char *str);
static bool parse_element_document (dom_node_t **root, char *str);
static bool parse_element_title (dom_node_t **root, char *str);
static bool parse_element_heading (dom_node_t **root, char *str);
static bool parse_element_para (dom_node_t **root, char *str);
static bool parse_element_br (dom_node_t **root, char *str);
static bool parse_element_endtag (dom_node_t **root, char *str);
static bool parse_element (dom_node_t **root, char *str);

static element_type_parse_cb_t parse_cb_array[] =
{
    parse_element_document,
    parse_element_title,
    parse_element_heading,
    parse_element_para,
    parse_element_br,
    parse_element_endtag,
    parse_element,
};

/**
 ******************************************************************************
 * @fn          dom_loadf
 * @brief       Generate a DOM from a file
 * @param[in]   input: file stream to generate from
 * @retval      pointer to head of new DOM
 *
 * @details
 ******************************************************************************
 */
dom_node_t *dom_loadf (FILE *input)
{
    char *str_buffer = NULL;
    size_t buffer_size = 0;
    size_t read_len = 0;
    dom_node_t *new_doc = NULL;

    if ( input != NULL )
    {
        if ( fseek(input, 0, SEEK_END) == 0 )
        {
            buffer_size = (size_t)ftell(input);
            str_buffer = (char *)malloc(sizeof(char) * (buffer_size + 1));

            if ( fseek(input, 0, SEEK_SET) == 0 )
            {
                read_len = fread(str_buffer, sizeof(char), buffer_size, input);
                str_buffer[read_len] = '\0';
            }
            else
            {
                // fseek error
                fprintf(stderr, "%s\n", "dom_loadf: error reading file");
                return NULL;
            }
        }
        else
        {
            fprintf(stderr, "%s\n", "dom_loadf: error reading file");
            return NULL;
        }
    }

    new_doc = dom_loads(str_buffer);
    free(str_buffer);

    return new_doc;
}


/**
 ******************************************************************************
 * @fn          dom_loadf
 * @brief       Generate a DOM from a string
 * @param[in]   html_markup: string to generate from
 * @retval      pointer to head of new DOM
 *
 * @details
 ******************************************************************************
 */
dom_node_t *dom_loads (const char *html_markup)
{
    size_t index = 0;
    size_t open_caret_pos = 0;
    size_t close_caret_pos = 0;
    bool parse_complete = false;
    bool tag_parse_complete = false;
    dom_node_t *root_doc = NULL;
    dom_node_t *parent_node = NULL;
    dom_node_t *new_node = NULL;
    char *node_tag_str = NULL;
    char *node_text_str = NULL;
    const char *search_str = NULL;
    bool tag_is_empty = false;

    // TODO: validate markup

    // Start search at beginning of markup
    search_str = html_markup;
    open_caret_pos = str_find_first(search_str, '<');
    close_caret_pos = str_find_first(search_str, '>');

    do
    {
        node_tag_str = str_extract_cpy(search_str, open_caret_pos + 1, close_caret_pos - 1);

        // TODO: parse attributes

        // Set up the new node
        if ( strcmp(node_tag_str, "/html") == 0 )
        {
            parse_complete = true;
        }
        else
        {
            tag_parse_complete = false;
            tag_is_empty = dom_tag_is_empty(node_tag_str);
            (void)tag_is_empty;

            tag_parse_complete = false;
            for ( index = 0; index < sizeof(parse_cb_array) / sizeof(element_type_parse_cb_t); index++ )
            {
                if ( !tag_parse_complete )
                {
                    tag_parse_complete = parse_cb_array[index](&parent_node, node_tag_str);
                    if ( dom_node_is_document(parent_node) == true )
                    {
                        root_doc = parent_node;
                    }
                }
            }

            // TODO: check if comment tag

            // Find the next open caret
            open_caret_pos = str_find_from_index(search_str, '<', close_caret_pos);
            if ( open_caret_pos != SIZE_MAX )
            {
                // Extract the text between carets and remove leading whitespace
                node_text_str = str_extract_cpy(search_str, close_caret_pos + 1, open_caret_pos - 1);
                str_rem_spc(node_text_str);

                // If there was more than just whitespace, found a text node
                // node_text_str strlen will be 0 if just whitespace
                if ( strlen(node_text_str) > 0 )
                {
                    new_node = dom_node_text();
                    dom_node_set_text(new_node, node_text_str);
                    dom_node_append(parent_node, new_node);
                }
                free(node_text_str);

                // Find the next close caret for next loop
                close_caret_pos = str_find_from_index(search_str, '>', open_caret_pos);
            }
            else
            {
                parse_complete = true;
            }
        }

        free(node_tag_str);

    } while ( !parse_complete );

    return root_doc;
}


/**
 ******************************************************************************
 * @fn          dom_tag_is_empty
 * @brief       Check to see if a HTML tag is empty
 * @param[in]   str: string containing tag to check
 * @retval      true: tag is empty element
 *
 * @details     There is a deliberate side effect of removing the trailing
 *              forward slash.
 ******************************************************************************
 */
static bool dom_tag_is_empty (char *str)
{
    bool retval = false;

    if ( str != NULL )
    {
        if ( str[strlen(str) - 1] == '/' )
        {
            str[strlen(str) - 1] = '\0';
            retval = true;
        }
    }

    return retval;
}


/**
 ******************************************************************************
 * @fn          parse_element_xxx
 * @brief       Parse an element and create the corresponding node
 * @param[out]  root: pointer to newly created node
 * @param[in]   str: string to parse
 * @retval      true: successfully created new node
 *              false: otherwise
 *
 * @details
 ******************************************************************************
 */
static bool parse_element_document (dom_node_t **root, char *str)
{
    dom_node_t *new_node = NULL;
    bool retval = false;

    if ( str != NULL )
    {
        if ( strcmp(str, "html") == 0 )
        {
            new_node = dom_node_document();
            root[0] = new_node;
            retval = true;
        }
    }

    return retval;
}


static bool parse_element_heading (dom_node_t **root, char *str)
{
    dom_node_t *new_node = NULL;
    bool retval = false;

    if ( str != NULL )
    {
        if ( strncmp(str, "h", 1) == 0 && isdigit(str[1]))
        {
            new_node = dom_node_h(str);
            dom_node_append(root[0], new_node);
            root[0] = new_node;
            retval = true;
        }
    }

    return retval;
}


static bool parse_element_title (dom_node_t **root, char *str)
{
    dom_node_t *new_node = NULL;
    bool retval = false;

    if ( str != NULL )
    {
        if ( strcmp(str, "title") == 0 )
        {
            new_node = dom_node_title();
            dom_node_append(root[0], new_node);
            root[0] = new_node;
            retval = true;
        }
    }

    return retval;
}


static bool parse_element_endtag (dom_node_t **root, char *str)
{
    bool retval = false;

    if ( str != NULL )
    {
        if ( str[0] == '/' )
        {
            root[0] =  root[0]->parent;
            retval = true;
        }
    }
    return retval;
}


static bool parse_element (dom_node_t **root, char *str)
{
    dom_node_t *new_node = NULL;
    bool retval = false;

    if ( str != NULL )
    {
        if ( true )
        {
            new_node = dom_node_element(str);
            dom_node_append(root[0], new_node);
            root[0] = new_node;
            retval = true;
        }
    }
    return retval;
}


static bool parse_element_br (dom_node_t **root, char *str)
{
    dom_node_t *new_node = NULL;
    bool retval = false;

    if ( str != NULL )
    {
        if ( strcmp(str, "br") == 0 )
        {
            new_node = dom_node_br();
            dom_node_append(root[0], new_node);
            retval = true;
        }
    }

    return retval;
}


static bool parse_element_para (dom_node_t **root, char *str)
{
    dom_node_t *new_node = NULL;
    bool retval = false;

    if ( str != NULL )
    {
        if ( strncmp(str, "p", 1) == 0 || strncmp(str, "p ", 2) == 0 )
        {
            new_node = dom_node_p();
            dom_node_append(root[0], new_node);
            root[0] = new_node;
            retval = true;
        }
    }

    return retval;
}
