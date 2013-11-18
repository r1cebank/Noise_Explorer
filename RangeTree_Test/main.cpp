#include <iostream>
#include "RangeTree.h"

using namespace std;

void printColor(RangeTree::color *color);

int main(int __argc, char** __argv){

	cout << "RangeTree Test." << endl;
	RangeTree tree(3);
	RangeTree::color *tempColor;
	for(int i = 50; i< 100; i++) {
		cout << "<<<<<<<<<<<<<T>>>>>>>>>>>    " << i << endl;
		tempColor = tree.insert((unsigned char)i);
		printColor(tempColor);
	}
	for(int i = 0; i< 50; i++) {
		cout << "<<<<<<<<<<<<<T>>>>>>>>>>>    " << i << endl;
		tempColor = tree.insert((unsigned char)i);
		printColor(tempColor);
	}
	system("pause");
	return 0;
}

void printColor(RangeTree::color *color) {
	//cout << "(" << color->r << "," << color->g << "," << color->b << ")" << endl;
}