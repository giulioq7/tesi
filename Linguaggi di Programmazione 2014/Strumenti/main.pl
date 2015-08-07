% Strutture

legge(anna, libro).
legge(luigi, libro(psicoanalisi, freud)).
legge(silvia, libro(psicoanalisi, autore(sigmund, freud))).

% ?- legge(X, libro).
% ?- legge(luigi, libro(psicoanalisi, X)).
% ?- legge(X, L).

% Operatori

p(2+3).
p(5+7).

% ?- p(X).
% ?- p(X), Y = X.
% ?- p(X), Y is X.
% ?- legge(anna, Y) = legge(X, libro).
% ?- X == 3.
% ?- X = 2, X == Y.
% ?- X = 2, Y = 2, X == Y.
% ?- X = 2, Y = 3, X == Y.
% ?- X = Y, X == Y.

% Comunicazione

parla(alberto, russo).
parla(roberto, inglese).
parla(maria, russo).
parla(maria, inglese).
comunica(X, Y) :- parla(X, L), parla(Y, L), X \= Y.

% ?- parla(Chi, russo).
% ?- comunica(roberto, alberto).
% ?- comunica(Chi, alberto).

% Genealogia

madre(maria, sara).
madre(maria, bruno).
madre(sara, nicola).
madre(sara, giulia).
madre(gina, rino).

padre(giovanni, sara).
padre(giovanni, bruno).
padre(roberto, nicola).
padre(roberto, giulia).
padre(bruno, rino).

genitore(X, Y) :- madre(X, Y).
genitore(X, Y) :- padre(X, Y).

nonno(X, Y) :- padre(X, Z), genitore(Z, Y).
nonna(X, Y) :- madre(X, Z), genitore(Z, Y).

fratello(X, Y) :- genitore(Z, X), 
                  genitore(Z, Y), X \= Y.

antenato(X, Y) :- genitore(X, Y).
antenato(X, Y) :- genitore(X, Z), 
                  antenato(Z, Y).

% ?- fratello(nicola, giulia).
% ?- nonno(Chi, rino).
% ?- antenato(Chi, giulia).

genitori(X,[]) :- \+(genitore(_,X)).
genitori(X,[M,P]) :- madre(M,X), padre(P,X).

antenati(X,A) :- genitori(X,G), antenati_aggregati(G,AG), append(G,AG,A).

antenati_aggregati([],[]).
antenati_aggregati([H|T],AG) :- antenati(H,AH), 
                                antenati_aggregati(T,AT), 
                                append(AH,AT,AG).

% ?- antenati(giulia, A).

% Asserzioni

inquilini(Palazzo) :- 
       Palazzo = [piano(_,5),piano(_,4),piano(_,3),piano(_,2),piano(_,1)],
       member(piano(barbara, B), Palazzo), B \= 5,
       member(piano(claudio, C), Palazzo), C \= 1,
       member(piano(francesca, F), Palazzo), F \= 1, F \= 5, 
       member(piano(marco, M), Palazzo), M > C,
       member(piano(silvia, S), Palazzo), 
       \+(adiacente(S, F)), \+(adiacente(F, C)).

adiacente(X, Y) :- X is Y+1.
adiacente(X, Y) :- X is Y-1.

% ?- inquilini(X).

% inquilini(I1, I2, I3, I4, I5) :- 
%       Piani = [piano(I5,5),piano(I4,4),piano(I3,3),piano(I2,2),piano(I1,1)],
%       member(piano(barbara, B), Piani), B \= 5,
%       member(piano(claudio, C), Piani), C \= 1,
%       member(piano(francesca, F), Piani), F \= 1, F \= 5, 
%       member(piano(marco, M), Piani), M > C,
%       member(piano(silvia, S), Piani), 
%       \+(adiacente(S, F)), \+(adiacente(F, C)).

% ?- inquilini(I1, I2, I3, I4, I5).
% ?- inquilini(I1, claudio, barbara, francesca, marco).
% ?- inquilini(claudio, I2, I3, I4, I5).

% Cut

servizi(Persona, Servizio) :- 
    inaffidabile(Persona), !, servizio_limitato(Servizio).

servizi(_, Servizio) :- servizio_generale(Servizio).

servizio_limitato(consultazione).
servizio_limitato(fotocopia).

servizio_esteso(prestito).
servizio_esteso(digitalizzazione).

servizio_generale(Servizio) :- servizio_limitato(Servizio).
servizio_generale(Servizio) :- servizio_esteso(Servizio).

inaffidabile(teodoro).

% ?- servizi(rino,S).
% ?- servizi(teodoro,S).

a(alfa).
a(beta).
b(alfa).
b(beta).
c(gamma).

r(X) :- a(X), b(X).
r(X) :- c(X).

% ?- r(x).

r1(X) :- a(X), !, b(X).
r1(X) :- c(X).

% ?- r1(x).

sommatoria(0,0).
sommatoria(N, S) :- N1 is N-1,
                    sommatoria(N1,S1),
                    S is S1+N.
% ?- sommatoria(10, S).

sommatoria1(0,0) :- !.
sommatoria1(N, S) :- N1 is N-1,
                     sommatoria1(N1,S1),
                     S is S1+N.

% ?- sommatoria1(10, S).
% ?- sommatoria1(-10, S).

