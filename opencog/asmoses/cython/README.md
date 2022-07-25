Python bindings for OpenCog
---------------------------

## Requirements ##

* Python 3.4 - these bindings may work with earlier versions,
  but they have not been tested at all.
* Cython 0.19 or later. http://www.cython.org/
* Nosetests3 - for running unit tests.

Both Cython and Nosetests can be installed with easy_install:

 sudo easy_install cython nose3

The bindings are written mostly using Cython, which allows writing
code that looks pythonic but gets compiled to C.  It also makes it
trivial to access both Python objects and C objects without using a
bunch of extra Python library API calls.

## Setting up ##

Go through the normal process of [[building MOSES]]. Then ensure that
the MOSES data directory is in your Python `sys.path`. By
default, the moses python wrapper is installed at
`/usr/local/lib/python3.9/dist-packages/opencog/` when you do
`make install`. You can modify your `PYTHONPATH` environment variable
to ensure Python checks this location. If you
just want to use your build dir you can use something like:

```
 $ export PYTHONPATH=$PYTHONPATH:/usr/local/lib/python3.9/dist-packages/opencog/:~/src/moses/build/moses/cython
```
