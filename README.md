# Quanta IR
## Introduction
Quanta IR is meant to be a simple and modifiable intermediate representation for compilers.
It is designed to be easier to get off the ground with than LLVM IR. Making it ideal for 
hobby projects, DSLs, and educational purposes. It is also designed to be easy to modify with its
own meta language that gives direct access to IR internal tree representation. This allows the simple
Quanta IR to be extended to fit the needs of any type of compiler. This also makes Quanta IR a good 
alternative to compiling to C code for simple compilers. Quanta IR itself compiles to C code, but
it provides a more modern and language agnostic syntax to target.