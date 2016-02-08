#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <assert.h>
#include <map>
#include "Edge.h"
using namespace std;

extern int node_count;

class Node
{
public:

	Node(string n);
	~Node();
	void set_parent(Edge* p_e);
	Node* add_child(string child_name);
	Node* add_child(string child_name, int weight, map<long, long> starts, map<long, long> ends, long total_time);
	string get_name() const {return name;}
	Node* get_parent() {
		if(parent_edge == NULL)
			return NULL;
		return this->parent_edge->get_caller();
	}
	vector<Edge*> get_edges() const {return edges;}
	int get_parent_weight() const {return parent_edge->get_weight();}
	void set_path(string s) {path = s;}
	string get_path() {return path;}
	void set_start(long t, long tid){
		//cout << "start: " << name << endl;
		//assert(ts_start == 0);
		starts[tid] = t;
		//ts_start = t;
	}

	void set_end(long t, long tid){
		//cout << "end: " << name << endl;
		//assert(ts_end == 0);
		//cout << t << " " <<starts[tid]  << " " << name << endl;
		assert(starts[tid] != 0 || name == "tmp");
		assert(t >= starts[tid]);
		ends[tid] = t;
		if(name == "tmp")
			return;
		//ts_end = t;
		total_time = total_time + (ends[tid] - starts[tid]);
		starts[tid] = 0;
		ends[tid] = 0;
	}
	long get_total_time() const{
		assert(total_time >= 0);
		return total_time;
	}

	void add_time(long ttime){total_time += ttime;}

	void set_starts(map<long,long> info){
		starts = info;
	}

	void set_ends(map<long,long> info){
		ends = info;
	}

	void set_total(long total){
		total_time = total;
	}

	map<long,long> get_starts(){
		return starts;
	}

	map<long,long> get_ends(){
		return ends;
	}

private:
	string name;
	Edge* parent_edge;
	vector<Edge*> edges;
	long ts_start;
	map<long, long> starts;
	map<long, long> ends;
	long total_time;

	string path; //int representation for path to a node
};

ostream& operator << (ostream& out, const Node* n);
