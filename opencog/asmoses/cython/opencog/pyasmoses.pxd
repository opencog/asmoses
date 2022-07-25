#!python
#cython: language_level=3

cdef extern from "opencog/asmoses/moses/main/moses_exec.h" namespace "opencog::moses":
    int moses_exec(int argc, char **argv) except +

