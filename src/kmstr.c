/*
 * ============================================================================
 *
 *       Filename:  kmstr.c
 *
 *    Description:  String handling functions
 *
 *        Version:  1.0
 *        Created:  22/03/14 13:02:11
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "kmstr.h"

inline int
str_ok (const str_t *str)
{
    return (str != NULL && str->s != NULL && str->m > 0);
}


inline void
print_str (const str_t *str, FILE *stream)
{
    if (km_likely(str_ok(str))) {
        fprintf(stream, "%s", str->s);
    }
}

inline void
destroy_str_cp (str_t *str)
{
    km_free(str->s);
}

inline void
destroy_str (str_t *str)
{
    destroy_str_cp(str);
    km_free(str);
}

inline int
str_nullify (str_t *str)
{
    if (km_unlikely(str == NULL)) return 0;
    bzero(str->s, str->m);
    str->l = 0;
    return 1;
}

inline int
str_fill_charptr_len (str_t *str, const char *cp, const size_t len)
{
    if (km_unlikely(str == NULL) || km_unlikely(cp == NULL)) return 0;
    if (km_unlikely(str->m < len + 1)) {
        while (str->m < len + 1) {
            str->m = kmroundupz(str->m);
        }
        str->s = km_realloc(str->s, str->m * sizeof(*str->s));
    }
    memcpy(str->s, cp, len);
    str->s[len] = '\0';
    str->l = len;
    return 1;
}

inline int
str_fill_charptr (str_t *str, const char *cp)
{
    size_t len = 0;
    if (km_unlikely(!str_ok(str)) || km_unlikely(cp == NULL)) return 0;
    len = strlen(cp);
    return str_fill_charptr_len(str, cp, len);
}

inline void
init_str (str_t *str, size_t capacity)
{
    if (km_unlikely(str == NULL)) return;
    str->l = 0;
    str->s = km_calloc(capacity, sizeof(*str->s));
    str->m = capacity;
}

inline str_t *
create_str (size_t capacity)
{
    str_t *str = km_calloc(1, sizeof(*str));
    /* We don't worry about NULL here. init_str will return before derefencing
       and we'll return NULL below. */
    init_str(str, capacity);
    return str;
}