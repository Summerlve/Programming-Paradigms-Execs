#lang racket

;; let 等价于 lambda
(println 
    (let ((x 1)
         (y 2))
         (+ x y)))
    
(println
    ((lambda (x y) (+ x y)) 1 2))