(Predicate "f1" (stv 0.3 1))
(Predicate "f2" (stv 0.2 1))
(Implication (stv 0.6 1)
   (Predicate "f2" (stv 0.2 1))
   (Predicate "f3"))

(Implication (stv 0.25 1)
   (Not
        (Predicate "f2" (stv 0.2 1)))
   (Predicate "f3"))


(Implication (stv 0.2 1)
    (And
        (Predicate "f1" (stv 0.3 1))
        (Predicate "f2" (stv 0.2 1)))
   (Predicate "f4"))


(Implication (stv 0.4 1)
    (Not
       (And
         (Predicate "f1" (stv 0.3 1))
         (Predicate "f2" (stv 0.2 1))))
   (Predicate "f4"))

(Implication (stv 0.5 1)
       (And
         (Not (Predicate "f1" (stv 0.3 1)))
         (Predicate "f2" (stv 0.2 1)))
   (Predicate "f4"))

(Implication (stv 0.1 1)
       (And
         (Predicate "f1" (stv 0.3 1))
         (Not (Predicate "f2" (stv 0.2 1))))
   (Predicate "f4"))

(Implication (stv 0.1 1)
    (Predicate "f4")
    (Predicate "f5"))

(Implication (stv 0.4 1)
    (Not (Predicate "f4"))
    (Predicate "f5"))