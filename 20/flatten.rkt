#lang racket
(define (flatten seq)
  (cond [(null? seq) seq]
        [(list? (car seq)) (append (car seq) (flatten (cdr seq)))]
        [else (cons (car seq) (flatten (cdr seq)))]))  
(begin
  (display (flatten '(1 2 (2 3 4)))))
