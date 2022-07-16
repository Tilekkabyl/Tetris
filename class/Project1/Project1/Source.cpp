#include <iostream>
//#include "classes.h"

using namespace std;

int a;
int* ptr = &a;

int main() {
	
	cin >> *ptr;
	cout << &a;
	cin >> *ptr;
	cout << &a;

	/*FF pairs[100000];
	for (int i = 0; i < 10; i++) {
		pairs[i].a = i;
		pairs[i].b = i + 1;
	}
	for (int i = 0; i < 10; i++)
		cout << pairs[i].a << " " << pairs[i].b << endl;
		*/


}