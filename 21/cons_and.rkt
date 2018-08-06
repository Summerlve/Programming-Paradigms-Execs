#lang racket

(define ns (make-base-namespace))

(println 
    (eval (cons 'and '(#t #f)) ns))

(println 
    (eval (cons 'and '(#t #t)) ns))