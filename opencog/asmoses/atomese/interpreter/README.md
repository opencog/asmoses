
Issue
=====
The interpreter needs to be redesigned (eliminated). Atomese is
already interpreted, it does not need to be done a second time.

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

