/* A simplified learned index implementation:
 *
 * Skeleton code written by Jianzhong Qi, April 2023
 * Edited by: [Add your name and student ID here for automatic recognition]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STAGE_NUM_ONE 1						  /* stage numbers */ 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "Stage %d\n==========\n" /* stage header format string */

#define DATASET_SIZE 100					  /* number of input integers */
#define DATA_OUTPUT_SIZE 10					  /* output size for stage 1 */

#define BS_NOT_FOUND (-1)					  /* used by binary search */
#define BS_FOUND 0

typedef int data_t; 				  		  /* data type */

/****************************************************************/

/* function prototypes */
void print_stage_header(int stage_num);
int cmp(data_t *x1, data_t *x2);
void swap_data(data_t *x1, data_t *x2);
void partition(data_t dataset[], int n, data_t *pivot,
		int *first_eq, int *first_gt);
void quick_sort(data_t dataset[], int n);
int binary_search(data_t dataset[], int lo, int hi, 
	data_t *key, int *locn);

void stage_one();
void stage_two();
void stage_three();
void stage_four();

/* add your own function prototypes here */

/****************************************************************/

/* main function controls all the action */
int
main(int argc, char *argv[]) {
	/* to hold all input data */
	data_t dataset[DATASET_SIZE];
	int max_err;

	/* to hold the mapping functions */


	/* stage 1: read and sort the input */
	stage_one(); 
	
	/* stage 2: compute the first mapping function */
	stage_two();
	
	/* stage 3: compute more mapping functions */ 
	stage_three();
	
	/* stage 4: perform exact-match queries */
	stage_four();
	
	/* all done; take some rest */
	return 0;
}

/****************************************************************/

/* add your code below; you can also modify the function return type 
   and parameter list 
*/

/* stage 1: read and sort the input */
void 
stage_one() {
	/* add code for stage 1 */
	/* print stage header */
	print_stage_header(STAGE_NUM_ONE);

	
	printf("\n");
}

/* stage 2: compute the first mapping function */
void 
stage_two() {
	/* add code for stage 2 */
	/* print stage header */
	print_stage_header(STAGE_NUM_TWO);

	printf("\n");
}

/* stage 3: compute more mapping functions */ 
void 
stage_three() {
	/* add code for stage 3 */
	/* print stage header */
	print_stage_header(STAGE_NUM_THREE);

	printf("\n");
}

/* stage 4: perform exact-match queries */
void stage_four() {
	/* add code for stage 4 */
	/* print stage header */
	print_stage_header(STAGE_NUM_FOUR);
	
	printf("\n");
}

/****************************************************************/
/* functions provided, adapt them as appropriate */

/* print stage header given stage number */
void 
print_stage_header(int stage_num) {
	printf(STAGE_HEADER, stage_num);
}

/* data swap function used by quick sort, adpated from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/sortscaffold.c
*/
void
swap_data(data_t *x1, data_t *x2) {
	data_t t;
	t = *x1;
	*x1 = *x2;
	*x2 = t;
}

/* partition function used by quick sort, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/quicksort.c 
*/
void
partition(data_t dataset[], int n, data_t *pivot,
		int *first_eq, int *first_gt) {
	int next=0, fe=0, fg=n, outcome;
	
	while (next<fg) {
		if ((outcome = cmp(dataset+next, pivot)) < 0) {
			swap_data(dataset+fe, dataset+next);
			fe += 1;
			next += 1;
		} else if (outcome > 0) {
			fg -= 1;
			swap_data(dataset+next, dataset+fg);
		} else {
			next += 1;
		}
	}
	
	*first_eq = fe;
	*first_gt = fg;
	return;
}

/* quick sort function, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/quicksort.c 
*/
void
quick_sort(data_t dataset[], int n) {
	data_t pivot;
	int first_eq, first_gt;
	if (n<=1) {
		return;
	}
	/* array section is non-trivial */
	pivot = dataset[n/2]; // take the middle element as the pivot
	partition(dataset, n, &pivot, &first_eq, &first_gt);
	quick_sort(dataset, first_eq);
	quick_sort(dataset + first_gt, n - first_gt);
}

/* comparison function used by binary search and quick sort, from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c 
*/
int
cmp(data_t *x1, data_t *x2) {
	return (*x1-*x2);
}

/* binary search between dataset[lo] and dataset[hi-1], adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/binarysearch.c 
*/
int
binary_search(data_t dataset[], int lo, int hi, 
		data_t *key, int *locn) {
	int mid, outcome;
	/* if key is in dataset, it is between dataset[lo] and dataset[hi-1] */
	if (lo>=hi) {
		return BS_NOT_FOUND;
	}
	mid = (lo+hi)/2;
	
	if ((outcome = cmp(key, dataset+mid)) < 0) {
		return binary_search(dataset, lo, mid, key, locn);
	} else if (outcome > 0) {
		return binary_search(dataset, mid+1, hi, key, locn);
	} else {
		*locn = mid;
		return BS_FOUND;
	}
}
