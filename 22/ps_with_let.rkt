#lang racket

(define (ps set)
    (if (null? set) '(())
        (let ((ps-rest (ps (cdr set))))
            (append ps-rest
                    (map
                        (lambda (subset) (cons (car set) subset))
                        ps-rest)))))

(println
    (ps '(1 2 3)))