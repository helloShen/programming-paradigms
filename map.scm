(define (unary-map fn seq)
    (if (null? seq) '()
        (cons (fn (car seq))
            (unary-map fn (cdr seq)))))

(unary-map car '((1 2 3) (4 5 6) (7 8 9)))