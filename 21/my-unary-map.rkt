#lang racket
(define (my-unary-map fn seq)
    (if (null? seq) '()
        (cons (fn (car seq))
              (my-unary-map fn (cdr seq)))))

(println
   (my-unary-map (lambda (x) (* 2 x)) '(1 2 3)))

(println
   (my-unary-map (lambda (x) (* 3 x)) '(1 2 3)))