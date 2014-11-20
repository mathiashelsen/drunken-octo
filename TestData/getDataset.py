#!/usr/bin/python
import pandas.io.data as web
import pandas.stats.moments as st
import datetime
import matplotlib.pyplot as plt
import sys
import pandas as pd 
import numpy.random as rd
import numpy as np

def geometric_brownian_motion(T = 1, N = 1000, mu = 0.1, sigma = 0.01, S0 = 20):        
    dt = float(T)/N
    t = np.linspace(0, T, N)
    W = np.random.standard_normal(size = N) 
    W = np.cumsum(W)*np.sqrt(dt) ### standard brownian motion ###
    X = (mu-0.5*sigma**2)*t + sigma*W 
    S = S0*np.exp(X) ### geometric brownian motion ###
    return S

start = datetime.datetime(2009, 1, 1)
stop = datetime.datetime(2014, 11, 19)
f = web.DataReader(sys.argv[1], 'yahoo', start, stop)
#f = pd.DataFrame(geometric_brownian_motion())
ma30 = st.rolling_mean(f, window=30)
ma50 = st.rolling_mean(f, window=50)
std30 = st.rolling_std(f, window=30)
upperBollinger = ma30 + std30
lowerBollinger = ma30 - std30
predRange = 20
for i in range(50, len(ma50)-predRange):
    todayClosing = f.iloc[i,3]
    dailyGain = f.iloc[i+1,3]/f.iloc[i,3] - 1.0
    weeklyGain = f.iloc[i+predRange,3]/f.iloc[i,3] - 1.0
    A1 = (f.iloc[i,3] - f.iloc[i,0])/(f.iloc[i,3] + f.iloc[i,0])
    A2 = (f.iloc[i,3] - f.iloc[i,1])/(f.iloc[i,3] + f.iloc[i,1])
    A3 = (f.iloc[i,3] - f.iloc[i,2])/(f.iloc[i,3] + f.iloc[i,2])
    A4 = 0.0
    #A1 = (todayClosing - ma50.iloc[i,0])/(todayClosing + ma50.iloc[i,0])
    #A2 = (todayClosing - ma30.iloc[i,0])/(todayClosing + ma30.iloc[i,0])
    #A3 = (todayClosing - upperBollinger.iloc[i,0])/(todayClosing + upperBollinger.iloc[i,0])
    #A4 = (todayClosing - lowerBollinger.iloc[i,0])/(todayClosing + lowerBollinger.iloc[i,0])
    print A1, "\t", A2, "\t", A3, "\t", A4, "\t", dailyGain, "\t", weeklyGain, "\t", todayClosing
