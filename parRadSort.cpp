//  PARALLEL RADIX SORT 
// 	GROUP: Skylar Rizzolo & Noah Thornton 
//	DATE MODIFIED: 3/9/19
//	TODO: Implement parallelization, get runs for varying n, p and range


/*	
	TO COMPILE & RUN: 
	g++ -fopenmp -o parRadSort parRadSort.cpp
	./parRadSort n range

	where n and range are ints 
*/ 

#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <omp.h>
#include <algorithm>
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

int main(int argc, char * argv[])
{
	int rank, numProcs;
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &numProcs );

	int N, range, root = 0;
	double seed, start, elapsed;
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

	vector<int> store;
	int arr[N];
	arrayGen(arr, N, range);

	for (int j=0; j<N; j++)
    {
    	store.push_back(arr[j]);
    }
    
	//int n = 15;
	int digits = (int) log10(range);
	vector<int> bin;
	MPI_Barrier(MPI_COMM_WORLD);
	start = omp_get_wtime();
	if(rank != root) // Root process setup
	{
		int count = 0;
		for(int k=0; k<N; k++)
		{
	   		int curr = (int) (store[k]/pow(10,digits-1));
	    	if(curr == rank-1 % 10)
	    	{
	        	bin.push_back(store[k]);
	        	count++;
	    	}
		}

		//Test diff sort here
	    sort(bin.begin(), bin.end());

		//wait here
		MPI_Irecv(&root, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, MPI_STATUS_IGNORE);

		/* Test print
		printf("Process %d has %d ints in bin (%ds)\n", rank, count, rank-1);
		printVector(bin, count); */

		MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(&bin[0], count, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}	

	//Send to next process to initialize print statements
	MPI_Isend(&root, 1, MPI_INT, (rank+1) % numProcs , 0, MPI_COMM_WORLD, &request);
	MPI_Wait(&request, MPI_STATUS_IGNORE);
	if(rank != root)
	{
		elapsed = omp_get_wtime() - start;
	}
	
	if(rank == root)
 	{
 		store.clear();
	    int tsize;
		vector<int> tempv;
	    //Store 1 pass of radix sorting: also must be sequential
	    for(int k=1; k<numProcs; k++)
		{
			MPI_Recv(&tsize, 1, MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			tempv.reserve(tsize);
			MPI_Recv(&tempv[0], tsize, MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			//Pop out each value, queue by queue from 0-9 into store
	    	for(int i=0; i<tsize; i++)
	    	{
	    		store.push_back(tempv[i]);
	    	}
	    	tempv.clear();
	    }
	    elapsed = omp_get_wtime() - start;

	    MPI_Irecv(&root, 1, MPI_INT, numProcs - 1, 0, MPI_COMM_WORLD, &request);
 		MPI_Wait(&request, MPI_STATUS_IGNORE);

 		//cout << "\nAfter radix sorting:";
 		//printVector(store, N);
 		//printf("Finishing ring print with process %d, and ending program.\n", rank);
 	}
 	double maxTime;
 	MPI_Reduce(&elapsed, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
 	if(rank == root)
	{
		printf("Took %f to run in parallel.\n", maxTime);
	}

 	MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
	return 0;
}

