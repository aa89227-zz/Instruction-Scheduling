#include "text.h"
#include <sstream>
void Text::push(string str)
{
	text.push_back(str);
	deal(str);
}

Text::inst_type Text::get(int index)
{
	return text_after_deal[index];
}

Text::inst_type Text::get(int index) const
{
	return text_after_deal[index];
}

const Text::text_type& Text::getText()
{
	return text;
}

size_t Text::size()
{
	return text.size();
}

size_t Text::size() const
{
	return text.size();
}

void Text::clear()
{
	text.clear();
	text_after_deal.clear();
}

void Text::deal(string& input)
{
	inst_type inst;
	string str;
	stringstream ss;
	str = input;
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == ',' || str[i] == '(' || str[i] == ')')
			str[i] = ' ';
	}
	ss << str;
	while (ss >> str) {
		inst.push_back(str);
	}
	text_after_deal.push_back(inst);
}
