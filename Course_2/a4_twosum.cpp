#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <regex>
#include <set>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <unordered_map>

using namespace std;

int SIZE = 1000000;

void find2sum(long long &sum, long long (&num_arr)[1000000], int &answer) {
	// initialize the hash map
	unordered_map<long long,long long> hash;
	hash[sum-num_arr[0]] = num_arr[0];
	// unordered_map<int,int>::iterator it = hash.begin();
	long long key;
	// construct the map, difference as key; find two sums as we go
	for (int i=1; i<SIZE; i++) {
		if ((hash.find(num_arr[i]) != hash.end()) && (hash[num_arr[i]] != num_arr[i])) {
			answer++;
			break;
		}
		key = sum-num_arr[i];
		hash[key] = num_arr[i];
	}
	// cout << "one done" << endl;
	// cout << "created a hashmap" << endl;
	// // confirm if distinctive two_sum exist
	
	// int diff;
	// while (it!=hash.end()) {
	// 	// non-distinctive, ignore
	// 	if (it->first == it->second) {
	// 		it++;
	// 		continue;
	// 	}
	// 	// distinctive two_sum found
	// 	if (hash.find(it->second) != hash.end()) {
	// 		answer++;
	// 		// cout << "found one!" << endl;
	// 		break;
	// 	}
	// 	it++;
	// }
}

int main() {
	// read in the data
	std::ifstream data_file("twosum.txt");
	long long arr[1000000];
	int i=0;
	while (data_file >> arr[i]) {
		i++;
	}
	cout << "finished reading in the data" << endl;
	cout << arr[999999] << endl;
	data_file.close();
	// count solutions
	int ans = 0;
	int progress = -5;
	for (long long j=-10000; j<10001; j++) {
		find2sum(j, arr, ans);
		if (j%1000 == 0) {
			progress+=5;
			cout << progress << " percent done" << endl;
		}
	}
	cout << ans << endl;
}