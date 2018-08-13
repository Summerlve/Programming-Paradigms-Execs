#lang racket

(define (ps set)
    (if (null? set) '(())
        (append (ps (cdr set))
                (map 
                    (lambda (subset) (cons (car set) subset))
                    (ps (cdr set))))))

(println
    (ps '(1 2 3)))