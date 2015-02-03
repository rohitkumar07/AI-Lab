def splitX(s):
	openparanthesis = 0
	closedparanthesis = 0
	answer = []
	for i in range(0,len(s)):
		if s[i] == '(' :
			openparanthesis = openparanthesis + 1
		if s[i] == ')' :
			closedparanthesis = closedparanthesis + 1
		if openparanthesis == closedparanthesis :
			a = s[:i+1]
			b = s[i+2:]
			if (len(a) > 1):
				if a[0] == '(' and a[len(a)-1] == ')':
					a = a[1:-1]
			if (len(b) > 1):
				if b[0] == '(' and b[len(b)-1] == ')':
					b = b[1:-1]
			answer.append(a)
			answer.append(b)
			return answer

def notIsSingle(s):
	if '(' in s or '>' in s:
		return True
	else:
		return False

def theorem1(s):
	if notIsSingle(s):
		temp = splitX(s)
		z = temp[0] + ">"
		z = z + "(" + temp[1] + ">" + temp[0] + ")"
#		z = s + ">" +z
		return z


def theorem2(s):
	temp = splitX(s)
	if notIsSingle(temp[1]):
		temp2 = splitX(temp[1])
		z = "(" + temp[0] + ">" + temp2[0] + ")"
		z = "(" + z + ">" + "(" + temp[0] + ">" + temp2[1] + "))"
		z = "(" + s + ")>" + z
		return z
	else:
		return "NULL"

def modusponens(lhs):
	lhs = list(set(lhs)) # remove duplicates
	rhs = []
	for i in lhs:
		for j in lhs:
			if notIsSingle(j):
				temp = splitX(j)
				if i == temp[0]:
					rhs.append(temp[1])
	return lhs + rhs


#print(theorem2("a>(b>c)"))
e = input()

#print(splitX(e))



lhs = []
while notIsSingle(e) :
	temp = splitX(e)
#	print(temp)
#	print("loop")
	lhs.append(temp[0])
	e = temp[1]

if e != "F":
	lhs.append(str(e + ">F"))
	e = "F"

lhs = list(set(lhs))
print("Initial LHS: ", lhs)
for i in range(0,10):
	if "F" in lhs:
		print("Found!", i)
		break
	else:
		lhsnew = modusponens(lhs)
		if lhsnew == lhs:
			print("I need a hint")
			t = int(input())
			if t == 1:
				p = input()
				q = input()
				z = str(p) + ">" + str(q)
				print(theorem1(z))
				lhsnew.append(theorem1(z))
			if t == 2:
				p = input()
				q = input()
				r = input()
				z = str(p) + ">" + "(" + str(q) + ">" + str(r) +")"
				print(theorem2(z))
				lhsnew.append(theorem2(z))
		else:
			print("Modified by modus-ponens")
			print(lhsnew)
			print(lhs)
		lhs = lhsnew
