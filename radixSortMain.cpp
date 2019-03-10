#include "radixsort.h"
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
	return;
}

int main(int argc, char * argv[])
{
	int N, range;
	sscanf (argv[1], "%d", &N);
	sscanf (argv[2], "%d", &range);

	
	int array[N];
	arrayGen(array, N, range);

	//int n = 15;
	int digits = (int) log10(range);

	cout << "Before radix sort:" << endl;
	printArray(array, N);
    cout << endl;

	double sorted = radSort(array, digits, N);
	cout << "Time to sequential sort: " << sorted << "s\nAfter radix sort:" << endl;
	printArray(array, N);
    cout << endl;

	return 0;
}
