def Spider_move(X, Y):
	X = abs(X)
	Y = abs(Y)
    g = lambda x : x*g(x-1) if x else 1