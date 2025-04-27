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

// ���q�ثe�O����ϥζq�]���GKB�^
size_t getCurrentMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return memInfo.WorkingSetSize / 1024;
}

size_t BeforeMemorysize = 0;//�I�s�禡�e���e��
size_t AfterMemorysize = 0;//�I�s�禡�Ыت�arr�e��
size_t Memorysize = 0;//�ƧǮɪ��e��

//���o�ۮt���O����ϥζq
void getMemorysize(int n) {//��e�B��e��-(�I�s�禡�Ыت�arr�e��-�I�s�禡�e���e��)=�ƧǮɪ��e��
    Memorysize = getCurrentMemoryUsage() - (AfterMemorysize - BeforeMemorysize);
}

// ���J�Ƨ�
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
    getMemorysize(arr.size());
    return arr;
}

// �ֳt�Ƨ� (Median-of-Three) �� partition ���
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

// �ֳt�Ƨ� (Median-of-Three)
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
    getMemorysize(arr.size());
    return arr;
}

// �X�ֱƧǡ]Iterative�^
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
    getMemorysize(arr.size());
    return arr;
}

// ��Ƨ�
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
    getMemorysize(arr.size());
    return arr;
}

// ��X�Ƨ�
vector<int> CompositeSort(vector<int> arr) {
    int n = arr.size();
    if (n <= 500) {
        return quickSortMedianOfThree(arr);
    }
    else {
        return mergeSortIterative(arr);
    }
}

//--- ���a���p��Ʋ��;� ---
//���J�ƧǡG�ϦV�ƦC
vector<int> generateWorstCaseInsertionSort(int n) {
    vector<int> arr(n);
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
//�T����ƪk�ֳt�ƧǡG�f���k
vector<int> generateWorstCaseQuickSortMedianOfThree(int n) {
    vector<int> arr(n);
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

//��ƧǩM�X�ֱƧǡG�H���ƦC
vector<int> randomgenerateWorstCase(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++)
    {
        arr[i] = i;
    }
    for (int i = n - 1; i >= 2; i--)//�����i=n�M j = rand() % i+1�|���w�W�Xn�}�C���d��,�ҥH�ק令i = n - 1�Mrand() % i
    {
        int j = rand() % i;
        swap(arr[j], arr[i]);
    }
    return arr;
}

//���ձƧǺt��k
void testSort(vector<int>(*sortFunc)(vector<int>), const string& name, vector<int>(*dataGen)(int), const string& complexityType, const string& spaceComplexity) {
    cout << name << " Worst Case Timing (ms):" << endl;
    vector<int> nList = { 500,1000,2000,3000,4000,5000 };
    size_t S_op = 0.0;
    double T_op = -1.0;

    for (int idx = 0; idx < nList.size(); idx++) {
        int n = nList[idx];
        int repetitions = 10;
        double totalTime = 0.0;

        for (int i = 0; i < repetitions; i++) {
            if (check) { // Heap Sort �MIterative Merge Sort�S�O�B�z
                double MaxTime = 0.0;
                for (int j = 0; j < 10; j++) {
                    vector<int> data = dataGen(n);
                    BeforeMemorysize = getCurrentMemoryUsage();
                    auto start = high_resolution_clock::now();
                    data = sortFunc(data);
                    auto end = high_resolution_clock::now();
                    //AfterMemorysize = getCurrentMemoryUsage();
                    auto duration = duration_cast<microseconds>(end - start);
                    if (MaxTime <= duration.count()) {
                        MaxTime = duration.count();
                    }
                }
                totalTime += MaxTime;
            }
            else { // ��L�Ƨ�
                vector<int> data = dataGen(n);
                BeforeMemorysize = getCurrentMemoryUsage();
                auto start = high_resolution_clock::now();
                data = sortFunc(data);
                auto end = high_resolution_clock::now();
                //AfterMemorysize = getCurrentMemoryUsage();
                auto duration = duration_cast<microseconds>(end - start);
                totalTime += duration.count();
            }
        }
        //Memorysize = AfterMemorysize - BeforeMemorysize;
        //BeforeMemorysize = AfterMemorysize;
        double avgTimeMs = totalTime / repetitions;

        // �z�L�Ĥ@����ơA�p�� S_op
        if (idx == 0) {
            if (spaceComplexity == "O(1)") S_op = Memorysize;
            else if (spaceComplexity == "O(log n)") S_op = Memorysize / log2(n);
            else if (spaceComplexity == "O(n)") S_op = Memorysize / n;
        }

        // �w���Ŷ�������
        double predictedSpace = 0;
        if (S_op) {
            if (spaceComplexity == "O(1)") predictedSpace = S_op;
            else if (spaceComplexity == "O(log n)") predictedSpace = S_op * log2(n);
            else if (spaceComplexity == "O(n)") predictedSpace = S_op * n;
        }
        // �z�L�Ĥ@����ơA�p�� T_op
        if (idx == 0) {
            if (complexityType == "n^2") T_op = avgTimeMs / (n * n);
            else if (complexityType == "nlogn") T_op = avgTimeMs / (n * log2(n));
        }
        // �w���ɶ�������
        size_t  predictedTime = 0.0;
        if (T_op > 0) {
            if (complexityType == "n^2") predictedTime = T_op * n * n;
            else if (complexityType == "nlogn") predictedTime = T_op * n * log2(n);
        }
        //��X�ɶ�������
        cout << "n = " << n;
        if (n <= 500) cout << "\t";
        cout << "\tTime: " << setprecision(7) << avgTimeMs << " ms";
        cout << "\tPredicted: " << setprecision(7) << predictedTime << " ms";
        //��X�Ŷ�������
        cout << "\tSpace " << spaceComplexity;
        cout << " | Predicted: " << predictedSpace << " KB";
        cout << " | Actual: " << Memorysize << " KB";

        cout << endl;
    }
    cout << endl;
}

int main() {
    srand(time(nullptr));
    testSort(insertionSort, "Insertion Sort", generateWorstCaseInsertionSort, "n^2", "O(1)");
    testSort(quickSortMedianOfThree, "Quick Sort (Median-of-Three)", generateWorstCaseQuickSortMedianOfThree, "n^2", "O(log n)");//�z�i�H�Nn^2�אּnlogn�|�o�{�w�����p��ڼW���o��,�w����ڼW���t������n^1.7���k
    testSort(mergeSortIterative, "Iterative Merge Sort", randomgenerateWorstCase, "nlogn", "O(n)");
    testSort(heapSort, "Heap Sort", randomgenerateWorstCase, "nlogn", "O(1)");
    testSort(CompositeSort, "Composite Sort", randomgenerateWorstCase, "nlogn", "O(n)");//�]���u��n=500�ϥ�Quick Sort (Median-of-Three)���᳣�OIterative Merge Sort,�ҥH��J"nlogn", "O(n)"
    system("pause");
    return 0;
}
