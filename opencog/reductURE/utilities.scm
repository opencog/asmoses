; taken from pln
(use-modules (srfi srfi-1))

(use-modules (opencog))
(use-modules (opencog exec))
(use-modules (opencog rule-engine))

; ----------------------------------------------------------------------------
(define-public (reduct-load-rules RULE-TYPE)
"
  reduct-load-rules RULE-TYPE

  Loads the different variations of the rules known by RULE-TYPE. A RULE-TYPE
  may include the categorization of the rule. For example, 'term/deduction'
  implies that the rule to be loaded is the term-logic deduction rule.
"
  ; NOTE:
  ; 1. If a rule needs formula defined in formulas.scm then the rule file
  ;    should load it.
  ; 2. Rule files are assumed to be named as "RULE-TYPE.scm"
  ; 3. load-from-path is used so as to be able to use build_dir/opencog/scm,
  ;    even when the module isn't installed.

  (load-from-path (string-append "opencog/reductURE/rules/" RULE-TYPE ".scm"))
)

(define-public reduct-atomspace (cog-new-atomspace))

(define-public (reduct-mk-rb)
"
  Create

  (Concept \"reduct-rb\")
"
  (cog-new-node 'ConceptNode "reduct-rb")
)

(define-public (reduct-rb)
"
  Get

  (Concept \"reduct-rb\")

  from reduct-atomspace
"
  (define prev-as (cog-set-atomspace! reduct-atomspace))
  (define reduct-atomspace-rb (reduct-mk-rb))
  (cog-set-atomspace! prev-as)
  reduct-atomspace-rb)

(define-public (reduct-load)
"
  Load and configure the reduct rule base.
"
  ;; Switch to reduct atomspace
  (define prev-as (cog-set-atomspace! reduct-atomspace))

  ;; Load rule files
  (reduct-load-rules "temp")
  (reduct-load-rules "contin-rules")

  ;; Attach rules to reduct rule-base
  (ure-add-rules-by-names
   (reduct-mk-rb)
   (list
   ;; list rules in rule files
    plus-zero-rule
    ))

  ;; Switch back to previous space
  (cog-set-atomspace! prev-as)

  ;; Avoid confusing the user with a return value
  *unspecified*)

(define-public (reduct-prt-atomspace)
"
  Print all reduct rules loaded in reduct-atomspace
"
  (define prev-as (cog-set-atomspace! reduct-atomspace))
  (cog-prt-atomspace)
  (cog-set-atomspace! prev-as)

  ;; Avoid confusing the user with a return value
  *unspecified*)

(define-public (reduct-rm-rules-by-names rule-names)
  (define prev-as (cog-set-atomspace! reduct-atomspace))
  (ure-rm-rules-by-names (reduct-mk-rb) rule-names)
  (cog-set-atomspace! prev-as)

  *unspecified*)

(define-public (reduct-fc . args)
"
  Wrapper around cog-fc using (reduct-rb) as rule base.

  See (help cog-fc) for more info.
"
  (apply cog-fc (cons (reduct-rb) args)))
