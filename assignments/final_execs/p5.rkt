#lang racket

(define (marry singles)
    (cond 
        [(= (length singles) 0) '()]
        [(= (length singles) 1) (list (car singles))]
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

(define (map-everywhere func structure)
    )