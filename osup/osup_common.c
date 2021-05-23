#include "osup_common.h"

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

OSUP_STORAGE osup_errcb errcb = NULL;
OSUP_STORAGE void* errcb_ptr = NULL;

static void osup_stderr_error_callback(const char* err, void* ptr) {
  fprintf(stderr, "[osup] %s\n", err);
}

OSUP_API void osup_set_error_callback(osup_errcb callback, void* ptr) {
  errcb = callback;
  errcb_ptr = ptr;
}

OSUP_API void osup_set_default_error_callback() {
  errcb = osup_stderr_error_callback;
}

OSUP_LIB void osup_error(const char* format, ...) {
  if (errcb) {
    char buffer[256];
    va_list va;
    va_start(va, format);
    vsnprintf(buffer, sizeof(buffer), format, va);
    va_end(va);
    errcb(buffer, errcb_ptr);
  }
}

char osup_temp_slice[256 + 1] = {};
OSUP_LIB const char* osup_temp_string_slice(const char* begin,
                                            const char* end) {
  assert(begin <= end);
  size_t len = end - begin;
  if (len > sizeof(osup_temp_slice) - 1) {
    len = sizeof(osup_temp_slice) - 1;
  }
  memcpy(osup_temp_slice, begin, len);
  osup_temp_slice[len] = '\0';
  return osup_temp_slice;
}

OSUP_LIB const char* osup_temp_string_slice_line_terminated(const char* line) {
  size_t i = 0;
  while (i < sizeof(osup_temp_slice) - 1) {
    if (osup_is_line_terminator(line[i])) {
      osup_temp_slice[i] = '\0';
      break;
    } else {
      osup_temp_slice[i] = line[i];
      i++;
    }
  }
  return osup_temp_slice;
}

OSUP_LIB osup_bool osup_strdup(const char* begin, const char* end,
                               char** value) {
  assert(begin <= end);
  size_t len = end - begin;
  *value = malloc(len + 1);
  if (!*value) {
    return osup_false;
  } else {
    memcpy(*value, begin, len);
    (*value)[len] = '\0';
    return osup_true;
  }
}

OSUP_INTERN osup_bool osup_parse_uint(const char* begin, const char* end,
                                      osup_int* value) {
  if (begin > end) return osup_false;
  *value = 0;
  const char* it = begin;
  while (it < end) {
    if (!isdigit(*it)) {
      return osup_false;
    }
    *value = *value * 10 + (*it - '0');
    ++it;
  }
  return osup_true;
}

OSUP_LIB osup_bool osup_parse_int(const char* begin, const char* end,
                                  osup_int* value) {
  if (*begin == '-') {
    if (!osup_parse_uint(begin + 1, end, value)) {
      return osup_false;
    }
    *value = -*value;
    return osup_true;
  } else {
    /* strings like +123 is not supported */
    return osup_parse_uint(begin, end, value);
  }
}

OSUP_INTERN osup_bool osup_parse_unsigned_decimal(const char* begin,
                                                  const char* end,
                                                  osup_decimal* value) {
  if (begin > end) return osup_false;
  enum { INT, FRACT } part = INT;
  *value = 0.0;
  double mult = 0.1;
  const char* it = begin;
  while (it < end) {
    switch (*it) {
      case '.':
        if (part == INT) {
          part = FRACT;
          ++it;
        } else {
          return osup_false;
        }
        break;
      case 'e':
      case 'E': {
        /* the exponent part is an integer, so we can use osup_parse_int to
         * parse it */
        osup_int exp;
        ++it;
        osup_parse_int(begin, end, &exp);
        *value *= pow(10.0, exp);
        return osup_true;
      }
      default:
        if (isdigit(*it)) {
          switch (part) {
            case INT:
              *value = *value * 10.0 + (*it - '0');
              break;
            case FRACT:
              *value += (*it - '0') * mult;
              mult *= 0.1;
          }
          ++it;
        }
    }
  }

  return osup_true;
}

OSUP_LIB osup_bool osup_parse_decimal(const char* begin, const char* end,
                                      osup_decimal* value) {
  if (*begin == '-') {
    if (!osup_parse_unsigned_decimal(begin + 1, end, value)) {
      return osup_false;
    }
    *value = -*value;
    return osup_true;
  } else {
    /* strings like +123.0 is not supported */
    return osup_parse_unsigned_decimal(begin, end, value);
  }
}

OSUP_LIB osup_bool osup_parse_bool(const char* begin, const char* end,
                                   osup_bool* value) {
  if (end - begin != 1) return osup_false;
  switch (*begin) {
    case '0':
    case '1':
      *value = *begin == '1';
      return osup_true;
    default:
      return osup_false;
  }
}

