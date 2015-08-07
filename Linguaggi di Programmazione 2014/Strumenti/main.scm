;;;;;;;;;;;;;;;;;;;;;;;;; TEMI D'ESAME ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Definizione della funzione atom?

(define (atom? x) (not (list? x)))

; Esercizio 1

;(define (reverse A)
;   (if (null? A) ()
;       (append (reverse (cdr A)) (list (car A)))
;    )
;)

; Esercizio 2

(define (spec L)
    (if (null? L) ()
        (append (spec (cdr L))(list (if (atom? (car L))
                                        (car L)
                                        (spec (car L))
                                     )
                               )
        )
     )
)

; Esercizio 3

(define (natoms L)
   (if (null? L) 0
       (if (atom? (car L))
           (+ 1 (natoms (cdr L)))
           (+ (natoms (car L)) (natoms (cdr L)))
        )
    )
)

; Esercizio 4

(define (atomi lista)
   (if (null? lista) ()
       (if (atom? (car lista))
           (cons (car lista) (atomi (cdr lista)))
           (append (atomi (car lista)) (atomi (cdr lista)))
        )
    )
)

; Esercizio 5

(define (occ x lista)
    (if (null? lista) 0
        (if (equal? x (car lista))
            (+ 1 (occ x (cdr lista)))
            (occ x (cdr lista))
        )
     )
) 

; Esercizio 6

(define (select fun lista)
  (if (null? lista) ()
      (if (fun (car lista))
          (cons (car lista) (select fun (cdr lista)))
          (select fun (cdr lista))
       )
  )
)

; Esercizio 7

(define (cancella elemento lista)
   (if (null? lista) ()
       (if (equal? (car lista) elemento) 
           (cancella elemento (cdr lista))
           (cons (car lista) (cancella elemento (cdr lista))))))

; Esercizio 8

(define (zip lista1 lista2)
  (if (or (null? lista1)(null? lista2))
      ()
      (cons (list (car lista1) (car lista2))
              (zip (cdr lista1) (cdr lista2)))))

; Esercizio 9

(define (shrink L)
  (if (null? L)()
      (if (null? (cdr L)) L
          (cons(car L)(shrink (cddr L))))))

; Esercizio 10

(define (remdup L)
  (if (null? L)()
      (if (member (car L)(cdr L))
          (remdup (cdr L))
          (cons (car L)(remdup (cdr L))))))

; Esercizio 11

;(define (remove f lista)
; (if (null? lista) ()
;     (if (f (car lista))
;         (remove f (cdr lista))
;         (cons (car lista) (remove f (cdr lista)))
;      )
; )
;)

; Esercizio 12

(define (serie n)
   (if (= n 0) ()
       (append (serie (- n 1)) (list n))
   )
) 

; Esercizio 13

(define (minimo interi)
   (if (null? (cdr interi))(car interi)
       (if (< (car interi) (minimo (cdr interi)))
           (car interi)
           (minimo (cdr interi))
       )
    )
) 

; Esercizio 14

;(define (apply f lista)
;   (f (eval lista)))

; Esercizio 15

