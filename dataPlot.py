import math
import numpy
import scipy
import matplotlib
import matplotlib.pyplot as plt
import re
#import transformations

def main():
	f = open('csc232_lab/build/SET3data.txt','r')

	dataset = [datapoint] * 20000
	orientations = [quat] * 20000
	counter = 0
	overall_counter = 0 
	baseline = 0
	x = 0
	y = 0
	z = 0

	timeset = [0.0] * 5000
	xset = [0.0] * 5000
	yset = [0.0] * 5000
	zset = [0.0] * 5000

	oxset = [0.0] * 5000
	oyset = [0.0] * 5000
	ozset = [0.0] * 5000
	owset = [0.0] * 5000

	for line in f:
		if counter == 0:
			if overall_counter ==0:
				baseline = line.strip()
			
			dataset[overall_counter].timestamp = line.strip()
			timeset[overall_counter] = float(line.strip()) - float(baseline)
			
		if counter == 2:
			split =  line.split()
			dataset[overall_counter].x = split[1]
			xset[overall_counter] = split[1]

		if counter == 3:
			split =  line.split()
			dataset[overall_counter].y = split[1]
			yset[overall_counter] = split[1]

		if counter == 4:
			split =  line.split()
			dataset[overall_counter].z = split[1]
			zset[overall_counter] = split[1]

		if counter == 6:
			split =  line.split()
			orientations[overall_counter].x = split[1]
			oxset[overall_counter] = split[1]

		if counter == 7:
			split =  line.split()
			orientations[overall_counter].y = split[1]
			oyset[overall_counter] = split[1]

		if counter == 8:
			split =  line.split()
			orientations[overall_counter].z = split[1]
			ozset[overall_counter] = split[1]

		if counter == 9:
			split =  line.split()
			orientations[overall_counter].w = split[1]
			owset[overall_counter] = split[1]

		counter=counter+1

		if counter == 10:
			counter = 0
			#print dataset[overall_counter].x
			overall_counter = overall_counter + 1
			#print "resetting"


	#Awkward stuff for matplotlib 
	#(extracting x values because I don't know how to extract them better)
	yawset = [0.0] * 5000
	yawcount = 0
	#print owset
	for point in oxset:
		#euler = transformations.euler_from_quaternion((quaternion.x,quaternion.y,quaternion.z,quaternion.w))
		euler = quat2euler(oxset[yawcount],oyset[yawcount],ozset[yawcount],owset[yawcount])

		yawset[yawcount] = euler[2]
		#print euler
		#print [quaternion.x,quaternion.y,quaternion.z,quaternion.w]

		yawcount = yawcount +1 
	#count = 0
	#for point in dataset:#

#		xset[count] = point.x
		#print point.x
#		yset[count] = point.y
#		zset[count] = point.z
#		count = count+1

	

	print yset
	print len(xset)
	print len(timeset)
	plt.plot(timeset,yset,'ro')
	#plt.plot(yawset,'ro')
	plt.ylabel('Y Position')
	plt.xlabel('Timestamps')
	#plt.axis([0,4.5,0,1])
	#plt.plot(yset)
	#plt.plot(zset)
	plt.show()


def quat2euler(x,y,z,w):
	euler = [0] * 3
	#print x
	x = float(x)
	y = float(y)
	z = float(z)
	#print w
	w = float(w)



	roll = math.atan2((2*(x*y + z+w)),(1-(2*(y**2 + z**2))))
	pitch = math.asin(2*(x*z - w*y))
	yaw =  math.atan2((2*(x*w + y*z)),(1-(2*(x**2 + z**2)))) # what we care about

	euler[0] = roll
	euler[1] = pitch
	euler[2] = yaw

	return euler



class datapoint:
    def __init__(self,x,y,z,pitch,timestamp):
        self.x=x
        self.y=y
        self.z=z
        self.pitch=pitch
        self.timestamp=timestamp




class quat:
    def __init__(self,x,y,z,w):
        self.x=x
        self.y=y
        self.z=z
        self.w=w

main()















