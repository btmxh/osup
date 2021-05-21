#include "osup_types.h"

#include <ctype.h>

static osup_bool osup_parse_uint(const char* begin, const char* end,
                                 osup_int* value) {
  *value = 0;
  const char* it = begin;
  while (it < end) {
    if (!isdigit(*it)) {
      return osup_false;
    }
    *value = *value * 10 + (*it - '0');
  }
  return osup_true;
}

OSUP_API osup_bool osup_parse_int(const char* begin, const char* end,
                                  osup_int* value) {
  if (*begin == '-') {
    if (!osup_parse_uint(begin + 1, end, value)) {
      return osup_false;
    }
    *value = -*value;
    return osup_true;
  } else {
    // strings like +123 is NOT supported
    return osup_parse_uint(begin, end, value);
  }
}

OSUP_API osup_bool osup_parse_bool(const char* begin, const char* end,
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

