(define target_feature_res
 (EvaluationLink
  (PredicateNode "AS-MOSES:table")
  (ListLink
   (ListLink
    (Schema "target") (Schema "input0")
    (Schema "input1") (Schema "input2"))
   (ListLink (Number 6) (Number 0) (Number 1) (Number 5))
   (ListLink (Number 7) (Number 1) (Number 0) (Number 3)))))

(define ignore_features_res
 (EvaluationLink
  (PredicateNode "AS-MOSES:table")
  (ListLink
   (ListLink (Schema "target") (Schema "input1") (Schema "input2"))
   (ListLink (Number 6) (Number 1) (Number 5))
   (ListLink (Number 7) (Number 0) (Number 3)))))
