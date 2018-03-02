#lang racket
(define (flatten seq)
  (if (null? seq) seq
      (if (not (list? seq)) seq
          (cons (car seq) (flatten (cdr seq))))))

(begin
  (display (flatten '(1 2 (2 3 4)))))
