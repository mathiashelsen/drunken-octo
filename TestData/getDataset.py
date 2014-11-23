#!/usr/bin/python
import pandas.io.data as web
import pandas.stats.moments as st
import datetime
import matplotlib.pyplot as plt
import sys
import pandas as pd 
import numpy.random as rd
import numpy as np
from matplotlib import pyplot as plt
import matplotlib.mlab as mlab

start = datetime.datetime(2007, 8, 1)
stop = datetime.datetime(2014, 11, 19)
#print "Retieving data on the energy market... "
energyMarket = web.DataReader("XLE", 'yahoo', start, stop)
#print "Retieving data on the broad market... "
broadMarket = web.DataReader("SPY", 'yahoo', start, stop)
#print "Retieving data on the oil market... "
oilMarket = web.DataReader("USO", 'yahoo', start, stop)
#print "Retieving data on the gas market... "
gasMarket = web.DataReader("UNG", 'yahoo', start, stop)
#print "Retrieving data about a particular stock in the energy market... "
stock = web.DataReader("HAL", 'yahoo', start, stop)
stock2 = web.DataReader("CVX", 'yahoo', start, stop)

lb = 30

EM30 = [energyMarket.iloc[i,3]/energyMarket.iloc[i-lb,3] - 1.0 for i in range(lb, len(energyMarket))]
BM30 = [broadMarket.iloc[i,3]/broadMarket.iloc[i-lb, 3] - 1.0 for i in range(lb, len(broadMarket))]
OM30 = [oilMarket.iloc[i,3]/oilMarket.iloc[i-lb,3] - 1.0 for i in range(lb, len(oilMarket))]
GM30 = [gasMarket.iloc[i,3]/oilMarket.iloc[i-lb,3] - 1.0 for i in range(lb, len(gasMarket))]
CVX30 = [stock2.iloc[i,3]/stock2.iloc[i-lb,3] - 1.0 for i in range(lb, len(gasMarket))]


HSK30 = [stock.iloc[i,3]/stock.iloc[i-lb,3] - 1.0 for i in range(lb, len(gasMarket))]
SK1 = [stock.iloc[i+1,3]/stock.iloc[i,3] - 1.0 for i in range(lb, len(stock)-1)]
SK30 = [stock.iloc[i+lb,3]/stock.iloc[i,3] - 1.0 for i in range(lb, len(stock)-lb)]

muSk = np.average(SK30)
sigmaSk = np.std(SK30)
#SK30 = rd.normal(loc=muSk, scale=sigmaSk, size=len(SK30)) 

AEM = [(EM30[i] - HSK30[i]) for i in range(len(EM30)-lb)]
ABM = [(BM30[i] - HSK30[i]) for i in range(len(EM30)-lb)]
AOM = [(OM30[i] - HSK30[i]) for i in range(len(EM30)-lb)]
AGM = [(GM30[i] - HSK30[i]) for i in range(len(EM30)-lb)]
ACVX = [(CVX30[i] - HSK30[i]) for i in range(len(EM30)-lb)]
mu = np.average(AEM)
sigma = np.std(AEM)

mu2 = np.average(AOM)
sigma2 = np.std(AOM)

mu3 = np.average(AGM)
sigma3 = np.std(AGM)

mu4 = np.average(ABM)
sigma4 = np.std(ABM)

"""
n, bins, patches = plt.hist(ABM, 50, normed=1)
plt.plot(bins, mlab.normpdf(bins, mu, sigma))
plt.show()
"""
for i in range(len(EM30)-lb):
    print AOM[i], "\t", \
	AGM[i], "\t", \
	ACVX[i], "\t", \
	ABM[i], "\t", \
	AEM[i], "\t", \
	SK30[i], "\t", \
	SK1[i]
