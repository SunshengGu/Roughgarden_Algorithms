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

int S = 1; // source node
int UNREACHABLE = 10000000;

// process each line of input
// template <size_t rows, size_t cols>
void process_line(string line, int (&adj_matrix)[201][200], vector<vector<int>> &adj_list) {
	// get the source vertex
	std::regex num (R"([0-9]+)");
    std::smatch nums;
    std::regex_search (line,nums,num);
    int source = std::stoi(nums[0].str());
    vector<int> neighs;
    // cout << "source node: " << source << endl;

    // get destination vertices and distances
    std::regex e (R"([0-9]+,[0-9]+)");
    std::smatch dists;
    vector<int> mynums;
    while (std::regex_search (line,dists,e)) {
        std::regex num (R"([0-9]+)");
        std::smatch nums;
        int ind = 0;
        std::string new_input = dists[0].str();
        while (std::regex_search (new_input,nums,num)) {
            int temp = std::stoi(nums[0].str());
            mynums.push_back(temp);
            ind++;
            new_input = nums.suffix().str();
        }
        // cout << "ok after processing a line" << endl;
        // cout << "destination node: " << mynums[0] << endl;
        // cout << "edge length: " << mynums[1] << endl;
        adj_matrix[source][mynums[0]] = mynums[1]; // record length of edge
        // cout << "ok after updating adj_mat" << endl;
        neighs.push_back(mynums[0]); // record id of the neighbour
        mynums.clear();
        line = dists.suffix().str(); // update input
    }
    adj_list.push_back(neighs); 
}

int main() {
	int adj_mat[201][200]; // use adjacency matrix to store distances between edges
	memset(adj_mat, -1, sizeof(adj_mat)); // initialize as all -1

	vector<vector<int>> adj_list; // have an adj_list as well to reduce time complexity
	vector<int> dummy;
	adj_list.push_back(dummy); // to account for starting at zero

	// reading the adjacency matrix
	string input;
	std::ifstream data_file("dijkstra.txt");
	while(std::getline(data_file, input)) {
		process_line(input, adj_mat, adj_list);
	}
	data_file.close();
	cout << "ok after reading input" << endl;

	multimap<int,int> dijkstra_map; // store Dijkstra score of each node, key as score
	dijkstra_map.insert(pair<int,int>(0,S));
	for (int i=2; i<=200; i++) {
		dijkstra_map.insert(pair<int,int>(10000000,i));
	}
	// dijkstra_map[0] = S;
	//map<int,int> rev_map; // store Dijkstra score of each node, value as score

	int dijkstra_arr[201]; // array to store the Dijkstra scores for each access
	for (int i=0; i<201; i++) {
		dijkstra_arr[i] = UNREACHABLE;
	}

	int X_cnt = 0;

	//proc_set.push_back(S); // source is process
	int proc_ind[201]; // indicates if a vertex has been processed
	memset(proc_ind, 0, sizeof(proc_ind)); // memset only works for 0 and -1
	//proc_ind[S] = 1;

	while (X_cnt < 200) {
		// record the best node in V_minus_X and its score, put it into X
		multimap<int,int>::iterator it = dijkstra_map.begin();
		int w = it->second;
		int opt_score = it->first;
		dijkstra_map.erase(it);
		X_cnt++;
		// V_minus_X.erase(w);
		// X.push_back(w);
		dijkstra_arr[w] = opt_score;
		proc_ind[w] = 1;
		cout << "current best score: " << dijkstra_arr[w] << endl;
		cout << "size of dijkstra_map: " << dijkstra_map.size() << endl;
		// cout << "ok after adding w to X" << endl;
		// modify the score of w's neighbours in V_minus_X
		for (int i=0; i<adj_list[w].size(); i++) {
			// cout << "looking at a neighbour" << endl;
			int dest = adj_list[w][i]; // destination of current edge
			if (proc_ind[dest]) {
				continue; // that means it's in x
			}
			cout << "a destination node: " << dest << endl;
			int prev = dijkstra_arr[dest]; // previous score for dest
			cout << "previous score: " << prev << endl;
			std::pair <std::multimap<int,int>::iterator, std::multimap<int,int>::iterator> ret;
			ret = dijkstra_map.equal_range(prev); // find all elements with this score
			// cout << "ok after finding nodes with the same score" << endl;
			if (ret.first == ret.second) {
				cout << "nothing found" << endl;
			}
			for (multimap<int,int>::iterator itr=ret.first; itr!=ret.second; itr++) {
				// cout << "found a node with the same score as prev" << endl;
				if (itr->second != dest) continue;
				// update the dest node's score, erase and re-insert into the map
				// cout << "found a neighbour outside of X" << endl;
				dijkstra_map.erase(itr);
				dijkstra_arr[dest] = min(prev, dijkstra_arr[w] + adj_mat[w][dest]);
				cout << "updated score of destination node " << dest << ": " << dijkstra_arr[dest] << endl;
				dijkstra_map.insert(pair<int,int>(dijkstra_arr[dest],dest));
				// dijkstra_map[dijkstra_arr[dest]] = dest;
				// cout << "ok after modifying a neighbour of w" << endl;
				break;
			}
		}
	}
	// 7,37,59,82,99,115,133,165,188,197
	cout << dijkstra_arr[7] << ',' << dijkstra_arr[37] << ',' << dijkstra_arr[59] << ',' << dijkstra_arr[82] << ',' \
	<< dijkstra_arr[99] << ',' << dijkstra_arr[115] << ',' << dijkstra_arr[133] << ',' << dijkstra_arr[165] << ',' \
	<< dijkstra_arr[188] << ',' << dijkstra_arr[197] << endl;
	return 0;
}