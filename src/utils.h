#ifndef UTILS_H
#define UTILS_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef UTILS_EXPORTS
#define UTILS_API __declspec(dllexport)
#else
#define UTILS_API __declspec(dllimport)
#endif

// ===== LIST =====
// Previously the list API used "list*" everywhere. Now we define a struct
// type (list_) and then typedef its pointer as "list", similar to how string is
// done.
typedef struct {
  size_t size;
  size_t capacity;
  size_t elem_size;
  void *data;
  bool __variable;
} list_;

typedef list_ *list;

UTILS_API list list_init(list this, size_t elem_size);
UTILS_API list list_add(list this, void *value);
UTILS_API void *list_get(list this, size_t index);
UTILS_API list list_set(list this, size_t index, void *value);
UTILS_API bool list_eq(list this, list other);
UTILS_API void list_free(list this);
UTILS_API list list_from_array(const void *items, size_t count,
                               size_t elem_size);

// A macro to conveniently create a list from an initializer list
#define list__new(type, ...)                                                   \
  list_from_array((type[]){__VA_ARGS__},                                       \
                  sizeof((type[]){__VA_ARGS__}) / sizeof(type), sizeof(type))
#define list__free(...) list_free(__VA_ARGS__)

// ===== STRING =====

typedef struct {
  size_t length;
  size_t capacity;
  char *data;
  bool __variable;
} string_;

typedef string_ *string;

UTILS_API string string_init(string this, const char *value);
UTILS_API string string_append(string this, const char *suffix);
UTILS_API const char *string_get(string this);
UTILS_API string string_set(string this, const char *value);
UTILS_API bool string_eq(string this, string other);
UTILS_API void string_free(string this);
UTILS_API string string_create(const char *input);

#define string__new(...) string_create(__VA_ARGS__)
#define string__free(...) string_free(__VA_ARGS__)

// ===== Bitwise Ops =====

#define BIT_AND(a, b) ((a) & (b))
#define BIT_OR(a, b) ((a) | (b))
#define BIT_XOR(a, b) ((a) ^ (b))
#define BIT_NOT(a) (~(a))

// Shift operations
#define BIT_SHL(a, n) ((a) << (n))
#define BIT_SHR(a, n) ((a) >> (n))

// Bit manipulation
#define BIT_SET(val, bit) ((val) | (1ULL << (bit)))
#define BIT_CLEAR(val, bit) ((val) & ~(1ULL << (bit)))
#define BIT_TOGGLE(val, bit) ((val) ^ (1ULL << (bit)))
#define BIT_CHECK(val, bit) (((val) >> (bit)) & 1ULL)

// Mask operations
#define BIT_MASK(n) ((1ULL << (n)) - 1ULL)
#define BIT_RANGE(val, from, to)                                               \
  (((val) >> (from)) & BIT_MASK((to) - (from) + 1))

// Rotate (for unsigned types only)
#define BIT_ROL(val, n, bits) (((val) << (n)) | ((val) >> ((bits) - (n))))
#define BIT_ROR(val, n, bits) (((val) >> (n)) | ((val) << ((bits) - (n))))

// ===== Print Utilities =====

#define print(x)                                                               \
  _Generic((x),                                                                \
      int: print_int,                                                          \
      float: print_float,                                                      \
      double: print_double,                                                    \
      char *: print_cstr,                                                      \
      const char *: print_cstr,                                                \
      string: print_string,                                                    \
      list: print_list,                                                        \
      default: print_unknown)(x)

UTILS_API void print_int(int x);
UTILS_API void print_float(float x);
UTILS_API void print_double(double x);
UTILS_API void print_cstr(const char *x);
UTILS_API void print_string(string x);
UTILS_API void print_list(list l);
UTILS_API void print_unknown(void *x);

#define __call(x, ...) (assert((x) != NULL), x(__VA_ARGS__))

#endif
