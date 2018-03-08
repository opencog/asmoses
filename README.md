# AS-MOSES

AS-MOSES is a port of MOSES (Meta-Optimizing Semantic Evolutionary
Search) for the AtomSpace.

## Motivation

The main purpose of porting MOSES to the AtomSpace is to facilitate
the synergies between MOSES and the rest of OpenCog. This should allow
to

1. Invoke MOSES with a problem description living in the AtomSpace and
   have it output its results to the AtomSpace as well.
3. Have MOSES' search take place in the AtomSpace to be subsequently
   analyzed by other components, or itself.
2. Have MOSES capable of using available knowledge from other
   components, or itself, to guide its search.
4. Ultimately, given a sufficiently modular implementation of
   AS-MOSES, experiment with various hybridizations. If AS-MOSES can,
   eventually, be wrapped in a manner as being an Atomese program,
   such a program can be suject to self-improvements.

## MOSES Recall

Here we briefly recall important aspects of MOSES. That doesn't mean
that all aspects have to be faithfully ported to AS-MOSES, but they
should be known to understand the port process.

### Deme

A deme is a population of programs covering a certain a region of the
program space.

### Meta-population

A meta-population is a population of demes.

### Deme Management

Deme management is the process of creating and destroying demes, as
well as selecting which deme to explore for subsequent optimization.

### Optimization

Optimization is the process of exploring a deme to find fit programs.

MOSES offers different optimization algorithms, such as Estimation of
Distribution Algorithm (EDA), Simulation Annealing, Stochastic
Hillclimding and Crossover.

### Exemplar

An exemplar is a program from which a deme may be spanwed. Thus the
meta-population can typically be seen as a collection of exemplars.

### Knob

A knob is a set of discrete or continuous positions. Turning a knob
amounts to selecting a position. Each position corresponds to a term
of the programming language used for learning. For instance the knob

```
[0, 0.5, 1]
```

has 3 discrete positions, the first one corresponds to the term "0",
the second to the term "0.5", and the last one corresponds to "1".

### Representation

By placing knobs at various places of an exemplar one may define a
region of the program space, i.e a deme. First, these knobs may define
a vector space, each point in that space corresponding to a certain
configuration of knob positions. Second, each point in that vector
space can be turned into a program, a variation of the exemplar. The
way knobs are placed on an exemplar, is called a representation. The
goal of course is to build a representation such that the associated
program subspace will contain good candidates, or at least good
exemplars, and will be efficient to explore.

For example given the exemplar

```
f(x) = x + 0.5
```

one may build the representation

```
f(x) = [1, -1]*x + [0, 0.5, 1]
```

where the term 0.5 has been replaced by the knob [0, 0.5, 1] and an
extra knob has been added next to multiply x. Such representation
defines the program subspace

```
{ f(x) = 1*x + 0,
  f(x) = 1*x + 0.5,
  f(x) = 1*x + 1,
  f(x) = -1*x + 0,
  f(x) = -1*x + 0.5,
  f(x) = -1*x + 1 }
```

### Syntax vs Semantics

The syntax of a program is the sequence (or tree) of symbols that
defines it. Its semantics is the mathematical function it represents,
that is the mapping between inputs and outputs.

### Program Reduction

Reduction, or normalization, is the process of transforming a program
into an equivalent canonical form. For instance the program

```
f(x) = 1*x + 0
```

is equivalent to

```
f(x) = x
```

Reduction can save resources as it may prevent from evaluating and
storing equivalent redundant programs. If done well it may even
simplify the fitness landscape, making it easier to explore, by for
instance increasing the correlation between syntax and
semantics. Indeed, usually, the chaotic aspect of the fitness
landscape lies between syntax and semantics rather than between
semantics and fitness.

### Multidimensional Fitness and Diversity

MOSES offers the possibility to split the fitness into features. For
instance the fitness of an athlete could be split into 3 features,
Strength, Speed and Endurance. This can be used during deme management
to obtain a measure of diversity between exemplars and incentivize the
exploration of diverse demes. Likewise it can be used to reduce the
meta-population to its Pareto front, which itself can save RAM and
increase diversity. Of course the effectiveness of this entirely
depends on what features consistute the fitness.

### Combo

Combo is the home-brewed programming language, initially created by
Moshe Looks, used to represent program candidates. Combo has been
described, by its creator, as "LISP with a bad haircut".

### Others

There are many other aspects that have been left out, such as feature
selection, boosting, Occam's razor, etc. As important as they are in
practice they do not really constitute what makes MOSES special and
probably do not need special attention for the port itself.

### PLOP

the original author of MOSES, Moshe Looks, has developped a LISP
version of MOSES, called PLOP

https://code.google.com/archive/p/plop/

it would probably be wise to familiarize ourselves with it as it might
contain interesting ideas to port as well.

## Port

In this Section we consider aspects of MOSES and offer suggestions of
how they might be ported to AS-MOSES.

### Learning Language

MOSES uses Combo as programming language to represent program
candidates. AS-MOSES should use Atomese, OpenCog's main language for
interfacing with the AtomSpace. An important aspect of Atomese is that
a program is itself an atom, making it a first class citizen for
OpenCog.

### Program Transformation

It will likely use the pattern matcher or URE to transform it's
programs, it's unclear at that point though what will guide the
evolution, it could be the URE itself as it possesses some limited
form of control.
