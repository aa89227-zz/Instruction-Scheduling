#include "Test.h"
#include <iostream>
#include <iomanip>
using namespace std;
Test::Test(const Text& text)
{
	rob_commit = 0;
	rob_issue = 0;
	addcycle = 1;
	subcycle = 1;
	mulcycle = 3;
	divcycle = 8;
	this->text = &text;
	RF.resize(6);
	RAT.resize(6);
	RS_1.resize(3);
	RS_2.resize(2);
	ROB.resize(8);
	RS_Buffer.resize(2);
}

void Test::setSize(int RF_size, int RS1_size, int RS2_size, int ROB_size)
{
	RF.resize(RF_size + 1);
	RAT.resize(RF_size + 1);
	RS_1.resize(RS1_size);
	RS_2.resize(RS2_size);
	ROB.resize(ROB_size);
	RS_Buffer.resize(2);
}
void Test::setCycle(int add_, int sub_, int mul_, int div_)
{
	addcycle = add_;
	subcycle = sub_;
	mulcycle = mul_;
	divcycle = div_;
}
void Test::setRF()
{
	int num;
	for (int i = 1; i < RF.size(); ++i) {
		cout << "RF" << i << ": ";
		cin >> num;
		RF[i] = num;
	}
}
void Test::run()
{
	while (step());
}
bool Test::step()
{
	bool change = false;
	bool buffer;
	buffer = writeResult();
	if (buffer) change = true;
	try {
		buffer = broadcast();

	}
	catch (bool a) {
		if (!a) {
			cout << "division by zero!" << endl;
			return false;
		}
	}
	if (buffer) change = true;
	buffer = execute();
	if (buffer) change = true;
	buffer = dispatch();
	if (buffer) change = true;
	buffer = issue();
	if (buffer) change = true;
	++cycle;
	if (change)
		printAll();
	if (index_text == text->size()) {
		for (int i = 0; i < ROB.size(); ++i) {
			if (ROB[i].REG != 0)
				return true;
		}
	}
	else return true;
	return false;
}
void Test::printAll()
{
	cout << "===================" << endl;
	cout << "cycle:" << cycle << endl;
	printRF();
	printRAT();
	printRS();
	printROB();
}
void Test::printRF()
{
	cout << "  --RF---" << endl;
	for (int i = 1; i < RF.size(); ++i) {
		cout << "F" << i << "|" << setw(5) << RF[i] << "|" << endl;
	}
	cout << "  -------" << endl << endl;
}
void Test::printRAT()
{
	cout << "  -RAT--" << endl;
	for (int i = 1; i < RAT.size(); ++i) {
		if (RAT[i] == nullptr)
			cout << "F" << i << "|" << "    " << "|" << endl;
		else
			cout << "F" << i << "|" << "ROB" << RAT[i] - &ROB[0] << "|" << endl;
	}
	cout << "  ------" << endl << endl;
}
void Test::printRS()
{
	cout << "   -RS-------------" << endl;
	for (int i = 0; i < RS_1.size(); ++i) {
		cout << "RS" << i + 1 << "|"
			<< setw(4) << RS_1[i].opprand << "|"
			<< setw(4) << RS_1[i].val1 << "|"
			<< setw(4) << RS_1[i].val2 << "|"<< endl;
	}
	cout << "   ----------------" << endl;
	if (RS_Buffer[0].which_rs == nullptr)
		cout << "BUFFER:" << "empty" << endl << endl;
	else {
		cout << "BUFFER:(RS" << RS_Buffer[0].which_rs - &RS_1[0] + 1 << ")"
			<< RS_Buffer[0].which_rs->val1
			<< RS_Buffer[0].which_rs->opprand
			<< RS_Buffer[0].which_rs->val2 << endl << endl;
	}
	//---- RS_2
	cout << "   -RS-------------" << endl;
	for (int i = 0; i < RS_2.size(); ++i) {
		cout << "RS" << RS_1.size() + i + 1 << "|"
			<< setw(4) << RS_2[i].opprand << "|"
			<< setw(4) << RS_2[i].val1 << "|"
			<< setw(4) << RS_2[i].val2 << "|" << endl;
	}

	cout << "   ----------------" << endl;
	if (RS_Buffer[1].which_rs == nullptr)
		cout << "BUFFER:" << "empty" << endl << endl;
	else {
		cout << "BUFFER:(RS" << RS_1.size() + RS_Buffer[1].which_rs - &RS_2[0] + 1 << ")"
			<< RS_Buffer[1].which_rs->val1
			<< RS_Buffer[1].which_rs->opprand
			<< RS_Buffer[1].which_rs->val2 << endl << endl;
	}

}
void Test::printROB()
{
	cout << "    -ROB-----------" << endl;
	for (int i = 0; i < ROB.size(); ++i) {
		if (ROB[i].REG != 0) {
			cout << "ROB" << i << "|"
				<< "F" << ROB[i].REG << "|"
				<< setw(4) << ROB[i].val << "|"
				<< setw(5) << ROB[i].done << "|"
				<< endl;
		}
		else {
			cout << "ROB" << i << "|"
				<< "  " << "|"
				<< setw(4) << " " << "|"
				<< setw(5) << " " << "|"
				<< endl;
		}
	}
	cout << "    ---------------" << endl;;
}
/*
1.如果有空位，放進RS
2.放進ROB
3.更新RAT(Rn)
*/
bool Test::issue()
{
	if (index_text == text->size()) return false;
	Text::inst_type tmp = text->get(index_text);
	int i = 0;
	rs_type* rs;
	if (tmp[0][0] == 'a' || tmp[0][0] == 's'
		|| tmp[0][0] == 'A' || tmp[0][0] == 'S') rs = &RS_1;
	else rs = &RS_2;

	for (; i < (*rs).size(); ++i)
		if ((*rs)[i].isEmpty) break;
	if (ROB[rob_issue].REG != 0) return false;
	if (i != (*rs).size()) {
		if (tmp[0][0] == 'a' || tmp[0][0] == 'A')
			(*rs)[i].opprand = '+';
		else if (tmp[0][0] == 's' || tmp[0][0] == 'S')
			(*rs)[i].opprand = '-';
		else if (tmp[0][0] == 'm' || tmp[0][0] == 'M')
			(*rs)[i].opprand = '*';
		else if (tmp[0][0] == 'd' || tmp[0][0] == 'D')
			(*rs)[i].opprand = '/';

		//val 1
		if (tmp[2][0] == 'F') { // reg
			int rat_index = tmp[2][1] - '0';
			if (RAT[rat_index] != nullptr) { //rob have something
				(*rs)[i].val1 = "ROB";
				char strbuf[10];
				_itoa_s((RAT[rat_index] - &ROB[0]), strbuf, 10);
				(*rs)[i].val1 += strbuf;
			}
			else {
				char buffer[10];
				_itoa_s(RF[rat_index], buffer, 10);
				(*rs)[i].val1 = buffer;
			}
		}
		//val 2
		if (tmp[3][0] == 'F') { // reg
			int rat_index = tmp[3][1] - '0';
			if (RAT[rat_index] != nullptr) { //rob have something
				char strbuf[10];
				(*rs)[i].val2 = "ROB";
				_itoa_s((RAT[rat_index] - &ROB[0]), strbuf, 10);
				(*rs)[i].val2 += strbuf;
			}
			else {
				char buffer[10];
				_itoa_s(RF[rat_index], buffer, 10);
				(*rs)[i].val2 = buffer;
			}
		}
		else {
			(*rs)[i].val2 = tmp[3];
		}
		(*rs)[i].isEmpty = false;
		(*rs)[i].rob = &ROB[rob_issue];
		ROB[rob_issue].REG = tmp[1][1] - '0';
		RAT[ROB[rob_issue].REG] = &ROB[rob_issue];
		++rob_issue;
		if (rob_issue == ROB.size()) rob_issue = 0;
		++index_text;
		return true;
	}
	return false;
	
}

