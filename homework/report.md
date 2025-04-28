# 41243256
# 41243215
## 解題說明
實作排序n個數字的函式，包含以下：  
Insertion Sort  
Quick Sort(Median-of-Three)  
Merge Sort(Iterative)  
Heap Sort  
  
在n=500, 1000, 2000, 3000, 4000, 5000時  
專門為每個排序建立最壞情況資料產生器：   
Insertion Sort：反向排序[n, n-1, n-2, ... , 1]  
Quick Sort(Median-of-Three)：將排序邏輯反轉，逆推出 worst-case  
Merge Sort(Iterative)：隨機產生並保留需排序最久的測試資料  
Heap Sort：隨機產生並保留需排序最久的測試資料  
  
計算每個排序函式的執行時間和記憶體，並繪製圖表  
計算排序法的時間複雜度，並與測試結果進行比對，確認結果是否正確  
最後根據以上四種排序查看圖表在n多少時使用何種排序最快，寫出一個合成排序  
### 解題策略
1. 我們可以先寫出四種排序的程式並透過題目提供的隨機排序產生器進行測試
2. 確認四種排序都能進行正常運作後就能開始寫最壞情況資料產生器
3. 將原先的隨機排序產生器改寫成題目要求的最壞情況資料產生器
4. 確認最壞情況資料產生器輸入給四種排序後大致符合預期輸出
5. 透過Windows API檢測記憶體占用，最後檢查是否大致符合預期輸出
## 效能分析
### 時間複雜度(最壞情況)  
   1. Insertion Sort：O($n^{2}$)   
      完全逆序時，每插入一個新元素，都要向前比對並移動所有已有元素，  
      第1次比較1次，第2次比較2次，…，第n-1次比較n-1次，  
      所以總比較次數是：1+2+3+⋯+(n−1)  
      總時間複雜度變成O($n^{2}$)  
   2. Quick Sort(Median-of-Three)：O($n^{2}$)   
      資料經過特別設計（像幾乎一樣大、排列特別的資料），  
      仍然可能讓每次劃分非常不平均（例如一邊有n-1個元素，另一邊0個元素）  
      這樣遞迴深度變成 n 層，每層處理 O(n) 個元素，  
      總時間複雜度變成O($n^{2}$)  
     
   4. Merge Sort(Iterative)：O(n log n)	  
      分成 log n 層（因為每次切半，直到每個元素單獨一個）  
      每層合併時，要遍歷所有 n 個元素來合併，  
      所以總時間是O(n log n)  
        
   5. Heap Sort：O(n log n)  
      建堆（heapify）整體需要 O(n) 時間  
      每次取最大元素並重建堆，需要 O(log n) 時間  
      要取 n 次元素（每次刪除堆頂），  
      所以總時間是O(n log n)  
### 空間複雜度
   1. Insertion Sort：O(1)  
      插入排序直接在原陣列上進行排序，只需要一點點暫存空間（例如存暫時拿出來的元素）  
      不需要額外開大範圍的記憶體，屬於原地排序（in-place），  
      所以空間複雜度是O(1)  

   2. Quick Sort(Median-of-Three)：O(log n)  
      快速排序本身是原地排序（只靠交換元素），但遞迴呼叫需要額外的堆疊空間，  
      每層遞迴最多是 log n 層，  
      所以空間複雜度是O(log n)

   3. Merge Sort(Iterative)：O(n)
      合併排序需要開一個暫存陣列來合併子陣列，  
      這個暫存陣列通常大小是 O(n)，即跟原始資料一樣大，
      所以空間複雜度是O(n)

   4. Heap Sort：O(1)
      堆積排序是直接在原陣列上調整堆結構，  
      不需要額外開大範圍的記憶體，只需要少數變數暫存交換，  
      所以空間複雜度是O(1)    
