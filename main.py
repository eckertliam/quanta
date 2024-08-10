from compiler0 import frontend

if __name__ == '__main__':
    # read example/math.q file
    with open("examples/math.q") as file:
        code = file.read()
        parse_tree = frontend.parse(code)