bool Test::dispatch()
{
	bool change = false;
	bool isReady = true;
	if (RS_Buffer[0].which_rs == nullptr)
		for (int i = 0; i < RS_1.size(); ++i) {
			if (!RS_1[i].isEmpty)
				if (RS_1[i].val1[0] != 'R' && RS_1[i].val2[0] != 'R')
				{
					RS_Buffer[0].which_rs = &RS_1[i];
					RS_Buffer[0].cycle = 0;
					change = true;
					break;
				}
		}
	if (RS_Buffer[1].which_rs == nullptr)
		for (int i = 0; i < RS_2.size(); ++i) {
			if (!RS_2[i].isEmpty)
				if (RS_2[i].val1[0] != 'R' && RS_2[i].val2[0] != 'R')
				{
					RS_Buffer[1].which_rs = &RS_2[i];
					RS_Buffer[1].cycle = 0;
					change = true;
					break;
				}
		}
	return change;
}

bool Test::execute()
{
	if (RS_Buffer[0].which_rs != nullptr) {
		++RS_Buffer[0].cycle;
	}
	if (RS_Buffer[1].which_rs != nullptr) {
		++RS_Buffer[1].cycle;
	}
	return false;
}

bool Test::broadcast()
{
	bool change = false;
	if (RS_Buffer[0].which_rs != nullptr) {
		if (RS_Buffer[0].cycle == getCycle(RS_Buffer[0].which_rs->opprand)) {
			
			if (RS_Buffer[0].which_rs->opprand == '+')
				RS_Buffer[0].which_rs->rob->val = add(atoi(RS_Buffer[0].which_rs->val1.c_str()),
					atoi(RS_Buffer[0].which_rs->val2.c_str()));
			if (RS_Buffer[0].which_rs->opprand == '-')
				RS_Buffer[0].which_rs->rob->val = sub(atoi(RS_Buffer[0].which_rs->val1.c_str()),
					atoi(RS_Buffer[0].which_rs->val2.c_str()));
			
			RS_Buffer[0].which_rs->isEmpty = true;
			RS_Buffer[0].which_rs->val1 = ' ';
			RS_Buffer[0].which_rs->val2 = ' ';
			RS_Buffer[0].which_rs->opprand = ' ';
			RS_Buffer[0].which_rs->rob->done = true;
			RS_Buffer[0].which_rs = nullptr;
			change = true;
		}
	}
	if (RS_Buffer[1].which_rs != nullptr) {
		if (RS_Buffer[1].cycle == getCycle(RS_Buffer[1].which_rs->opprand)) {
			if (RS_Buffer[1].which_rs->opprand == '*')
				RS_Buffer[1].which_rs->rob->val = mul(atoi(RS_Buffer[1].which_rs->val1.c_str()),
					atoi(RS_Buffer[1].which_rs->val2.c_str()));
			if (RS_Buffer[1].which_rs->opprand == '/') {
				if (atoi(RS_Buffer[1].which_rs->val2.c_str()) == 0)
					throw false;
				RS_Buffer[1].which_rs->rob->val = div(atoi(RS_Buffer[1].which_rs->val1.c_str()),
					atoi(RS_Buffer[1].which_rs->val2.c_str()));
			}
			RS_Buffer[1].which_rs->isEmpty = true;
			RS_Buffer[1].which_rs->val1 = ' ';
			RS_Buffer[1].which_rs->val2 = ' ';
			RS_Buffer[1].which_rs->opprand = ' ';
			RS_Buffer[1].which_rs->rob->done = true;
			RS_Buffer[1].which_rs = nullptr;
			change = true;
		}
	}
	return change;
}

