(use-modules (opencog))

(define-module (opencog asmoses)
  #:use-module (opencog)
  #:use-module (opencog asmoses-config)
)


; Load the C library; this calls the nameserver to load the types.
(load-extension (string-append atom_types_path "libasmoses_types") "asmoses_types_init")

; Load various parts....
(load-from-path  "opencog/asmoses/types/atom_types.scm")
(export)