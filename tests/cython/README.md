
HOWTO run these tests by hand:
------------------------------

You need to set up the PYTHON path:
```
export PYTHONPATH=build/opencog/asmoses/cython
```

You may also need to specify the library path:
```
export LD_LIBRARY_PATH=build/asmoses/cython/opencog
```

Then, from the project root directory:
```
nosetests -vs tests/cython/moses/
```

If you modify the cython bindings, you may need to manually remove
some build files to get a clean rebuild.  Basically, the CMakefiles
for cython/python are buggy, and fail to rebuild when changes are made.
So, for example:
```
rm build/opencog/asmoses/cython/opencog/pyasmoses.cpp
```
