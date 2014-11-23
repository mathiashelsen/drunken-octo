/*
The MIT License (MIT)

Copyright (c) 2014 Mathias Helsen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef _FINANCE_HPP
#define _FINANCE_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <assert.h>
#include <vector>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_fit.h>

#include "sorted_ll.hpp"
#include "drunken_octo.hpp"

using namespace boost;
using namespace std;

#define NDIMS 5
#define PDIMS 1

typedef struct
{
    double x[NDIMS];
}X;

typedef struct
{
    double y[PDIMS];
}Y;

typedef drunken_octo<Y, X> datapoint;

int compare( X *a, X *b, int rank);
double metric( X *a, X *b );
double projectedDistance(X *a, X *b, int rank);

void readDatafile(ifstream *file, vector<vector<double> *> *list);
void createDataset( vector<vector<double> *> *list, datapoint **root, vector<datapoint*> **verification, double *scale );
double runTest(datapoint *root, vector<datapoint *> *verification);

#endif
