#pragma once
#include <vector>
#include <string>
#include "Text.h"
class Test
{
public:
	struct rob;
	struct rs
	{
		char opprand = ' ';
		string val1;
		string val2;
		rob* rob = nullptr;
		bool isEmpty = true;
	};

	struct rob
	{
		int REG = 0; //index in RF
		int val = 0; //value
		bool done = false;
	};
	struct rsbuffer
	{
		rs* which_rs;
		int cycle = 0;
	};
	using rf_type = vector<int>;
	using rat_type = vector<rob*>;
	using rs_type = vector<rs>;
	using rs_buffer = rsbuffer;
	using rob_type = vector<rob>;
	Test(const Text&);
	void setSize(int, int, int, int);// RF_size, RS_size, RS_size, ROB_size
	void setCycle(int, int, int, int); //add, sub, mul, div
	void setRF();
	void run();

	
private:
	bool step(); //do 1 cycle
	void printAll();
	void printRF();
	void printRAT();
	void printRS();
	void printROB();
	const Text *text;
	int index_text = 0;

	rf_type RF; //register file
	rat_type RAT; //
	rs_type RS_1;
	rs_type RS_2;
	vector<rs_buffer> RS_Buffer;

	rob_type ROB;
	int rob_commit;
	int rob_issue;
	int cycle = 0;
	bool issue();
	bool dispatch();
	bool execute();
	bool broadcast();
	bool writeResult();

	int getCycle(char);

	int addcycle;
	int subcycle;
	int mulcycle;
	int divcycle;

	int add(int, int);
	int sub(int, int);
	int mul(int, int);
	int div(int, int);
};

