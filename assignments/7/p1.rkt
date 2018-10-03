#lang racket
(define (k-subsets set k)
    (cond
        [(= k 0) '(())]
        [(< (length set) k) '()]
        [(cdr set) => (lambda (subset)
                        (if (null? subset) '(())
                            (let ([kk-subsets-rest (k-subsets subset (- k 1))])
                                (append kk-subsets-rest
                                    (map
                                        (lambda (kk-subsets-rest-item) 
                                            (cons (car set) kk-subsets-rest-item)) kk-subsets-rest)))))]))

(println
    (k-subsets '(1 2 3 4) 2))