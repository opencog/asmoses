# AS-MOSES

AS-MOSES is a port of MOSES (Meta-Optimizing Semantic Evolutionary
Search) for the AtomSpace.

## Motivation

The main purpose of porting MOSES to the AtomSpace is to facilitate
the synergies between MOSES and the rest of OpenCog. This should allow
to

1. Invoke AS-MOSES with a problem description living in the AtomSpace
   and have it output its results to the AtomSpace as well.
3. Have AS-MOSES' search take place in the AtomSpace to be
   subsequently analyzed by other components.
2. Have AS-MOSES capable of using available knowledge from other
   components to guide its search.
4. Ultimately, given a sufficiently modular implementation of
   AS-MOSES, experiment with various hybridizations. If AS-MOSES can,
   eventually, be wrapped in a manner as being an Atomese program,
   such a program can be subject to self-improvements.

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
Hillclimbing and Crossover.

### Exemplar

An exemplar is a program from which a deme may be spawned. Thus a
meta-population can be regarded as a collection of exemplars.

### Knob

A knob is a set of discrete or continuous positions. Turning a knob
amounts to selecting a position. Each position corresponds to a term
of the programming language used for learning. For instance the knob

```
[0, 0.5, 1]
```

has 3 discrete positions, the first one corresponds to the term `0`,
the second to the term `0.5`, and the last one corresponds to `1`.

The setting of a knob is the term corresponding to its current
position.

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

one may build the representation (also called prototype)

```
f(x) = [1, -1]*x + [0, 0.5, 1]
```

where the term 0.5 has been replaced by the knob [0, 0.5, 1] and an
extra knob has been added to be multiplied by x. Such representation
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
defines it. Its semantics is the mathematical function it calculates,
that is a mapping between inputs and outputs.

### Program Reduction

Reduction, or normalization, is the process of transforming a program
into an semantically equivalent canonical form. For instance the
program

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

MOSES allows to split the fitness measure into features. For instance
the fitness of an athlete could be split into 3 features, Strength,
Speed and Endurance. This can be used during deme management to obtain
a measure of diversity between exemplars and incentivize the
exploration of diverse demes. Likewise it can be used to reduce the
meta-population to its Pareto front, which itself can save RAM and
increase diversity. Of course the effectiveness of this depends on the
choice of the features.

### Combo

Combo is the home-brewed programming language, created by Moshe Looks,
to represent program candidates. Combo has been described, by its
creator, as "LISP with a bad haircut".

### Others

There are many other aspects that have been left out, such as feature
selection, boosting, Occam's razor, etc. As important as they are in
practice they do not really constitute what makes MOSES special and
probably do not need special attention for the port.

### PLOP

the original author of MOSES, Moshe Looks, has developed a LISP
version of MOSES, called PLOP

https://code.google.com/archive/p/plop/

it would probably be wise to familiarize ourselves with it as it might
contain interesting ideas to port as well.

## Port

In this Section we offer suggestions of how these aspects of MOSES
could be ported to AS-MOSES.

### Learning Language

MOSES uses Combo as programming language to represent program
candidates. AS-MOSES should use Atomese, OpenCog's main language for
interfacing with the AtomSpace. An important aspect of Atomese is that
a program is an atom, thus a first class citizen of OpenCog.

For instance the following program

```
f(x) = x + 0.5
```

can be represented in Atomese by

```scheme
(Lambda
  (Variable "$x")
  (Plus
    (Variable "$x")
    (Number 5)))
```

### Representation and Program Transformation

We likely want use the pattern matcher to transform a representation
(i.e. prototype) into a program candidate. For instance, the following
prototype:

```
f(x) = [1, -1]*x + [0, 0.5, 1]
```

may be represented in Atomese by

```scheme
(Quote
  (Lambda
    (Variable "$x")
    (Plus
      (Times
        (Unquote (Variable "$k0"))
        (Variable "$x"))
      (Unquote (Variable "$k1")))))
```

where `$k0` and `$k1` are the knob variables for knobs `[1, -1]` and
`[0, 0.5, 1]`.

One then can use `PutLink` to generate candidates. For instance,
executing the following (where `$k1` has been set to `1` and `$k2` has
been set to `0`)

```scheme
(Put
  (VariableList
    (Variable "$k0")
    (Variable "$k1"))
  (Quote
    (Lambda
      (Variable "$x")
      (Plus
        (Times
          (Unquote (Variable "$k0"))
          (Variable "$x"))
        (Unquote (Variable "$k1")))))
  (List (Number 1) (Number 0))
```

