(define plus-zero-rule
  (DefinedSchemaNode "plus-zero-rule-name"))

(DefineLink
  plus-zero-rule
  (BindLink
    (GlobNode "$op")
    (ChoiceLink
      (PlusLink
        (GlobNode "$op")
        (NumberNode 0))
      (PlusLink
        (NumberNode 0)
        (GlobNode "$op")))
    (ReductToLink
      ;; I am not sure this is the right way
       ; to do it, but will do for now.
      (ChoiceLink
        (PlusLink
          (GlobNode "$op")
          (NumberNode 0))
        (PlusLink
          (NumberNode 0)
          (GlobNode "$op")))
      (PlusLink (GlobNode "$op")))))
