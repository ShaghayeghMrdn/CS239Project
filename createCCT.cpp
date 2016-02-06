#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include "Node.h"
using namespace std;

//class MyException() {};

void dfs_add_child(Node* tmpTree, Node* current){
	if(tmpTree->get_name() != "tmp"){
		Node* n = current -> add_child(tmpTree->get_name(), tmpTree->get_parent_weight());
		for(int i = 0; i < tmpTree->get_edges().size(); i++){
			dfs_add_child(tmpTree->get_edges()[i]->get_callee(), n);
		}
	}else{
		for(int i = 0; i < tmpTree->get_edges().size(); i++){
			dfs_add_child(tmpTree->get_edges()[i]->get_callee(), current);
		}
	}
}

int main(int argc, char* argv[]) {

	if(argc < 2){
		cerr << "Usage: " << argv[0] << " path_to_trace" << endl;
		return 1;
	}

	map<long, Node*> current;
	vector<Node*> starts;
	Node* root = new Node("main");
	current[1] = root;

	Node* tmpTree = new Node("tmp");
	Node* zombie = NULL;

	ifstream input;
	string line = "";
	input.open(argv[1]);
	if(input.fail()){
		cerr << "File open failed" << endl;
		return 1;
	}
	int starts_num = 0;
	int threads_num = 0;
	int level = 0;
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
			cout << line << endl;
			string method_name = line.substr(12);
			map<long, Node*>::iterator it = current.find(tid);
			//cerr<<"--->"<<method_name<<endl;
			if(it != current.end()){
				current[tid] = it->second->add_child(method_name);
				level++;
				cout << "level++" << endl;
				if(tid == 1 && method_name == "java/lang/Thread::start"){
					starts_num++;
					starts.push_back(current[1]);
				}
			}
			else //new thread
			{
				//assert(starts.size() != 0);
				if(starts.size() == 0){
					Node* new_thread = current[1] -> add_child(method_name);
					level++;
					cout << "level++" << endl;
					current[tid] = new_thread;
					threads_num++;
				}else{
					Node* new_thread = starts.front()->add_child(method_name);
					level++;
					cout << "level++" << endl;
					current[tid] = new_thread;
					starts.erase(starts.begin());
					threads_num++;
				}
			}

		}
		else if(line.substr(0, 10) == "[Entry ts]")
		{
			//cout << line << endl;
			 /*stringstream ss(line);
			 string tmp;
			 long t;
			 ss >> tmp >> tmp;
			 ss >> t;
			 current[tid] -> set_start(t);*/
		}
		else if(line.substr(0, 10) == "[Call end]")
		{
			// for exception
			string method_name = line.substr(10);
			std::size_t found = method_name.find("Exception::<init>");
			if( found != std::string::npos && zombie == NULL){
				zombie = current[tid] -> get_parent();
				current[tid] = tmpTree;
				continue;
			}
			cout << line << endl;
			map<long, Node*>::iterator it = current.find(tid);
			if(it != current.end())
			{
				string pname = it->second->get_name();
				cout << "pname: " << pname << endl;
				cout << "method_name: "<< method_name << endl;
				if(pname == method_name){
					current[tid] = it -> second -> get_parent();
					level--;
					cout << "level--" << endl;
				}else{
					if(zombie == NULL){
						//where we exception is not created by junit
						zombie = current[tid] -> get_parent();
					}
					while(true){
						if(zombie->get_name() == method_name){
							dfs_add_child(tmpTree, zombie);
							current[tid] = zombie -> get_parent();
							tmpTree = new Node("tmp");
							zombie = NULL;
							break;
						}else{
							zombie = zombie -> get_parent();
						}
					}

				}
				//current[tid] = it->second->get_parent();

			}
			else
				exit(1);
		}
		else if (line.substr(0, 9) == "[Exit ts]")
		{
			//cout << line << endl;
			/*stringstream ss(line);
			string tmp;
			long t;
			ss >> tmp >> tmp;
			ss >> t;
			current[tid] -> set_end(t);*/
		}

	}
	input.close();
	if(current[1] != root){
		assert(tmpTree->get_edges().size()>0);
		dfs_add_child(tmpTree, root);
	}
	cout<<root<<endl;

	cout << starts_num << " " << threads_num << endl;
	cout << level << endl;

}
