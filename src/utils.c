#include "utils.h"

// ===== LIST IMPLEMENTATION =====

list list_init(list this, size_t elem_size) {
  this->elem_size = elem_size;
  this->capacity = 4;
  this->data = malloc(this->capacity * this->elem_size);
  this->size = 0;
  return this;
}

list list_add(list this, void *value) {
  if (this->size >= this->capacity) {
    this->capacity *= 2;
    this->data = realloc(this->data, this->capacity * this->elem_size);
  }
  void *target = (char *)(this->data) + (this->size * this->elem_size);
  memcpy(target, value, this->elem_size);
  this->size++;
  return this;
}

void *list_get(list this, size_t index) {
  if (index >= this->size) {
    return NULL;
  }
  return (char *)(this->data) + (index * this->elem_size);
}

list list_set(list this, size_t index, void *value) {
  if (index >= this->size) {
    return this;
  }
  void *target = (char *)(this->data) + (index * this->elem_size);
  memcpy(target, value, this->elem_size);
  return this;
}

bool list_eq(list this, list other) {
  if (this->size != other->size) {
    return false;
  }
  if (this->elem_size != other->elem_size) {
    return false;
  }
  return memcmp(this->data, other->data, this->size * this->elem_size) == 0;
}

void list_free(list this) {
  free(this->data);
  this->data = NULL;
  this->size = 0;
  this->capacity = 0;
  this->elem_size = 0;
  // Note: the struct itself is not freed—similar to string_free.
}

list list_from_array(const void *items, size_t count, size_t elem_size) {
  list lst = malloc(sizeof(list_));
  if (!lst)
    return NULL;
  *lst = (list_){0}; // Zero initialize
  list_init(lst, elem_size);
  for (size_t i = 0; i < count; i++) {
    const void *item = (const char *)items + i * elem_size;
    list_add(lst, (void *)item);
  }
  return lst;
}

// ===== STRING IMPLEMENTATION =====

string string_init(string this, const char *value) {
  this->capacity = 16;
  this->length = 0;
  this->data = malloc(this->capacity);
  this->data[0] = '\0';
  if (value != NULL) {
    string_set(this, value);
  }
  return this;
}

string string_create(const char *input) {
  string this = malloc(sizeof(string_));
  *this = (string_){0};
  string_init(this, input);
  return this;
}

string string_append(string this, const char *suffix) {
  size_t new_len = this->length + strlen(suffix);
  if (new_len + 1 > this->capacity) {
    while (new_len + 1 > this->capacity) {
      this->capacity *= 2;
    }
    this->data = realloc(this->data, this->capacity);
  }
  strcpy_s(this->data + this->length, this->capacity - this->length, suffix);
  this->length = new_len;
  return this;
}

const char *string_get(string this) { return this->data; }

string string_set(string this, const char *value) {
  size_t val_len = strlen(value);
  if (val_len + 1 > this->capacity) {
    this->capacity = val_len + 1;
    this->data = realloc(this->data, this->capacity);
  }
  strcpy_s(this->data, this->capacity, value);
  this->length = val_len;
  return this;
}

bool string_eq(string this, string other) {
  return strcmp(this->data, other->data) == 0;
}

void string_free(string this) {
  free(this->data);
  this->data = NULL;
  this->length = 0;
  this->capacity = 0;
}

// ===== PRINT FUNCTIONS =====

void print_int(int x) { printf("%d\n", x); }
void print_float(float x) { printf("%f\n", x); }
void print_double(double x) { printf("%lf\n", x); }
void print_cstr(const char *x) { printf("%s\n", x); }

void print_string(string x) {
  if (x->data != NULL) {
    printf("%s\n", x->data);
  } else {
    printf("<empty string>\n");
  }
}

void print_list(list l) {
  if (l == NULL || l->data == NULL) {
    printf("<null list>\n");
    return;
  }
  printf("<list size=%zu, capacity=%zu, elem_size=%zu>\n", l->size, l->capacity,
         l->elem_size);
  if (l->elem_size == sizeof(int)) {
    printf("[");
    for (size_t i = 0; i < l->size; i++) {
      int *elem = (int *)((char *)l->data + i * l->elem_size);
      printf("%d", *elem);
      if (i < l->size - 1)
        printf(", ");
    }
    printf("]\n");
  } else {
    printf("<cannot print elements of unknown type>\n");
  }
}

void print_unknown(void *x) { printf("<unknown type at %p>\n", x); }
