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
	int num_jobs, weight, length;
	unsigned long long sum_time = 0, comp_time = 0;
	double score;
	// vector<int> job_info
	multimap<double,int,greater<int>> job_order;
	vector<int> weights, lengths;

	ifstream data_file("jobs.txt");
	string line;
	getline(data_file, line);
	num_jobs = std::stoi(line);
	
	for (int i=0; i<num_jobs; i++) {
		getline(data_file, line);
		if (i<20) {
			cout << "initial line: " << line << endl;
		}
		// cout << line << endl;
		std::regex num (R"([0-9]+)");
	    std::smatch nums;
	    int temp[2];
	    int j=0;
	    while (std::regex_search (line,nums,num)) {
	    	if (i<20 && j==1) {
				cout << "updated line: " << line << endl;
			}
	    	temp[j] = std::stoi(nums[0].str());
	    	if (i<20) {
	    		cout << "temp[" << j << "]: " << temp[j] << endl;
	    	}
	    	line = nums.suffix().str(); 
	    	j++;
	    }
	    weight = temp[0];
	    length = temp[1];
	    // cout << "ok" << endl;
		// job_info.push_back(weight);
		// job_info.push_back(length);
		score = weight*1.0/length;
		job_order.insert(pair<double,int>(score,i));
		weights.push_back(weight);
		lengths.push_back(length);
	}
	data_file.close();

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
		comp_time += lengths[it->second];
		sum_time += comp_time * weights[it->second];
		it++;
	}
	cout << "weighted sum of completion time is: " << sum_time << endl;
	return 0;
}