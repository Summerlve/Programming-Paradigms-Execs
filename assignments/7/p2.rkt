#lang racket
(define (is-up-down? list comp)
    (cond 
        [(= (length list) 0) #t]
        [(= (length list) 1) #t]
        [(take list 2) 
            => (lambda (first-2-item-list)
                    (and 
                        (apply comp first-2-item-list) 
                        (is-up-down? (cdr list) (if (eq? comp <) > <))))]))
                
(println
    (is-up-down? '() <)) ;; #t
(println
    (is-up-down? '(1) <)) ;; #t
(println 
    (is-up-down? '(1 2 2 3) <)) ;; #f
(println
    (is-up-down? '(1 6 2 4 3 5) <)) ;; #t
(println
    (is-up-down? '(1 6 2 4 3 5) >)) ;; #f
(println
    (is-up-down? '(4 8 3 5 1 7 6 2) <)) ;; #f