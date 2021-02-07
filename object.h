#include "string.h"

enum Type;
typedef enum Type Type;
enum Type {
  VOID,
  STRING
};

union Value;
typedef union Value Value;
union Value {
  int32_t integer;
  String* string;
};

struct Object;
typedef struct Object Object;
struct Object {
  Type type;
  Value value;
};

void Object_initialize(Object* self, Type type, Value value) {
  self->type = type;
  self->value = value;
}

void Object_deinitialize(Object* self) {
  switch(self->type) {
    case STRING:
      String_destruct(self->value.string);
      break;
    default:
      assert(false);
  }
}

Object Object_create(Type type, Value value) {
  Object result;
  Object_initialize(&result, type, value);
  return result;
}

Object Object_create_from_string(String* string) {
  return Object_create(STRING, (Value)String_reference(string));
}
