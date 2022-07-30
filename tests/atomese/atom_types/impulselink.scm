(use-modules (opencog) (opencog exec) (opencog asmoses))

(define true_link
	(Mpulse
    (TrueLink (stv 1 1))))

(define or_link
    (Mpulse
     (Or (True (stv 1 1)) (True (stv 0 1)))))

(define and_link
	 (Mpulse (And (True) (False))))
