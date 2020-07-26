#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <regex>
#include <set>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <map>

using namespace std;

int NULL_NODE = 1000000; 

bool is_odd(int input) {
	if (input % 2 == 0) {
		return false;
	}
	return true;
}

class Heap{
	int arr[10000];
	int tail;
	bool max_heap;
public:
	Heap(); // initialize tail
	void set_max_heap();
	int get_parent(int& c);
	int get_smaller_child(int& p);
	void insert(int& k);
	int extract_min();
	int extract_max();
	int access_root();
	int get_size();
	void print();
};

Heap::Heap() {
	for (int i=0; i<10000; i++) {
		// use NULL_NODE to indicate that an element is null
		arr[i] = NULL_NODE;
	}
	tail = -1;
	max_heap = false;
}

void Heap::set_max_heap() {
	max_heap = true;
}

int Heap::get_parent(int& c_node) {
	if (is_odd(c_node)) {
		return floor(c_node/2.0);
	}
	return c_node/2-1;
}

int Heap::get_smaller_child(int& p_node) {
	int left_child = p_node*2+1;
	int right_child = p_node*2+2;
	// deal with empty child first
	if (arr[left_child]==NULL_NODE && arr[right_child]==NULL_NODE) {
		return -1; // indicating no child
	}
	if (arr[right_child]==NULL_NODE) {
		return left_child;
	}
	if (arr[left_child]==NULL_NODE) {
		return right_child;
	}
	// pick the smaller child
	if (arr[right_child] < arr[left_child]) {
		return right_child;
	}
	return left_child;
}

void Heap::insert(int& k) {
	tail++;
	if (max_heap) {
		// cout << "max heap ";
		arr[tail] = -1*k;
	} else {
		// cout << "min heap ";
		arr[tail] = k;
	}
	// cout << "inserted value is: " << arr[tail] << endl;
	int curr = tail;
	if (tail != 0) { // not the first element
		// bubble up
		int parent = get_parent(curr);
		// make sure parent id is valid
		while (parent>=0 && arr[parent] > arr[curr]) {
			// cout << "parent getting swapped: " << parent << endl;
			// swap the values
			int temp = arr[curr];
			arr[curr] = arr[parent];
			arr[parent] = temp;
			// update curr and parent
			curr = parent;
			parent = get_parent(curr);
		}
	}
}

int Heap::extract_min() {
	int root = arr[0];
	// move the tail to the root position
	arr[0] = arr[tail];
	arr[tail] = NULL_NODE;
	tail--;
	// if (max_heap) {
	// 	cout << "max heap ";
	// } else {
	// 	cout << "min heap ";
	// }
	// cout << "removed value is: " << root << endl;
	//cout << "new root is: " << arr[0] << endl;
	// bubble down
	int parent = 0;
	int child = get_smaller_child(parent);
	while (child!=-1 && arr[parent]>arr[child]) {
		// swap values
		int temp = arr[child];
		arr[child] = arr[parent];
		arr[parent] = temp;
		// update child and parent
		parent = child;
		child = get_smaller_child(parent);
	}
	return root;
}

int Heap::extract_max() {
	// reverse back to positive number
	int root_val = extract_min();
	return -1*root_val;
}

int Heap::access_root() {
	if (max_heap) {
		return -1*arr[0];
	}
	return arr[0];
}

int Heap::get_size() {
	return tail+1;
}

void Heap::print() {
	if (max_heap) {
		cout << "max_heap elements: ";
	} else {
		cout << "min_heap elements: ";
	}
	for (int i=0; i<=tail; i++) {
		cout << arr[i] << ' ';
	}
	cout << endl;
}

int main() {
	Heap h_max, h_min;
	h_max.set_max_heap();
	std::ifstream data_file("heapdata.txt");
	int num, median, count = 0, sum_of_median = 0;
	while (data_file >> num) {
		// insert num to the correct heap
		if (num <= h_max.access_root()) {
			// cout << num << " is inserted into h_max" << endl;
			h_max.insert(num);
		} else {
			// cout << num << " is inserted into h_min" << endl;
			h_min.insert(num);
		}
		// cout << "after initial insert: " << endl;
		// h_max.print();
		// h_min.print();
		// re-balance the heaps, h_max always equal to or 1 bigger than h_min
		while (h_max.get_size() > h_min.get_size()+1) {
			median = h_max.extract_max();
			// cout << median << " is extracted from h_max" << endl;
			h_min.insert(median);
		}
		while (h_max.get_size() < h_min.get_size()) {
			median = h_min.extract_min();
			// cout << median << " is extracted from h_min" << endl;
			h_max.insert(median);
		}
		// // debug message
		// cout << "after re-balancing: " << endl;
		// h_max.print();
		// h_min.print();
		// accumulate the sum
		sum_of_median += h_max.access_root();
		// cout << "current median is: " << h_max.access_root() << " " << endl << endl;
	}
	// cout << endl;
	data_file.close();
	cout << sum_of_median % 10000 << endl;
	return 0;
}