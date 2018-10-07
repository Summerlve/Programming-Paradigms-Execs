#lang racket
(define (k-subsets set k)
    (cond
        [(= k 0) '(())]
        [(< (length set) k) '()]
        [(cdr set) => 
            (lambda (subset)
                (if (null? subset) (list set)
                    (let ([k-subsets-rest (k-subsets subset k)]
                          [kk-subsets-rest (k-subsets subset (- k 1))])
                        (append k-subsets-rest
                            (map
                                (lambda (kk-subsets-rest-item) 
                                    (cons (car set) kk-subsets-rest-item)) kk-subsets-rest)))))]))

(println
    (k-subsets '(1 2 3 4) 2))
(println
    (k-subsets '(1 2 3 4) 4))
(println
    (k-subsets '(1 2 3 4 5 6) 1))
(println
    (k-subsets '(#\a #\b #\c #\d) 0))
(println
    (k-subsets '(#\a #\b #\d #\d) 5))
    