#include <assert.h>
#include <string.h>

#include "osup/osup_common.h"

void testInt(const char* str, osup_int expected) {
  osup_int v;
  assert(osup_parse_int(&str, str + strlen(str), &v) && v == expected);
}

void testDecimal(const char* str, osup_decimal expected) {
  static const osup_decimal epsilon = 0.001;
  osup_decimal v;
  assert(osup_parse_decimal(&str, str + strlen(str), &v) &&
         v - expected < epsilon && expected - v < epsilon);
}

void testRGB(const char* str, uint8_t r, uint8_t g, uint8_t b) {
  osup_rgb v;
  assert(osup_parse_rgb(&str, str + strlen(str), &v) && v.red == r &&
         v.green == g && v.blue == b);
}

int main() {
  testInt("123", 123);
  testInt("-782", -782);

  testDecimal("0.1", 0.1);
  testDecimal("-0.1", -0.1);
  testDecimal("2.1", 2.1);
  testDecimal("-2.1", -2.1);
  testDecimal("2.1e10", 2.1e10);
  testDecimal("-2.1E9", -2.1E9);

  testRGB("100,20,30", 100, 20, 30);

  return 0;
}