produces

```scheme
(Lambda
  (Variable "$x")
  (Plus
    (Times
      (Number 1)
      (Variable "$x"))
    (Number 0)))
```

corresponding to candidate

```
f(x) = 1*x + 0
```

### Program Reduction

The URE should be able to handle reduction. There are different ways
to go about that, the current way (currently in the work by Yidne) is
to explicitly construct the normalization relationship between
programs, such as

```scheme
(Evaluation (stv 1 1)
  (Predicate "reduce-to")
  (List
    (Lambda
      (Variable "$x")
      (Plus
        (Times
          (Number 1)
          (Variable "$x"))
        (Number 0)))
    (Lambda
      (Variable "$x")
      (Variable "$x"))
```

meaning that

```
f(x) = 1*x + 0
```

reduces to the identity

```
f(x) = x
```

All that is required is the axiomatization of predicate `reduce-to`
and operators `Plus`, `Times`, etc, in particular their algebraic
properties.

### Representation Building

Deciding how to build a representation such as

```
f(x) = [1, -1]*x + [0, 0.5, 1]
```

is not trivial at all. We want to avoid hard-wiring anything. Ideally
it should be handled as a form of reasoning, surely with some initial
restrictions or hand-crafted control rules, to avoid utter
inefficiency, but eventually should be let free. That is because the
representation associated to a deme can have a massive impact on its
optimization quality. And also the knowledge needed to build a good
representation are for the most part unknown. In practice
representation building is driven by heuristic rules. What we want is
to learn these heuristics.

### Deme Management

Deme management is also a hard problem, and should also probably be
handled as a form of reasoning.

### Optimization

Like deme management and representation building, optimization could
be handled by reasoning. Indeed, we only want to go through the effort
of evaluating candidates that we have reasons to believe to be
fit. These reasons are usual hardwired in the optimization algorithm
in the form of heuristics. We can choose instead to make these
heuristics explicit by representing them as axioms and see the
optimization process as construction of proofs of fitness.

The axioms should capture the inherent uncertainties of these
heuristics, and PLN inference properly propagate these
uncertainties. Fitness evaluation may also be seen as a form of
reasoning usually resulting in higher levels of certainty of
fitness. However it might sometimes be preferable if fitness
evaluation did not produce high certainty results, such as for
instance the case of curve fitting with a noisy observations.

This, seeing optimization as reasoning, also allows to integrate
background knowledge seamlessly. Of course that doesn't tell us how to
efficiently control it (such integration would likely result in
greater levels of combinatorial explosion) but the hope is that this
can be learned overtime, and eventually it could surpass the initial
performances.

#### Example of Axiomatization

Let's take one of the simplest optimization algorithm, Hillclimbing,
and see how it can be axiomatized to be implemented as a reasoning
process.

##### Hillclimbing Recall

Let's first recall how Hillclimbing works, in particular the flavor
implemented in MOSES.

Initialization:

* Let be a discrete multidimensional space of programs, defined by a
  representation (knobs place on the exemplar).
* Let C be point of that space, set to the exemplar.
* Let D be the distance from C, set to 1.
* Let N be the maximum number of candidates to evaluate during a given
  iteration.

Algorithm:

1. Sample and evaluation N candidates at distance D from C.
2. If a better candidate has been discovered, assign it to C,
   otherwise increment D.
3. If a termination criterion has been met stops, otherwise go to 1.

##### Hillclimbing Axiomatization

Let us now attempt to capture the assumptions that make this algorithm
worth using. We have introduced some arbitrary truth values to convey
how uncertainty can be represented as well.

* (1) Candidates tend to be unfit
  ```
  Implication (stv 0.01 0.01)
    Predicate "candidate"
    Predicate "fitness"
  ```
  where predicate `candidate` is a Boolean predicate that indicate if
  an atom is a program candidate, the second predicate `fitness`
  measures its fitness.

* (2) Syntactically similar candidates tend to be loosely semantically
  similar
  ```
  Implication (stv 0.4 0.01)
    Predicate "similar-syntax"
    Predicate "similar-semantics"
  ```
  where predicate `similar-syntax` is a binary predicate that
  evaluates how syntactically similar 2 candidates are. Predicate
  `similar-semantics` is a binary Predicate that evaluates how
  semantically similar 2 candidates are.