(define (prefix? L1 L2)
  (if (null? L1) #t
    (if (null? L2) #f
        (if (equal? (car L1) (car L2))
            (prefix?(cdr L1) (cdr L2))
            #f))))

; Esercizio 16

(define (appartiene x set)
  (if (null? set)
      #f
      (if (equal? x (car set))
          #t
          (appartiene x (cdr set)))))

(define (intersezione set1 set2)
  (if (null? set1)
      ()
      (if (appartiene (car set1) set2)
          (cons (car set1) (intersezione (cdr set1) set2))
        (intersezione (cdr set1) set2))))

; Esercizio 17

(define (manca x L)
  (if (null? L)
      #t
      (if (equal? x (car L))
          #f
          (manca x (cdr L)))))

(define (unione L1 L2)
  (if (null? L1)
      L2
      (if (manca (car L1) L2)
          (cons (car L1) (unione (cdr L1) L2))
            (unione (cdr L1) L2))))

; Esercizio 18 

(define (take n lista)
  (if (or (equal? n 0)(null? lista))
      ()
      (cons (car lista)(take (- n 1) (cdr lista)))))

; Esercizio 19

(define (domino lista)
  (if (or (null? lista) (null? (cdr lista))) #t
      (if (not (equal? (cadar lista) (caadr lista))) #f
           (domino (cdr lista)))))

; Esercizio 20 

(define (sublist? S L)
   (if (> (length S)(length L)) #f
       (if (prefix? S  L) #t
           (sublist? S (cdr L)))))

; Esercizio 21

(define (indexing A key)
  (if (null? A) 
      'errore
      (if (equal? key (car (car A)))
          (car (cdr (car A)))
          (indexing (cdr A) key))))

; Esercizio 22 

(define (duplica lista)
  (if (null? lista) 
      ()
      (cons (list (car lista) (car lista)) (duplica (cdr lista)))))


; Esercizio 23

(define (fib n)
  (if (= n 0) 0
      (if (= n 1) 1
          (+ (fib (- n 1))
             (fib (- n 2))))))
          
(define (listafib n)
  (if (= n 0) '(0)
      (append (listafib (- n 1))
              (list (fib n)))))
              
; Esercizio 24

(define (iniziale lista)
  (if (null? (cdr lista))
      ()
      (cons (car lista)(iniziale (cdr lista)))))

; Esercizio 25

(define (inserisci n lista)
  (if (null? lista)
      (list n)
      (if (<= n (car lista))
          (cons n lista)
          (cons (car lista) (inserisci n (cdr lista))))))

(define (ordina lista)
  (if (null? lista) ()
      (inserisci (car lista) (ordina (cdr lista)))))

; Esercizio 26

(define (fact n)
  (if (= n 0) 1
      (* n (fact (- n 1)))))

(define (fattoriali n)
  (if (= n 0) '(1)
      (append (fattoriali (- n 1)) (list (fact n)))))

; Esercizio 27

(define (revatoms L)
  (if (null? L) ()
      (if (atom? (car L))
          (append (revatoms (cdr L)) (list (car L)))
          (revatoms (cdr L)))))

; Esercizio 28

(define (sum lista)
  (if (null? lista) 0
      (+ (car lista) (sum (cdr lista)))))

(define (media lista)
  (/ (sum lista) (length lista)))

; Esercizio 29

(define (itera f x n)
    (if (= n 0) (list x)
        (append (itera f (f x) (- n 1)) (list x))))

(define (itera2 f x n)
    (if (= n 0) (list x)
        (cons x (itera2 f (f x) (- n 1)))))

; Esercizio 30

(define (valori n f)
  (if (= n 0) (list (f 0))
      (append (valori (- n 1) f) (list (f n)))))

;(define (fact n)
;  (if (= n 0) 1
;      (* n (fact (- n 1)))))

; Esercizio 31

(define (crescente lista)
  (if (or (null? lista)(null? (cdr lista))) #t
      (if (> (+ (caar lista)(cadar lista))
             (+ (caadr lista)(cadadr lista))) #f
          (crescente (cdr lista)))))

; Esercizio 32

(define (somma2 numeri)
  (if (or (null? numeri)
          (null? (cdr numeri)))
           numeri
           (cons (+ (car numeri)(cadr numeri))
                 (somma2 (cddr numeri)))))

; Esercizio 33

(define (hanoi n)
  (sposta n 'sinistra 'centro 'destra))

(define (sposta n sorgente destinazione appoggio)
  (if (= n 0) '()
      (append (append (sposta (- n 1) sorgente appoggio destinazione) 
                      (list (list sorgente destinazione)))
              (sposta (- n 1) appoggio destinazione sorgente))))

(define (hmosse n)
  (if (= n 0) 0
      (+ (* 2 (hmosse (- n 1))) 1)))

(define (listhmosse n)
  (if (= n 0) '(0)
      (append (listhmosse (- n 1))(list (hmosse n)))))

; Esercizio 34

(define (maxval f dominio)
  (if (null? (cdr dominio))
      (f (car dominio))
      (if (> (f (car dominio))(maxval f (cdr dominio)))
          (f (car dominio))
          (maxval f (cdr dominio)))))

; Esercizio 35

(define (clear lista)
  (if (null? lista) lista
      (if (list? (car lista))
          (cons (clear (car lista)) (clear (cdr lista)))
          (clear (cdr lista)))))

; Esercizio 36

(define (sumfun n f)
  (if (= n 0) (f 0)
      (+ (sumfun (- n 1) f) (f n))))

; Esercizio 37

(define (ordinate lista)
  (if (null? lista) ()
      (if (> (caar lista)(cadar lista))
          (cons (car lista) (ordinate (cdr lista)))
          (ordinate (cdr lista)))))

; Esercizio 38

(define (combina funzione lista1 lista2)
  (if (or (null? lista1) (null? lista2)) ()
      (cons (funzione (car lista1) (car lista2)) 
            (combina funzione (cdr lista1) (cdr lista2)))))

; Esercizio 39

(define (init lista)
  (reverse (cdr (reverse lista))))

(define (last lista)
  (car (reverse lista)))

(define (power x n)
  (if (= n 0) 1
      (* x (power x (- n 1)))))

(define (sommapotenze numeri)
  (if (empty? numeri) 0
      (+ (sommapotenze (init numeri)) (power (last numeri) (length numeri)))))

; oppure:

;(define (sommapotenze numeri) 
;   (revsommapot (reverse numeri)))

;(define (revsommapot numeri)
;  (if (empty? numeri) 0
;      (+ (power (car numeri) (length numeri)) (revsommapot (cdr numeri)))))

; Esercizio 40

(define (somme numeri)
  (sommeaux 0 numeri))

(define (sommeaux n numeri)
  (if (null? numeri) '()
      (let ((testa (+ (car numeri) n)))
         (cons testa (sommeaux testa (cdr numeri))))))

; Esercizio 41

(define (funpair f list1 list2)
  (if (or (null? list1) (null? list2))
      ()
      (cons (f (car list1) (car list2))
            (funpair f (cdr list1) (cdr list2)))))

; Esercizio 42

(define (medie numeri)
  (reverse (medaux (reverse numeri))))

(define (medaux lista)
  (if (null? lista) '()
  (cons (media lista)(medaux (cdr lista)))))

(define (media lista)
  (/ (eval (cons '+ lista)) (length lista)))

; Esercizio 43

(define (cartesiano lista1 lista2)
  (if (or (null? lista1) (null? lista2)) '()
      (append (combina (car lista1) lista2)
              (cartesiano (cdr lista1) lista2))))

(define (combina elemento lista)
  (if (null? lista) '()
  (cons (list elemento (car lista))
        (combina elemento (cdr lista)))))

; Esercizio 44

(define (revpairs coppie)
  (if (null? coppie) '()
      (cons (list (cadar coppie) (caar coppie))
            (revpairs (cdr coppie)))))

; Esercizio 45

(define (drop n lista)
  (take (- (length lista) n) lista))

(define (take n lista)
  (if (or (< n 1) (null? lista)) 
      ()
      (cons (car lista)(take (- n 1)(cdr lista)))))

; Esercizio 46

(define (singletons lista)
  (if (null? lista) '()
      (cons (list (car lista)) (singletons (cdr lista)))))

; Esercizio 47

(define (swapairs lista)
  (if (null? lista) '()
      (append (list (cadr lista) (car lista)) (swapairs (cddr lista)))))

; Esercizio 48

(define (sumpairs lista)
  (if (null? lista) '()
      (if (null? (cdr lista)) (list (car lista))
       (cons (+ (car lista)(cadr lista)) (sumpairs (cddr lista))))))











