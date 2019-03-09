//  PARALLEL RADIX SORT 
// 	GROUP: Skylar Rizzolo & Noah Thornton 
//	DATE MODIFIED: 3/9/19


#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <omp.h>

using namespace std;
const int r = 33333;

void arrayGen(int *a, int n, int range)
{
    int *temp = new int[n];
    for(int i=0; i<n; i++)
    {
        temp[i] = rand() % range;
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
	return;
}

double radSort(int arr[], int digits, int n)
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
		//radix sort into specific bins
	    for(int k=0; k<n; k++)
	    {
	        int curr = (int) (store[k]/pow(10,j));
	        curr %= 10;
	        bins[curr].push(store[k]);
	    }

	    //reset store
	    store.clear();
	   
	    //Store 1 pass of radix sorting
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

int main(int argc, char * argv[])
{
	int N, range;
	sscanf (argv[1], "%d", &N);
	sscanf (argv[2], "%d", &range);

	//Use queues
	//Implement parallelization
	int array[N];
	arrayGen(array, N, range);

	//int n = 15;
	int digits = (int) log10(range);

	cout << "Before radix sort:" << endl;
	for (int j=0; j<N; j++)
    {
    	cout << array[j] << endl;
    }
    cout << endl;

	double sorted = radSort(array, digits, N);
	cout << "Time to radix sort: " << sorted << "s\nAfter radix sort:" << endl;
	for (int j=0; j<N; j++)
    {
    	cout << array[j] << endl;
    }
    cout << endl;


	return 0;
}
