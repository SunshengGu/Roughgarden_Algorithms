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

int END=200, START=1; // start and end index for the vertices, used for random generation
int LEAST_CUT = INT_MAX; // keep track of minimum cut

// randomly generate a vertex to be combined
int random_int_gen(int min, int max) {
	unsigned random_value = 0; //Declare value to store random number into
	size_t size = sizeof(unsigned); //Declare size of data
	std::ifstream urandom("/dev/urandom", ios::in|ios::binary); //Open the stream
	if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 5000; // use 5000 as an error code
    }
    urandom.read(reinterpret_cast<char*>(&random_value), size); //Read from urandom
	double inter = round(random_value / 4294967295.0 * (max-min)); // rescale the random value
	int result = min+(int)inter;
    //std::cout << "Read random value: " << result << std::endl;
    urandom.close();
    return result;
}

vector<vector<int>> copy_list(vector<vector<int>>& in_list) {
	/* create a temperary adjacency list for modification*/
	vector<vector<int>> dup_list;
	for (int a=0; a<in_list.size(); a++) {
		vector<int> temp_neigh;
		for (int b=0; b<in_list[a].size(); b++) {
			temp_neigh.push_back(in_list[a][b]);
		}
		dup_list.push_back(temp_neigh);
	}
	return dup_list;
}

void contract(vector<vector<int>>& my_adj_list, set<int>& include, int& n_nodes) {
	/* contract an edge by combining two vertices
	1. generate a node_0 position (in include) at random
	2. randomly select node_1 from node_0's neighbours (in include)
	3. select the longer list to keep, shorter one removed from include
	4. for shorter one's neighbours, for their neighbours, replace shorter with longer; and remove contracted edges
	5. stuff the shorter one's neighbours into longer one's neighbours
	6. n_nodes--
	*/

	/* step 1 */
	int node_0, node_1, node_0_pos_in_set, node_1_pos;
	int set_len = include.size();
	node_0_pos_in_set = random_int_gen(0, set_len-1); // randomly gets the position of node_0
	set<int>::iterator it_0 = include.begin();
	std::advance(it_0, node_0_pos_in_set);
	node_0 = *it_0;

	/* step 2 */
	int node_0_list_len = my_adj_list[node_0].size();
	node_1_pos = random_int_gen(0, node_0_list_len-1);
	node_1 = my_adj_list[node_0][node_1_pos];
	while (include.find(node_1)==include.end()) { // ensures that node_1 is in the include set
		node_1_pos = random_int_gen(0, node_0_list_len-1);
		node_1 = my_adj_list[node_0][node_1_pos];
	}
	// cout << "the while loop for finding node_1 is finished." << endl;

	/* step 3 -- motiviation: reduce the number of copy operations */
	int longer, shorter, node_1_list_len;
	node_1_list_len = my_adj_list[node_1].size();
	if (node_0_list_len < node_1_list_len) {
		longer = node_1;
		shorter = node_0;
	} else {
		longer = node_0;
		shorter = node_1;
	}
	include.erase(shorter); // so that we won't generate this node again

	/* step 4 */
	for (int i=0; i<my_adj_list[shorter].size(); i++) {
		// loop through neighbours of the shorter one
		int neigh = my_adj_list[shorter][i];
		if (neigh == longer || neigh == shorter) {
			// removes contracted edge and self loops
			my_adj_list[shorter].erase(my_adj_list[shorter].begin()+i); 
			i--; // decrement index since an element is erased
			continue;
		}
		for (int j=0; j<my_adj_list[neigh].size(); j++) {
			// loop through neigh's neighbours
			if (my_adj_list[neigh][j] == shorter) {
				my_adj_list[neigh][j] = longer;
			}
		}
	}
	for (int k=0; k<my_adj_list[longer].size(); k++) {
		if (my_adj_list[longer][k] == shorter || my_adj_list[longer][k] == longer) {
			// removes contracted edge and self loops
			my_adj_list[longer].erase(my_adj_list[longer].begin()+k); 
			k--;
		}
	}

	/* step 5 */
	for (int m=0; m<my_adj_list[shorter].size(); m++) {
		my_adj_list[longer].push_back(my_adj_list[shorter][m]);
	}

	/* step 6 */
	n_nodes--; // a node is eliminated
	my_adj_list[shorter].clear(); // no need to have this list now
}

int main() {
	/* read in the data */
	vector<vector<int>> adj_list; // to store adjacency list, 0th element empty, 1st-200th non-empty
	vector<int> place_holder; 
	adj_list.push_back(place_holder); // put a place holder for first element for convinience
	std::ifstream data_file("min_cut.txt");
	string input;
	while(std::getline(data_file, input)) {
		std::regex num (R"([0-9]+)");
        std::smatch nums;
        int ind = 0;
        vector<int> neigh;
        while (std::regex_search (input,nums,num)) {
        	int temp = std::stoi(nums[0].str());
        	input = nums.suffix().str(); // so that we can get the next number
            if (ind==0) { // reading the id of the edge, no need to record
            	ind++;
            	continue;
            } else {
            	neigh.push_back(temp);
            	ind++;
            }
        }
        adj_list.push_back(neigh); // push this bunch of neighbors into the adj list
	}
	data_file.close();

	// /* make sure that the adj list was read correctly */
	// for (int i=START; i<=END; i++) {
	// 	cout << "Vertex: " << i << " " << "Neighbours: ";
	// 	for (int j=0; j<adj_list[i].size(); j++) {
	// 		cout << adj_list[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }
	
	// set<int> keep; // record vertices eliminated due to contraction
	// for (int j=1; j<201; j++) {
	// 	keep.insert(j);
	// }

	/* repeat many times to find the minimum cut */
	int tries = 50; // n^2 * log(n) tries to ensure we get the best answer
	for (int i=0; i<tries; i++) {
		vector<vector<int>> temp_adj_list = copy_list(adj_list);
		int n_nodes = END;
		set<int> keep; // record vertices eliminated due to contraction
		for (int j=1; j<201; j++) {
			keep.insert(j);
		}
		while (n_nodes > 2) {
			/* perform contraction */
			contract(temp_adj_list, keep, n_nodes);
			// cout << "number of nodes remaining: " << n_nodes << endl;
		}
		/* compute number of cuts */
		set<int>::iterator A = keep.begin();
		int group_a = *A;
		int cuts = temp_adj_list[group_a].size();
		cout << "attempt# " << i << endl;
		cout << "cuts: " << cuts << endl << endl;
		LEAST_CUT = min(cuts, LEAST_CUT);
	}
	cout << "least cut: " << LEAST_CUT << endl;
	return 0;
}