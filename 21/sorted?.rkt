#lang racket
(define (sorted? seq cmp)
    (or (< (length seq) 2)
        (and (cmp (car seq) (cadr seq))
             (sorted? (cdr seq) cmp))))

(println 
    (sorted? '(1 2 8 5 4)
             (lambda (prev last) (<= prev last))))

(println 
    (sorted? '(1 2 3 3 3 4 4 5)
             (lambda (prev last) (<= prev last))))