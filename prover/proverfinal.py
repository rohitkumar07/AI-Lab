def deParenthesize(s):
	openparanthesis = 0
	closedparanthesis = 0
	if len(s) == 1:
		return s
	for i in range(0,len(s)):
		if s[i] == '(' :
			openparanthesis = openparanthesis + 1
		if s[i] == ')' :
			closedparanthesis = closedparanthesis + 1
		if openparanthesis == closedparanthesis :
			if i == len(s) -1 :
				return deParenthesize(s[1:-1])
			else:
				return s

def parenthesize(s):
	if '>' in s:
		return "(" + deParenthesize(s) + ")"
	else:
		return deParenthesize(s)



def notIsSingle(s):
	if '(' in s or '>' in s:
		return True
	else:
		return False

def splitX(s):
	openparanthesis = 0
	closedparanthesis = 0
	s = deParenthesize(s)
	if not "(" in s:
		return s.split(">")
	else:
		answer = []
		for i in range(0,len(s)):
			if s[i] == '(' :
				openparanthesis = openparanthesis + 1
			if s[i] == ')' :
				closedparanthesis = closedparanthesis + 1
			if openparanthesis == closedparanthesis :
				a = s[:i+1]
				b = s[i+2:]
				answer.append(deParenthesize(a))
				answer.append(deParenthesize(b))
				return answer



def formImplies(a,b):
	return parenthesize(a) + ">" + parenthesize(b)

def theorem1(a, b):
	return formImplies(a,formImplies(b,a))

def theorem2(a,b,c):
	return formImplies(formImplies(a, formImplies(b,c)), formImplies(formImplies(a,b), formImplies(a,c)))

def theorem3(a):
	return formImplies(formImplies(formImplies(a,"F"),"F"),a)

def contrapositve(p,q):
	return formImplies(formImplies(p,q), formImplies(formImplies(q,"F"),formImplies(p,"F")))


def convertNot(s):
	s = deParenthesize(s)
	if not "~" in s:
		return s
	i = s.find("~")
	openparanthesis = 0
	closedparanthesis = 0
	for x in range(i+1,len(s)):
		if s[x] == '(' :
			openparanthesis = openparanthesis + 1
		if s[x] == ')' :
			closedparanthesis = closedparanthesis + 1
		if openparanthesis == closedparanthesis :
			z = s[:i] + "(" + parenthesize(s[i+1:x+1]) + ">F)" + s[x+1:]
			return convertNot(deParenthesize(z))



def convertOr(s):
	s = deParenthesize(s)
	if not "v" in s:
		return s
	i = s.find("v")
	openparanthesis = 0
	closedparanthesis = 0

	l = i - 1
	r = i + 1

	while 1:
		if s[l] == '(' :
			openparanthesis = openparanthesis + 1
		if s[l] == ')' :
			closedparanthesis = closedparanthesis + 1
		if openparanthesis == closedparanthesis :
			break
		l = l - 1

	while 1:
		if s[r] == '(' :
			openparanthesis = openparanthesis + 1
		if s[r] == ')' :
			closedparanthesis = closedparanthesis + 1
		if openparanthesis == closedparanthesis :
			break
		r = r + 1

	#  p v q = (p>F)>q
	s = s[:l] + formImplies(formImplies(s[l:i], "F"), s[i+1:r+1]) + s[r+1:]
	return convertOr(s)

# print(convertOr("(pvq)>(pvq)"))

def convertAnd(s):
	s = deParenthesize(s)
	if not "^" in s:
		return s
	i = s.find("^")
	openparanthesis = 0
	closedparanthesis = 0

	l = i - 1
	r = i + 1

	while 1:
		if s[l] == '(' :
			openparanthesis = openparanthesis + 1
		if s[l] == ')' :
			closedparanthesis = closedparanthesis + 1
		if openparanthesis == closedparanthesis :
			break
		l = l - 1

	while 1:
		if s[r] == '(' :
			openparanthesis = openparanthesis + 1
		if s[r] == ')' :
			closedparanthesis = closedparanthesis + 1
		if openparanthesis == closedparanthesis :
			break
		r = r + 1

	#  p ^ q = (p>(q>F))>F
	s = s[:l] + formImplies(formImplies(s[l:i], formImplies(s[i+1:r+1], "F")) , "F")  + s[r+1:]
	return convertAnd(s)

# print(convertAnd("(p^q)>(p^q)"))

def parse(e):
	e = e.replace(" ","")	# removing whitespaces
	e = e.replace("->",">")
	e = convertNot(e)
	e = convertOr(e)
	e = convertAnd(e)
	print("Parsed form: ", e)
	return e

def modusponens(lhs):
	lhs = list(set(lhs)) # remove duplicates
	rhs = []
	for i in lhs:
		for j in lhs:
			if notIsSingle(j):
				temp = splitX(j)
				if i == temp[0]:
					rhs.append(temp[1])
	return list(set(lhs + rhs))



#print(parenthesize("a"))
'''
print(contrapositve("p","q"))
print(theorem2("a", "b>q", "c"))
print(theorem1("a","(b>c)"))
print(theorem3("a"))
#print(deParenthesize("(a>(b>c))"))
'''
def formHypothesisSet(e):
	lhs = []
	while notIsSingle(e):
		temp = splitX(e)
		lhs.append(temp[0])
		e = temp[1]
	if e != "F":
		lhs.append(str(e + ">F"))
		e = "F"
	return lhs


def hypoRecurse(lhs):	# lhs is set of hypothesis
	lhs = list(set(lhs))
	lhs.sort()
	if len(lhs) == 1:
		if not lhs[0] == "F" and not notIsSingle(lhs[0]):
			return False

	print("\nInitial Hypothesis set: ", lhs)
	for i in range(0,10000):
		if "F" in lhs:
			print("Found!", i, "\n")
			return True
		else:
			lhsnew = modusponens(lhs)			
			lhsnew.sort()
			if lhsnew == lhs:
				Found = False
				for j in range(0,len(lhsnew)):
					if notIsSingle(lhsnew[j]):
						temp = splitX(lhsnew[j])
						x = formHypothesisSet(temp[0])
						x = x + lhsnew[:j] + lhsnew[j+1:]
						x = list(set(x))
						x.sort()
						if not x == lhsnew:
							if hypoRecurse(x):
								Found = True
								lhsnew = lhsnew[:j] + lhsnew[j+1:]
								lhsnew.append(temp[0])
								lhsnew.append(temp[1])
								lhsnew = list(set(lhsnew))
								print("New hypothesis set: ", lhsnew)
								break
				if not Found:
					return Found

			else:
				print("Modified by modus-ponens!")
				print(lhs)
				print(lhsnew)
			lhs = lhsnew
	return 0

e = parse(input())
if hypoRecurse(formHypothesisSet(e)):
	print ("Theorem Proved!")
else :
	print ("Theorem not correct!")

