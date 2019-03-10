#lang racket

; a)
(define (longest-common-prefix seq1 seq2)
    (letrec ([inner-proc (lambda (seq1 seq2) 
                (if (or (null? seq1) (null? seq2)) '()
                    (let ([elem1 (car seq1)]
                          [elem2 (car seq2)])
                            (if (equal? elem1 elem2)
                                (cons elem1 (inner-proc (cdr seq1) (cdr seq2))) '()))))])
        (if (equal? (take seq1 2) (take seq2 2))
            (append (take seq1 2) (inner-proc (list-tail seq1 2) (list-tail seq2 2))) '())))

(println
    (longest-common-prefix '(#\a #\b #\c) '(#\a #\b #\d #\f)))
(println
    (longest-common-prefix '(#\s #\t #\e #\r #\n) '(#\s #\t #\e #\r #\n #\u #\m)))
(println
    (longest-common-prefix '(1 2 3) '(0 1 2 3)))
(println
    (longest-common-prefix '(1 2 3) '(1 3 4)))

; b)
(define (mdp func sequence)
    (if (null? sequence) '()
        (cons (func sequence) (mdp func (cdr sequence)))))

(println
    (mdp length '(#\w #\x #\y #\z)))
(println
    (mdp cdr '(2 1 2 8)))
(println
    (mdp reverse '("ba" "de" "foo" "ga")))
    
; c)

