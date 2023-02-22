#include <stdio.h>
#include <cs50.h>

void merge(int nums[], int lo, int mid, int hi){
    int left = lo;
    int right = mid + 1;
    int temp[(hi - lo + 1)];
    int i = 0;
    while(left <= mid && right <= hi){
        if(nums[left] <= nums[right]){
            temp[i] = nums[left];
            left++;
        } else {
            temp[i] = nums[right];
            right++;
        }
        i++;
    }
    if (left == mid + 1){
        while (right <= hi){
            temp[i] = nums[right];
            right++;
            i++;
        }
    } else{
        while (left <= mid){
            temp[i] = nums[left];
            left++;
            i++;
        }
    }
    for (int j = 0; j < hi - lo; j++){
        nums[(j + lo)] = temp[j];
    }
}

void mergeSort(int nums[], int lo, int hi){
    if (lo == hi){
        return;
    }
    int mid = (lo + hi) / 2;
    mergeSort(nums, lo, mid);
    mergeSort(nums, mid + 1, hi);
    merge(nums, lo, mid, hi);
}

void printArray(int array[], int n){
    for (int i = 0; i < n; i++){
       printf("%d ", array[i]);
    }
    printf("\n");
}

int main(void){
    int nums[] = {2,4,6,3,1,8,3,6,10};
    mergeSort(nums, 0, sizeof(nums) - 1);
    printArray(nums, sizeof(nums));
}