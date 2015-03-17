:- dynamic signal/2.
high(X) :- signal(X,1).
low(X) :- signal(X,0).
my_connect(X,Y) :- low(X),!, assert(signal(Y,0)).
my_connect(X,Y) :- high(X),!,assert(signal(Y,1)).	
my_and(X) :- high(in(1,X)),high(in(2,X)),!,assert(signal(output(X),1)).
my_and(X) :- assert(signal(output(X),0)).
my_or(X) :- low(in(1,X)),low(in(2,X)),!,assert(signal(output(X),0)).
my_or(X) :- assert(signal(output(X),1)),!.
my_not(X) :- high(in(1,X)),!,assert(signal(output(X),0)).
my_not(X) :- assert(signal(output(X),1)).