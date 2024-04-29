#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

void bubbleSort(vector<int> &arr)
{
    int n = arr.size();
    bool swapped = true;

    while (swapped)
    {
        swapped = false;
#pragma omp parallel for // parallelize
        for (int i = 0; i < n - 1; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
    }
}

void merge(vector<int> &arr, int l, int m, int r) // merge function for mergesort
// arrays are [l....m] and [m+1....r]
{
    vector<int> temp; // temp vector of ints
    int left = l;
    int right = m + 1;

    while (left <= m && right <= r) // while we haven't reached the end of either arrays
    {
        // add the smaller element to temp
        if (arr[left] <= arr[right])
        {
            temp.push_back(arr[left]);
            left++;
        }
        else
        {
            temp.push_back(arr[right]);
            right++;
        }
    }

    while (left <= m) // right array ended so copy left as it is
    {
        temp.push_back(arr[left]);
        left++;
    }

    while (right <= r) // left array ended so copy right as it is
    {
        temp.push_back(arr[right]);
        right++;
    }

    for (int i = l; i <= r; i++)
    {
        arr[i] = temp[i - l]; // copy temp to origianl array
    }
}

void mergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
#pragma omp parallel sections // each section to be run in a different thread
        {
#pragma omp section // thread 1
            mergeSort(arr, l, m);
#pragma omp section // thread 2
            mergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

int main()
{
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    clock_t bubbleStart = clock();
    bubbleSort(arr);
    clock_t bubbleEnd = clock();
    cout << "Sorted array using Bubble Sort: ";
    for (int num : arr)
        cout << num << " ";
    cout << endl;

    clock_t mergeStart = clock();
    mergeSort(arr, 0, n - 1);
    clock_t mergeEnd = clock();
    cout << "Sorted array using Merge Sort: ";
    for (int num : arr)
        cout << num << " ";
    cout << endl;

    double bubbleDuration = double(bubbleEnd - bubbleStart) / CLOCKS_PER_SEC;
    double mergeDuration = double(mergeEnd - mergeStart) / CLOCKS_PER_SEC;

    cout << "Bubble sort time in seconds: " << bubbleDuration << endl;
    cout << "Merge sort time in seconds: " << mergeDuration << endl;

    return 0;
}
