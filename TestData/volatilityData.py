#!/usr/bin/python
import pandas.io.data as web
import pandas.stats.moments as st
import datetime
import pandas as pd 
import numpy as np
import sys

def rolling_window(a, window):
    shape = a.shape[:-1] + (a.shape[-1] - window + 1, window)
    strides = a.strides + (a.strides[-1],)
    return np.lib.stride_tricks.as_strided(a, shape=shape, strides=strides)

start = datetime.datetime(2004, 11, 21)
stop = datetime.datetime(2014, 11, 21)
broadMarket = web.DataReader(sys.argv[1], 'yahoo', start, stop)
broadMarket = broadMarket['Close']
returns = broadMarket.pct_change()

vol5 = st.rolling_std(returns, window=5)/5.0
vol30 = st.rolling_std(returns, window=30)/30.0
vol60 = st.rolling_std(returns, window=60)/60.0
vol120 = st.rolling_std(returns, window=120)/120.0
vol240 = st.rolling_std(returns, window=240)/240.0

for i in range(240, len(vol240)-30):
    print vol5.iloc[i], "\t", \
	vol30.iloc[i], "\t", \
	vol60.iloc[i], "\t", \
	vol120.iloc[i], "\t", \
	vol240.iloc[i], "\t", \
	vol30.iloc[i+30]
#returns = np.array([broadMarket.iloc[i,3]/broadMarket.iloc[i-1,3] - 1.0 for i in range(1, len(broadMarket))])
