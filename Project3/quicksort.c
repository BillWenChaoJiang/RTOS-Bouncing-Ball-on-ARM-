#include <LPC17xx.h>
#include <RTL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "quicksort.h"
#include "array_tools.h"

// You decide what the threshold will be
#define USE_INSERTION_SORT 5

// Local prototypes



//structs
typedef struct {
	array_t array;
	size_t start;
	size_t end;
} array_interval_t;

typedef struct{
	array_interval_t interval;
	unsigned char priority;
} qsort_task_parameters_t;

array_type median_of_three(array_interval_t interval){
    array_type a = interval.array.array[interval.start];
    array_type b = interval.array.array[(interval.start + interval.end)/2];
    array_type c = interval.array.array[interval.end];

    if (a<b) {
       // partial order = a,b
        if (b<c) {
            // 2 comparisons: order is a,b,c
            return b;
        }
        else { // order is a,c,b or c,a,b
            if (a<c) {
                // order is a,c,b -- 3 comparisons
                return c;
            }
            else {
                // order is c,a,b -- 3 comparisons
                return a;
            }
        }
    } else {
        // partial order = b,a
        if (c<b) {
            // 2 comparisons: order is c,b,a
            return b;
        }
        else {  // order is b,c,a or b,a,c
            if (c>a) {
                // order is b,a,c -- 3 comparisons
                return a;
            }
            else {
                // order is b,c,a -- 3 comparisons
                return c;
            }
        }
    }
}

void insertion_sort( array_interval_t interval ) {
    size_t j = 0;
    array_type temp= NULL;

    for (int i = interval.start; i <= interval.end; i++){
		j = i;
		while (j > interval.start && arr[j] < arr[j-1]){
            temp = arr[j];
            arr[j] = arr[j-1];
            arr[j-1] = temp;
            j--;
        }
    }
}

void quickSort(array_type arr[], size_t left, size_t right) {
      size_t i = left, j = right;
      array_type tmp;
      array_type pivot = arr[(left + right) / 2];

      /* partition */
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };

      /* recursion */
      if (left < j)
            quickSort(arr, left, j);
      if (i < right)
            quickSort(arr, i, right);
}

__task void quick_sort_task( void* void_ptr){
    qsort_task_parameters_t *task_param = (qsort_task_parameters_t*)void_ptr;
    quickSort(task_param.interval.array.array, task_param.interval.start, task_param.interval.end);
}

void quicksort( array_t array ) {
	array_interval_t interval;
	qsort_task_parameters_t task_param;

	// Based on MTE 241 course notes--you can change this if you want
	//  - in the course notes, this sorts from a to c - 1
	interval.array =  array;
	interval.start     =  0;
	interval.end     =  array.length - 1;

	task_param.interval = interval;

	// If you are using priorities, you can change this
	task_param.priority = 10;

	//start the quick_sort threading
	os_tsk_create_ex( quick_sort_task, task_param.priority, &task_param );
}