#pragma once
#ifndef _PARSER_INCLUDED_
#define _PARSER_INCLUDED_

#include <string>

constexpr int SUBSTRING_OPERATION_FAILED = -1;

class Parser
{
public:
	Parser(std::string str);

	int subString(std::string subString);
	void shrinkString();
	std::string getNextWord();
	void reset();

private:
	std::string string;
	unsigned int innerIndex = 1;
};

#endif