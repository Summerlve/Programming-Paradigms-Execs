#lang racket

; a)
(define (longest-common-prefix seq1 seq2)
    (letrec ([inner-proc (lambda (seq1 seq2) 
                (if (or (null? seq1) (null? seq2)) '()
                    (let ([elem1 (car seq1)]
                          [elem2 (car seq2)])
                            (if (equal? elem1 elem2)
                                (cons elem1 (inner-proc (cdr seq1) (cdr seq2))) '()))))])
        (if (and (not (or (< (length seq1) 2) (< (length seq2) 2)))
                 (equal? (take seq1 2) (take seq2 2)))
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
;; quicksort algorithm, copy from wikipedia.
(define (quicksort s comp eq-fn)
    (if (< (length s) 2) s
        (append (quicksort (filter (lambda (e) (comp e (last s))) s) comp eq-fn)
                (filter (lambda (e) (eq-fn e (last s))) s)
                (quicksort (filter (lambda (e) (comp (last s) e)) s) comp eq-fn))))

(define (longest-common-sublist seq1 seq2)
    (car (quicksort (apply append (mdp (lambda (sublist-of-seq1) 
                                    (mdp (lambda (sublist-of-seq2)
                                        (longest-common-prefix sublist-of-seq1 sublist-of-seq2)) seq2)) seq1))
            (lambda (pre nxt)
                (> (length pre)
                   (length nxt))) 
            (lambda (pre nxt)
                (= (length pre)
                   (length nxt))))))

(println
    (longest-common-sublist '(#\a #\b #\d #\f #\g #\j) '(#\b #\c #\d #\f #\g #\h #\j #\k)))
(println
    (longest-common-sublist '(#\a #\b #\c #\d) '(#\x #\y)))
(println
    (longest-common-sublist '(4 5 6 7 8 9 10) '(2 3 4 5 6 7 8)))
