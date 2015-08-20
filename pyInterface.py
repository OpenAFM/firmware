from	time	import	sleep
import	serial
import matplotlib.pyplot as plt
import re
ser	=	serial.Serial('COM4',	9600)	#	Establish	the	connection	on	a	specific	port
ser.write('SW;')
while	True:
	ser.write('SW;')
	#	Convert	the	decimal	number	to	ASCII	then	send	it	to	the	Arduino
	data=	ser.readline()	
	#	Read	the	newest	output	from	the	Arduino
	matchnum=re.compile("[0-9]")
	matchdata=re.compile(".*;")

	print type(data)
	try:
		str=matchnum.search(data[0:5]).group()
	except:
		str=""
		
	if not str:
		print "empty"
	if str:
		data=matchdata.search(data).group()

		data=[int(x) for x in data.split(',')[:-1] if x]
		print data
		plt.plot(range(len(data)),data)
		plt.show()

	sleep(.1)	#	Delay	for	one	tenth	of	a	second

