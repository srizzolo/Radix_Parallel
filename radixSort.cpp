//  PARALLEL RADIX SORT 
// 	GROUP: Skylar Rizzolo & Noah Thornton 
//	DATE MODIFIED: 3/9/19


#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <omp.h>

using namespace std;
const int r = 33333;

void arrayGen(int *a, int n)
{
    int *temp = new int[n];
    for(int i=0; i<n; i++)
    {
        temp[i] = rand() % 101;
    }
    copy(temp, temp+n, a);
    delete[] temp;
}

int getMax(int arr[], int n)
{
	int m = arr[0];
	for (int j=1; j<n; j++)
		if (arr[j] > m)
			m = arr[j];

	return m;
}

void printArray(vector<int> v, int n)
{
	cout << endl;
	for(int j=0; j<n; j++)
	{
		cout << v[j] << endl;
	}
	
}

void radSort(int arr[], int digits, int n)
{
	vector<vector<int> > bins(n);
    vector<int> store(0);
    
    //initialize 
    for (int j=0; j<n; j++)
    {
    	store.push_back(arr[j]);
    	bins[j] = vector<int>(0);
    }

	for (int j=0; j<digits; j++)
	{
		//radix sort into specific bins
	    for(int k=0; k<n; k++)
	    {
	        int curr = (int) store[k]/pow(10,j);
	        curr %= 10;
	        vector<int> b = bins[curr];
	        b.push_back(store[k]);
	        bins[curr] = b;
	    }

	    /* DO NOT NEED TO SORT BINS, POP OUT LIKE QUEUE
	    for(int k=0; k<n; k++)
	    {
	    	vector<int> row = bins[k];
	    	
	    	if(row.size() > 1 && row.size() < 16)
	    	{
	    		insSort(row, row.size());
	    		bins[k] = row;
	    	}
	    }
	    */

	    //reset store
	    store.clear();

	    //Store 1 pass of radix sorting
	    for(int k=0; k<n; k++)
	    {
	    	vector<int> row = bins[k];
	    	while(!row.empty())
	    	{
	    		store.push_back(row.back());
	    		row.pop_back();
	    	}		
	    }

	    //reset bins
	    for (int k=0; k<n; k++)
    	{
    		bins[k].clear();
    	}
	}
	
	//Replace with sorted array 
    for (int j=0; j<n; j++)
    {
    	arr[j] = store[j];
    }
}

int main(int argc, char * argv[])
{
	int N;
	sscanf (argv[1], "%d", &N);

	//Use queues
	//Implement parallelization
	int array[N];
	arrayGen(array, N);

	//int n = 15;
	int digits = (int) log10(getMax(array, 10)) + 1;

	cout << "Before radix sort:" << endl;
	for (int j=0; j<N; j++)
    {
    	cout << array[j] << endl;
    }
    cout << endl;

	radSort(array, digits, N);

	cout << "After radix sort:" << endl;
	for (int j=0; j<N; j++)
    {
    	cout << array[j] << endl;
    }
    cout << endl;


	return 0;
}
