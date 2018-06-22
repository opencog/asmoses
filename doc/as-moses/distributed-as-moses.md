Distributed AtomSpace-MOSES
===========================

MOSES is a program learner. It works by evolving islands of programs
called *demes*, each representing a subregion of the program
space. Optimizing a deme consists of searching that subregion for
programs that maximize a given fitness function. Optimizing multiple
demes is embarrassingly parallel, i.e. each deme can be created and
optimized almost in isolation w.r.t. the other demes (assuming they do
explore relatively disjoint subregions).

Once a deme has been optimized (searched till some criteria are met)
its most promising candidates are sent to the *meta-population*. The
meta-population is a population of programs that can be used to spawn
more demes. For that a selected program is turned into a template,
called an exemplar, where some parts of it can be mutated. The set of
all possible mutations of that exemplar defines the program region of
the deme.

With MOSES being ported to the AtomSpace, programs will soon be
represented as atoms. Each deme thus far explored will be stored in
its own AtomSpace and the meta-population will be an AtomSpace too.

On top of the program candidates, the fitness function (including for
instance associated data, in the case of data fitting) will have to be
copied to each deme so MOSES can evaluate each candidate.

So we would start with a centralized architecture with a master
AtomSpace containing the meta-population, and peripheral slave
AtomSpaces containing demes being optimized. Then eventually move
towards a more hierarchical architecture to avoid a bottleneck on the
master.

MOSES actually already supports such centralized distributed
architecture built around MPI. However no AtomSpace is currently used,
not yet. Program candidates are coded in MOSES' home-brewed language
called Combo, and are being exchanged back and forth between master
and slaves as strings. These exchanges though only happen at the
creation and destruction of a deme. At the creation the examplar is
sent to a slave, and at destruction promising candidates are sent back
to the master. Communications happening during deme optimization could
be useful in principle but can probably be ignored for the time being.

Input Data
----------

Both demes and the meta-population contain Atomese programs. Here are
some examples of programs

```scheme
;; f1 + f2
(Plus
  (Schema "f1")
  (Schema "f2"))

;; (p1 and p2) or p3
(Or
  (Predicate "p3")
  (And
    (Predicate "p1")
    (Predicate "p2")))

;; if p1 then f1 else f2
(IfThenElse
  (Predicate "p1")
  (Schema "f1")
  (Schema "f2")
```

where `p1`, `p2` and `p3` and boolean features and `f1` and `f2` are
numerical features of some dataset, and usual operators are high level
overloads (for instance `f1 + f2` means the sum of function `f1` and
`f2`). Obviously the average program size will be much larger than
what is presented here. In addition, though the exact form remains to
be determined, programs will be explicitely marked as members of a
deme, such that

```scheme
;; [f1 + f2] is a member of deme1
(Member
  (Plus
    (Schema "f1")
    (Schema "f2"))
  (Concept "deme1"))

;; [(p1 and p2) or p3] is a member of deme1
(Member
  (Or
    (Predicate "p3")
    (And
      (Predicate "p1")
      (Predicate "p2")))
  (Concept "deme1"))

;; [if p1 then f1 else f2] is a member of deme1
(Member
  (IfThenElse
    (Predicate "p1")
    (Schema "f1")
    (Schema "f2"))
  (Concept "deme1"))
```

Finally, the whole fitness function will have to be duplicated in each
deme AtomSpace. The format of the fitness function is to be
determined, but it might look like

```scheme
Lambda
  Variable "$P"
  <fitness>
```

where `<fitness>` could be a least squared error between `$P` and some
dataset.

Once a deme is done being optimized it will send its most promissing
candidates to the meta-population, which will look like

```scheme
;; [if p1 then f1 else f2] is a member of deme1
(Member
  (IfThenElse
    (Predicate "p1")
    (Schema "f1")
    (Schema "f2"))
  (Concept "meta-population"))
```

AtomSpace Queries/Searches
--------------------------

I guess it's premature to tell how the data will be queried and
exchanged. On can imagine queries such as the following to fetch
promising candidates

```scheme
(Get
  (And
    (Member
      (Variable "$P")
      (Concept "deme1"))
    (Evaluation
      (GroundedPredicate "is-promising")
      (Variable "$P"))))
```

Knowledge Based Properties
--------------------------

As a rough estimation, the number of nodes should generally be limited
to a dozen of thousands corresponding to the number of features in the
data to fit (the most common case for a fitness function). So nodes of
that types

```scheme
(Schema "f1")
...
```

or

```scheme
(Predicate "p1")
...
```

if the features are boolean.

The number of links however is only limited by the complexity of the
models being evolved. It is not unusual to evolve models with dozens
of operators, and since the upper limit of the number of links grows
exponentially with the number of operators, that would be going into
the billions. In practice though, because evaluating the fitness
function on each candidate is so costly, deme optimization would
rarely goes above millions of candidates, meaning likely millions of
atoms, because candidates share most of their atoms. Given that an
atom takes in average about 1.5K of RAM a million atoms would take
about 1.5GB of RAM.

The requirements for the meta-population are probably similar.

There can be extra RAM needed if fitness evaluation memoization is
used, but that's another problem. It is expected that the fitness
function (including data) will be negligible compared to the deme
population and meta-population.

Actual Databases and Test Scripts
---------------------------------

The AtomSpace MOSES port being in its infancy there is no existing
code to test that. MOSES which is itself however quite mature can be
found here https://github.com/opencog/as-moses (if that helps).
