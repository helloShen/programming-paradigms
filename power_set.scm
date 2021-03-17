(define (ps set)
    (if (null? set) '(())
        (let ((ps-rest (ps (cdr set))))
            (append ps-rest
                (map (lambda (subset) (cons (car set) subset))
                    ps-rest)))))

(ps '(1 2 3))