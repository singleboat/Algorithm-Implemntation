import random
import math
import time
import csv
import os
import numpy as np
import DataProcess as dp

n = 132

#read the path of file
trainSetPath = raw_input('input the train set file path, "data/train_data.txt" as default: ')
if trainSetPath == '' : trainSetPath = '../../data/logistic regression/train_data.txt'

testSetPath = raw_input('input the test set file path, "data/test_data.txt" as default: ')
if testSetPath == '' : testSetPath = '../../data/logistic regression/test_data.txt'

resultPath = raw_input('input the result file path, "submission.txt" by default: ')
if resultPath == '' : resultPath = '../../data/logistic regression/submission.txt'


#get the average of data
average = [0]*n
average_count = [0]*n
dmin = [500]*n
dmax = [0]*n
ran = [0]*n

if not os.path.exists('average_and_range.csv') :
	for feat, ref in dp.dataGenerator(trainSetPath) :
		for i in range(len(feat)) :
			if not feat[i] == -1 : 
				dmax[i] = feat[i] if feat[i] > dmax[i] else dmax[i]
				dmin[i] = feat[i] if feat[i] < dmin[i] else dmin[i]
				average[i] += feat[i]
				average_count[i] += 1

	ran = dman-dmin

	for i in range(n) :
		if not average_count[i] == 0 : average[i] /= average_count[i]

	#save it for other compution
	with open('average_and_range.csv', 'w') as feature :
		writer = csv.writer(feature)
		for i in range(n) : writer.writerow([average[i], dmax[i]-dmin[i]])
else :
	with open('average_and_range.csv', 'r') as feature :
		i = 0
		for line in feature :
			sub = line.split(',')
			average[i] = float(sub[0])
			ran[i] = float(sub[1])
			i += 1
print "average & range of features have get"


#initialize the thetas
param = [0]*(n+1)
if not os.path.exists('theta.csv'):
	for i in range(n) : param[i] = random.uniform(-9, 9)
else:
	with open('theta.csv', 'r') as thetaFile :
		i = 0
		for line in thetaFile :
			param[i] = float(line)
			i += 1
theta = np.array(param)


#Gradient Decent
count = 0
lastResult = 0
a = 0.1
cost = 500

print 'start regression'
while abs(cost-lastResult) > 0.0001:
	start = time.clock()
	lastResult = cost
	cost = 0
	m = 0
	t1 = 0
	t2 = np.array([0]*n, dtype=np.float)

	for feat, ref in dp.dataGenerator(trainSetPath, True) :
		m += 1
		for i in range(n) : 
			if feat[i] == -1 : feat[i] = 0
			else : feat[i] = (feat[i]-average[i])/ran[i]
		
		h = 1 / (1 + math.exp(-1 * ((theta[1: n+1] * feat).sum() + theta[0])))
		cost += ref*math.log(h) + (1-ref)*math.log(1-h)
		distance = h - ref
		t1 += a*distance
		t2 += a*distance*feat

	theta[0] -= t1
	theta[1: n+1] -= t2
	
	cost /= m
	print count, m, cost
	count += 1

	print (time.clock()-start)/m
	if count > 5000 : break

with open('theta.csv', 'w') as thetaFile :
		writer = csv.writer(thetaFile)
		for i in range(n+1) : writer.writerow([theta[i]])


#predict new value
with open(resultPath, 'w') as submission :
	submissionWriter = csv.writer(submission)
	submissionWriter.writerow(['id', 'label'])

	lastIndex = -1
	for feat, index in dp.dataGenerator(testSetPath) :
		#ignore the repeated sample in test set
		if index <= lastIndex : continue
		lastIndex = index

		for i in range(n) : 
			if feat[i] == -1 : feat[i] = 0
			else : feat[i] = (feat[i]-average[i])/ran[i]
		
		#print feat.shape[0]

		h = 1 / (1 + math.exp(-1 * ((theta[1: n+1] * feat).sum() + theta[0])))
		
		submissionWriter.writerow([str(index), str(h)])