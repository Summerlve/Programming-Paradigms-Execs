#lang racket

(define (permute items)
    (if (null? items) '(())
        (apply append 
            (map (lambda (elem)
                (map (lambda (permutation)
                    (cons elem permutation)) (permute (remove elem items)))) items))))

(println
    (permute '(1 2 3 4)))