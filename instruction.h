#ifndef INSTRUCTION_H
#define INSTRUCTION_H

void instruction_print(Stack* stack, Object argument) {
  assert(argument.type == VOID);

  Object obj = Stack_pop(stack);

  assert(obj.type == STRING);

  String_print(obj.value.string);
  String_destruct(obj.value.string);
}

void instruction_push_string(Stack* stack, Object argument) {
  assert(argument.type == STRING);
  Stack_push(stack, argument);
}

#endif
