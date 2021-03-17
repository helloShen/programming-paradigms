(define (remove items elem)
    (apply append
        (map (lambda (item)
            (if (eqv? item elem) '()
            (list item)))
            items)))

(remove '(1 2 3 4) 1)


(define (permute items) 
    (if (null? items) '(())
        (apply append
            (map (lambda (elem) 
                (map (lambda (permutation)
                    (cons elem permutation))
                        (permute (remove items elem))))
                items))))

(permute '(1 2 3 4))