// System headers
#include <stdio.h>          // snprintf, vsnprintf, etc
#include <stdlib.h>         // malloc
#include <assert.h>         // assert
#include <stdbool.h>
#include <ctype.h>

// Project headers
#include "str_utils.h"

/**
 ******************************************************************************
 * @fn          str_extract_cpy
 * @brief       Extract a copy of characters between two points in a string
 * @param[in]   str: string to search
 * @param[in]   start_pos: starting position in string
 * @param[in]   end_pos: ending position in string
 * @retval      success: pointer to a new null terminated string
 *              failure: NULL
 *
 * @details     String should have NULL termination, otherwise end_pos may
 *              end up beyond array bounds.
 ******************************************************************************
 */
char *str_extract_cpy (const char *str, size_t start_pos, size_t end_pos)
{
    char *str_extract = NULL;
    size_t str_extract_len = 0;
    const char *str_extract_start = NULL;

    assert(start_pos <= end_pos);   // should never happen

    /*
     * Set extracted string length
     *   Add 1 additional byte to correct off-by-one
     *   Add 1 additional byte for guaranteed NULL termination
     */

    str_extract_len = (end_pos - start_pos) + 2;

    if ( str != NULL )
    {
        str_extract = (char *)malloc(str_extract_len * sizeof(char));
        if ( str_extract != NULL )
        {
            str_extract_start = (str + start_pos);
            strncpy(str_extract, str_extract_start, str_extract_len);
            str_extract[str_extract_len - 1] = '\0'; // force NULL termination
        }
        else
        {
            // malloc was NULL, fall through to return NULL
        }
    }

    return str_extract;
}


/**
 ******************************************************************************
 * @fn          str_find_from_index
 * @brief       Find the first occurance of a printable character in a string
 * @param[in]   str: string to search
 * @param[in]   character: printable character to search for
 * @param[in]   index: index into string to start parsing
 * @retval      if found: position in char array
 *              if not: SIZE_MAX
 *
 * @details     Use this to avoid pointer arithmetic in calling function
 ******************************************************************************
 */
size_t str_find_from_index (const char *str, int character, size_t index)
{
    bool found = false;
    size_t retval = SIZE_MAX;

    if ( str != NULL )
    {
        while ( str[index] != '\0' && found == false )
        {
            if ( character == str[index] )
            {
                retval = index;
                found = true;
            }
            else
            {
                index++;
            }
        }
    }

    return retval;
}


/**
 ******************************************************************************
 * @fn          str_prepend
 * @brief       Prepend a string to another
 * @param[io]   dst: string to prepend to
 * @param[in]   src: source string to prepend
 * @param[in]   size: size of dst string
 * @retval      none
 *
 * @details
 ******************************************************************************
 */
void str_prepend (char *dst, const char *src, size_t size)
{
    char *tmp_str = NULL;

    if ( dst != NULL && src != NULL )
    {
        tmp_str = (char *)malloc(size);
        strncpy(tmp_str, src, size);
        strncat(tmp_str, dst, size);
        strncpy(dst, tmp_str, size);
        free(tmp_str);
    }
}


/**
 ******************************************************************************
 * @fn          strrep
 * @brief       Replicate a string into a destination
 * @param[io]   dst: pointer to where to store the string
 * @param[in]   src: source string to replicate
 * @param[in]   num_replicate: number of times to replicate
 * @param[in]   len: length of destination string
 * @retval      none
 *
 * @details
 ******************************************************************************
 */
void strrep (char *dst, const char *src, size_t num_replicate, size_t len)
{
    size_t index = 0;

    if ( dst != NULL && src != NULL )
    {
        strncpy(dst, "", len);
        for ( index = 0; index < num_replicate; index++ )
        {
            strncat(dst, src, len);
        }
    }
    else
    {
        // fall through
    }
}


/**
 ******************************************************************************
 * @fn          str_rem_spc
 * @brief       Remove leading whitespace from a string
 * @param[in]   src: string to parse
 * @retval      none
 *
 * @details
 ******************************************************************************
 */
void str_rem_spc (char *src)
{
    char *tmp_str = NULL;
    size_t index = 0;

    if ( src != NULL )
    {
        tmp_str = (char *)malloc(MAX_STR_LEN);
        while ( isspace(src[index]) && src[index] != '\0' )
        {
            index++;
        }

        strncpy(tmp_str, src, MAX_STR_LEN);
        strncpy(src, &tmp_str[index], strlen(src) + 1);
        free(tmp_str);
    }
}
