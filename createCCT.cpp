#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include "Node.h"
using namespace std;

int node_count = 0;

void DFS (Node* root, map<int, int>& frequencies, vector<pair<string, int> >& heavies,
			vector<pair<string, long> >& longests, map<string, long>& mtimes)
{
	vector<Edge*> edges = root->get_edges();
	for(int i = 0; i < edges.size(); ++i)
	{
		Node* child = edges[i]->get_callee();
		//string path = root->get_path()+to_string(i); //edges[i]->get_callee()->get_name()
		string path = root->get_path() +  "->\n" + child->get_name();
		child->set_path(path);
		int w = child->get_parent_weight();
		map<int, int>::iterator it = frequencies.find(w);
		if(it != frequencies.end())
			frequencies[w] += 1;
		else
			frequencies[w] = 1;

		//long exec_t = child->get_total_time()/child->get_parent_weight();
		long exec_t = child -> get_total_time();
		for(int i = 0; i<10; ++i)
		{
			if(w > heavies[i].second)
			{
				heavies.insert(heavies.begin()+i, pair<string, int>(path, w));
				heavies.erase(heavies.begin()+10);
				break;
			}
		}

		assert(exec_t >= 0);
		//all_times.push_back(exec_t);
		mtimes[child->get_name()] += exec_t;
		for(map<string, long>::iterator mit = mtimes.begin(); mit != mtimes.end(); mit++){
			for(int i = 0; i<10; ++i)
			{
				if((mit -> second) >= longests[i].second)
				{
					longests.insert(longests.begin()+i, pair<string, int>(mit -> first, mit -> second));
					longests.erase(longests.begin()+10);
					break;
				}
			}
		}

		DFS(child, frequencies, heavies, longests, mtimes);
	}
}

/*void DFS_ts (Node* root, map<int, int>& frequencies)
{
	vector<Edge*> edges = root->get_edges();
	for(int i = 0; i < edges.size(); ++i)
	{
		Node* child = edges[i]->get_callee();
		child->set_path(root->get_path()+to_string(i));
		int w = ;
		map<int, int>::iterator it = frequencies.find(w);
		if(it != frequencies.end())
			frequencies[w] += 1;
		else
			frequencies[w] = 1;

		DFS(child, frequencies);
	}
}*/

void dfs_add_child(Node* tmpTree, Node* current)
{
	if(tmpTree->get_name() != "tmp"){
		Node* n = current -> add_child(tmpTree->get_name(), tmpTree->get_parent_weight(),
			tmpTree -> get_starts(), tmpTree -> get_ends(), tmpTree-> get_total_time());
		for(int i = 0; i < tmpTree->get_edges().size(); i++){
			dfs_add_child(tmpTree->get_edges()[i]->get_callee(), n);
		}
	}else{
		map<long, long> starts = tmpTree->get_starts();
		map<long, long> ends = tmpTree->get_ends();
		long total_time = tmpTree->get_total_time();
		current->add_time(total_time);
		for(map<long, long>::iterator it = starts.begin(); it != starts.end(); it++){
			long tid = it->first;
			long stime = it->second;
			if(stime!=0)
				current->set_start(stime, tid);
			else{
				//assert(current->get_start(tid) != 0);
			}
		}
		for(map<long, long>::iterator it = ends.begin(); it != ends.end(); it++){
			long tid = it->first;
			long etime = it->second;
			if(etime!=0)
				current->set_end(etime, tid);
			else{
				//assert(current->get_end(tid) != 0);
			}
		}
		for(int i = 0; i < tmpTree->get_edges().size(); i++){
			dfs_add_child(tmpTree->get_edges()[i]->get_callee(), current);
		}
	}
}

void generate_data(Node* root)
{
	ofstream out;
	map<int, int> freqs;
	vector<pair<string, int> > heavies;
	vector<pair<string, long> > longests;
	map<string, long> mtimes;

	for(int i = 0; i<10; ++i)
	{
		heavies.push_back(pair<string, int>("", 0));
		longests.push_back(pair<string, int>("", 0));
	}

	out.open("nw-f.csv");
	root->set_path("0");
	DFS(root, freqs, heavies, longests, mtimes);
	// --call stack (node) frequencies:
	out<<"weight,frequency"<<endl;
	for(map<int, int>::iterator it = freqs.begin(); it != freqs.end(); ++it)
		out<<it->first<<","<<it->second<<endl;
	out.close();

	out.open("n-w.csv");
	out<<"call_stack,weight"<<endl;
	for(int i = 0; i<heavies.size(); ++i)
		out<<heavies[i].first<<","<<heavies[i].second<<endl;
	out.close();

	out.open("n-t.csv");
	out<<"method,execution_time"<<endl;
	for(int i = 0; i<longests.size(); ++i)
		out<<longests[i].first<<","<<longests[i].second<<endl;
	out.close();

	out.open("times.csv");
	out<<"execution_time"<<endl;
	for(map<string, long>::iterator it = mtimes.begin(); it != mtimes.end(); ++it)
		out<<it->second<<endl;
	out.close();
}

