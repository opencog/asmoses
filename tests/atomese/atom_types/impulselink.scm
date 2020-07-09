(use-modules (opencog) (opencog exec) (opencog asmoses))

(define true_link
	(Impulse
    (TrueLink (stv 1 1))))

(define or_link
    (Impulse
     (Or (True (stv 1 1)) (True (stv 0 1)))))

(define and_link
	 (Impulse (And (True) (False))))
