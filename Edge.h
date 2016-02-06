#include <string>

using namespace std;

class Node;

class Edge{
public:
	Edge(Node* src, Node* dst): caller(src), callee(dst) {weight = 1;}
    Edge(Node* src, Node* dst, int weight): caller(src), callee(dst) {this->weight = weight;}
	int get_weight() {return weight;}
	Node* get_caller() {return caller;}
	Node* get_callee() {return callee;}
	void inc_weight() {weight++;}
    void inc_weight(int weight) {this->weight += weight;}
private:
	int weight;
	Node* caller;
	Node* callee;
};
