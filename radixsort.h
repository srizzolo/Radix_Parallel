#ifndef _RADIXSORT_H
#define _RADIXSORT_H

#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <omp.h>

double radSort(int arr[], int digits, int n);
double parRadSort(int arr[], int digits, int n);

#endif