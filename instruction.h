#ifndef INSTRUCTION_H
#define INSTRUCTION_H

void instruction_print(Stack* stack, Object argument) {
  assert(argument.type == VOID);

  Object obj = Stack_pop(stack);

  assert(obj.type == ROPE);

  Rope_print(obj.value.string);
  Rope_destruct(obj.value.string);
}

void instruction_push_string(Stack* stack, Object argument) {
  assert(argument.type == ROPE);
  Stack_push(stack, argument);
}

#endif
