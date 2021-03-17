(define (translate points delta)
    (map (
        lambda (x) (+ x delta)
    ) points))

(translate '(1 2 3 4 5) 100)
(translate '() 100)