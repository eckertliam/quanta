# run commands to build the parser for compiler0
from subprocess import call
import os

PARSER_DIR = "compiler0/generated"

if os.path.exists(PARSER_DIR):
    # remove the generated parser
    for file in os.listdir(PARSER_DIR):
        # if file is not __init__.py
        if file != "__init__.py":
            os.remove(os.path.join(PARSER_DIR, file))

call(["antlr4", "-Dlanguage=Python3", "-visitor", "-o", PARSER_DIR, "Quanta.g4"])