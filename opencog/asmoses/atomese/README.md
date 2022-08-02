Atomese Design Notes
====================
The code here implements a version of MOSES that can act directly on
the AtomSpace. The design used here is in a state of flux; the first
attempt at doing this represented all data as Atoms in teh AtomSpace.
A new design is underway that places data as Atomese Values hanging
on Atoms.

There are multiple reasons why the use of Values is superior to Atoms.
* Values are significantly faster than Atoms, and require less RAM.
  Both access and insertion are faster.
* Values hold vectors of floats, strings, bools or more complex
  structures, and are thus well suited for holding columns from a table,
  or for working with streaming data (from sensory inputs)
* Atomese provides a built-in interpreter for working with Values:
  Atomese provides assorted arithmetic (`PlusLink`, `MinusLink`,
  `TimesLink`, `DivideLink`) and math operators for `FloatValue`s:
  (`Log2Link`, `SineLink`, etc.)  The `BoolAnd`, `BoolOrLink` provide
  boolean operators on `BoolValue`s.
* The above system is extensible, in that new data types can be added
  (e.g. for handling sensory data, chemistry, genetics, etc.) without
  any need to alter anything in MOSES or the core AtomSpace. The new
  types can be created in independent projects.

Values are different from Atoms in th following ways:
* Values are not placed in the AtomSpace; thus, they cannot be searched.
  Thats OK, because nothing in MOSES requires a search of tabular data.
  By not placing them in the Atomspace, the CPU and RAM overhead of
  creating the AtomSpace indexes is eliminated. This overhead can be quite
  large.


Issues & TODO Items
===================
The Interpreter in this directory needs to be redesigned (and mostly
eliminated). Atomese is already interpreted (and sometimes quasi-
compiled), it does not need to be done a second time.

Design Notes
------------
Sample of what it currently receives:

From ASMOSESUTest:
```
(AndLink
  (OrLink
    (PredicateNode "$2")
    (AndLink
      (PredicateNode "$3")
      (PredicateNode "$1")))
  (OrLink
    (PredicateNode "$3")
    (PredicateNode "$1")))
```

This appears to be the ONLY unit test that hits the interpreter...

What it should be generating:

```
(BoolAnd
	(BoolOr
		(BoolValueOf (Concept "some dataset") (Predicate "$2"))
		(BoolAnd
			(BoolValueOf (Concept "some dataset") (Predicate "$3"))
			(BoolValueOf (Concept "some dataset") (Predicate "$1")))
	(BoolOr
		(BoolValueOf (Concept "some dataset") (Predicate "$3"))
		(BoolValueOf (Concept "some dataset") (Predicate "$1")))))
```

Note that this is almost the same, except that the name of the dataset
is explicitly specified, as well as the column name in that dataset.
Note also BoolAnd and BoolOr are used instead of And, Or. This is because
the BoolAnd and BoolOr are able to handle vectors of bools, (i.e. columns
of bools) whereas the regular AndLink & OrLink cannot.

Dead code
---------
The following appears to be dead code:

* `data/representation/load_table.h` and .cc too ... the only user
  is `load_tableUTest` ...
