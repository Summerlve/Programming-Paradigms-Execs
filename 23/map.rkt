#lang racket

(define (unary-map fn seq)
  (if (null? seq) '()
    (cons (fn (car seq)) (unary-map fn (cdr seq)))))

(unary-map car '((1 2 3) (2 3 4)))

(define (my-map fn first-list . other-lists)
  (if (null? first-list) '()
      (cons (apply fn 
        (cons (car first-list)
              (unary-map car other-lists)))
        (apply my-map (cons fn (cons (cdr first-list) (unary-map cdr other-lists)))))))

(my-map + '(1 2 100) '(3 4 100) '(5 6 100) '(9 10 100))
