#!/usr/bin/python
import pandas.io.data as web
import pandas.stats.moments as st
import datetime
import matplotlib.pyplot as plt

start = datetime.datetime(2009, 1, 1)
stop = datetime.datetime(2014, 11, 14)
f = web.DataReader("GSZ.PA", 'yahoo', start, stop)
ma30 = st.rolling_mean(f, window=30)
ma50 = st.rolling_mean(f, window=50)
std30 = st.rolling_std(f, window=30)
upperBollinger = ma30 + std30
lowerBollinger = ma30 - std30
dailygain = f.pct_change()
for i in range(50, len(ma50)-5):
    dailygain = f.iloc[i+1,4]/f.iloc[i,4] - 1.0
    weeklygain = f.iloc[i+5,4]/f.iloc[i,4] - 1.0
    print f.iloc[i, 0], '\t', f.iloc[i, 1], '\t', f.iloc[i, 2], '\t', f.iloc[i, 3], '\t', f.iloc[i, 4], \
	ma30.iloc[i, 0], '\t', ma50.iloc[i,0], '\t', upperBollinger.iloc[i,0], '\t', lowerBollinger.iloc[i,0], '\t', dailygain , '\t', weeklygain
