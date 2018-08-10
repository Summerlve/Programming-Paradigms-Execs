#lang racket

(define (translate points delta)
    (map (lambda (x) (+ x delta)) points))

(println
    (translate '(1 2 3 4 5) 10))