* (3) Semantically similar candidates tend to have similar fitnesses
  ```
  Implication (stv 0.6 0.1)
    Predicate "similar-semantics"
    Predicate "similar-fitness"
  ```
  where predicate `similar-fitness` is a binary predicate that
  evaluates how similar the fitnesses of 2 candidates are.

* (4) Candidates with similar knob settings tend to be syntactically
  similar
  ```
  Implication (stv 0.8 0.2)
    Predicate "similar-knob-settings"
    Predicate "similar-syntax"
  ```
  where predicate `similar-knob-settings` is a binary predicate that
  evaluates how similar the knob settings of 2 candidates are.

* (5) If candidate P1 and P2 have similar fitnesses, and P1 has fitness
  f1, then P2 has a fitness close to f1
  ```
  ImplicationScope (stv 1 1)
    $P1, $P2
    And
      Evaluation
        Predicate "similar-fitness"
        List
          $P1
          $P2
      Evaluation
        Predicate "fitness"
        $P1
    Evaluation
      Predicate "fitness"
      $P2
  ```

#### Possible Emulation of Hillclimbing with Reasoning

Given these axioms, one may hand the following query to the URE with
the objective to maximize the TV strength and confidence of the
following

```
Evaluation
  Predicate "fitness"
  $X
```

thus finding instances of `$X`, program candidates with high
fitnesses.

Here's an example of reasoning that might results from these axioms.

1. Assume we have a initial candidate to begin with

```
Evaluation TV
  Predicate "fitness"
  <initial-candidate>
```

if the TV is lower than 0.01 (the average fitness as determined by
axioms (1)), then the URE will have an incentive to use axiom (1) to
produce a random candidate, since we have nothing better to do at that
point. At some point it will have generated a candidate above the
expected average fitness that can be used as initial candidate.

Using axiom (5) and partial conditional instantiation may produce

```
ImplicationScope
  $P2
  And
    Evaluation
      Predicate "similar-fitness"
      List
        <initial-candidate>
        $P2
    Evaluation
      Predicate "fitness"
      <initial-candidate>
  Evaluation
    Predicate "fitness"
    $P2
```

Applying the fuzzy conjunction rule on the implicant will lead to the
sub-target

```
Evaluation
  Predicate "similar-fitness"
  List
    <initial-candidate>
    $P2
```

At this point the URE can chain axioms (4), (2) and (3), in that
order, using the deduction rule to produce the knowledge that a
candidate with similar knob settings (that is local neighbors) will
likely produce candidates with similar fitnesses

```
Implication
  Predicate "similar-knob-settings"
  Predicate "similar-fitness"
```

Combined with conditional instantiation rule will likely result in
estimating that a nearby candidate has a similar fitness the initial
candidate, which is higher than average, thus worthwhile evaluating.

As soon as a better candidate than the initial one is evaluated, the
same reasoning will happen on the better candidate, now the new center
in the Hillclimbing algorithm. If no better candidate has been found,
then the URE will want to attempt to instantiate that later with less
similar knob settings as the center, following again the behavior of
the variation of Hillclimbing described here.

So we've seen that axiomatizing Hillclimbing and turning it into a
reasoning process is not particularly complicated. It is expected that
axiomatizing an EDA algorithm would be even more natural, due to the
fact that the URE supports well uncertain reasoning.

### Custom Policy

Although it's been shown that in principle a learning can be managed
as a form of reasoning, it's not clear at all that it would be
effective in practice. There are at least 2 ways we can counter that

1. Designing control rules to guide the URE (see TODO: add reference
   to documentation about inference control in the URE once available)
2. Craft an algorithm by gluing together more elementary calls from
   the URE (like unifying and applying rules), to control exactly how
   and in which order rules are being chained and executed. This would
   allow to maintain the same sort of freedom conventional programming
   offers, while still making transparent the reasoning part. This
   requires to expose URE primitives to Scheme, which shouldn't be
   difficult.

The first option (designing control rules) is preferable in order to
seamlessly integrate background knowledge and teach how to control
AS-MOSES. The second option could be a good middle ground to move
AS-MOSES closer to reasoning while still retaining its original
efficiency. Interestingly both allow self-improvements to take
place. In the first case inference control rules can be learned. In
the second one AS-MOSES would be an Atomese program anyway, so could
be improved by AS-MOSES itself.
