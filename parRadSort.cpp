#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <stdio.h>
#include <omp.h>
#include "radixsort.h"

using namespace std;

double parRadSort(int arr[], int digits, int n)
{
	vector<queue<int> > bins(10);
    vector<int> store(0);
    
    //initialize store vector and queues for bins
    for (int j=0; j<n; j++)
    {
    	store.push_back(arr[j]);
    }
    for(int j=0; j<10; j++)
    {
    	bins[j] = queue<int>();
    }

    double start = omp_get_wtime();
    //One pass for each digit
	for (int j=0; j<digits; j++)
	{
		//Sort into specific bins: must be sequential
	    for(int k=0; k<n; k++)
	    {
	        int curr = (int) (store[k]/pow(10,j));
	        curr %= 10;
	        bins[curr].push(store[k]);
	    }

	    //reset store
	    store.clear();
	   
	    //Store 1 pass of radix sorting: also must be sequential
	    for(int k=0; k<10; k++)
		{
			//Pop out each value, queue by queue from 0-9 into store
			queue<int> row = bins[k];
	    	while(!row.empty())
	    	{
	    		store.push_back(row.front());
	    		row.pop();
	    	}
	    }

	    //reset bins
	    for(int k=0; k<10; k++)
		{
			queue<int> empty;
			bins[k] = empty;
		}
	}
	double elapsed = omp_get_wtime() - start;

	//Replace with sorted array 
    for (int j=0; j<n; j++)
    {
    	arr[j] = store[j];
    }
    return elapsed;
}