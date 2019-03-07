#lang racket

; a)
(define (marry singles)
    (cond 
        [(<= (length singles) 1) singles]
        [else (cons (take singles 2) (marry (list-tail singles 2)))]))

(println 
    (marry '("Adam" "Eve" "George" "Martha" "Albert" "Costello")))
(println
    (marry '("Fred" "Wilma")))
(println
    (marry '()))
(println
    (marry '("Lucy" "Schroeder" "Patty" "Linus" "Charlie Brown")))
(println
    (marry '("INFP")))

; b)
(define (map-everywhere func structure)
    (if (list? structure) 
        (if (= (length structure) 0) structure
            (cons
                (map-everywhere func (car structure))
                (map-everywhere func (cdr structure))))
        (func structure)))

(println
    (map-everywhere (lambda (x) (+ x 1)) '(1 (2 (3) 4 (5 (6))) (8 9)))) 
(println
    (map-everywhere list '("a" ("b" ("c" ("d" "e" "f" "g"))))))
(define (factorial index) 
    (cond 
        [(= index 1) 1]
        [else (* index (factorial (- index 1)))]))
(println
    (map-everywhere factorial '(4 ((5)) (6 (7)))))
