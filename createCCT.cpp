#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include "Node.h"
using namespace std;

//class MyException() {};
int node_count = 0;

void DFS (Node* root, map<int, int>& frequencies)
{
	vector<Edge*> edges = root->get_edges(); 
	for(int i = 0; i < edges.size(); ++i)
	{
		Node* child = edges[i]->get_callee();
		child->set_path(root->get_path()+to_string(i));
		int w = child->get_parent_weight();
		map<int, int>::iterator it = frequencies.find(w);
		if(it != frequencies.end())
			frequencies[w] += 1;
		else
			frequencies[w] = 1;

		DFS(child, frequencies);
	}
}

int main(int argc, char* argv[]) {

	if(argc < 2){
		cerr << "Usage: " << argv[0] << " path_to_trace" << endl;
		return 1;
	}

	map<long, Node*> current;
	map<string, int> method_counts;
	vector<Node*> starts;
	Node* root = new Node("main");
	current[1] = root;

	ifstream input;
	string line = "";
	input.open(argv[1]);
	if(input.fail()){
		cerr << "File open failed" << endl;
		return 1;
	}
	int starts_num = 0;
	int threads_num = 0;
	while(getline(input, line))
	{
		string tid_str, name = "";
		long tid;

		if(line.size() < 8)
			continue;

		tid_str = line.substr(6, line.find("]")-6);
		stringstream ss(tid_str);
		ss>>tid;
		line = line.substr(line.find("]")+1);

		if(line.substr(0, 12) == "[Call begin]")
		{
			string method_name = line.substr(12);
			//cerr<<"begin"<<method_name<<endl;

			map<string, int>::iterator mit = method_counts.find(method_name);
			if(mit != method_counts.end())
				mit->second += 1;
			else
				method_counts[method_name] = 1;


			map<long, Node*>::iterator it = current.find(tid);
			if(it != current.end())
			{
				current[tid] = it->second->add_child(method_name);

				if(tid == 1 && method_name == "java/lang/Thread::start"){
					starts.push_back(current[1]);
					starts_num += 1;
				}
			}
			else //new thread
			{
				Node* new_thread;
				if(starts.size() == 0)
					new_thread = current[1]->add_child(method_name);
				else
				{
					assert(starts.size() != 0);
					new_thread = starts.front()->add_child(method_name);
					starts.erase(starts.begin());
				}	
				current[tid] = new_thread;
				threads_num += 1;
			}

		}
		else if(line.substr(0, 10) == "[Entry ts]")
		{
			// stringstream ss(line);
			// string tmp;
			// long t;
			// ss >> tmp >> tmp;
			// ss >> t;
			// current -> set_start(t);
		}
		else if(line.substr(0, 10) == "[Call end]")
		{
			
			map<long, Node*>::iterator it = current.find(tid);
			if(it != current.end())
			{
				current[tid] = it->second->get_parent();
				//cerr<<line<<endl;
			}
			else
				exit(1);
		}
		else if (line.substr(0, 9) == "[Exit ts]")
		{
			// stringstream ss(line);
			// string tmp;
			// long t;
			// ss >> tmp >> tmp;
			// ss >> t;
			// current -> set_end(t);
		}
	}
	input.close();
	//cout<<root<<endl;
	// cout<<starts_num<<" "<<threads_num<<endl;
	// assert(starts_num == threads_num);
	// cout<<"numbr of methods: "<<method_counts.size()<<endl;
	// cout<<"--method frequencies: "<<endl;
	// for(map<string, int>::iterator it = method_counts.begin(); it != method_counts.end(); ++it)
		// cout<<it->first<<" "<<it->second<<endl;
	// cout<<"----------------------"<<endl;
	// assert(current[1] == root);

	//DFS on tree
	// cout<<"number of nodes: "<<node_count<<endl;
	map<int, int> freqs;
	root->set_path("0");
	DFS(root, freqs);

	// cout<<"--call stack (node) frequencies: "<<endl;
	cout<<"weight,frequency"<<endl;
	for(map<int, int>::iterator it = freqs.begin(); it != freqs.end(); ++it)
		cout<<it->first<<","<<it->second<<endl;
}
