#!/usr/bin/python
import csv
import random
import numpy
import regularization as reg
import FeatureScaling as featscale


def GradientDescent(trainSetPath, testSetPath, resultPath) :
	dataset = []
	reference = []
	theta = []


	#read the training samples
	print "reading data..."
	with open(trainSetPath) as trainSet :
		dataset = numpy.loadtxt(trainSet, delimiter = ',', skiprows = 1)
		reference = numpy.matrix(dataset[0:len(dataset), len(dataset[0])-1]).getT()
		dataset = numpy.matrix(dataset[0:len(dataset), 1:len(dataset[0])-1])

	print "data has been read"


	#feature scaling, delete the feature if it has none effect
	detail = featscale.featureScaling(dataset)
	dataset = numpy.delete(dataset, detail.irrelevant, 1)


	#record the amount of samples and features
	m = dataset.shape[0]
	n = dataset.shape[1]


	#regularization
	print "begin regularizating..."
	#lamda = 0
	lamda = reg.regularize(dataset, reference, detail, 3000, 1000)
	print "regularization has finished, lamda="+str(lamda)


	#initialize the parameters
	param = []
	for i in range(n+1) :
		param.append(random.uniform(-9, 9))
	theta = numpy.matrix(param).T


	#gradient descent
	lastResult = 100
	counter = 0 
	a = 0.1
	cost = 0

	while abs(lastResult-cost) > 0.0001 :
		lastResult = cost

		#update the learning rate
		
		#this value will be used for two times
		#so just compute it in advance
		distance = dataset*theta[1:n+1]+theta[0]-reference
		
		#update values of theta
		theta[0] = theta[0] * (1 - a*lamda/m) - a/m*distance.sum()
		theta[1:n+1] = theta[1:n+1] * (1 - a*lamda/m) - a/m*dataset.T*distance

		#calculate the cost function
		cost = ((numpy.array(distance)**2).sum() + lamda*(numpy.array(theta)**2).sum()) / (2*m)

		#for observation
		if counter % 10 == 0 : print counter,cost

		if counter > 5000: break
		counter += 1


	#predict the new value
	with open(resultPath, 'w') as submission :
		submissionWriter = csv.writer(submission)
		submissionWriter.writerow(['Id', 'reference'])

		with open(testSetPath) as testSet :
			dataset = numpy.loadtxt(testSet, delimiter = ',', skiprows = 1)
			dataset = numpy.matrix(dataset[0:len(dataset), 1:len(dataset[0])])

			for i in range(dataset.shape[1]) :
				if detail.ran[i] != 0 : dataset[:, i] = (dataset[:, i] - detail.means[i]) / detail.ran[i]
			dataset = numpy.delete(dataset, detail.irrelevant, 1)

			reference = numpy.array(dataset*theta[1:n+1]+theta[0])

			for i in range(len(reference)) :
				submissionWriter.writerow([str(i), str(reference[i][0])])
