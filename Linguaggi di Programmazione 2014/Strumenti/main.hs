module Main  where 
import Data.Char

pari, dispari, diecipari, naturalpari, quadrati :: [Integer]

pari = [0,2,4,6,8]
dispari = [1,3,5,7,9]
diecipari = [ 2*x | x <- [0..10]]
naturalpari = [ 2*x | x <- [0,1..]]
quadrati = [ n*n | n <- [0..] ]

quad :: Integer -> Integer
quad n = n*n

cube :: Integer -> Integer
cube n = n*n*n

max3 :: Integer -> Integer -> Integer -> Integer
max3 x y z
  | x >= y && x >= z	= x
  | y >= z		= y
  | otherwise		= z

serie :: Integer -> [Integer]
serie 0 = []
serie n = serie(n-1) ++ [n]

fact :: Integer -> Integer
fact n
  | n == 0 = 1
  | n > 0  = n * fact(n-1)

regioni :: Integer -> Integer
regioni n 
  | n == 0  = 1
  | n > 0   = regioni(n-1) + n

divide :: Integer -> Integer -> Integer
divide n m
  | n < m       = 0
  | otherwise   = 1 + divide (n-m) m

sumF :: (Integer -> Integer) -> Integer -> Integer
sumF f n 
  | n == 0   = f(0)
  | n > 0    = sumF f (n-1) + f n

sumFacs n = sumF fact n

sommatoria :: [Integer] -> Integer
sommatoria []            = 0
sommatoria (testa:coda)  = testa + sommatoria(coda)

ins :: Integer -> [Integer] -> [Integer]
ins n [] = [n]
ins n (testa:coda)
  | n <= testa = n:(testa:coda)
  | otherwise  = testa : ins n coda

iSort :: [Integer] -> [Integer]
iSort []	= []
iSort (n:coda) = ins n (iSort coda)

quickSort :: [Integer] -> [Integer]
quickSort [] = []
quickSort (n:coda) = 
  quickSort [n' | n' <- coda, n'<=n] ++ [n] ++ quickSort [n' | n' <- coda, n'>n]


-------------- Fibonacci ---------------------------------------------------------

fib :: Integer -> Integer
fib n 
  | n == 0   = 0
  | n == 1   = 1
  | n > 1    = fib(n-2) + fib(n-1)
  
fibs :: Integer -> [Integer]
fibs 0 = [1]
fibs n = fibs (n-1) ++ [fib n]
  
fcont :: Integer -> Integer
fcont 0 = 1
fcont 1 = 1
fcont n = fcont(n-2) + fcont(n-1) + 1
  
fconts :: Integer -> [Integer]
fconts 0 = [1]
fconts n = fconts (n-1) ++ [fcont n]

fibStep :: (Integer, Integer) -> (Integer, Integer)
fibStep (u, v) = (v, u+v)

fibPair :: Integer -> (Integer, Integer)
fibPair n
  | n == 0			= (0, 1)
  | otherwise		= fibStep (fibPair (n-1))

fastFib :: Integer -> Integer
fastFib = fst . fibPair

fcontStep :: (Int, Int) -> (Int, Int)
fcontStep (u, v) = (v, u+v+1)

fcontPair :: Int -> (Int, Int)
fcontPair n
  | n == 0			= (1, 1)
  | otherwise		= fcontStep (fcontPair (n-1))

fastFcont :: Int -> Int
fastFcont = fst . fcontPair

-- Generalizzazione

fseries :: Integer -> Integer -> (Integer -> Integer-> Integer) -> Integer -> Integer
fseries v0 v1 fcomb n
  | n == 0  =  v0
  | n == 1  =  v1
  | n > 0   =  fcomb (fseries v0 v1 fcomb (n-2)) (fseries v0 v1 fcomb (n-1))
  
fib' = fseries 0 1 (+)
fcont' = fseries 1 1 (\u v -> u + v + 1)

----------------------------------------------------------------------------------  
isEven :: Integer -> Bool
isEven n =  (n `mod` 2) == 0

addPairs :: [(Integer,Integer)] -> [Integer]
addPairs pairList = [ m+n | (m,n) <- pairList ]

