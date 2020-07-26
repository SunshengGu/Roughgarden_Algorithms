#include <iostream>
#include <fstream>

using namespace std;

int SIZE = 10000;
unsigned long N_COMP = 0;

void swap(int& first, int& second) {
	int temp = first;
	first = second;
	second = temp;
}

int partition(int* a, int& l, int& r) {
	swap(a[l], a[r]);
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
	// int p = a[r];
	// int i = l;
	// for (int j=i; j<r; j++) {
	// 	if (a[j] < p) {
	// 		swap(a[j],a[i]);
	// 		i++;
	// 	}
	// }
	// swap(a[r],a[i]);
	// return i; // return the new pivot position
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
	printf("number of comparisons: %lu \n", N_COMP);
	return 0;
}