#include "string.h"

enum Type;
typedef enum Type Type;
enum Type {
  ROPE,
  VOID
};

union Value;
typedef union Value Value;
union Value {
  int32_t integer;
  Rope* string;
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
    case ROPE:
      Rope_destruct(self->value.string);
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

Object Object_createFromRope(Rope* value) {
  return Object_create(ROPE, (Value)Rope_reference(value));
}

Object Object_createFromCString(char* cString) {
  return Object_createFromRope(Rope_constructFromCString(cString));
}