int main(int argc, char* argv[])
{

	if(argc < 2){
		cerr << "Usage: " << argv[0] << " path_to_trace" << endl;
		return 1;
	}

	map<long, Node*> current;
	map<string, int> method_counts;
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
			map<string, int>::iterator mit = method_counts.find(method_name);
			if(mit != method_counts.end())
				mit->second += 1;
			else
				method_counts[method_name] = 1;

			map<long, Node*>::iterator it = current.find(tid);
			if(it != current.end())
			{
				current[tid] = it->second->add_child(method_name);
				if(tid == 1 && method_name == "java/lang/Thread::start")
				{
					starts_num++;
					starts.push_back(current[1]);
				}
			}
			else //new thread
			{
				//assert(starts.size() != 0);
				if(starts.size() == 0){
					Node* new_thread = current[1]->add_child(method_name);
					current[tid] = new_thread;
					threads_num++;
				}else{
					Node* new_thread = starts.front()->add_child(method_name);
					current[tid] = new_thread;
					starts.erase(starts.begin());
					threads_num++;
				}
			}
			std::size_t found = method_name.find("Exception::<init>");
			if( found != std::string::npos && zombie == NULL){
				zombie = current[tid];
			}
		}
		else if(line.substr(0, 10) == "[Entry ts]")
		{
			 stringstream ss(line);
			 string tmp;
			 long t;
			 ss >> tmp >> tmp;
			 ss >> t;
			 //cout << "set thread " << tid << " START to " << t << endl;
			 current[tid]->set_start(t, tid);
		}
		else if(line.substr(0, 10) == "[Call end]")
		{
			// for exception
			string method_name = line.substr(10);
			size_t found = method_name.find("Exception::<init>");
			if(found != std::string::npos && zombie == current[tid]){
				current[tid] = tmpTree;
				continue;
			}
			map<long, Node*>::iterator it = current.find(tid);
			if(it != current.end())
			{
				string pname = it->second->get_name();
				if(pname != method_name) //When it's not the same as we expect it to be
				{
					if(zombie == NULL){
						//where the exception is not created by junit
						zombie = current[tid];
					}else if (current[tid] != tmpTree)
					{
						//get the root of current[tid] to see if in tmpTree
						Node* r = current[tid];
						while(r -> get_parent()!= NULL)
							r = r->get_parent();

						if(r->get_name() == "tmp")
						{
							r = current[tid]->get_parent();
							while(true)
							{
								if(r -> get_name() == method_name){
									current[tid] = r;
									break;
								}
								r = r -> get_parent();
								if(r == NULL)
									break;
							}
							if(r != NULL)
								continue; //***** hmm?
						}
					}

					zombie = zombie->get_parent(); //coming out of the scope that exception was thrown, in to the upper scope
					while(true)
					{
						if(zombie->get_name() == method_name)
						{
							dfs_add_child(tmpTree, zombie);
							//Don't pop it now, pop in exit ts
							current[tid] = zombie;
							tmpTree = new Node("tmp");
							zombie = NULL;
							break;
						}else
							zombie = zombie->get_parent();
					}
				}

			}
			else
				exit(1); //Invalid stack trace
		}
		else if (line.substr(0, 9) == "[Exit ts]")
		{
			stringstream ss(line);
			string tmp;
			long t;
			ss >> tmp >> tmp;
			ss >> t;
			if(current[tid]->get_name() == "tmp")
			{
				assert(zombie != NULL);
				zombie ->set_end(t, tid);
			} else
			{
				current[tid]->set_end(t, tid);
				current[tid] = current[tid]->get_parent(); //Pop it here!
			}
		}
	}
	input.close();

	if(current[1] != root){
		assert(tmpTree->get_edges().size()>0);
		dfs_add_child(tmpTree, root);
	}

	generate_data(root);

	// cout<<"--method frequencies: "<<endl;
	// for(map<string, int>::iterator it = method_counts.begin(); it != method_counts.end(); ++it)
	// {
	// 	size_t found = it->first.find_last_of("/");
	// 	string simple_name = it->first;
	// 	if(found != string::npos)
	// 		simple_name = simple_name.substr(found+1);
	// 	cout<<simple_name<<","<<it->second<<endl;
	// }

}
