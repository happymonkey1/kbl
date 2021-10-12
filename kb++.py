import sys

LANG_NAME = "kb++"
COUNTER = 0
def iota(reset = False) -> int:
    global COUNTER
    if reset:
        COUNTER = 0
    i = COUNTER
    COUNTER += 1
    return i

''' Define Operations '''
OP_ASSIGN = iota(True)
OP_ADD    = iota()
OP_PRINT  = iota()
COUNT_OPS = iota()

''' Define Types '''
TYPE_i32 = iota(True)
COUNT_TYPES = iota()

''' Operation Implementations '''


def assign():
    assert False, "assign not implemented"

def add():
    assert False, "add not implemented"
    
def pprint():
    assert False, "print not implemented"

def unpack(packed) -> tuple:
    return (packed[0], packed[1:] if len(packed) > 1 else None)

def find_col(line : str, start : int, predicate):
    while start < len(line) and predicate(line[start]):
        start += 1
    return start

def lex_line(line : str) -> tuple:
    # TODO(Sean) implement semi colon line endings?
    tokens: List[tuple[str, int]] = []
    col: int = find_col(line, 0, lambda x: x.isspace())
    while col < len(line):
        token_end = find_col(line, col, lambda x: not x.isspace())
        
        token_str = line[col:token_end]
        tokens.append((token_str, col))
        
        col = token_end
        
        col = find_col(line, col, lambda x: x.isspace())
    return tokens

def lex_file(file) -> list[tuple]:
    lines: List[List[tuple[str, int]]] = []
    with open(file, 'r') as f:
        for row_index, line in enumerate(f.readlines()):
            line_tokens = lex_line(line)
            lines.append(line_tokens)
    return lines

def token_to_op(token : tuple) -> int:
    assert COUNT_OPS == 3, "Not all operations are implmented in token_to_op."
    token_repr = token[0]
    if token_repr == "<-":
        return OP_ASSIGN
    elif token_repr == "+":
        return OP_ADD
    elif token_repr == "print":
        return OP_PRINT
    else:
        return -1

def parse_line(line_tokens : list[tuple[str, int]], row : int) -> list[tuple[int, int]]:
    # TODO(Sean) kb++ only supports one operation per line
    operations = []
    i = 0
    while i < len(line_tokens):
        token = line_tokens[i]
        if (op := token_to_op(token)) != -1:
            operations.append((op, row))
            break
        i += 1
    #stack += [token for index, token in enumerate(line_tokens) if index != i ]
    return operations

def build_program(file) -> list[tuple[int, int]]:
    lines = lex_file(file)
    ops: list[tuple[int, int]] = []
    for row, tokens in enumerate(lines):
        op_token: tuple[int, int] = parse_line(tokens, row)
        ops.append(op_token)
    
    return ops
        
def interpret_program(program : list[tuple[int, int]]):
    # TODO: more types 
    stack: dict[str, int] = {}
    for line in program:
        for op in line:
            assert COUNT_OPS == 3, "Exhaustive list of implemented ops in interpret_program"
            print(op)
            if op[0] == OP_ASSIGN:
                
                stack[var_name] = var_data
            elif op[0] == OP_ADD:
                add()
            elif op[0] == OP_PRINT:
                pprint()
            else:
                assert False, "Unreachable code in interpret_program"
            

def compile_program(file):
    assert False, "compilation not implemented"

def usage(file):
    print(f"[USAGE]: {file} [COMMAND] -[OPTIONAL ARGS] [FILE]")
    print(f"    int    interpret the {LANG_NAME} program")
    print(f"    com    compiles the {LANG_NAME} program to assembly")

if __name__ == "__main__":
    argv = sys.argv
    assert len(argv) >= 1
    compiler, *argv = argv
    
    if len(argv) < 2:
        usage(argv[0])
        print("[ERROR]: no command specified")
    
    assert len(argv) == 2, f"{compiler} only supports [COMMAND] [FILE]"    
    command, program_path = argv
    
    program = build_program(program_path)
    if command == "int":
        interpret_program(program)
    elif command == "com":
        compile_program(program)
    else:
        usage(compiler)
        print(f"[ERROR]: unknown command '{command}' specified")