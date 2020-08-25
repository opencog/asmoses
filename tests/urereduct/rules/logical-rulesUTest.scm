(define testcase1
    (AndLink
       (False)
       (False)
       (True)))

(define expected1
 (SetLink
   (ReducedTo
     (AndLink
     (False)
     (False)
     (True))
     (False)
     (False))))

(define testcase2
  (OrLink
       (False)
       (False)
       (True)))

(define expected2
 (SetLink
   (ReducedTo
     (OrLink
     (False)
     (False)
     (True))
     (True))))

(define expected3
   (SetLink
    (ReducedTo
     (AndLink
     (False)
     (True)
     (False))
     (False))))


(define expected4
(SetLink
   (ReducedTo
     (OrLink
     (True)
     (False)
     (False))
     (True)
     (False))))

(define expected5
   (SetLink
     (ReducedTo
     (OrLink
     (False)
     (False)
     (True))
     (True)
     (False)
     (False))))

;;(define testcase6
;;(AndLink
;;       (OrLink
;;         (True)
;;         (False))))

;;(define expected6
;;   (SetLink
;;    (ReducedTo
;;     (AndLink
;;      (OrLink
;;        (True)
;;        (False)))
;;     (AndLink
;;      (True)
;;      (False)))))

;;(define testcase7
;;(OrLink
;;       (AndLink
;;         (True)
;;         (False))))

;;(define expected7
;;(SetLink
;; (ReducedTo
;;     (OrLink
;;      (AndLink
;;        (True)
;;        (False)))

;;     (OrLink
;;      (True)
;;      (False)))))

(define testcase8
(NotLink
        (NotLink
          (False))))

(define expected8
  (SetLink
    (ReducedTo
      (NotLink
       (NotLink
         (False)))
       (False))))


;;(define expected9
;; (SetLink
;;      (ReducedTo
;;      (NotLink
;;       (AndLink
;;         (True)
;;         (False)))

;;       (OrLink
;;         (NotLink
;;         (True))
;;         (NotLink
;;         (False))))))


;;(define expected10
;;  (SetLink
;;   (ReducedTo
;;      (NotLink
;;       (AndLink
;;         (False)
;;         (False)))

;;       (AndLink
;;         (NotLink
;;         (False))
;;         (NotLink
;;         (False))))))

