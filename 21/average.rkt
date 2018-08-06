#lang racket

(define (average num-list)
    (/ (apply + num-list)
       (length num-list)))

(println (average '(1 2 3 4 5)))