## 程式實作
### Insertion Sort
|![Not_Found](/homework/report_image/Insertion_Sort.png)|insertionSort函式<br>從第二個元素開始，依序拿出每個元素，與前面的元素比較，如果比前面小就一直向左移動，直到找到正確位置插入|
|:----------------------------------------------|:-----------------------------------------|
### Quick Sort(Median-of-Three)
|![Not_Found](/homework/report_image/Quick_Sort_p.png)![Not_Found](/homework/report_image/Quick_Sort.png)|1. partitionMedianOfThree函式<br>為了避免最壞情況（例如已排序的資料），這裡不是直接拿第一個或最後一個元素當 pivot，而是取左、中、右三個元素中值作為 pivot，讓 partition 更平衡<br><br>2. quickSortMedianOfThreeHelper函式<br>使用partitionMedianOfThree函式的回傳值作為基準點，把小於 pivot 的放左邊，大於 pivot 的放右邊，然後遞迴排序左右兩邊<br><br>3. quickSortMedianOfThree函式<br>包裝函數，統一排序函式呼叫格式|
|:----------------------------------------------|:-----------------------------------------|
### Merge Sort(Iterative)
|![Not_Found](/homework/report_image/Merge_Sort(Iterative).png)|1. merge函式<br>將arr中的兩個子區段[left, mid-1]和[mid, right-1]（兩段已經是排序好的）合併成一段有序的區間，暫存於temp中，然後複製回原本的arr<br><br>2. mergeSortIterative函式<br>建立一個同樣大小的 temp 陣列作為合併時的暫存空間，然後針對當前size大小，不斷以2*size為單位，抓取left,mid,right三個界線進行合併，最後呼叫merge函式，將 arr[left...mid-1]和arr[mid...right-1]合併|
|:----------------------------------------------|:-----------------------------------------|
### Heap Sort
|![Not_Found](/homework/report_image/Heap_Sort.png)|1. heapify函式<br>將陣列arr中以索引i為根的子樹，調整成「最大堆（Max-Heap）」，首先largest 一開始設成自己i，然後找左右子節點，接著比較arr[left]和arr[right]是否比arr[largest]大，找到三者中最大的值，更新 largest，如果最大的不是自己（largest != i），就交換arr[i]和arr[largest]，交換後遞迴呼叫自己，讓被換下去的子樹繼續滿足堆的性質<br><br>2. heapSort函式<br>從最後一個非葉子節點n/2-1開始，往前逐個節點呼叫heapify函式，把整個陣列轉換成一個最大堆，接著把堆頂（最大值arr[0]）和最後一個元素交換，再對新的堆頂呼叫heapify函式，讓剩下的元素重新形成最大堆，就這樣一輪一輪交換，最大元素逐步排到陣列的尾端|
|:----------------------------------------------|:-----------------------------------------|
### CompositeSort
|![Not_Found](/homework/report_image/CompositeSort.png)|generateWorstCaseInsertionSort函式<br>|
|:----------------------------------------------|:-----------------------------------------|
### Insertion Sort最壞情況資料產生器
|![Not_Found](/homework/report_image/gen_Insertion_Sort.png)|generateWorstCaseInsertionSort函式<br>透過for迴圈產生一個反向排列陣列|
|:----------------------------------------------|:-----------------------------------------|
### Quick Sort(Median-of-Three)最壞情況資料產生器
|![Not_Found](/homework/report_image/gen_Quick_Sort.png)|1. buildWorstCase函式<br>遞迴地製造一個最壞情況的排列，使每次選到的 pivot 都極端不平衡，首先如果left>=right，代表區間只剩0或1個元素，不需要再做直接回傳，接著計算中間位置mid=(left+right)/2，隨後交換arr[mid]和arr[right]，把中間的元素搬到右端，最後遞迴呼叫自己，不斷地縮小範圍（往左邊縮），繼續破壞中間的結構<br><br>2. generateWorstCaseQuickSortMedianOfThree函式<br>首先設定左右邊界left=0、right=n-1，接著當left<right時計算中間位置mid=(left+right)/2，隨後交換arr[mid]和arr[right]，把中間的數字推到右邊，最後right--，縮小考慮範圍（往左縮小），重複這個動作|
|:----------------------------------------------|:-----------------------------------------|
### Merge Sort(Iterative)和Heap Sort最壞情況資料產生器
|![Not_Found](/homework/report_image/gen_random.png)|randomgenerateWorstCase函式<br>透過for迴圈產生一個正向排列陣列,再隨機打亂順序|
|:----------------------------------------------|:-----------------------------------------|
### Memory相關計算函式
|![Not_Found](/homework/report_image/getMemory.png)|1. getCurrentMemoryUsage函式<br>獲得當前正在實際使用、載入到RAM的記憶體量<br><br>2. getMemorysize函式<br>AfterMemorysize會在呼叫該排序函式中取得，BeforeMemorysize會在呼叫該排序函式前取得，getCurrentMemoryUsage放在排序最後結束時，最後計算該排序占用的記憶體空間，計算方式為:當前運行占用-(呼叫函式創建的arr占用-呼叫函式前的占用)=排序時的占用|
|:----------------------------------------------|:-----------------------------------------|
### testSort函式(計算資料和輸出)
|![Not_Found](/homework/report_image/testSort_01.png)|建立所需變數|
|---------------------------------------------|----------------------------------------|
|![Not_Found](/homework/report_image/testSort_02.png)|**這段透過repetitions控制需要重複排序次數，當Merge Sort(Iterative)和Heap Sort運作時check會被設為1，則取10次中運行最長的時間作為totalTime加總的單次時間，其他排序就正常加總計算所需時間**|
|![Not_Found](/homework/report_image/testSort_03.png)|**首先將totalTime/repetitions得到平均時間avgTimeMs，Memorysize則是先前排序最後呼叫getCurrentMemoryUsage取得，隨後透過呼叫該函式的參數決定單次時間或空間的算法，最後乘上該排序的時間或空間複雜度得到推測值**|
|![Not_Found](/homework/report_image/testSort_04.png)|**輸出avgTimeMs實際運行時間和predictedTime推測時間，輸出Memorysize實際運行空間和predictedSpace推測空間**|
### main函式
|![Not_Found](/homework/report_image/main.png)|依照格式:testSort(排序函式,"排序名稱",最壞情況資料產生器函式,"時間複雜度","空間複雜度");分別進行呼叫|
|---------------------------------------------|----------------------------------------|
## 測試與驗證
### 測試輸出結果
![Not_Found](/homework/report_image/output.png)
### 繪製時間複雜度折線圖(非上述輸出繪製,但結果不影響)
|![Not_Found](/homework/report_image/Time_Line_chart_01.png)|![Not_Found](/homework/report_image/Time_Line_chart_02.png)|
|---------------------------------------------|----------------------------------------|
---
▲可以看到實際輸出大致符合預期,就是Quick Sort(Median-of-Three)和預期稍微有點偏差,計算後成長曲線約為O($n^{1.7}$) ,也接近O($n^{2}$)
### 繪製空間複雜度折線圖(非上述輸出繪製,但結果不影響)
|![Not_Found](/homework/report_image/Space_Line_chart_01.png)|![Not_Found](/homework/report_image/Space_Line_chart_02.png)|
|---------------------------------------------|----------------------------------------|
---
▲可以看到實際和預期偏差非常大，我們推測是Windows透過**階梯式記憶體分配**從而導致該現象，因為Windows不可能你需要多少記憶體它就準確給你多少記憶體，
 這樣子太消耗系統效能了，所以它會階梯式調整，舉個例子:假設系統一開始設定2048KB作為一個區間，你邀求100KB，系統會直接給你2048KB，如果你邀求3000KB
 ，系統就會給你4096KB，所以你會看到記憶體實際是提高一段維持一段，並非我們預期的曲線，而且n在500到5000可能對系統來說不夠大，所以數值波動也不明顯
## 申論及開發報告
