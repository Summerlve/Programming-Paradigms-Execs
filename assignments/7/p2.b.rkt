#lang racket

(define (up-down-permute num-list)
    (if (null? num-list) '(())
        (apply append 
            (map (lambda (elem)
                (map (lambda (permutation)
                    (let ([consed-permutation (cons elem permutation)])
                        (if (= 1 (length consed-permutation)) consed-permutation
                            (let* ([is-num-list-even (even? (length num-list))]
                                [is-consed-permutation-even (even? (length consed-permutation))]
                                [predicate-function
                                    (if is-num-list-even (if is-consed-permutation-even < >)
                                        (if is-consed-permutation-even > <))])
                                           (if (apply predicate-function (take consed-permutation 2)) consed-permutation '())))))
                (up-down-permute (remove elem num-list))))
            num-list))))

(println
    (up-down-permute '(1 2 3)))