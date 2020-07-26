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
#include <unordered_map>

using namespace std;

// int S = 1; // pick 1 as the starting node

int main() {
	int total_cost; // this is the output

	int adj_mat[501][501]; // maps edge to cost
	unordered_map<int,vector<int>> adj_list; 
	// unordered_multimap<int,vector<int>> cost_map; // map cost to edges
	// non-existing edges have infinite cost
	for (int i=0; i<501; i++) {
		for (int j=0; j<501; j++) {
			adj_mat[i][j] = INT_MAX;
		}
	}

	// record edge costs
	int num_v, num_e, u, v, cost;
	ifstream data_file("mst.txt");
	data_file >> num_v >> num_e;
	while (data_file >> u >> v >> cost) {
		adj_mat[u][v] = cost;
		adj_mat[v][u] = cost;
		// vector<int> edge;
		// edge.push_back(u);
		// edge.push_back(v);
		// cost_map.insert(pair<int,vector<int>>(cost,edge));
	}
	data_file.close();

	// creates adjacency list
	for (int i=1; i<501; i++) {
		for (int j=1; j<501; j++) {
			if (adj_mat[i][j] == INT_MAX) continue;
			adj_list[i].push_back(j);
			adj_list[j].push_back(i);
		}
	}

	set<int> X; // the explored nodes
	// int x_ = 0; // number of vertices in the MST

	/* simple implementation start */
	/* simple implementation start */

	/* smart implementation start */
	// initialize heap for all vertices
	multimap<int,int> v_heap; // best crossing cost as key, vertex index as value
	map<int,int> v_map; // vertex index as key, best crossing cost as value

	for (int i=1; i <= num_v; i++) { // initialize crossing cost as infinite
		v_heap.insert(pair<int,int>(INT_MAX,i));
		v_map.insert(pair<int,int>(i,INT_MAX));
	}

	while (X.size() < num_v) {
		// perform extract-min
		auto top = v_heap.begin();
		int vert = top->second;
		int edge_cost = top->first;
		// update total_cost, add the vertex to the set, remove from heap
		if (X.size() != 0) {
			total_cost += edge_cost;
		}
		X.insert(vert);
		v_heap.erase(top);
		v_map.erase(vert);
		for (int i=0; i<adj_list[vert].size(); i++) {
			int w = adj_list[vert][i];
			if (X.find(w) != X.end()) {
				continue; // w already explored!
			}
			int opt_cost = v_map[w];
			std::pair <std::multimap<int,int>::iterator, std::multimap<int,int>::iterator> ret;
			ret = v_heap.equal_range(opt_cost);
			for (multimap<int,int>::iterator itr=ret.first; itr!=ret.second; itr++) {
				if (itr->second != w) continue; // not interested
				// found the entry corresponding to w, delete, update, re-insert
				v_heap.erase(itr);
				int new_cost = min(adj_mat[vert][w], opt_cost);
				v_map[w] = new_cost;
				v_heap.insert(pair<int,int>(new_cost,w));
				break; // w updated, no need to keep looking
			}
		}
	}
	/* smart implementation start */
	cout << "cost of MST: " << total_cost << endl;
	return 0;
}