#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include "Node.h"
using namespace std;

//class MyException() {};

int main(int argc, char* argv[]) {

	if(argc < 2){
		cerr << "Usage: " << argv[0] << " path_to_trace" << endl;
		return 1;
	}

	Node* root = new Node("main");
	Node* current = root;
	ifstream input;
	string line = "";
	input.open(argv[1]);
	if(input.fail()){
		cerr << "File open failed" << endl;
		return 1;
	}
	while(getline(input, line))
	{
		string name = "";
		if(line.substr(0, 12) == "[Call begin]")
		{
			current = current->add_child(line.substr(12));
			cout << "push " << current -> get_name() << endl;
		}
		/*else if(line.substr(0, 10) == "[Entry ts]")
		{
			stringstream ss;
			string tmp;
			long t;
			ss << line;
			ss >> tmp >> tmp;
			ss >> t;
			current -> set_start(t);
		}*/
		else if(line.substr(0, 10) == "[Call end]")
		{
			if (current != root)
			{
				cout << "pop " << current -> get_name();
				current = current->get_parent();
				cout << "; now " << current -> get_name() << endl;
			}
			else
				exit(1);
		}/*
		else if (line.substr(0, 9) == "[Exit ts]")
		{
			stringstream ss;
			string tmp;
			long t;
			ss << line;
			ss >> tmp >> tmp;
			ss >> t;
			current -> set_end(t);
		}*/
	}
	input.close();

	cout<<root<<endl;

}
