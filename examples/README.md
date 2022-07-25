
AS-MOSES Examples
=================
AS-MOSES can be used in one of three ways: 
* Directly from the command line,
* By using AtomSpace Atomese interfaces,
* Embedding the low-level API into C++ programs.

For many users, the command-line interface is strongly recommended.

For users interested in integrating with other AtomSpace processes,
the Atomese interfaces are required.

Command-line examples
---------------------
Toe run MOSES from the command line, it is enough to just specify a
dataset, and the correct flags to process that dataset. Examples
can be found here:

* [datasets](./datasets): Simple data sets on which moses can be run.

Python programming API examples
-------------------------------
An example of invoking MOSES from python can be found in the
[python examples(./python) directory.

Scheme and Atomese examples
---------------------------
Examples of uses MOSES to work with Atomese can be found in the
[atomese examples(./atomese) directory.

C++ programming API examples
----------------------------
These must first be built by saying (in the build dir):
```
    make examples
```
The available examples are:
* [ant-trails](./ant-trails): Santa Fe Trail ant example
* [c++-api](./c++-api): Example C++ programs.
