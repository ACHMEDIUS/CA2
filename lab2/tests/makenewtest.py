import subprocess
import sys

name = input('Enter name of the openrisc assembly instruction without l. for the new test: ')
filename = name + '.s'
conffilename = name + '.conf'
print('Making '+ filename + ' and ' + conffilename + ' for you: ')
f = open(filename, mode='w')
middlebody = name+""" 	r5,r1,r2 #1 % 2
	l."""+name +""" 	r6,r3,r4 #3 % 0"""
if name.endswith('i'):
	middlebody = name+""" 	r5,r1,2 #1 % 2
	l."""+name +""" 	r6,r3,0 #3 % 0"""
body = """#todo
	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	l."""+middlebody+ """
	.word	0x40ffccff
	.size	_start, .-_start
"""

f.write(body)
f.close()

fconf = open(conffilename, mode='w')
confbody = """[pre]
R1=1
R2=2
R3=3
R4=0

[post]
R1=1
R2=2
R3=3
R4=0
R5=12
R6=30
"""
fconf.write(confbody)

fconf.close()
execname = 'make ' + name + '.bin;'
# source: https://stackoverflow.com/questions/20114390/python-call-makefile-compiling-and-capture-makes-output
make_process = subprocess.Popen(execname, shell=True, stdout=subprocess.PIPE, stderr=sys.stdout.fileno())
while True:
	line = make_process.stdout.readline()
	if not line:
		break
	print(line),  # output to console in time
	sys.stdout.flush()
if make_process.wait() != 0:
	print('There was an error compiling the file. Maybe ask Lucas to run this part')
else:
	print('compilation seems to be successful, so appending new test to tests.tx and opening windows for editing')
	ftests = open('tests.txt',mode='a')
	ftests.write(name+': added by makenewtest.py. todo: implement .conf- and .s-files \n')
	ftests.close()
