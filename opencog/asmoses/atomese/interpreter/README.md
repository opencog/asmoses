
Issue
=====
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
