:- dynamic signal/2.
high(X) :- signal(X,1).
low(X) :- signal(X,0).
%my_connect(_,Y) :- is_earthed(Y),!,write('Wire is grounded at:'),write(Y),assert(signal(Y,0)).
%my_connect(X,Y) :- is_open(X,Y),!,write('Wire is open between '),write(X),write(' and ') ,write(Y),assert(signal(Y,0)).
my_connect(X,Y) :- low(X),!, assert(signal(Y,0)).
my_connect(X,Y) :- high(X),!,assert(signal(Y,1)).	
my_and(X) :- high(in(1,X)),high(in(2,X)),!,assert(signal(output(X),1)).
my_and(X) :- assert(signal(output(X),0)).
my_or(X) :- low(in(1,X)),low(in(2,X)),!,assert(signal(output(X),0)).
my_or(X) :- assert(signal(output(X),1)),!.
my_not(X) :- high(in(1,X)),!,assert(signal(output(X),0)).
my_not(X) :- assert(signal(output(X),1)).
%is_earthed(in(1,a4)).
%is_open(_,_):-!,fail.


fulladder_truthtable([1,1,1],[1,1]).
fulladder_truthtable([1,1,0],[0,1]).
fulladder_truthtable([1,0,1],[0,1]).
fulladder_truthtable([0,1,1],[1,1]).
fulladder_truthtable([0,0,1],[1,0]).
fulladder_truthtable([0,1,0],[1,0]).
fulladder_truthtable([1,0,0],[1,0]).
fulladder_truthtable([0,0,0],[0,0]).

xor_truthtable([0,0],0).
xor_truthtable([1,1],0).
xor_truthtable([0,1],1).
xor_truthtable([1,0],1).

