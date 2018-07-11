(define boolean_data_repr_unfolded
 (Set
  (List
   (Execution (Schema "o") (Node "r0") (True))
   (Execution (Schema "i1") (Node "r0") (False))
   (Execution (Schema "i2") (Node "r0") (True)))

  (List
   (Execution (Schema "o") (Node "r1") (True))
   (Execution (Schema "i1") (Node "r1") (True))
   (Execution (Schema "i2") (Node "r1") (False)))

  (List
   (Execution (Schema "o") (Node "r2") (False))
   (Execution (Schema "i1") (Node "r2") (False))
   (Execution (Schema "i2") (Node "r2") (False)))
 ))

(define boolean_data_repr_unfolded_eval
 (Set
  (List
   (Evaluation (stv 1 1) (Predicate "o") (Node "r0"))
   (Evaluation (stv 0 1) (Predicate "i1") (Node "r0"))
   (Evaluation (stv 1 1) (Predicate "i2") (Node "r0")))

  (List
   (Evaluation (stv 1 1) (Predicate "o") (Node "r1"))
   (Evaluation (stv 1 1) (Predicate "i1") (Node "r1"))
   (Evaluation (stv 0 1) (Predicate "i2") (Node "r1")))

  (List
   (Evaluation (stv 0 1) (Predicate "o") (Node "r2"))
   (Evaluation (stv 0 1) (Predicate "i1") (Node "r2"))
   (Evaluation (stv 0 1) (Predicate "i2") (Node "r2")))
 ))

(define boolean_data_repr_io
 (List
  (Similarity (stv 1 1)
   (Schema "o")
   (Set
    (List (Node "r0") (True))
    (List (Node "r1") (True))
    (List (Node "r2") (False))))

  (Similarity (stv 1 1)
   (List (Schema "i1") (Schema "i2"))
   (Set
    (List (Node "r0") (List (False) (True)))
    (List (Node "r1") (List (True) (False)))
    (List (Node "r2") (List (False) (False)))
   ))
 ))

(define boolean_data_repr_compact
 (Evaluation
  (Predicate "AS-MOSES:table")
  (List
   (List (Schema "o") (Schema "i1") (Schema "i2"))
   (List (True) (False) (True))
   (List (True) (True) (False))
   (List (False) (False) (False))
  ))
)

(define boolean_data_repr_similarity
 (Similarity (stv 1 1)
  (List (Schema "o") (Schema "i1") (Schema "i2"))
  (Set
   (List (Node "r0") (List (True) (False) (True)))
   (List (Node "r1") (List (True) (True) (False)))
   (List (Node "r2") (List (False) (False) (False)))
  ))
)
