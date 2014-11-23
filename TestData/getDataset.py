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

start = datetime.datetime(2012, 11, 17)
stop = datetime.datetime(2014, 11, 21)
energyMarket = web.DataReader("XLE", 'yahoo', start, stop)

stock1 = web.DataReader("XOM", 'yahoo', start, stop)
stock2 = web.DataReader("CVX", 'yahoo', start, stop)
stock3 = web.DataReader("SLB", 'yahoo', start, stop)
stock4 = web.DataReader("COP", 'yahoo', start, stop)
stock5 = web.DataReader("OXY", 'yahoo', start, stop)

lb = 1

EM30 = [energyMarket.iloc[i,3]/energyMarket.iloc[i-lb,3] - 1.0 for i in range(lb, len(energyMarket))]

S1_30 = [stock1.iloc[i,3]/stock1.iloc[i-lb,3] - 1.0 for i in range(lb, len(stock1))]
S2_30 = [stock2.iloc[i,3]/stock2.iloc[i-lb,3] - 1.0 for i in range(lb, len(stock1))]
S3_30 = [stock3.iloc[i,3]/stock3.iloc[i-lb,3] - 1.0 for i in range(lb, len(stock1))]
S4_30 = [stock4.iloc[i,3]/stock4.iloc[i-lb,3] - 1.0 for i in range(lb, len(stock1))]
S5_30 = [stock5.iloc[i,3]/stock5.iloc[i-lb,3] - 1.0 for i in range(lb, len(stock1))]

A1 = [(S1_30[i] - EM30[i]) for i in range(len(EM30)-lb)]
A2 = [(S2_30[i] - EM30[i]) for i in range(len(EM30)-lb)]
A3 = [(S3_30[i] - EM30[i]) for i in range(len(EM30)-lb)]
A4 = [(S4_30[i] - EM30[i]) for i in range(len(EM30)-lb)]
A5 = [(S5_30[i] - EM30[i]) for i in range(len(EM30)-lb)]

ret30 = [energyMarket.iloc[i+lb,3]/energyMarket.iloc[i,3] - 1.0 for i in range(lb, len(energyMarket)-lb)]

for i in range(len(ret30)-lb):
    print A1[i], "\t", \
	A2[i], "\t", \
	A3[i], "\t", \
	A4[i], "\t", \
	A5[i], "\t", \
	ret30[i]
