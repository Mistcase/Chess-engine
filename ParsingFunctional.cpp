#include "ParsingFunctional.h"

int SubString(std::string str, std::string subString)
{
	for (unsigned int i = 0; i < str.size(); i++)
	{
		bool flag = true;

		for (unsigned int j = 0; j < subString.size(); j++)
		{
			if (i + j < str.size() && str[i + j] != subString[j])
			{
				flag = false;
				break;
			}
		}
		if (flag)
			return i;
	}
	return -1;
}

bool GetStringBefore(std::string str, std::string keyword, std::string * result)
{
	int index = SubString(str, keyword);
	
	if (index != -1)
	{
		for (int i = 0; i < index; i++)
			*result += str[i];
		return true;
	}
	return false;
}

bool GetStringAfter(std::string str, std::string keyword, std::string * result)
{
	int index = SubString(str, keyword);

	if (index != -1)
	{
		for (int i = index + keyword.size(); i < (int)str.size(); i++)
			*result += str[i];
		return true;
	}
	return false;
}

bool GetStringBetween(std::string str, std::string keywordBefore, std::string keywordAfter, std::string * result)
{
	int indexBefore = SubString(str, keywordBefore),
		indexAfter = SubString(str, keywordAfter);

	if (indexBefore != -1 && indexAfter != 1)
	{
		for (int i = indexBefore + keywordBefore.size(); i < indexAfter; i++)
			*result += str[i];
		return true;
	}

	return false;
}

bool GetNextWord(std::string str, std::string keyword, std::string * result)
{
	int index = SubString(str, keyword);

	if (index != -1)
	{
		unsigned int newIndex = index + keyword.size();
		bool flag = false;
		
		while (true)
		{
			if (str[newIndex] != ' ' && str[newIndex] != '	' && str[newIndex] != '\0')
			{
				*result += str[newIndex];
				flag = true;
			}
			if (flag && (str[newIndex] == ' ' || str[newIndex] == '	' || str[newIndex] == '\0'))
				break;
			newIndex++;
		}


		return !result->empty();
	}

	return false;
}

void ShrinkString(std::string & inputString)
{
	int first = 0, last = 0;
	for (unsigned int i = 0; i < inputString.size(); i++)
	{
		if (inputString[i] != ' ' && inputString[i] != '	')
			break;
		first++;
	}
	for (int i = (int)inputString.size() - 1; i >= 0; i--)
	{
		if (inputString[i] != ' ' && inputString[i] != '	')
			break;
		last++;
	}
	last = inputString.size() - (last + 1);

	if (last <= 0)
		return;

	bool flag = false;
	std::string outString = "";
	for (int i = first; i <= last; i++)
	{
		if (inputString[i] != ' ' && inputString[i] != '	')
		{
			outString += inputString[i];
			flag = false;
		}
		else
		{
			if (!flag)
				outString += inputString[i];
			flag = true;
		}
	}

	inputString = outString;
}

bool SplitOffOneWord(std::string inputString, std::string * word, std::string * otherString)
{
	if (inputString == "")
		return false;

	word->clear();
	otherString->clear();
	ShrinkString(inputString);

	size_t index = 0;
	while (inputString[index] != ' ' && inputString.size() > index)
		*word += inputString[index++];
	if (index < inputString.size())
	{
		do
		{
			*otherString += inputString[index];
		} while (++index != inputString.size());
	}
	return !word->empty();
}