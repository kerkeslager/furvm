import string

MAIN_TEMPLATE = '''
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../stack.h"
#include "../instruction.h"

int main(int argc, char** argv) {
  Stack* stack = Stack_construct();
$instructions

  Stack_destruct(stack);
  return 0;
}
'''

INSTRUCTIONS = set([
    'print',
    'push_string',
])

def assemble_to_c(source):
    source_lines = source.split('\n')

    instructions = []

    for source_line_index, source_line in enumerate(source_lines):
        if source_line.strip() == '':
            continue

        source_line_number = source_line_index + 1

        tokens = source_line.split(maxsplit=1)

        try:
            instruction, argument = tokens
        except:
            instruction = tokens[0]
            argument = None

        if not instruction in INSTRUCTIONS:
            raise Exception(
                'Error on line {}: invalid instruction {}.'.format(
                    source_line_number,
                    instruction,
                ),
            )

        instructions.append((instruction, argument))

    def format_argument(argument):
        if argument is None:
            return 'Object_create(VOID, (Value)0)'

        if isinstance(argument, str):
            return 'Object_create_from_string(String_construct_from_c_string({}))'.format(
                argument,
            )

        raise Exception()

    instructions_formatted = '\n'.join(
        '  instruction_{}(stack, {});'.format(
            instruction,
            format_argument(argument),
        )
        for instruction, argument in instructions
    )

    template = string.Template(MAIN_TEMPLATE)

    return template.substitute({
        'instructions': instructions_formatted,
    })

def assemble(source, target_language='c'):
    return assemble_to_c(source)

if __name__ == '__main__':
    import sys

    with open(sys.argv[1], 'r') as f:
        source = f.read()

    with open(sys.argv[1] + '.c', 'w') as f:
        f.write(assemble(source, target_language='c'))
