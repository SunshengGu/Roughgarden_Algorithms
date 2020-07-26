#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <regex>
#include <set>
#include <algorithm>
#include <iterator>
#include <cmath>

using namespace std;

// to store fwd and reverse adjacency lists
vector<vector<int>> ADJ_LIST, REV_ADJ_LIST;
/* stores finishing time (index as finishing time, value as node index,
this way, sorting can be avoided)
and if the node have been visited */
int F[875715], VISITED[875715], VISITED_II[875715]; 
int SIZE = 875714;
int T = 0; // finishing time

// DFS on the reversed graph
void rev_DFS(int node) {
	VISITED[node] = 1;
	for (int j=0; j<REV_ADJ_LIST[node].size(); j++) {
		if (!VISITED[REV_ADJ_LIST[node][j]]) {
			// call rev_DFS is this one has not been visited
			rev_DFS(REV_ADJ_LIST[node][j]);
		}
	}
	T++;
	F[T] = node;
}

// DFS on the fwd graph
void fwd_DFS(int node, int &cluster_size) {
	VISITED_II[node] = 1;
	for (int j=0; j<ADJ_LIST[node].size(); j++) {
		if (!VISITED_II[ADJ_LIST[node][j]]) {
			// call rev_DFS is this one has not been visited
			fwd_DFS(ADJ_LIST[node][j], cluster_size);
		}
	}
	cluster_size++;
}

int main() {
	std::ifstream data_file("scc.txt");
	int s, t;
	int prev_s = 0;
	// neighs are the neighbours of each node
	vector<int> neighs, rev_neighs;
	// sort as we insert
	multiset<int> cluster_sizes;
	// int max_len = 0, leng = 0;

	/* construct empty arrays */
	for (int i=0; i<SIZE+1; i++) {
		ADJ_LIST.push_back(neighs);
		REV_ADJ_LIST.push_back(neighs);
		VISITED[i] = 0; // initially nothin is visited
	}

	/* constructing the fwd adj list */
	while(data_file >> s >> t) {
		ADJ_LIST[s].push_back(t);
		REV_ADJ_LIST[t].push_back(s);
	}
	data_file.close();

	// /* debug message, make sure the lists were processed properly */
	// for (int i=1; i<21; i++) {
	// 	cout << "node: " << i << " destinations:";
	// 	for (int j=0; j<ADJ_LIST[i].size(); j++) {
	// 		cout << " " << ADJ_LIST[i][j];
	// 	}
	// 	cout << endl;
	// }
	// for (int i=1; i<21; i++) {
	// 	cout << "node: " << i << " sources:";
	// 	for (int j=0; j<REV_ADJ_LIST[i].size(); j++) {
	// 		cout << " " << REV_ADJ_LIST[i][j];
	// 	}
	// 	cout << endl;
	// }

	/* call DFS on the reverse graph and compute the f score */
	for (int i=SIZE; i>=1; i--) {
		if (!VISITED[i]) {
			rev_DFS(i);
		}
	}

	for (int k=SIZE; k>=1; k--) {
		int comp_size = 0;
		if (!VISITED_II[F[k]]) { // F[k]: node with finishing_time = k
			fwd_DFS(F[k], comp_size);
		}
		// record the size of the strongly connected component
		cluster_sizes.insert(comp_size);
	}

	multiset<int>::iterator it = cluster_sizes.end();
	it--;
	int count = 0;

	while (count < 5) {
		cout << *it << ",";
		if (it == cluster_sizes.begin()) break;
		it--;
		count++;
	}
	cout << endl;
}