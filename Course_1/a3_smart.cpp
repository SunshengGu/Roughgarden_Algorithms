#include <iostream>
#include <fstream>

using namespace std;

int SIZE = 10000;
unsigned long N_COMP = 0;
unsigned long REAL_COMP = 0;

void swap(int& first, int& second) {
	int temp = first;
	first = second;
	second = temp;
}

// returns index of the median (not the median itself)
int find_median(int*a, int& l, int& m, int& r) {
	// Compare each three number to find middle  
    // number. Enter only if a > b 
    if (a[l] > a[m])  
    { 
        if (a[m] > a[r]) {
        	REAL_COMP += 2;
            return m; 
        }
        else if (a[l] > a[r]) {
        	REAL_COMP += 3;
            return r; 
        }
        else {
        	REAL_COMP += 3;
            return l; 
        }
    } 
    else 
    { 
        // Decided a is not greater than b. 
        if (a[l] > a[r]) {
        	REAL_COMP += 2;
            return l; 
        }
        else if (a[m] > a[r]) {
        	REAL_COMP += 3;
            return r; 
        }
        else {
        	REAL_COMP += 3;
            return m; 
        }
    }
}

int partition(int* a, int& l, int& r) {
	int p_index;
	if (l-r == 1) {
		// only two elements
		p_index = l;
	} else {
		int middle = (l+r)/2;
		p_index = find_median(a, l, middle, r);
	}
	swap(a[p_index],a[l]); // move the median of the 3 to the l position for convenience
	int p = a[l];
	int i = l+1;
	for (int j=i; j<=r; j++) {
		if (a[j] < p) {
			swap(a[j],a[i]);
			i++;
		}
	}
	swap(a[l],a[i-1]);
	return i-1; // return the new pivot position
}

void quick_sort(int* a, int& l, int& r) {
	if (l==r) {
		return; // length is one, nothing to sort
	}
	N_COMP += r-l; // r-l is length-1
	// printf("left: %d right: %d \n",l,r);
	int pivot = partition(a, l, r);
	int pivot_l = pivot-1;
	int pivot_r = pivot+1;
	if (pivot_l<l) {
		pivot_l = l; // preventing the right end smaller than right end
		// printf("pivot is %d \n",pivot);
	}
	if (pivot_r>r) {
		pivot_r = r;
	}
	quick_sort(a, l, pivot_l);
	quick_sort(a, pivot_r, r);
}

int main() {
	int a[10010]; // leave some extra spaces
	// read in the data
	std::fstream data_file("a3_num.txt", std::ios_base::in);
	for (int i=0; i<SIZE; i++) {
		data_file >> a[i];
	}
	data_file.close();
	int end = SIZE -1;
	int start = 0;
	quick_sort(a, start, end);
	// to confirm that numbers are actually sorted
	for (int i=0; i<20; i++) {
		cout << a[i] << endl;
	}
	REAL_COMP += N_COMP; // takes account of comparisons involved in getting the median
	printf("number of comparisons partition: %lu \n", N_COMP);
	printf("number of all comparisons: %lu \n", REAL_COMP);
	return 0;
}