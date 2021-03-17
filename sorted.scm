(define (sorted1? num-list)
    (or (< (length num-list) 
           2)
        (and (<= (car num-list) 
                 (cadr num-list))
             (sorted1? (cdr num-list)))))

(sorted1? '(1 2 3 4 5))
(sorted1? '(2 1 5 3 4))


(define (sorted2? seq cmp)
    (or (< (length seq)
           2)
        (and (cmp (car seq)
                  (cadr seq))
             (sorted2? (cdr seq) cmp))))

(sorted2? '(1 2 3 4 5) <)
(sorted2? '(5 4 3 2 1) >)
(sorted2? '(2 1 5 3 4) <)