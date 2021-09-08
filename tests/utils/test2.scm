(Concept "f1" (stv 0.6 1))
(Concept "f2" (stv 0.5 1))
(Concept "f3" (stv 0.4 1))
(Concept "f4" (stv 0.3 1))
(Concept "f5" (stv 0.2 1))
(Concept "f6" (stv 0.2 1))
(Concept "f7" (stv 0.45 1))
(Concept "f8" (stv 0.02 1))
(Concept "f9" (stv 0.68 1))
(Concept "f10" (stv 0.81 1))
(Concept "f11" (stv 0.95 1))
(Concept "f12" (stv 0.01 1))
(Concept "f13" (stv 0.75 1))

(Implication (stv 0.9 1)
   (Concept "f3" (stv 0.4 1))
   (Concept "f4" (stv 0.3 1)))

(Implication (stv 0.6 1)
    (And
        (Concept "f1" (stv 0.6 1))
        (Concept "f2" (stv 0.5 1)))
   (Concept "f5" (stv 0.2 1))
)

(Concept "f7" (stv 0.45 1))

(Implication (stv 0.8 1)
    (Concept "f1" (stv 0.6 1))
    (Concept "f11" (stv 0.95 1)))

(Implication (stv 0.5 1)
    (Concept "f9" (stv 0.68 1))
    (Concept "f8" (stv 0.02 1)))

(Implication (stv 0.4 1)
    (And
        (Concept "f6" (stv 0.2 1))
        (Concept "f10" (stv 0.81 1)))
    (Concept "f12" (stv 0.01 1)))

(Implication (stv 0.25 1)
   (Concept "f13" (stv 0.75 1))
   (Concept "f3" (stv 0.4 1)))

