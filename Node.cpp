#include "Node.h"

Node::Node(string n): name(n)
{
		parent_edge = NULL;
		edges = vector<Edge*>();
		total_time = 0;
		path = "";
}

void Node::set_parent(Edge* p_e)
{
	this->parent_edge = p_e;
}

Node* Node::add_child(string child_name)
{
	for(int i = 0; i<edges.size(); ++i)
	{
		Node* callee = edges[i]->get_callee();
		if(callee->get_name() == child_name)
		{
			edges[i]->inc_weight();
			return callee;
		}
	}

	Node* child = new Node(child_name);
	node_count++;
	Edge* new_edge = new Edge(this, child);
	child->set_parent(new_edge);
	edges.push_back(new_edge);
	return child;
}

Node* Node::add_child(string child_name, int weight,
	map<long, long> starts, map<long, long> ends, long total_time)
{
	for(int i = 0; i<edges.size(); ++i)
	{
		Node* callee = edges[i]->get_callee();
		callee -> add_time(total_time);
		if(callee->get_name() == child_name)
		{
			edges[i]->inc_weight(weight);
			return callee;
		}
	}

	Node* child = new Node(child_name);
	Edge* new_edge = new Edge(this, child, weight);
	child->set_parent(new_edge);
	edges.push_back(new_edge);
	child->set_starts(starts);
	child->set_ends(ends);
	child->set_total(total_time);
	return child;
}

Node::~Node()
{
}

ostream& operator << (ostream& out, const Node* n)
{
	out<<n->get_name();
	if(n->get_name() != "main") out<<"+"<<n->get_parent_weight() << "; avg time: " << (n->get_total_time()/n->get_parent_weight());
	vector<Edge*> e = n->get_edges();
	if(e.size() != 0)
	{
		out<<" [";
			for(int i = 0; i<e.size(); ++i)
			{
				out<<e[i]->get_callee();
				if(i != e.size()-1)
					out<<", ";
			}
		out<<"]";
	}
	return out;
}
