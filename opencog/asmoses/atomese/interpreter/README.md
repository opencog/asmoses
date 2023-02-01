
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


It should work like this:
https://github.com/opencog/atomspace/blob/master/examples/atomspace/flows.scm
formulas.scm flows.scm flow-formulas.scm

Demo:
```
(use-modules (opencog) (opencog exec))


(define foo (Concept "foo"))
(define bar (Concept "bar"))
(define key (Predicate "some key"))
(define kee (Predicate "other key"))

(cog-set-value! foo key (FloatValue 0 1 0 1 0))
(cog-set-value! bar kee (FloatValue 1 1 0 1 1))

(cog-execute! (ValueOf foo key))

(cog-execute! (Or (ValueOf foo key) (ValueOf bar kee)))
```
