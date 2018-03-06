#lang racket
(define (sorted? num-list)
    (or (< (length num-list) 2) 
        (and (<= (car num-list) (cadr num-list))
              (sorted? (cdr num-list)))))
(println
    (sorted? '(1 1 2 3)))
(println
    (sorted? '(3 2))) 