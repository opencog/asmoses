(define and-identity-true
  (DefinedSchemaNode "and-identity-true"))

(define or-identity-true
  (DefinedSchemaNode "or-identity-true"))

(define and-identity-false
  (DefinedSchemaNode "and-identity-false"))

(define or-identity-false
  (DefinedSchemaNode "or-identity-false"))

(define or-identity
  (DefinedSchemaNode "or-identity"))

(define and-or-identity
  (DefinedSchemaNode "and-or-identity"))

(define or-and-identity
  (DefinedSchemaNode "or-and-identity"))

(define double-not
  (DefinedSchemaNode "double-not"))

(define distribuitive-and
  (DefinedSchemaNode "distribuitive-and"))

(define distribuitive-or
  (DefinedSchemaNode "distribuitive-or"))


;; and(true X)->X
(DefineLink
 and-identity-true
   (BindLink
   (Glob "$op")
     (Present
     (AndLink
       (Glob "$op")
       (True)))

     (ReducedTo
     (AndLink
     (Glob "$op")
     (True))
     (Glob "$op"))))


;; or(true X)->true
(DefineLink
 or-identity-true
   (BindLink
     (TypedVariableLink
      (Glob "$op")
      (TypeChoice
      (TypeNode "TrueLink")
      (TypeNode "FalseLink")))

     (Present
     (OrLink
       (Glob "$op")
       (True)))

     (ReducedTo
     (OrLink
     (Glob "$op")
     (True))
     (True))))


;; and(false X)->false
(DefineLink
 and-identity-false
   (BindLink
     (TypedVariableLink
      (Glob "$op")
      (TypeChoice
      (TypeNode "TrueLink")
      (TypeNode "FalseLink")))

     (Present
     (AndLink
       (Glob "$op")
       (False)))

     (ReducedTo
     (AndLink
     (Glob "$op")
     (False))
     (False))))

;; or(false X)->X
(DefineLink
 or-identity-false
   (BindLink
   (TypedVariableLink
      (Glob "$op")
      (TypeChoice
      (TypeNode "TrueLink")
      (TypeNode "FalseLink")))

     (Present
     (OrLink
       (Glob "$op")
       (False)))

     (ReducedTo
     (OrLink
     (Glob "$op")
     (False))
     (Glob "$op"))))


;; or(X)->X
(DefineLink
 or-identity
   (BindLink
     (TypedVariableLink
     (Glob "$op")
     (TypeChoice
     (TypeNode "TrueLink")
     (TypeNode "FalseLink")))
     (Present
     (OrLink
       (Glob "$op")))

     (ReducedTo
     (OrLink
     (Glob "$op"))
     (Glob "$op"))))


;; and(or X) -> and(X)
(DefineLink
  and-or-identity
   (BindLink
     (TypedVariableLink
          (Glob "$op")
          (TypeChoice
          (TypeNode "TrueLink")
          (TypeNode "FalseLink")))

     (Present
     (AndLink
       (OrLink
         (Glob "$op"))))

     (ReducedTo
     (AndLink
      (OrLink
        (Glob "$op")))

     (AndLink
      (Glob "$op")))))


;; or(and X) -> or(X)
(DefineLink
  or-and-identity
   (BindLink
     (TypedVariableLink
          (Glob "$op")
          (TypeChoice
          (TypeNode "TrueLink")
          (TypeNode "FalseLink")))
     (Present
     (OrLink
       (AndLink
         (Glob "$op"))))

     (ReducedTo
     (OrLink
      (AndLink
        (Glob "$op")))

     (OrLink
      (Glob "$op")))))


;; !!a->a,
 (DefineLink
   double-not
    (BindLink
      (TypedVariableLink
           (Variable "x")
           (TypeChoice
           (TypeNode "TrueLink")
           (TypeNode "FalseLink")))
      (Present
      (NotLink
        (NotLink
          (Variable "x"))))

      (ReducedTo
      (NotLink
       (NotLink
         (Variable "x")))
       (Variable "x"))))


;; !(a&&b)->(!a||!b)
 (DefineLink
   distribuitive-and
    (BindLink
    (VariableList
      (TypedVariableLink
           (Variable "a")
           (TypeChoice
           (TypeNode "TrueLink")
           (TypeNode "FalseLink")))

      (TypedVariableLink
           (Variable "b")
           (TypeChoice
           (TypeNode "TrueLink")
           (TypeNode "FalseLink"))))

      (Present
      (NotLink
        (AndLink
          (Variable "a")
          (Variable "b"))))

      (ReducedTo
      (NotLink
       (AndLink
         (Variable "a")
         (Variable "b")))

       (OrLink
         (NotLink
         (Variable "a"))
         (NotLink
         (Variable "b"))))))


;; !(a||b)->(!a&&!b)
(DefineLink
   distribuitive-or
    (BindLink
    (VariableList
        (TypedVariableLink
             (Variable "a")
             (TypeChoice
             (TypeNode "TrueLink")
             (TypeNode "FalseLink")))

        (TypedVariableLink
             (Variable "b")
             (TypeChoice
             (TypeNode "TrueLink")
             (TypeNode "FalseLink"))))

      (Present
      (NotLink
        (OrLink
          (Variable "a")
          (Variable "b"))))

      (ReducedTo
      (NotLink
       (AndLink
         (Variable "a")
         (Variable "b")))

       (AndLink
         (NotLink
         (Variable "a"))
         (NotLink
         (Variable "b"))))))



