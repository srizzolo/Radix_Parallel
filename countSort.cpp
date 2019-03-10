#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

void arrayGen(int *a, int n)
{
    int *temp = new int[n];
    for(int i=0; i<n; i++)
    {
        temp[i] = rand() % 1000001;
    }
    copy(temp, temp+n, a);
    delete[] temp;
}

void printArray(int *a, int n)
{
    for (int i=0; i<n; i++)
    {
        cout << a[i] << " ";
    }

    cout << endl << endl;
}

double CountSort (int *a, int n)
{
    int count;
    int *temp = new int[n];

    double t = omp_get_wtime();
    for (int i = 0; i < n; ++i)
    {
        count = 0;
        for (int j = 0; j < n; ++j)
        {
            if (a[j] < a[i])
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        }
        temp[count] = a[i];
    }
    double elapsed = omp_get_wtime() - t;

    copy(temp, temp+n, a);
    delete[] temp;

    return elapsed;
}

int cSort (int *a, int n, int i)
{
    int count = 0;
    #pragma omp parallel for num_threads(16) schedule(auto) reduction(+:count)
    for (int j = 0; j < n; ++j)
    {
        if (a[j] < a[i])
            count += 1;
        else if (a[j] == a[i] && j < i)
            count += 1;
    }
    return count;
}

double pCountSort (int *a, int n)
{
    int *temp = new int[n];
    
    double t = omp_get_wtime();
    #pragma omp parallel for num_threads(4) schedule(auto)
    for (int i=0; i<n; i++)
    {
        temp[cSort(a,n,i)] = a[i];
    }
    double elapsed = omp_get_wtime() - t;
    
    copy(temp, temp+n, a);
    delete[] temp;
   
    return elapsed;
}



int main(int argc, char * argv[])
{
    int n;
    srand((int) omp_get_wtime());
    sscanf (argv[1], "%d", &n);
    int arr[n];

    arrayGen(arr, n); 
    //cout << "Before sorting:\n";
    //printArray(arr,n);

    double sortTime = CountSort(arr, n);
    cout << "Took " << sortTime << "s to sort sequentially\n";
    //printArray(arr,n);
   

    arrayGen(arr, n); 
    sortTime = pCountSort(arr, n);
    cout << "Took " << sortTime << "s to sort in parallel\n";
    //printArray(arr,n);

    return 0;
}
