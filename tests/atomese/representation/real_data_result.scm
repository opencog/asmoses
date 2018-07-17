(define real_data_repr_unfolded
 (Set
  (List
   (Execution (Schema "o") (Node "r0") (Number 2))
   (Execution (Schema "i1") (Node "r0") (Number 0))
   (Execution (Schema "i2") (Node "r0") (Number 2)))

  (List
   (Execution (Schema "o") (Node "r1") (Number 1))
   (Execution (Schema "i1") (Node "r1") (Number 1))
   (Execution (Schema "i2") (Node "r1") (Number 0)))

  (List
   (Execution (Schema "o") (Node "r2") (Number 0))
   (Execution (Schema "i1") (Node "r2") (Number 0))
   (Execution (Schema "i2") (Node "r2") (Number 0)))
 ))

(define real_data_repr_io
 (List
  (Similarity (stv 1 1)
   (Schema "o")
   (Set
    (List (Node "r0") (Number 2))
    (List (Node "r1") (Number 1))
    (List (Node "r2") (Number 0))))

  (Similarity (stv 1 1)
   (List (Schema "i1") (Schema "i2"))
   (Set
    (List (Node "r0") (List (Number 0) (Number 2)))
    (List (Node "r1") (List (Number 1) (Number 0)))
    (List (Node "r2") (List (Number 0) (Number 0)))
   ))
 ))

(define real_data_repr_compact
 (Evaluation
  (Predicate "AS-MOSES:table")
  (List
   (List (Schema "o") (Schema "i1") (Schema "i2"))
   (List (Number 2) (Number 0) (Number 2))
   (List (Number 1) (Number 1) (Number 0))
   (List (Number 0) (Number 0) (Number 0))
  ))
)

(define real_data_repr_similarity
 (Similarity (stv 1 1)
  (List (Schema "o") (Schema "i1") (Schema "i2"))
  (Set
   (List (Node "r0") (List (Number 2) (Number 0) (Number 2)))
   (List (Node "r1") (List (Number 1) (Number 1) (Number 0)))
   (List (Node "r2") (List (Number 0) (Number 0) (Number 0)))
  ))
)
