
Python Wrapper for MOSES
========================
Wrapper for MOSES. Uses the C++ `moses_exec` function to access
MOSES functionality.

Options for using the pyasmoses wrapper:
1. Within the CogServer, from the interactive Python shell
2. In your Python IDE or interpreter. You need to ensure that
   your path includes '{PROJECT_BINARY_DIR}/opencog/cython'

Loading the module:
-------------------

```
from opencog.pyasmoses import moses
moses = moses()
```

Example usage of run:
---------------------
Example #1: XOR example with Python output and Python input
```
input_data = [[0, 0, 0], [1, 1, 0], [1, 0, 1], [0, 1, 1]]
output = moses.run(input=input_data, python3=True)
print output[0].score # Prints: 0
model = output[0].eval
model([0, 1])  # Returns: True
model([1, 1])  # Returns: False
```

Example #2: Run the majority demo problem, return only one candidate, and use
Python output
```
output = moses.run(args="-H maj -c 1", python3=True)
model = output[0].eval
model([0, 1, 0, 1, 0])  # Returns: False
model([1, 1, 0, 1, 0])  # Returns: True
```

Example #3: Load the XOR input data from a file, return only one candidate,
and use Combo output
```
output = moses.run(args="-i /path/to/input.txt -c 1")
combo_program = output[0].program
print combo_program  # Prints: and(or(!$1 !$2) or($1 $2))
```

Example #4: XOR example with Scheme output and Python input
```
input_data = [[0, 0, 0], [1, 1, 0], [1, 0, 1], [0, 1, 1]]
output = moses.run(input=input_data, scheme=True)
scheme_program = output[0].program
print scheme_program # Prints : (AndLink (OrLink (NotLink (PredicateNode "$1")) (NotLink (PredicateNode "$2"))) 
                               (OrLink (PredicateNode "$1") (PredicateNode "$2"))) 

write_scheme(output) # writes the moses results in an output file (moses_result.scm)

##
moses_result.scm

(AndLink (OrLink (NotLink (PredicateNode "$1")) (NotLink (PredicateNode "$2"))) (OrLink (PredicateNode "$1") (PredicateNode "$2"))) 
(OrLink (AndLink (NotLink (PredicateNode "$1")) (PredicateNode "$2")) (AndLink (PredicateNode "$1") (NotLink (PredicateNode "$2")))) 
(OrLink (AndLink (OrLink (NotLink (PredicateNode "$1")) (NotLink (PredicateNode "$2"))) (OrLink (PredicateNode "$1") (PredicateNode "$2"))) (AndLink (NotLink (PredicateNode "$1")) (PredicateNode "$2"))) 

##

```

Example usage of run_manually:
```
moses.run_manually("-i input.txt -o output.txt")
```
