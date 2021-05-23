#ifndef OSUP_COMMON_H
#define OSUP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef OSUP_API
#ifdef OSUP_PRIVATE
#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199409L))
#define OSUP_API static inline
#elif defined(__cplusplus)
#define OSUP_API static inline
#else
#define OSUP_API static
#endif
#else
#define OSUP_API extern
#endif
#endif

#ifndef OSUP_NAN
#ifdef NAN
#define OSUP_NAN NAN
#else
#define OSUP_NAN (0.0 / 0.0)
#endif
#endif

#ifndef OSUP_INF
#ifdef INFINITY
#define OSUP_INF INFINITY
#else
#define OSUP_INF (1.0 / 0.0)
#endif
#endif

#ifndef OSUP_NINF
#define OSUP_NINF (-OSUP_INF)
#endif

#ifndef OSUP_LIB
#define OSUP_LIB extern
#endif

#define OSUP_INTERN static
#define OSUP_STORAGE static

typedef void (*osup_errcb)(const char*, void*);

OSUP_API void osup_set_error_callback(osup_errcb callback, void* ptr);
OSUP_API void osup_set_default_error_callback();
OSUP_LIB void osup_error(const char* format, ...);
/* return a temporary null-terminated string with content taken from begin to
 * end
 */
OSUP_LIB const char* osup_temp_string_slice(const char* begin, const char* end);
/* osup_temp_string_slice for line-terminated strings */
OSUP_LIB const char* osup_temp_string_slice_line_terminated(const char* line);

#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
typedef bool osup_bool;
#define osup_true true
#define osup_false false
#else
typedef enum { osup_true = 1, osup_false = 0 } osup_bool;
#endif

typedef int osup_err_t;
typedef int32_t osup_int;
typedef int64_t osup_long;
typedef double osup_decimal;

typedef uint8_t osup_bitfield8;
typedef uint32_t osup_bitfield32;

typedef struct {
  const char* begin;
  const char* end;
} osup_slice;

typedef struct {
  uint8_t red, green, blue;
} osup_rgb;

typedef struct {
  osup_int x, y;
} osup_vec2;

typedef enum {
  OSUP_MODE_OSU,
  OSUP_MODE_TAIKO,
  OSUP_MODE_CATCH,
  OSUP_MODE_MANIA
} osup_gamemode;

/* parsing functions, but also increment the current pointer for "better
 * performance" */
OSUP_LIB osup_bool osup_strdup(const char* begin, const char* end,
                               char** value);
OSUP_LIB osup_bool osup_parse_int(const char* begin, const char* end,
                                  osup_int* value);
OSUP_LIB osup_bool osup_parse_bool(const char* begin, const char* end,
                                   osup_bool* value);
OSUP_LIB osup_bool osup_parse_decimal(const char* begin, const char* end,
                                      osup_decimal* value);
OSUP_LIB osup_bool osup_parse_rgb(const char* begin, const char* end,
                                  osup_rgb* value);
OSUP_LIB osup_bool osup_parse_ubyte(const char* begin, const char* end,
                                    uint8_t* byte);
/* parse int until a non-digit character, used to parse int lists */
OSUP_LIB osup_bool osup_parse_int_until_nondigit_char(const char** begin,
                                                      osup_int* value);
/* split string by delimiter */
OSUP_LIB osup_bool osup_split_string(char delimiter, const char** splitBegin,
                                     const char** splitEnd,
                                     const char* stringEnd);
OSUP_LIB osup_bool osup_is_line_terminator(char c);
OSUP_LIB osup_bool osup_split_string_line_terminated(char delimiter,
                                                     const char** splitBegin,
                                                     const char** splitEnd);
/* for quoted string, splitEnd is not at the end of the token, so there will be
 * issues when parsing, this is why we need another pointer */
OSUP_LIB osup_bool osup_split_string_line_terminated_quoted(
    char delimiter, const char** splitBegin, const char** splitEnd,
    const char** splitQuoteEnd);

#ifdef __cplusplus
}
#endif

#endif

