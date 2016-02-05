#include <iostream>
#include <string>
#include <fstream>
#include "Node.h"
using namespace std;

//class MyException() {};

int main() {

	Node* root = new Node("main");
	Node* current = root;
	ifstream input;
	string line = "";
	input.open("junit_out");
	while(getline(input, line))
	{
		string name = "";
		if(line.substr(0, 12) == "[Call begin]")
			current = current->add_child(line.substr(12));
		else if(line.substr(0, 10) == "[Call end]")
		{
			if (current != root)
				current = current->get_parent();
			else
				exit(1);
		}
		
	}
	input.close();

	cout<<root<<endl;
	
}