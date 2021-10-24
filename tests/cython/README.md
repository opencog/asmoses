
HOWTO run these tests by hand:
------------------------------
This works:

```
cd build/tests/cython
nosetests3 -vs ../../../tests/cython/moses/
```

XXX The instructions below are not quite right, because the current
build infrastructure is placing the cython files into wacky directories
that are inconsistent with the module naming conventions. This needs to
be fixed.

You need to set up the PYTHON path:
```
export PYTHONPATH=build/opencog/asmoses/cython
```
If installed, try
```
export PYTHONPATH=/usr/local/lib/python3/dist-packages
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
