#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include "Node.h"
using namespace std;

//class MyException() {};

int main(int argc, char* argv[]) {

	if(argc < 2){
		cerr << "Usage: " << argv[0] << " path_to_trace" << endl;
		return 1;
	}

	map<long, Node*> current;
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
	while(getline(input, line))
	{
		string tid_str, name = "";
		long tid;
		tid_str = line.substr(6, line.find("]")-6);
		stringstream ss(tid_str);
		ss>>tid;
		line = line.substr(line.find("]")+1);

		if(line.substr(0, 12) == "[Call begin]")
		{
			string method_name = line.substr(13);
			map<long, Node*>::iterator it = current.find(tid);
			//cerr<<"--->"<<method_name<<endl;
			if(it != current.end()){
				current[tid] = it->second->add_child(method_name);

				if(tid == 1 && method_name == "java/lang/Thread::start")
					starts.push_back(current[1]);
			}
			else //new thread
			{
				assert(starts.size() != 0);
				Node* new_thread = starts.front()->add_child(method_name);
				current[tid] = new_thread;
				starts.erase(starts.begin());
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
	assert(current[1] == root);
	cout<<root<<endl;

}
