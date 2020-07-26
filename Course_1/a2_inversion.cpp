#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int SIZE = 100000;

/* 
template to get a sub compoment of a vector
source: https://www.tutorialspoint.com/getting-a-subvector-from-a-vector-in-cplusplus
*/
template<typename T>
vector<T> s(vector<T> const &v, int m, int n) {
   auto first = v.begin() + m;
   auto last = v.begin() + n;
   vector<T> sub_vector(first, last);
   return sub_vector;
}

// counts the number of split inversions, also produces the sorted vector
unsigned long long count_split_inv(const vector<int>& b, const vector<int>& c, int len, vector<int>& d) {
	int i=0, j=0;
	unsigned long long count=0;
	int b_len = b.size();
	int c_len = c.size();
	// printf("in count_split_inv.  b_len: %d c_len: %d \n",b_len,c_len);
	for (int k=0; k<len; k++) {
		if (i<b_len && j<c_len) {
			// when we have not finished any of the halves
			if (b[i] <= c[j]) {
				d.push_back(b[i]);
				i++;
			} else {
				d.push_back(c[j]);
				j++;
				count += b_len-i;
			}
		} else if (i>=b_len) {
			// when the first half is finished
			d.push_back(c[j]);
			j++;
		} else {
			// when the second half is finished
			d.push_back(b[i]);
			i++;
		}
	}
	return count;
}

// recursive function to merge sort and count number of inversions
// a is unsorted while d is sorted
unsigned long long sort_n_count(vector<int>& a, int len, vector<int>& d) {
	if (len == 1) {
		d.push_back(a[0]);
		return 0; // single elment array, no inversions possible
	}
	// printf("in sort_n_count. len: %d \n",len);
	int half_len = len/2;
	int remain_len = len - half_len;
	vector<int> first_half = s(a,0,half_len);
	vector<int> second_half = s(a,half_len,len);
	vector<int> b; // sorted first half
	vector<int> c; // sorted second half
	unsigned long long x = sort_n_count(first_half, half_len, b);
	unsigned long long y = sort_n_count(second_half, remain_len, c);
	unsigned long long z = count_split_inv(b, c, len, d);
	return x+y+z;
}

int main() {
	// {11,12,13,10,9,8,6,7,1,2,4,3}
	vector<int> unsorted; // assign some extra spaces
	vector<int> sorted; 
	std::fstream data_file("a2_num.txt", std::ios_base::in);
	// read in the data
	int temp;
	for (int i=0; i<SIZE; i++) {
		data_file >> temp;
		unsorted.push_back(temp);
	}
	data_file.close();
	unsigned long long result = sort_n_count(unsorted, SIZE, sorted);
	// printf("first %d sorted values: \n",SIZE);
	// for (int j=0; j<SIZE; j++) {
	// 	cout << sorted[j] << endl;
	// }
	printf("number of inverstions in first %d elements: %llu \n",SIZE,result);
	return 0;
}