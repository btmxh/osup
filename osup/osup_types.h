#ifndef OSUP_TYPES_H
#define OSUP_TYPES_H

#include <stdint.h>
#include <stddef.h>

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
  char* begin;
  char* end;
} osup_slice;

typedef struct {
  uint8_t red, green, blue;
} osup_rgb;

typedef struct {
  osup_int x, y;
} osup_vec2;

typedef enum { OSU, TAIKO, CATCH, MANIA } osup_gamemode;

#endif

