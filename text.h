#pragma once
#include <string>
#include <vector>
using namespace std;
class Text
{
public:
	using text_type = vector<string>;
	using inst_type = vector<string>;
	void push(string); //push a instruction into 
	inst_type get(int); //get a instruction
	inst_type get(int) const; //get a instruction
	const text_type& getText(); //get text 
	size_t size(); //text size
	size_t size() const; //text size
	void clear();

private:
	void deal(string&);
	text_type text;
	vector<inst_type> text_after_deal;
};

