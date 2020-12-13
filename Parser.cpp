#include "Parser.h"

Parser::Parser(std::string str)
{
	this->string = str;
}

int Parser::subString(std::string subString)
{
	return static_cast<int>(this->string.find(subString));
}

void Parser::shrinkString()
{
	int first = 0, last = 0;
	for (unsigned int i = 0; i < this->string.size(); i++)
	{
		if (this->string[i] != ' ' && this->string[i] != '	')
			break;
		first++;
	}
	for (int i = (int)this->string.size() - 1; i >= 0; i--)
	{
		if (this->string[i] != ' ' && this->string[i] != '	')
			break;
		last++;
	}
	last = this->string.size() - (last + 1);

	if (last <= 0)
		return;

	bool flag = false;
	std::string outString = "";
	for (int i = first; i <= last; i++)
	{
		if (this->string[i] != ' ' && this->string[i] != '	')
		{
			outString += this->string[i];
			flag = false;
		}
		else
		{
			if (!flag)
				outString += this->string[i];
			flag = true;
		}
	}

	this->string = outString;
}

std::string Parser::getNextWord()
{
	unsigned int counter = 0;
	size_t symbolIndex = 0;
	bool flagNewWord = true;
	std::string str("");

	while (counter <= this->innerIndex && symbolIndex < this->string.size())
	{
		if (this->string[symbolIndex] != ' ' && this->string[symbolIndex] != '	' && flagNewWord)
		{
			counter++;
			flagNewWord = false;
		}
		else if (this->string[symbolIndex] == ' ' || this->string[symbolIndex] == '	')
			flagNewWord = true;

		if (counter == this->innerIndex && (this->string[symbolIndex] != ' ' && this->string[symbolIndex] != '	'))
			str += this->string[symbolIndex];

		symbolIndex++;
	}
	this->innerIndex++;

	return str;
}

void Parser::reset()
{
	this->innerIndex = 1;
}
