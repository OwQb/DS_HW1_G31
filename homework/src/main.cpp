#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <Psapi.h>
#include <math.h>
using namespace std;
using namespace chrono;
bool check = 0;

// 測量目前記憶體使用量（單位：KB）
size_t getCurrentMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return memInfo.WorkingSetSize/1024;
}

size_t BeforeMemorysize = 0;//呼叫函式前的占用
size_t AfterMemorysize = 0;//呼叫函式創建的arr占用
size_t Memorysize = 0;//排序時的占用

//取得相差的記憶體使用量
void getMemorysize() {//當前運行占用-(呼叫函式創建的arr占用-呼叫函式前的占用)=排序時的占用
    Memorysize = getCurrentMemoryUsage()-(AfterMemorysize - BeforeMemorysize);
}

// 插入排序
vector<int> insertionSort(vector<int> arr) {
    AfterMemorysize = getCurrentMemoryUsage();
    int n = arr.size(); 
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
    getMemorysize();
    return arr;
}

// 快速排序 (Median-of-Three) 的 partition 函數
int partitionMedianOfThree(vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    int pivotIdx = (arr[low] < arr[mid]) ?
        ((arr[mid] < arr[high]) ? mid : (arr[low] < arr[high] ? high : low)) :
        ((arr[low] < arr[high]) ? low : (arr[mid] < arr[high] ? high : mid));
    swap(arr[pivotIdx], arr[high]);
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// 快速排序 (Median-of-Three)
void quickSortMedianOfThreeHelper(vector<int>& arr, int low, int high) {
    while (low < high) {
        int pi = partitionMedianOfThree(arr, low, high);
        if (pi - low < high - pi) {
            quickSortMedianOfThreeHelper(arr, low, pi - 1);
            low = pi + 1;
        }
        else {
            quickSortMedianOfThreeHelper(arr, pi + 1, high);
            high = pi - 1;
        }
    }
}
vector<int> quickSortMedianOfThree(vector<int> arr) {
    AfterMemorysize = getCurrentMemoryUsage();
    quickSortMedianOfThreeHelper(arr, 0, arr.size() - 1);
    getMemorysize();
    return arr;
}

// 合併排序（Iterative）
void merge(vector<int>& arr, int left, int mid, int right, vector<int>& temp) {
    int i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i < mid) temp[k++] = arr[i++];
    while (j < right) temp[k++] = arr[j++];
    for (i = left; i < right; ++i) arr[i] = temp[i];
}

vector<int> mergeSortIterative(vector<int> arr) {
    AfterMemorysize = getCurrentMemoryUsage();
    int n = arr.size();
    vector<int> temp(n);
    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - size; left += 2 * size) {
            int mid = left + size;
            int right = min(left + 2 * size, n);
            merge(arr, left, mid, right, temp);
        }
    }
    getMemorysize();
    return arr;
}

// 堆排序
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

vector<int> heapSort(vector<int> arr) {
    AfterMemorysize = getCurrentMemoryUsage();
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    getMemorysize();
    return arr;
}

// 綜合排序
vector<int> CompositeSort(vector<int> arr) {
    int n = arr.size();
    if (n <= 500) {
        return quickSortMedianOfThree(arr);
    }
    else  {
        return mergeSortIterative(arr);
    }
}

//--- 最壞情況資料產生器 ---
//插入排序：反向排列
vector<int> generateWorstCaseInsertionSort(int n) {
    vector<int> arr(n); check = 0;
    for (int i = 0; i < n; i++)
        arr[i] = n - i;
    return arr;
}

void buildWorstCase(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    swap(arr[mid], arr[right]);
    buildWorstCase(arr, left, right - 1);
}
//三中位數法快速排序：逆推法
vector<int> generateWorstCaseQuickSortMedianOfThree(int n) {
    vector<int> arr(n); check = 0;
    for (int i = 0; i < n; ++i) {
        arr[i] = i;
    }

    int left = 0;
    int right = n - 1;
    while (left < right) {
        int mid = (left + right) / 2;
        swap(arr[mid], arr[right]);
        --right;
    }
    return arr;
}

//堆排序和合併排序：隨機排列
vector<int> randomgenerateWorstCase(int n) {
    vector<int> arr(n); check = 1;
    for (int i = 0; i < n; i++)
    {
        arr[i] = i;
    }
    for (int i = n - 1; i >= 2; i--)//原先的i=n和 j = rand() % i+1會指定超出n陣列的範圍,所以修改成i = n - 1和rand() % i
    {
        int j = rand() % i;
        swap(arr[j], arr[i]);
    }
    return arr;
}

