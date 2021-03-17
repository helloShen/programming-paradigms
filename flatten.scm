(define (flatten seq)
    (if (not (list? seq)) (list seq)
    (apply append (map flatten seq))))

(flatten '((1 2) ((3) ((4) 5)) 10))