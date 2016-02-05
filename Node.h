#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include "Edge.h"
using namespace std;

class Node
{
public:

	Node(string n): name(n) {
		parent_edge = NULL;
		edges = vector<Edge*>();
		ts_start = 0;
		ts_end = 0;
		total_time = 0;
	}
	~Node();

	void set_parent(Edge* p_e);
	Node* add_child(string child_name);
	string get_name() const {return name;}
	Node* get_parent() {return this->parent_edge->get_caller();}
	vector<Edge*> get_edges() const {return edges;}
	int get_parent_weight() const {return parent_edge->get_weight();}
	void set_start(long t){
		cout << "start: " << name << endl;
		assert(ts_start == 0 );
		ts_start = t;
	}
	void set_end(long t){
		cout << "end: " << name << endl;
		assert(ts_end == 0);
		ts_end = t;
		assert(ts_end >= ts_start);
		total_time = total_time + (ts_end - ts_start);
		ts_start = 0;
		ts_end = 0;
	}
	long get_total_time() const{return total_time;}

private:
	string name;
	Edge* parent_edge;
	vector<Edge*> edges;
	long ts_start;
	long ts_end;
	long total_time;
};

ostream& operator << (ostream& out, const Node* n);
