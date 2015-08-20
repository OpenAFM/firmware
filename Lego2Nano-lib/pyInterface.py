from	time	import	sleep
import	serial
ser	=	serial.Serial('COM4',	9600)	#	Establish	the	connection	on	a	specific	port

while	True:
	ser.write('IN;')	#	Convert	the	decimal	number	to	ASCII	then	send	it	to	the	Arduino
	print	ser.readline()	#	Read	the	newest	output	from	the	Arduino
	sleep(.1)	#	Delay	for	one	tenth	of	a	second