//測試排序演算法
void testSort(vector<int>(*sortFunc)(vector<int>), const string& name, vector<int>(*dataGen)(int), const string& complexityType, const string& spaceComplexity) {
    cout << name << " Worst Case Timing (ms):" << endl;
    vector<int> nList = { 500,1000,2000,3000,4000,5000 };
    size_t S_op = 0.0;
    double T_op = -1.0;

    for (int idx = 0; idx < nList.size(); idx++) {
        int n = nList[idx];
        int repetitions = 1000;
        double totalTime = 0.0;

        for (int i = 0; i < repetitions; i++) {
            vector<int> data = dataGen(n);
            if (check) { // Heap Sort 和Iterative Merge Sort特別處理
                double MaxTime = 0.0;
                for (int j = 0; j < 10; j++) {
                    BeforeMemorysize = getCurrentMemoryUsage();
                    auto start = high_resolution_clock::now();
                    data = sortFunc(data); 
                    auto end = high_resolution_clock::now();
                    auto duration = duration_cast<microseconds>(end - start);
                    if (MaxTime <= duration.count()) {
                        MaxTime = duration.count();
                    }
                }
                totalTime += MaxTime;
            }
            else { // 其他排序
                BeforeMemorysize = getCurrentMemoryUsage();
                auto start = high_resolution_clock::now();
                data = sortFunc(data);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                totalTime += duration.count();
            }
        }

        double avgTimeMs = totalTime / repetitions;
        
        // 透過第一筆資料，計算 S_op
        if (idx == 0) {
            if (spaceComplexity == "O(1)") S_op = Memorysize;
            else if (spaceComplexity == "O(log n)") S_op = Memorysize / log2(n);
            else if (spaceComplexity == "O(n)") S_op = Memorysize / n;
        }

        // 預測空間複雜度
        double predictedSpace = 0;
        if (S_op) {
            if (spaceComplexity == "O(1)") predictedSpace = S_op;
            else if (spaceComplexity == "O(log n)") predictedSpace = S_op * log2(n);
            else if (spaceComplexity == "O(n)") predictedSpace = S_op * n;
        }
        // 透過第一筆資料，計算 T_op
        if (idx == 0) {
            if (complexityType == "O(n^2)") T_op = avgTimeMs / (n * n);
            else if (complexityType == "O(nlogn)") T_op = avgTimeMs / (n * log2(n));
        }
        // 預測時間複雜度
        size_t  predictedTime = 0.0;
        if (T_op > 0) {
            if (complexityType == "O(n^2)") predictedTime = T_op * n * n;
            else if (complexityType == "O(nlogn)") predictedTime = T_op * n * log2(n);
        }
        //輸出時間複雜度
        cout << "n = " << n;
        if (n <= 500) cout << "\t";
        cout << "\tTime " << complexityType << " : ";
        cout<< " Actual: " << setprecision(6) << avgTimeMs << " ms";
        cout << "\tPredicted: " << setprecision(6) << predictedTime << " ms"; 
        cout << " Actual: " << Memorysize << " KB";
        cout << " \tPredicted: " << predictedSpace <<" KB";
       
        cout << endl;
    }
    cout << endl;
}

int main() {
    srand(time(nullptr));
    testSort(insertionSort, "Insertion Sort", generateWorstCaseInsertionSort, "O(n^2)", "O(1)");
    testSort(quickSortMedianOfThree, "Quick Sort (Median-of-Three)", generateWorstCaseQuickSortMedianOfThree, "O(n^2)", "O(log n)");//您可以將n^2改為nlogn會發現預測不如實際增長得快,預估實際增長速度應為n^1.7左右
    testSort(mergeSortIterative, "Iterative Merge Sort", randomgenerateWorstCase, "O(nlogn)", "O(n)");
    testSort(heapSort, "Heap Sort", randomgenerateWorstCase, "O(nlogn)", "O(1)");
    testSort(CompositeSort, "Composite Sort", randomgenerateWorstCase, "O(nlogn)", "O(n)");//因為只有n=500使用Quick Sort (Median-of-Three)之後都是Iterative Merge Sort,所以輸入"nlogn", "O(n)"
    system("pause");
    return 0;
}