addOrdPairs :: [(Integer,Integer)] -> [Integer]
addOrdPairs pairList = [ m+n | (m,n) <- pairList, m<n ]

allEven :: [Integer] -> Bool
allOdd :: [Integer] -> Bool
allEven numeri = ([n | n<-numeri, isEven n] == numeri)
allOdd numeri  = ([n | n<-numeri, isEven n] == [])

existsEven :: [Integer] -> Bool
existsOdd :: [Integer] -> Bool
existsEven numeri = ([n | n<-numeri, isEven n] /= [])
existsOdd numeri  = ([n | n<-numeri, isEven n] /= numeri)

type Picture = [[Char]]

invertChar :: Char -> Char
invertChar '.' = '#'
invertChar '#' = '.'

invertLine :: [Char] -> [Char]
invertLine line = [ invertChar c | c <- line ]

invertColour :: Picture -> Picture
invertColour pic = [ invertLine line | line <- pic ]

addCor :: [Integer] -> [Integer] -> [Integer]    
addCor list1 list2 =                           
  [ m+n | (m,n) <- zip list1 list2 ]

addCor' :: [Integer] -> [Integer] -> [Integer] 
addCor' list1 list2 = 
  front ++ back
  where
  lungMin = min (length list1) (length list2)
  front   = addCor (take lungMin list1)(take lungMin list2)
  back    = drop lungMin list1 ++ drop lungMin list2
  
qSort :: Ord a => [a] -> [a]
qSort [] = []
qSort (n:coda) = 
  qSort [n' | n' <- coda, n'<=n] ++ [n] ++ qSort [n' | n' <- coda, n'>n]

select :: (a -> Bool) -> [a] -> [a]
select p lista = [ x | x <- lista, p x ]

map' :: (a -> b) -> [a] -> [b]
map' f [] = []
map' f (testa:coda) = f testa : map' f coda

isSorted :: [Int] -> Bool
isSorted [] = True
isSorted [_] = True
isSorted (n:(n2:coda2)) = n <= n2 && isSorted coda2

compose :: [a->a] -> (a->a)
compose = foldr (.) id

compose' :: [a->a] -> (a->a)
compose' = foldl (.) id

------------------- Biblioteca ----------------------------------

type Persona  = String
type Libro    = String
type Prestiti = [ (Persona, Libro) ]

prestiti :: Prestiti
prestiti = [("Alice", "TinTin"),
            ("Anna", "Piccole Donne"),
            ("Alice", "Asterix"),
            ("Rosy", "TinTin")]

libriInPrestito :: Persona -> [Libro]
libriInPrestito persona =
  [ l | (p, l) <- prestiti, p==persona ]

personeConLibro :: Libro -> [Persona]
personeConLibro libro =
  [ p | (p, l) <- prestiti, l==libro ]

inPrestito :: Libro -> Bool
inPrestito libro =
  [ l | (p, l) <- prestiti, l==libro ] /= []

numeroLibri :: Persona -> Int
numeroLibri persona =
  length [ l | (p, l) <- prestiti, p==persona ]
  
--------------------- Cifratura di Cesare --------------------------

let2int :: Char -> Int
let2int c = ord c - ord 'a'

int2let  :: Int -> Char
int2let n = chr(ord 'a' + n)

shift :: Int -> Char -> Char
shift n c
  | isLower c = int2let ((let2int c + n) `mod` 26)
  | otherwise = c
  
encode' :: Int -> String -> String
encode' n s = [shift n c | c <- s]

table :: [Float]
table = [8.2,1.5,2.8,4.3,12.7,2.2,2.0,6.1,7.0,0.2,0.8,4.0,2.4,
         6.7,7.5,1.9,0.1,6.0,6.3,9.1,2.8,1.0,2.4,0.2,2.0,0.1]

percent :: Int -> Int -> Float
percent n m = (fromIntegral n / fromIntegral m) * 100

lowers :: String -> Int
lowers s = length [c | c <- s, isLower c]

