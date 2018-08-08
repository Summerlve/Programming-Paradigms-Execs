#lang racket

(define (flatten seq)
    (if (not (list? seq)) (list seq)
        (apply append
            (map flatten seq))))

(define seq '((1 2) 
              ((3) ((4) 5))
              10))

(println 
    (flatten seq))