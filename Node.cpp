#include "Node.h"

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
	Edge* new_edge = new Edge(this, child);
	child->set_parent(new_edge);
	edges.push_back(new_edge);
	return child;
}

Node::~Node()
{

}

ostream& operator << (ostream& out, const Node* n)
{
	out<<n->get_name();
	if(n->get_name() != "main") out<<"+"<<n->get_parent_weight();
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