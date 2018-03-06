#lang racket
(define (sorted? seq fn)
    (or (< (length seq) 2) 
        (and (fn (car seq) (cadr seq))
              (sorted? (cdr seq) fn))))
(println
    (sorted? '(1 1 2 3) <=))
(println
    (sorted? '(3 2) <)) 
(println
    (sorted? '(3 3 3) =)) 
(println
    (sorted? '("a" "b" "c") string<?)) 
