#include <iostream>
#include <vector>
#include <string>
#include "Edge.h"
using namespace std;

class Node 
{
public:

	Node(string n): name(n) {
		parent_edge = NULL;
		edges = vector<Edge*>();
	}
	~Node();

	void set_parent(Edge* p_e);
	Node* add_child(string child_name);
	string get_name() const {return name;}
	Node* get_parent() {return this->parent_edge->get_caller();}
	vector<Edge*> get_edges() const {return edges;}
	int get_parent_weight() const {return parent_edge->get_weight();}

private:
	string name;
	Edge* parent_edge;
	vector<Edge*> edges;
};

ostream& operator << (ostream& out, const Node* n);