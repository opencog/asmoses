(use-modules (opencog))

(define-module (opencog reduct)
  #:use-module (opencog)
  #:use-module (opencog asmoses-config)
)
(load-extension (string-append reduct_types_path "libreduct-types") "reduct_types_init")
(load-from-path  "opencog/urereduct/types/reduct_types.scm")
(load-from-path "opencog/utilities.scm")

