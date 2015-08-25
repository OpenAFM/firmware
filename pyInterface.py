from	time	import	sleep
import	serial
import	matplotlib
matplotlib.use('TkAgg')
import	matplotlib.pyplot	as	plt
import	matplotlib.cm	as	cm
from	matplotlib	import	animation
import	re

figure=plt.figure()
ser	=	serial.Serial('COM5',	9600)	#	Establish	the	connection	on	a	specific	port
img=[]

def	update():
	ser.write('RDY;')
	#	Convert	the	decimal	number	to	ASCII	then	send	it	to	the	Arduino
	buffer_string	=	''
	print	"read data"
	while	True:
		buffer_string	=	buffer_string	+	ser.read(ser.inWaiting())
		if	'RDY;'	in	buffer_string:
			buffer_string	=	''
		elif ';' in buffer_string:
			data=buffer_string
			break
	print	data

	#	Read	the	newest	output	from	the	Arduino
	
	matchnum=re.compile("[0-9]")
	matchdata=re.compile(".*;")
	try:
		str=matchnum.search(data[0:5]).group()
	except:
		str=""
		
	if	not	str:
		print	"empty"
		return	[0,	0]
	if	str:
		data=matchdata.search(data).group()	
		data=[int(x)	for	x	in	data.split(',')[:-1]	if	x]
		print	data
		return	data

def	animate(frame):
	plt.cla()
	data=update()
	img.append(data)	
	plt.imshow(img,	cmap=cm.Greys_r)

#while	True:
#		print	'go'
#		ser.write('GO;')
#		sleep(.1)
#		data=	ser.readline()
#		print	'done'
#		print	data
#		if	data=='GO':
#			break;
	
anim	=	animation.FuncAnimation(figure,	animate,
								frames=50,	interval=50)
plt.show()