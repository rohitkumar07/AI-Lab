import subprocess

exe = "./festival"

grapheme = raw_input("Enter word to be pronounced: ")

outputpipe = subprocess.Popen(exe, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
outputpipe.stdin.write(grapheme)
outputpipe.stdin.close()

# output = "FEHSTIHVAHL"
output = outputpipe.stdout.read()
output = output.strip()
# print(output)


inputpipe = subprocess.Popen('festival --tts', shell=True, stdin=subprocess.PIPE)
inputpipe.stdin.write(output)
inputpipe.stdin.close()