count :: Char -> String -> Int
count c s = length [c' | c' <- s, c == c']

freqs :: String -> [Float]
freqs s = [percent(count c s) n | c <- ['a'..'z']]
          where n = lowers s

chisqr :: [Float] -> [Float] -> Float
chisqr oss att = sum [((o - a)^2)/a | (o,a) <- zip oss att]

rotate :: Int -> [a] -> [a]
rotate n lista = drop n lista ++ take n lista

positions :: Eq a => a -> [a] -> [Int]
positions x lista = [i | (x',i) <- zip lista [0..n], x == x']
  where n = length lista - 1

crack :: String -> String
crack s = encode' (-factor) s
  where
    factor = head (positions (minimum chitab) chitab)
    chitab = [chisqr (rotate n table') table | n <- [0..25]]
    table' = freqs s
    
------------------------ Forme funzionali --------------------------

twice :: (a -> a) -> a -> a
twice f x = f (f x)

quadruplo :: Int -> Int
quadruplo = twice (*2)
    
------------------- Trasmissione di stringhe -----------------------

type Bit = Int

bin2int :: [Bit] -> Int
bin2int = foldr (\x y -> x + 2 * y) 0

int2bin :: Int -> [Bit]
int2bin 0  =  []
int2bin n  =  n `mod` 2 : int2bin (n `div` 2)

-- make8 :: [Bit] -> [Bit]
make8 bits = take 8 (bits ++ repeat 0)

encode :: String -> [Bit]
encode = concat . map (make8 . int2bin . ord)

chop8 :: [Bit] -> [[Bit]]
chop8 []    =  []
chop8 bits  =  take 8 bits:chop8(drop 8 bits)

decode :: [Bit] -> String
decode  =  map (chr . bin2int) . chop8

transmit :: String -> String
transmit  =  decode . channel . encode

channel :: [Bit] -> [Bit]
channel = id

------------------- Dichiarazione di tipi ---------------------
type Board = [Pos]
type Pos = (Int,Int)

type Assoc k v = [(k,v)]

find :: Eq k => k -> Assoc k v -> v
find key tab = head [v | (k,v) <- tab, k == key]

data Move = Left' | Right' | Up' | Down'

move :: Move -> Pos -> Pos
move Left' (x,y)  = (x-1, y)
move Right' (x,y) = (x+1, y)
move Up' (x,y)    = (x, y-1)
move Down' (x,y)  = (x, y+1)

moves :: [Move] -> Pos -> Pos
moves [] p       = p
moves (m:coda) p = moves coda (move m p)

flip :: Move -> Move
flip Left'  = Right'
flip Right' = Left'
flip Up'    = Down'
flip Down'  = Up'

data Shape = Circle Float | Rect Float Float

square :: Float -> Shape
square n  = Rect n n

area :: Shape -> Float
area (Circle r) = pi * r^2
area (Rect x y) = x * y

data Nat = Zero | Succ Nat

nat2int :: Nat -> Int
nat2int Zero = 0
nat2int (Succ n) = 1 + nat2int n

int2nat :: Int -> Nat
int2nat 0     = Zero
int2nat n = Succ (int2nat (n-1))

add :: Nat -> Nat -> Nat
add m n = int2nat (nat2int m + nat2int n)

add' :: Nat -> Nat -> Nat
add' Zero n     = n
add' (Succ m) n = Succ (add' m n)

data List a = Nil | Cons a (List a)

len :: List a -> Int
len Nil = 0
len (Cons _ coda) = 1 + len coda

data Tree = Leaf Int | Node Tree Int Tree

albero :: Tree
albero = Node (Node (Leaf 1) 3 (Leaf 4)) 5 (Node (Leaf 6) 7 (Leaf 9))

occurs :: Int -> Tree -> Bool
occurs m (Leaf n)  =  m == n
occurs m (Node left n right)  =  m == n || occurs m left || occurs m right

flatten :: Tree -> [Int]
flatten (Leaf n)            = [n]
flatten (Node left n right) = flatten left ++ [n] ++ flatten right

occurs' :: Int -> Tree -> Bool
occurs' m (Leaf n)  =  m == n
occurs' m (Node left n right)
     | m == n     =  True
     | m < n      =  occurs' m left
     | otherwise  =  occurs' m right
     
------------------- Verifica di tautologie ---------------------

data Prop = Const Bool
          | Var Char
          | Not Prop
          | And Prop Prop
          | Imply Prop Prop

p1 :: Prop
p1 = And(Var 'A')(Not (Var 'A'))

p2 :: Prop
p2 = Imply(And(Var 'A')(Var 'B'))(Var 'A')

p3 :: Prop
p3 = Imply(Var 'A')(And(Var 'A')(Var 'B'))

p4 :: Prop
p4 = Imply(And(Var 'A')(Imply(Var 'A')(Var 'B')))(Var 'B')

type Subst = Assoc Char Bool

eval :: Subst -> Prop -> Bool
eval _ (Const b)   = b
eval s (Var x)     = find x s
eval s (Not p)     = not(eval s p)
eval s (And p q)   = eval s p && eval s q
eval s (Imply p q) = eval s p <= eval s q

vars :: Prop -> [Char]
vars (Const _)   = []
vars (Var x)     = [x]
vars (Not p)     = vars p
vars (And p q)   = vars p ++ vars q
vars (Imply p q) = vars p ++ vars q

{-
bools n = map (map conv . make n . int2bin) [0..limit]
          where
             limit = (2 ^ n) - 1;
             make n bs = take n (bs ++ repeat 0)
             conv 0 = False
             conv 1 = True
-}

bools :: Int -> [[Bool]]
bools 0     = [[]]
bools n = map (False:) bn ++ map (True:) bn
          where 
             bn = bools (n-1)
             
remdup :: Eq a => [a] -> [a]
remdup [] = []
remdup (testa:coda) = testa:remdup(select (/= testa) coda)

substitute :: Prop -> [Subst]
substitute p = map (zip vs) (bools (length vs))
               where
                  vs = remdup (vars p)

isTaut :: Prop -> Bool
isTaut p = and [eval s p | s <- substitute p]

------------------- Numeri primi ------------------------------

factors :: Int -> [Int]
factors n = [x | x <- [1..n], n `mod` x == 0]

prime :: Int -> Bool
prime n = factors n == [1,n]

primes :: Int -> [Int]
primes n = [x | x <- [2..n], prime x ]

primi :: [Int]
primi = setaccio [2..]

setaccio :: [Int] -> [Int]
setaccio (p:coda) = p : setaccio [x | x <- coda, x `mod` p /= 0]

------------------- Valutazione stretta -----------------------

sumwith :: Int -> [Int] -> Int
sumwith v [] = v
sumwith v (testa:coda) = sumwith (v + testa) coda

sumwith' :: Int -> [Int] -> Int
sumwith' v [] = v
sumwith' v (testa:coda) = (sumwith' $! (v + testa)) coda

--------------------- TEMI D'ESAME ----------------------------
-- Esercizio 1

member :: Ord a => a -> [a] -> Bool
member n ordlist
   | ordlist == []         = False
   | head ordlist < n     = member n (tail ordlist)
   | head ordlist == n    = True
   | otherwise            = False

-- quadrati :: [Integer]
-- quadrati = [ n^2 | n <- [ 0.. ] ]

is_square :: Integer -> Bool
is_square n =
  member n quadrati

-- Esercizio 2

reverse' :: [a] -> [a]
reverse' []            =    [] 
reverse' (testa:coda)  =    (reverse' coda) ++ [testa]

-- Esercizio 3

type Codice = Integer
type Nome = String
type Prezzo = Integer

type Prodotti = [(Codice, Nome, Prezzo)]

selezione :: Prodotti -> Prezzo -> Prezzo -> [ Nome ]
selezione prodotti min max = [ n | (c, n, p) <- prodotti, p >= min, p <= max ]

-- Esercizio 4

type Matricola = Integer
type Anno = Integer
type Corso = String
-- type Nome = String

type Studenti = [(Matricola, Anno, Corso)]
type Docenti = [(Nome, Corso)]

docenti_dello_studente :: Studenti -> Docenti -> Matricola -> [ Nome ]
docenti_dello_studente stud doc mat = 
   [ n | (m, a, c) <- stud, (n, c') <- doc, c==c',m==mat ]

-- Esercizio 5

-- type Matricola = Integer
-- type Corso = String
type Voto = Integer
type Esami = [(Matricola, Corso, Voto)]

esamiSostenuti :: Esami -> Matricola -> [ Corso ]
esamiSostenuti esami matricola = [ c | (m, c, v) <- esami, m==matricola ]

-- Esercizio 6

-- type Corso = String
type Giorno = String
type Ora = Int
type Aula = String 
type Lezione = (Corso, Giorno, Ora, Ora, Aula)
type Orario = [Lezione]

auleOccupate :: Orario -> Giorno -> Ora -> [Aula]
auleOccupate orario giorno ora = [ a | (c, g, o1, o2, a) <- orario,
                                          g == giorno, 
                                          ora >= o1, ora <= o2 ]

-- Esercizio 7

type Titolo = String
type Autore = String
type Edizione = String
-- type Nome = String
type Docente = String 
-- type Libro = String
type Cognome = String
-- type Corso = String
type Libri = [(Titolo, Autore, Edizione)]
type Corsi = [(Nome, Docente, Libro)]
type Studenti' = [(Cognome, Corso)]

studenti_autori :: Libri -> Corsi -> Studenti' -> [ Cognome ]
studenti_autori libri corsi studenti = [ cg | (cg, c) <- studenti, 
                                              (n, d, l) <- corsi,
                                              (t, a, e) <- libri,
                                               c == n, l == t, a == cg ]

-- Esercizio 8

type NomeCitta = String
type NumAbitanti = Int
type NomeFiume = String
type Lunghezza = Int

type Citta = [(NomeCitta, NumAbitanti)]
type Fiumi = [(NomeFiume, Lunghezza)]
type Attraversamenti = [(NomeFiume, NomeCitta)]

grosse_citta_con_lunghi_fiumi :: Citta -> Fiumi -> Attraversamenti -> [ NomeCitta ]
grosse_citta_con_lunghi_fiumi citta fiumi attraversamenti =
  [ c | (c, n) <- citta, (f, l) <- fiumi, (nf, nc) <- attraversamenti, 
        c == nc, f == nf, n > 1000000, l > 500 ]

-- Esercizio 9

computa :: (Integer -> Integer) -> (Integer -> Integer) -> [Integer] -> [Integer]
computa f g [] = []
computa f g (p:coda) = (f p + g p):(computa f g coda)

computa' :: (Integer -> Integer) -> (Integer -> Integer) -> [Integer] -> [Integer]
computa' f g lista = [f n + g n | n <- lista]

-- Esercizio 10

last' :: [a] -> a
last' (testa:[])  = testa
last' (testa:coda) = last' coda 

-- Esercizio 11

shrink :: [a] -> [a]
shrink [] = []
shrink [x] = [x]
shrink (testa:(testa2:coda2)) = testa:(shrink coda2)

-- Esercizio 12

campionato :: [String] -> [(String, String)]
campionato squadre =
    [(x,y) | x <- squadre, y <- squadre, x /= y ]

-- Esercizio 13

unzip' lista = ([x | (x,_) <- lista], [y | (_,y) <- lista])

unzip'' lista = (partex lista, partey lista) 
               where
			     partex [] = []
			     partex ((x,_):coda) = x:(partex coda)
			     partey [] = []
			     partey ((_,y):coda) = y:(partey coda)

unzip''' []           = ([],[])
unzip''' ((x,y):coda) = (x:listax, y:listay)
                      where 
                       (listax, listay) = unzip''' coda

-- Esercizio 14

take' :: Int -> [a] -> [a]
take' 0 _ = []
take' _ [] = []
take' n (testa:coda) = testa:(take' (n-1) coda)

-- Esercizio 15

prefix :: Eq a => [a] -> [a] -> Bool
prefix [] _                            =   True
prefix _ []                            =   False
prefix (testa1:coda1) (testa2:coda2)   =   testa1 == testa2 && prefix coda1 coda2

-- Esercizio 16

all :: Eq a => [a] -> (a -> Bool) -> Bool
all lista f = ([x | x <- lista, f x ] == lista)

all' :: Eq a => [a] -> (a -> Bool) -> Bool
all' [] f = True
all' (testa:coda) f = (f testa) && (all' coda f)

-- Esercizio 17

manca :: Eq a => a -> [a] -> Bool
manca _ [] = True
manca x (testa:coda) = x /= testa && manca x coda

unione :: Eq a => [a] -> [a] -> [a]
unione [] lista2 = lista2
unione (testa:coda) lista2 = if manca testa lista2 then 
                                   testa:(unione coda lista2) 
                             else 
                                   unione coda lista2

-- Esercizio 18

naturals ::  Int -> [Int]
naturals 0 = []
naturals n = naturals (n-1) ++ [n-1]

-- Esercizio 19

catena :: [Integer -> Integer] -> Integer -> Integer
catena [] valore = valore
catena (f:coda) valore = f(catena coda valore)

catena' funzioni valore = ((foldr (.) id) funzioni) valore

-- Esercizio 20

type Id = String
type Madre = String
type Padre = String
type Figlio = String
type Famiglie = [(Id, Madre, Padre, [ Figlio ])]

istanza :: Famiglie
istanza = [("rossi", "anna", "andrea", ["alessio", "angela"]),
            ("bianchi", "rosa", "rino", ["rino", "renata", "rachele"]),
            ("neri", "monica", "mino", ["mina", "mauro"]),
            ("gialli", "serena", "sergio", ["sonia", "sofia", "serena"])
            ]

omonimie :: Famiglie -> [ Id ]
omonimie famiglie = 
  [ i | (i, m, p, f) <- famiglie, n <- f, n==m || n==p ] 

-- Esercizio 21

type Clienti = [(String, String, String)]
type Filiali = [(String, String, Integer)]

clienti :: Clienti
clienti = [("luigi", "viale italia", "milano"),
           ("anna", "via manzoni", "pavia"),
           ("piero", "via xx settembre", "brescia"),
           ("luisa", "via capecelatro", "milano"),
           ("andrea", "viale europa", "pavia"),
           ("rita", "via miramonti", "roma")]
           
filiali :: Filiali
filiali = [("koala", "brescia", 20000000),
           ("pantera", "roma", 35000000),
           ("coccodrillo", "pavia", 11000000),
           ("zebra", "milano", 45000000),
           ("leone", "venezia", 18000000),
           ("gatto", "catania", 22000000)]

stessa_citta :: Clienti -> Filiali -> [(String, String)]
stessa_citta clienti filiali = 
  [ (c,f) | (c, i, cc) <- clienti, (f, cf, d) <- filiali, cc==cf ] 

-- Esercizio 22

applica :: (Integer -> Integer) -> (Integer -> Integer) -> [Integer] -> [Integer]
applica f g [] = []
applica f g (testa:coda) = (f  (g testa)):(applica f g coda)

applica' :: (Integer -> Integer) -> (Integer -> Integer) -> [Integer] -> [Integer]
applica' f g lista = map (f . g) lista

-- Esercizio 23

dispari' :: Int -> [Int]
dispari' 0 = []
dispari' n = dispari' (n-1) ++ [n*2-1]

-- Esercizio 24

coppie :: [a] -> (a -> a -> Bool) -> [(a,a)] 
coppie lista f = [ (x,y) | x <- lista, y <- lista, f x y ]

-- Esercizio 25

gen :: [(String,[String])]
gen = [("guido", ["luisa","franco","elena"]),
       ("luisa", ["andrea","dario","gino"]),
       ("franco", ["giovanni","paola","letizia","sofia"]),
       ("elena", ["emma","zeno"]),
       ("andrea", ["rino","anna"])]

discendenti :: [(String,[String])] -> String -> [ String ]

discendenti genealogia persona = 
  [ f | (genitore,figli) <- genealogia, genitore == persona, f <- figli ] ++
  [ d | (genitore,figli) <- genealogia, genitore == persona, 
                       f <- figli, d <- (discendenti genealogia f) ]

-- Esercizio 26

-- type Libri = [(String,String,Int)]
-- type Prestiti = [(String,String)]
type Lettori = [(String,Int,String)]

autolettori :: Libri -> Prestiti -> Lettori -> String -> Int -> [String]

autolettori libri prestiti lettori citta anno = 
  [n | (t,a,_) <- libri, (l,le) <- prestiti, (n,an,c) <- lettori,
       t == l, le == n, a == le, an < anno, c == citta ]

-- Esercizio 27

medie :: [Float->Float] -> [Float->Float] -> [Float] -> [Float]

medie [] [] [] = []
medie (f:codaF) (g:codaG) (r:codaR) = (((f r)+(g r))/2):(medie codaF codaG codaR)

-- Esercizio 28

integrale :: (Float -> Float) -> Float -> Float -> Int -> Float
integrale f x1 x2 0 = 0
integrale f x1 x2 n = (f x1) * dx + (integrale f (x1 + dx) x2 (n-1))
                        where 
                          dx = (x2 - x1) / fromIntegral n

-- Esercizio 29

uguali :: Eq a => [a] -> Bool
uguali [] = True
uguali [_] = True
uguali(testa:(testa2:coda2)) = testa == testa2 && uguali(testa2:coda2)

-- Esercizio 30

affetta :: Int -> [a] -> [[a]]
affetta _ [] = []
affetta n lista = (take n lista):(affetta n (drop n lista))

-- Esercizio 31

type Anagrafe = [(String, Integer, String)]

compaesaniCoscritti :: Anagrafe -> String -> [String]

compaesaniCoscritti anagrafe persona =
  [ p' | (p, a, c) <- anagrafe, 
         (p', a', c') <- anagrafe,
          p == persona, p' /= persona, a' == a, c == c' ]
          
-- Esercizio 32

type Studenti'' = [(String, [(String, Integer)])]

superato :: String -> Studenti'' -> [String]

superato esame studenti =
  [ s | (s, e) <- studenti, (c, v) <- e, esame == c ]
  
-- Esercizio 33

data Expr = Const' Int
	    | Var' String
	    | Plus Expr Expr
	    | Minus Expr Expr
	    | Mult Expr Expr
	 
type Stato = [(String, Int)]

valore' :: String -> Stato -> Int
valore' i s = head [v | (nome,v) <- s, nome == i ]

valexpr :: Expr -> Stato -> Int
valexpr (Const' n) _ = n
valexpr (Var' i) s = valore' i s
valexpr (Plus x y) s = (valexpr x s) + (valexpr y s)
valexpr (Minus x y) s = (valexpr x s) - (valexpr y s)
valexpr (Mult x y) s = (valexpr x s) * (valexpr y s)

-- Esercizio 34

data Albero = Nodo Int [Albero]

frassino :: Albero
frassino = (Nodo 3 [(Nodo 4 [(Nodo 1 []),(Nodo 5 [])]),(Nodo 2 []),(Nodo 7 [])])

sommalbero :: Albero -> Int
sommalbero(Nodo n figli) = n + (somma figli)

somma :: [Albero] -> Int
somma [] = 0
somma (x:xs) = (sommalbero x) + (somma xs)

sommalbero' :: Albero -> Int
sommalbero' (Nodo n figli) = n + sum [ sommalbero' f | f <- figli ]

-- Esercizio 35

data Lexpr = List [Int]
	     | Var'' String
	     | Cat Lexpr Lexpr
	     | Rev Lexpr
	 
type State = [(String, [Int])]

valore :: String -> State -> [Int]
valore n s = head [lista | (nome,lista) <- s, nome == n ]

eval' :: Lexpr -> State -> [Int]
eval' (List lista) _ = lista
eval' (Var'' n) s = valore n s
eval' (Cat x y) s = (eval' x s) ++ (eval' y s)
eval' (Rev x) s = reverse (eval' x s)

{-

-- Esercizio 36

-- Use ghci -XFlexibleInstances

class AddNeg a where
  add :: a -> a -> a
  neg :: a -> a

instance AddNeg Int where
  add n m = n + m
  neg n = -n

instance AddNeg Bool where
  add b1 b2 = b1 || b2
  neg b =  not b

instance AddNeg [Int] where
  add [] _ = []
  add _ [] = []
  add  (x:xs) (y:ys) = (x+y):(add xs ys)
  neg [] = []
  neg (x:xs) = (-x):(neg xs)
  
-- Esercizio 37

data Expr = Set [Int]
	    | Var String
	    | Union Expr Expr
	    | Inter Expr Expr
	    | Select (Int -> Bool) Expr
	 
type State = [(String, [Int])]

computa :: Expr -> State -> [Int]
computa (Set xs) _ = xs
computa (Var nome) stato = head [ l | (n,l) <- stato, n == nome ]
computa (Union e1 e2) s = unione (computa e1 s) (computa e2 s)
computa (Inter e1 e2) s = intersezione (computa e1 s) (computa e2 s)
computa (Select p e) s = seleziona p (computa e s)

unione [] ys = ys
unione (x:xs) ys = if elem x ys then unione xs ys else x:(unione xs ys)
		      
intersezione [] ys = []
intersezione (x:xs) ys = if elem x ys then x:(intersezione xs ys) else intersezione xs ys
			    
seleziona _ [] = []
seleziona p (x:xs) = if p x then x:(seleziona p xs) else seleziona p xs
			
-- Esercizio 38

data Tree = Node Int [Tree]

somma :: Tree -> Int
somma' :: [Tree] -> Int

somma' [] = 0
somma' ((Node n figli):coda) = n + (somma' figli) + (somma' coda)

somma t = somma' [t]

-- oppure:

somma :: Tree -> Int
somma (Node n figli) = n + sum (map somma figli)

-- Esercizio 39

data Expr = Vec [Float]
	    | Var String
	    | Sum Expr Expr
	    | Sub Expr Expr
	    | Mul Expr Expr
	    | Div Expr Expr
	 
type State = [(String, [Float])]

matVec :: [Float] -> (Float -> Float -> Float) -> [Float] -> [Float]
matVec [] _ _ = []
matVec _ _ [] = []
matVec (x:xs) op (y:ys) = (op x y):(matVec xs op ys)

eval :: Expr -> State -> [Float]
eval (Vec xs) _ = xs
eval (Var nome) stato = head [ l | (n,l) <- stato, n == nome ]
eval (Sum e1 e2) s = matVec (eval e1 s) (+) (eval e2 s)
eval (Sub e1 e2) s = matVec (eval e1 s) (-) (eval e2 s)
eval (Mul e1 e2) s = matVec (eval e1 s) (*) (eval e2 s)
eval (Div e1 e2) s = matVec (eval e1 s) (/) (eval e2 s)

-- Esercizio 40

data Albero = Nodo Int [Albero]

concatena :: [[Int]] -> [Int]
concatena [] = []
concatena (x:xs) = x ++ (concatena xs)

appiattisci :: Albero -> [Int]
appiattisci (Nodo n figli) = [n]++(concatena (map appiattisci figli))

complementa :: Albero -> Albero
complementa (Nodo n figli) = (Nodo (-n) (map complementa figli))

-- Esercizio 41

data Albero = Nodo Float [Albero]

somma :: Albero -> Float
somma (Nodo n figli) = n + sum [somma f | f <- figli ]

num :: Albero -> Float
num (Nodo n figli) = 1 + sum [num f | f <- figli ]

media :: Albero -> Float
media a = s / n
          where s = somma a
                n = num a

-- Esercizio 42

class Expr a where
  somma :: a-> a-> a

instance Expr Int where
  somma n m = n + m

instance Expr [Int] where
  somma numeri1 numeri2 = [n1+n2 | (n1,n2) <- zip numeri1 numeri2]

instance Expr [[Int]] where
  somma lista1 lista2 = [num1++num2 | (num1,num2) <- zip lista1 lista2]
  
-- Esercizio 43
  
  data Expr = Number Int
          | Const String
          | Plus Expr Expr
          | Fun (Int->Int) Expr

type Bindings = [(String, Int)]

computa :: Expr -> Bindings -> Int
computa (Number n) _ = n
computa (Const s) bs = head [ n | (name, n) <- bs, name == s ]
computa (Plus e1 e2) bs = (computa e1 bs) + (computa e2 bs)
computa (Fun f e) bs = f (computa e bs)
  
  -}

