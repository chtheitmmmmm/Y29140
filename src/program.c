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
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

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

void binary_search_range(data_t dataset[], int lo, int hi,
		data_t *key, int *locn);

void stage_one();
void stage_two();
void stage_three();
void stage_four();

/* add your own function prototypes here */

/****************************************************************/
data_t dataset[DATASET_SIZE];
int max_err;

struct function {
    int a;
    int b;
    int max_element;
};

int functions_num = 0;
struct function all_functions[DATASET_SIZE];

/* main function controls all the action */
int
main(int argc, char *argv[]) {
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
    for (int i = 0; i < DATASET_SIZE; ++i) {
        scanf("%d", dataset + i);
    }
    quick_sort(dataset, DATASET_SIZE);
    printf("First %d numbers:", DATA_OUTPUT_SIZE);
    for (int i = 0; i < DATA_OUTPUT_SIZE; ++i) {
        printf(" %d", dataset[i]);
    }
    printf("\n");
	printf("\n");
}

/* stage 2: compute the first mapping function */
int
f(int a, int b, int key) {
    return b ? (int)ceil((key + a) * 1.0 / b) : a;
}

static inline void
construct_function(int i, struct function * func) {
    if (i + 1 >= DATASET_SIZE) {
        func->a = DATASET_SIZE - 1;
        func->b = 0;
        func->max_element = dataset[i];
    } else {
        if (dataset[i] != dataset[i + 1]) {
            func->a = -(dataset[i] - (dataset[i + 1] - dataset[i]) * i);
            func->b = dataset[i + 1] - dataset[i];
        } else {
            func->a = i;
            func->b = 0;
        }
    }
}

void
stage_two() {
	/* add code for stage 2 */
	/* print stage header */
	print_stage_header(STAGE_NUM_TWO);
    const int a = -dataset[0];
    const int b = dataset[1] - dataset[0];
    int max_error = 0;
    int max_error_idx = 0;
    for (int i = 1; i < DATASET_SIZE; ++i) {
        int prediction = f(a, b, dataset[i]);
        int the_error = abs(i - prediction);
        if (the_error > max_error) {
            max_error = the_error;
            max_error_idx = i;
        }
    }
    printf("Maximum prediction error: %d\n", max_error);
    printf("For key: %d\n", dataset[max_error_idx]);
    printf("At position: %d\n", max_error_idx);
	printf("\n");
}

/* stage 3: compute more mapping functions */ 
void 
stage_three() {
	/* add code for stage 3 */
	/* print stage header */
	print_stage_header(STAGE_NUM_THREE);
    scanf("%d", &max_err);
    construct_function(0, all_functions + functions_num);
    functions_num++;
    struct function * current_function = all_functions + functions_num - 1;
    for (int i = 2; i < DATASET_SIZE; ++i) {
        if (i + 1 < DATASET_SIZE) {
            int prediction = f(current_function->a, current_function->b, dataset[i]);
            int the_error = abs(i - prediction);
            if (the_error > max_err) {
                current_function->max_element = dataset[i - 1];
                construct_function(i, all_functions + functions_num);
                current_function = all_functions + functions_num;
                functions_num++;
                i += 1;
            }
        } else {
            current_function->max_element = dataset[i];
        }
    }
    printf("\n");
    for (int i = 0; i < functions_num; ++i) {
        printf("Function %2d: a = %4d, b = %4d, max element = %3d\n",
               i, all_functions[i].a, all_functions[i].b, all_functions[i].max_element);
    }
	printf("\n");
}

/* stage 4: perform exact-match queries */
void stage_four() {
	/* add code for stage 4 */
	/* print stage header   */
	print_stage_header(STAGE_NUM_FOUR);
    int key;
    while (scanf("%d", &key) != EOF) {
        printf("Searching for %d:\n", key);
        printf("Step 1: ");
        if (key < dataset[0] || key > dataset[DATASET_SIZE - 1]) {
            printf("not found!\n");
            continue;
        } else {
            printf("search key in data domain.\n");
        }
        printf("Step 2: ");
        int max_elements[DATASET_SIZE];
        for (int i = 0; i < functions_num; ++i) {
            max_elements[i] = all_functions[i].max_element;
        }
        int seg_idx;
        binary_search_range(max_elements, 0, functions_num, &key, &seg_idx);
        printf("\n");
        printf("Step 3: ");
        struct function the_func = all_functions[seg_idx];
        int the_prediction = f(the_func.a, the_func.b, key);
        int the_loc;
        if (binary_search(dataset,
                          MAX(0, the_prediction - max_err),
                          MIN(DATASET_SIZE, the_prediction + max_err + 1), &key, &the_loc) == BS_FOUND) {
            printf("@ dataset[%d]!", the_loc);
        } else {
            printf("not found!");
        }
        printf("\n");
    }
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
	printf("%d ", dataset[mid]);    // output to stdout
	if ((outcome = cmp(key, dataset+mid)) < 0) {
		return binary_search(dataset, lo, mid, key, locn);
	} else if (outcome > 0) {
		return binary_search(dataset, mid+1, hi, key, locn);
	} else {
		*locn = mid;
		return BS_FOUND;
	}
}

void binary_search_range(data_t dataset[], int lo, int hi,
		data_t *key, int *locn) {
	int mid, outcome;
	/* if key is in dataset, it is between dataset[lo] and dataset[hi-1] */
	if (lo>=hi) {
		*locn = lo;
        return;
	}
	mid = (lo+hi)/2;
	printf("%d ", dataset[mid]);    // output to stdout
	if ((outcome = cmp(key, dataset+mid)) < 0) {
		binary_search_range(dataset, lo, mid, key, locn);
	} else if (outcome > 0) {
		binary_search_range(dataset, mid+1, hi, key, locn);
	} else {
		*locn = mid;
	}
}
