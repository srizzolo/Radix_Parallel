#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;
const int r = 33333;
const int N = 15;

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

void insSort(vector<int> vec, int n)
{
	vector<int> v = vec;
	for(int j=1; j<v.size(); j++)
    {
        int k = v[j]; //key holding the element to be inserted
        int i = j-1;
        //Inserts A[j] into the previously sorted subarray A[1...j-1]
        while (i>=0 && v[i]>k)
        {
            v[i+1] = v[i];
            i--;
        }
        v[i+1] = k; //the element is inserted into its correct spot
    }
    return;
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

	    //Sort each bin using counting sort
	    for(int k=0; k<n; k++)
	    {
	    	vector<int> row = bins[k];
	    	
	    	if(row.size() > 1 && row.size() < 16)
	    	{
	    		insSort(row, row.size());
	    		bins[k] = row;
	    	}
	    	else if (row.size() == 1)
	    	{
	    	
	    	}
	    }
	    
	    //reset store
	    store.clear();

	    //Store 1 pass of radix sorting
	    for(int k=0; k<n; k++)
	    {
	    	vector<int> row = bins[k];
	    	if(row.size() == 1)
	    		store.push_back(row[0]);
	    	else if(row.size() > 1 && row.size() < n)
	    	{
	    		for(int l=0; l<row.size(); l++)
	    		{
	    			store.push_back(row[l]);
	 	   		}
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

int main()
{
	int array[N] = {1,222,2,3,4,45,36,27,18,111,222,333,444,55,1};
	int sizeArr = sizeof(array) / sizeof(array[0]);
	vector<int> array2(array, array + sizeArr);
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
