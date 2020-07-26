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

int main() {
	int num_jobs, weight, length, score;
	unsigned long long sum_time = 0, comp_time = 0;
	// vector<int> job_info
	multimap<int,int,greater<int>> job_order;
	vector<int> weights, lengths;

	ifstream data_file("jobs.txt");
	data_file >> num_jobs;
	for (int i=0; i<num_jobs; i++) {
		data_file >> weight >> length;
		if (i<20) {
			cout << weight << ' ' << length << endl;
		}
		// job_info.push_back(weight);
		// job_info.push_back(length);
		score = weight-length;
		job_order.insert(pair<int,int>(score,i));
		weights.push_back(weight);
		lengths.push_back(length);
	}

	// debug message 
	auto iter = job_order.begin();
	int ind = 0; 
	while (ind < 10) {
		cout << "task index: " << iter->second << endl;
		cout << "score: " << iter->first << " length: " << lengths[iter->second] << " weight: " << weights[iter->second] << endl;
		iter++;
		ind++;
	}

	auto it = job_order.begin();
	while (it != job_order.end()) {
		int curr_score, next_score, job_id;
		curr_score = it->first;
		job_id = it->second;
		it++;
		if (it != job_order.end()) { // next element exists
			next_score = it->first;
			if (next_score != curr_score) { // no tie, proceed as normal
				comp_time += lengths[job_id];
				sum_time += comp_time * weights[job_id];
				// continue;
			} else { // tie exists, need to break it
				std::pair <std::multimap<int,int,greater<int>>::iterator, std::multimap<int,int,greater<int>>::iterator> ret;
				ret = job_order.equal_range(curr_score); // find all elements with this score
				multimap<int,int,greater<int>> weight_map;
				for (auto itr=ret.first; itr!=ret.second; itr++) {
					weight_map.insert(pair<int,int>(weights[itr->second],itr->second));
					//it = itr;
				}
				//it++;
				for (auto iter = weight_map.begin(); iter != weight_map.end(); iter++) {
					comp_time += lengths[iter->second];
					sum_time += comp_time * weights[iter->second];
				}
				it = ret.second; // update it
			}
		} else { // reached the end, there's no next element
			comp_time += lengths[job_id];
			sum_time += comp_time * weights[job_id];
		}
	}
	cout << "weighted sum of completion time is: " << sum_time << endl;
	return 0;
}