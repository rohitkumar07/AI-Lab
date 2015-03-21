lines = [line.strip() for line in open('data.txt') if line[0] <= "Z" and line[0] >= "A"]

#lines.sort()
graphemetable = []

for l in lines:
        graphemetable.append(l.split())

temp = []
for i in range(len(graphemetable)):
    if i > 0 :
        if graphemetable[i][0].startswith(graphemetable[i-1][0]) and "(" in graphemetable[i][0]:
            continue
    temp.append(graphemetable[i])

graphemetable = temp

# state generation
states = {}             # maps string to state

states[""] = 0          # state 0 is null string(epsilon)

stateNum = 1
for i in range(len(graphemetable)):
    for j in range(1, len(graphemetable[i])):
        if not graphemetable[i][j] in states:
            states[graphemetable[i][j]] = stateNum
            stateNum += 1

# print(states)

# forward table


