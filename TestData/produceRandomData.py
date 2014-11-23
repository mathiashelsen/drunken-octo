#!/usr/bin/python
import sys
import numpy as np

N = int(sys.argv[1])
for i in range(N):
    todayClosing = np.random.normal()
    dailyGain = np.random.normal()
    weeklyGain = np.random.normal()
    A1 = np.random.normal()
    A2 = np.random.normal()
    A3 = np.random.normal()
    A4 = np.random.normal()
    #A1 = (todayClosing - ma50.iloc[i,0])/(todayClosing + ma50.iloc[i,0])
    #A2 = (todayClosing - ma30.iloc[i,0])/(todayClosing + ma30.iloc[i,0])
    #A3 = (todayClosing - upperBollinger.iloc[i,0])/(todayClosing + upperBollinger.iloc[i,0])
    #A4 = (todayClosing - lowerBollinger.iloc[i,0])/(todayClosing + lowerBollinger.iloc[i,0])
    print A1, "\t", A2, "\t", A3, "\t", A4, "\t", dailyGain, "\t", weeklyGain, "\t", todayClosing