bool Test::writeResult()
{
	bool change = false;
	string current_rob = "ROB";
	char strbuf[10];
	for (; ROB[rob_commit].done;) {
		current_rob = "ROB";
		_itoa_s(rob_commit, strbuf, 10);
		current_rob += strbuf;
		_itoa_s(ROB[rob_commit].val, strbuf, 10);
		for (int i = 0; i < RS_1.size(); ++i) {
			if (RS_1[i].val1 == current_rob) RS_1[i].val1 = strbuf;
			if (RS_1[i].val2 == current_rob) RS_1[i].val2 = strbuf;
		}
		for (int i = 0; i < RS_2.size(); ++i) {
			if (RS_2[i].val1 == current_rob) RS_2[i].val1 = strbuf;
			if (RS_2[i].val2 == current_rob) RS_2[i].val2 = strbuf;
		}
		RF[ROB[rob_commit].REG] = ROB[rob_commit].val;
		change = true;
		if (RAT[ROB[rob_commit].REG] == &ROB[rob_commit]) RAT[ROB[rob_commit].REG] = nullptr;
		ROB[rob_commit].REG = 0;
		ROB[rob_commit].done = false;
		++rob_commit;
		if (rob_commit == ROB.size()) rob_commit = 0;
	}
	return change;
}

int Test::getCycle(char c)
{
	if (c == '+')
		return addcycle;
	else if (c == '-')
		return subcycle;
	else if (c == '*')
		return mulcycle;
	else if (c == '/')
		return divcycle;
	return 0;
}

int Test::add(int num1, int num2)
{
	return num1 + num2;
}

int Test::sub(int num1, int num2)
{
	return num1 - num2;
}

int Test::mul(int num1, int num2)
{
	return num1 * num2;
}

int Test::div(int num1, int num2)
{
	return num1 / num2;
}
