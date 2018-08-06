#lang racket

(define ns (make-base-namespace))

(println (eval '(+ 1 2 3) ns))
(println (apply + '(1 2 3)))