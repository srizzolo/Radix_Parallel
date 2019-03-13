//  PARALLEL RADIX SORT 
// 	GROUP: Skylar Rizzolo & Noah Thornton 
//	DATE MODIFIED: 3/9/19
//	TODO: Implement parallelization, get runs for varying n, p and range


/*	
	TO COMPILE & RUN: 
	g++ -fopenmp -o radixSort radixSort.cpp
	./radixSort n range

	where n and range are ints 
*/ 

#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <omp.h>
#include "mpi.h"

using namespace std;

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

void printVector(vector<int> v, int n)
{
	cout << endl;
	for(int j=1; j<=n; j++)
	{
		cout << v[j-1] << "\t";
		if(j%10==0)
			cout << endl;
	}
	return;
}

void printArray(int *a, int n)
{
	for(int j=1; j<=n; j++)
	{
		cout << a[j-1] << "\t";
		if(j%10==0)
			cout << endl;
	}
	fflush(stdout);
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

int main(int argc, char * argv[])
{
	int rank, numProcs;
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &numProcs );

	int N, range, root = 0;
	double seed, seqTime, avgSeqTime;
	if(rank == root)
	{
		sscanf (argv[1], "%d", &N);
		sscanf (argv[2], "%d", &range);
		seed = omp_get_wtime();
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&range, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&seed, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Request request;
	srand(seed);

	int array[N];
	arrayGen(array, N, range);
	

	//int n = 15;
	int digits = (int) log10(range);

	if(rank == root) // Root process setup
	{
		//printf("Before radix sort:\n");
		//printArray(array, N);

	    seqTime = radSort(array, digits, N);

		//printf("\nAfter radix sort:\n");
		//printArray(array, N);

		
		printf("\nSequential Algorithm:\n\tRun #%d took %fs\n", rank, seqTime);
		
	}
	else // Waits for root to run with MPI_Recv
	{
		seqTime = radSort(array, digits, N);

		MPI_Irecv(&root, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, MPI_STATUS_IGNORE);
		printf("\tRun #%d took %fs\n", rank, seqTime);
		fflush(stdout);
		
	}

	//Send to next process to initialize print statements
	MPI_Isend(&root, 1, MPI_INT, (rank+1) % numProcs , 0, MPI_COMM_WORLD, &request);
	MPI_Wait(&request, MPI_STATUS_IGNORE);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Reduce(&seqTime, &avgSeqTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(rank == root)
 	{
 		MPI_Irecv(&root, 1, MPI_INT, numProcs - 1, 0, MPI_COMM_WORLD, &request);
 		MPI_Wait(&request, MPI_STATUS_IGNORE);
 		avgSeqTime /= N;
 		cout << "Average Sequential time (" << numProcs << " processes) = " << avgSeqTime <<"s\n";
 		printf("Finishing ring print with process %d, and ending program.\n", rank);
 	}

    MPI_Finalize();
	return 0;
}
