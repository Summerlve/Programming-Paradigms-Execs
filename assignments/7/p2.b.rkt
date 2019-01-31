#lang racket

(define (up-down-permute num-list)
    (letrec ([is-num-list-even (even? (length num-list))]
             [fn (lambda (list)
                (if (null? list) '(())
                (apply append 
                    (map (lambda (elem)
                        (map (lambda (permutation)
                            (let ([consed-permutation (cons elem permutation)])
                                (if (= 1 (length consed-permutation)) consed-permutation
                                    (let* ([is-consed-permutation-even (even? (length consed-permutation))]
                                        [predicate-function(if is-num-list-even (if is-consed-permutation-even < >)
                                                (if is-consed-permutation-even > <))])
                                                (if (apply predicate-function (take consed-permutation 2)) consed-permutation '())))))
                        (fn (remove elem list))))
                    list)
                )))])
    (filter (lambda (permutation) (if (= (length permutation) (length num-list)) #t #f)) (fn num-list))))

(println 
    (up-down-permute '()))
(println 
    (up-down-permute '(1)))
(println
    (up-down-permute '(1 2)))
(println
    (up-down-permute '(1 2 3)))
(println
    (up-down-permute '(1 2 3 4 5)))