OSUP_LIB osup_bool osup_parse_rgb(const char* begin, const char* end,
                                  osup_rgb* value) {
  osup_int i;
  if (!osup_parse_int_until_nondigit_char(&begin, &i) || i < 0 || i > 255 ||
      *begin != ',' || ++begin > end) {
    return osup_false;
  }
  value->red = i;
  if (!osup_parse_int_until_nondigit_char(&begin, &i) || i < 0 || i > 255 ||
      *begin != ',' || ++begin > end) {
    return osup_false;
  }
  value->green = i;
  if (!osup_parse_int_until_nondigit_char(&begin, &i) || i < 0 || i > 255 ||
      ++begin == end) {
    return osup_false;
  }
  value->blue = i;
  return osup_true;
}

OSUP_LIB osup_bool osup_parse_ubyte(const char* begin, const char* end,
                                    uint8_t* byte) {
  osup_int value;
  if (!osup_parse_uint(begin, end, &value) || value > 255) {
    return osup_false;
  }
  *byte = value;
  return osup_true;
}

OSUP_INTERN osup_bool osup_parse_uint_until_nondigit_char(const char** begin,
                                                          osup_int* value) {
  if (!isdigit(**begin)) {
    return osup_false;
  }
  *value = 0;
  do {
    *value = *value * 10 + (**begin - '0');
    ++(*begin);
  } while (isdigit(**begin));
  return osup_true;
}

OSUP_LIB osup_bool osup_parse_int_until_nondigit_char(const char** begin,
                                                      osup_int* value) {
  if (**begin == '-') {
    ++(*begin);
    if (!osup_parse_uint_until_nondigit_char(begin, value)) {
      return osup_false;
    }
    *value = -*value;
    return osup_true;
  } else {
    /* strings like +123 is not supported */
    return osup_parse_uint_until_nondigit_char(begin, value);
  }
}

OSUP_LIB osup_bool osup_is_line_terminator(char c) {
  return c == '\0' || c == '\r' || c == '\n';
}

OSUP_LIB osup_bool osup_split_string(char delimiter, const char** splitBegin,
                                     const char** splitEnd,
                                     const char* stringEnd) {
  /* example: 123,456,789 (delimiter is ',')
   *          ^ ^        ^
   *          b e        E
   * (b is splitBegin, e is splitEnd, E is stringEnd) */
  *splitBegin = ++(*splitEnd);
  /* 123,456,789
   *     ^      ^
   *     b,e    E */
  if (*splitEnd > stringEnd) {
    return osup_false;
  }
  /* 123,456,789
   *     ^      ^
   *     b,e    E */
  while (**splitEnd != delimiter && *splitEnd < stringEnd) {
    ++(*splitEnd);
  }
  /* 123,456,789
   *     ^  ^   ^
   *     b  e   E */
  return osup_true;
}

OSUP_LIB osup_bool osup_split_string_line_terminated(char delimiter,
                                                     const char** splitBegin,
                                                     const char** splitEnd) {
  /* check if we are actually at the end of the string */
  if (*splitBegin && osup_is_line_terminator(**splitEnd)) return osup_false;
  /* basically the same algorithm as osup_split_string */
  *splitBegin = ++(*splitEnd);
  while (**splitEnd != delimiter && !osup_is_line_terminator(**splitEnd)) {
    ++(*splitEnd);
  }

  return osup_true;
}

/* like osup_split_string_line_terminated but with support for quotes */
OSUP_LIB osup_bool osup_split_string_line_terminated_quoted(
    char delimiter, const char** valueBegin, const char** valueEnd,
    const char** splitEnd) {
  /* check if we are actually at the end of the string */
  if (*valueBegin && osup_is_line_terminator(**splitEnd)) return osup_false;
  *valueBegin = ++(*splitEnd);
  if (**valueBegin == '"') {
    /* increase both valueBegin and splitEnd, since the string starts after the
     * " character */
    *valueBegin = ++(*splitEnd);
    /* find next " character, also check if we are out of bounds */
    while (**splitEnd != '"') {
      if (osup_is_line_terminator(**splitEnd)) {
        return osup_false;
      }
      ++(*splitEnd);
    }
    *valueEnd = *splitEnd;
    ++(*splitEnd);
    return osup_true;
  } else {
    /* same algorithm from osup_split_string_terminated */
    while (**splitEnd != delimiter && !osup_is_line_terminator(**splitEnd)) {
      ++(*splitEnd);
    }
    *valueEnd = *splitEnd;

    return osup_true;
  }
}

