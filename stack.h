#ifndef STACK_H
#define STACK_H

#include "object.h"

struct StackNode;
typedef struct StackNode StackNode;
struct StackNode {
  Object value;
  StackNode* next;
};

void StackNode_deinitialize(StackNode*);
void StackNode_destruct(StackNode*);

void StackNode_initialize(StackNode* self, Object value, StackNode* next) {
  self->value = value;
  self->next = next;
}

StackNode* StackNode_construct(Object value, StackNode* next) {
  StackNode* result = calloc(1, sizeof(StackNode));
  StackNode_initialize(result, value, next);
  return result;
}

void StackNode_deinitialize(StackNode* self) {
}

void StackNode_destruct(StackNode* self) {
  free(self);
}

struct Stack;
typedef struct Stack Stack;
struct Stack {
  StackNode* root;
};

void Stack_initialize(Stack* self) {
  self->root = NULL;
}

Stack* Stack_construct() {
  Stack* result = calloc(1, sizeof(Stack));
  Stack_initialize(result);
  return result;
}

void Stack_deinitialize(Stack* self) {
  StackNode_destruct(self->root);
}

void Stack_destruct(Stack* self) {
  Stack_deinitialize(self);
  free(self);
}

void Stack_push(Stack* self, Object value) {
  self->root = StackNode_construct(value, self->root);
}

Object Stack_pop(Stack* self) {
  assert(self->root != NULL);
  StackNode* node = self->root;
  Object value = node->value;
  self->root = node->next;
  StackNode_destruct(node);
  return value;
}

#endif
