def Spider_move(X, Y):
	X = abs(X)
	Y = abs(Y)
	def f(x):
		if (x) return x*f(x-1)
		return 1

	return f(X+Y)/f(X)/f(Y)
