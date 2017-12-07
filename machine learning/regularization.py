#!usr/bin/python
import numpy
import random


def regularize(dataset, reference, detail, trainSetSize = 900, testSetSize = 100, decentTime = 5000) :
	m = dataset.shape[0]
	boundary = trainSetSize+testSetSize
	if (boundary > m) : return 0

	n = dataset.shape[1]

	lamda = (0.1, 1, 10, 100, 1000)
	lamdaCost = [0, 0, 0, 0, 0]

	for i in range(len(lamda)) :
		param = []
		for j in range(n+1) :
			param.append(random.uniform(-9, 9))
		theta = numpy.matrix(param).T

		a = 0.1
		cost = 0

		for j in range(decentTime) :
			lastResult = cost

			#update learning rate
			if a > 2000 : a = 0.03

			#the value will be used for serval times, so just calculate it in advance
			distance = dataset[0:trainSetSize]*theta[1:n+1]+theta[0]-reference[0:trainSetSize]

			#update the value of theta
			theta[0] = theta[0] - a/trainSetSize*distance.sum()
			theta[1:n+1] = theta[1:n+1]*(1-a*lamda[i]/trainSetSize) - a/trainSetSize*dataset[0:trainSetSize].T*distance

			cost = ((numpy.array(distance)**2).sum()+lamda[i]*(numpy.array(theta)**2).sum()) / (2*trainSetSize)
			if(j % 100 == 0) : print "lamda: "+str(lamda[i])+" "+str(j)+" "+str(cost)
			if abs(lastResult-cost) < 0.0001 : break

		#calculate the cost for test set
		distance = dataset[trainSetSize:boundary]*theta[1:n+1]+theta[0]-reference[trainSetSize:boundary];
		lamdaCost[i] = (numpy.array(distance)**2/(2*testSetSize)).sum()
		print "for lamda= "+str(lamda[i])+", cost= "+str(lamdaCost[i])

	best = 0
	for i in range(len(lamdaCost)):
		if lamdaCost[i] < lamdaCost[best] : best = i

	return lamda[best]