sommatoria2(0,0).
sommatoria2(N, S) :-  N > 0,
                N1 is N-1,
                sommatoria2(N1,S1),
                S is S1+N.

% ?- sommatoria2(-10, S).

append1([], X, X) :- !.
append1([H|T], Y, [H|Z]) :- append1(T, Y, Z).

% ?- append(X,Y,[a,b,c]).
% ?- append1([a,b,c],X,Y).
% ?- append1(X,Y,[a,b,c]).

 last(X, [X]).
 last(X,[_|Y]) :- last(X, Y).

consecutivi(X, Y, [X,Y|_]).
consecutivi(X, Y, [_|Z]) :- consecutivi(X, Y, Z).

prefix([],_).
prefix([X|Coda1], [X|Coda2]) :- prefix(Coda1, Coda2).

numero_di_genitori(adamo, 0) :- !.
numero_di_genitori(eva, 0) :- !.
numero_di_genitori(_, 2).

% ?- numero_di_genitori(eva, N).
% ?- numero_di_genitori(luisa, N).
% ?- numero_di_genitori(eva, 2).

numero_di_genitori1(adamo, N) :- !, N = 0.
numero_di_genitori1(eva, N) :- !, N = 0.
numero_di_genitori1(_, 2).

% ?- numero_di_genitori1(eva, 2).
% ?- numero_di_genitori1(P, N).


numero_di_genitori2(adamo, 0).
numero_di_genitori2(eva, 0).
numero_di_genitori2(X, 2) :- X \= adamo, X \= eva.

% ?- numero_di_genitori2(eva, 2).
% ?- numero_di_genitori2(P, N).

 delete(_, [], []).
 delete(X, [X|L], D) :- !, delete(X, L, D).
 delete(X, [Y|L1], [Y|L2]) :- !, delete(X, L1, L2).

subst(_, [], _, []).
subst(E, [E|L1], S, [S|L2]) :- !, subst(E, L1, S, L2).
subst(E, [X|L1], S, [X|L2]) :- subst(E, L1, S, L2).

remdup(L,R):- dupacc(L, [], R).

dupacc([], A, A).
dupacc([Testa|Coda], A, R) :- member(Testa, A), !, dupacc(Coda, A, R).
dupacc([Testa|Coda], A, R) :- dupacc(Coda, [Testa|A], R).

subset([], _).
subset([X|Coda], Y) :- member(X, Y), subset(Coda, Y).

union([], X, X).
union([X|Coda], Y, Z) :-  member(X, Y), !, union(Coda, Y, Z).
union([X|Coda], Y, [X|Z]) :-  union(Coda, Y, Z).

diff([],_,[]).
diff([X|Coda], Y, Z) :- member(X, Y), !, diff(Coda, Y, Z).
diff([X|Coda], Y, [X|Z]) :- diff(Coda, Y, Z).

intersection([],_,[]).
intersection([X|Cx], Y, [X|Cz]) :- member(X, Y), !, intersection(Cx, Y, Cz).
intersection([_|Cx], Y, Z) :- intersection(Cx, Y, Z).

cartprod([], _, []).
cartprod([Hx|Tx], Y, Z) :- combine(Hx, Y, Pz), cartprod(Tx, Y, Sz), append(Pz, Sz, Z).

combine(_, [], []).
combine(Hx, [Hy|Ty], [pair(Hx, Hy)|Tz]) :- combine(Hx, Ty, Tz).

% Le torri di Hanoi

hanoi(N) :- muovi(N, sinistra, centro, destra).

muovi(0, _, _, _) :- !.
muovi(N, S, D, A) :- M is N-1,
                     muovi(M, S, A, D),
                     visualizza_mossa(S, D),
                     muovi(M, A, D, S).
                     
visualizza_mossa(X, Y) :- writeln([muovi, un, disco, dal, polo, di, X, al, polo, di, Y]).
% Ricerca in un labirinto

porta(a, b).
porta(b, c).
porta(b, e).
porta(c, d).
porta(d, e).
porta(e, f).
porta(g, e).

ha_telefono(g).

comunicanti(X, Y) :- porta(X, Y).
comunicanti(X, Y) :- porta(Y, X).

vai(X, X, L) :- reverse(L, R), writeln(R).
vai(X, Y, L) :- comunicanti(X, Z), \+(member(Z, L)), vai(Z, Y, [Z|L]).

% Copertura di un grafo

nodi([a,b,c,d,e]).
archi([arco(a,b),arco(a,c),arco(b,c),arco(c,e),arco(e,d),arco(d,b),arco(b,e),arco(c,d)]).

collegati(X,Y,A) :- member(arco(X,Y),A).
collegati(X,Y,A) :- member(arco(Y,X),A).

consumato(X,Y,C) :- consecutivi(X,Y,C).
consumato(X,Y,C) :- consecutivi(Y,X,C).

cammino(X) :- nodi(N), archi(A), length(A,La), member(X,N), copri(X,A,La,[X]).

copri(_,_,La,C) :- length(C,Lc), Lc is La+1, reverse(C,R), writeln(R).
copri(X,A,La,C) :- collegati(X,Y,A), \+(consumato(X,Y,C)), copri(Y,A,La,[Y|C]).



