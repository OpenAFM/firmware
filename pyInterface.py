from	time	import	sleep
import	serial
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
from matplotlib import animation
import re

figure=plt.figure()
ser	=	serial.Serial('COM5',	9600)	#	Establish	the	connection	on	a	specific	port


def update():
	ser.write('RDY;')
	#	Convert	the	decimal	number	to	ASCII	then	send	it	to	the	Arduino
	print "read data"
	data=	ser.readline()	
	data=	ser.readline()
	print "done"
	sleep(.1)	
	#	Read	the	newest	output	from	the	Arduino
	
	matchnum=re.compile("[0-9]")
	matchdata=re.compile(".*;")
	try:
		str=matchnum.search(data[0:5]).group()
	except:
		str=""
		
	if not str:
		print "empty"
		return [0, 0]
	if str:
		data=matchdata.search(data).group()	
		data=[int(x) for x in data.split(',')[:-1] if x]
		print data
		return data

def animate(frame):
   plt.cla()
   axes=plt.axes(xlim=(0,512), ylim=(0,2100))
   data=update()
   axes.scatter(range(len(data)),data)

anim = animation.FuncAnimation(figure, animate,
                               frames=50, interval=50)
plt.show()