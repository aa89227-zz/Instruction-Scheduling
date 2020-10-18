#include <iostream>
#include <fstream>
#include <string>
#include "text.h"
#include "Test.h"
using namespace std;

int main() {

	string str;
	ifstream inFile("input.txt", ios::in);

	Text text;
	while (getline(inFile, str)) {
		text.push(str);
	}
	char wantToSet;
	cout << "是否設置 RF,ROB,RS 大小? (Y/N) :";
	cin >> wantToSet;
	Test test(text);

	if (wantToSet == 'Y') {
		int RF_size;
		int RS1_size;
		int RS2_size;
		int ROB_size;
		cout << "RF size? ";
		cin >> RF_size;
		cout << "RS(ADD/SUB) size? ";
		cin >> RS1_size;
		cout << "RS(MUL/DIV) size? ";
		cin >> RS2_size;
		cout << "ROB size? ";
		cin >> ROB_size;
		test.setSize(RF_size, RS1_size, RS2_size, ROB_size);
	}
	cout << "是否設置 RF 內容? (Y/N) :";
	cin >> wantToSet;
	if (wantToSet == 'Y') {
		test.setRF();
	}
	cout << "是否設置 cycle(ALU)? (Y/N) :";
	cin >> wantToSet;
	if (wantToSet == 'Y') {
		int add, sub, mul, div;
		cout << "ADD:";
		cin >> add;
		cout << "SUB:";
		cin >> sub;
		cout << "MUL:";
		cin >> mul;
		cout << "DIV:";
		cin >> div;
		test.setCycle(add, sub, mul, div);
	}
	test.run();

}