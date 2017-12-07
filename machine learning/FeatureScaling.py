# !usr/bin/python

import numpy


class Detail :
	'descript feature of the dataset, including means and range'
	means = []
	ran = []
	irrelevant = []


def featureScaling(dataset) :
	detail = Detail()

	for i in range(dataset.shape[1]) :
		detail.means.append(dataset[:, i].sum() / dataset.shape[0])
		detail.ran.append(numpy.max(dataset[:, i]) - numpy.min(dataset[:, i]))

		print i,detail.means[i],detail.ran[i]
		
		if detail.ran[i] != 0 : dataset[:, i] = (dataset[:, i] - detail.means[i]) / detail.ran[i]
		else : detail.irrelevant.append(i)

	return detail