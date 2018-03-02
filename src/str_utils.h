#ifndef __STR_UTILS_H__
#define __STR_UTILS_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>         // size-t, SIZE_MAX
#include <string.h>

#define MAX_STR_LEN 2048

typedef struct str_char_pos_s
{
    size_t *array;      // variable array to store char positions in
    size_t count;       // size of variable array
} str_char_pos_t;

char *str_extract_cpy (const char *str, size_t start_pos, size_t end_pos);
size_t str_find_from_index (const char *str, int character, size_t index);
#define str_find_first(str, character) str_find_from_index(str, character, 0)
void str_prepend (char *dst, const char *src, size_t size);
void strrep (char *dst, const char *src, size_t num_replicate, size_t len);
void str_rem_spc (char *src);

#ifdef __cplusplus
}
#endif

#endif
