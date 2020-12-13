#pragma once
#ifndef _PARSING_FUNCTIONAL_INCLUDED_
#define _PARSING_FUNCTIONAL_INCLUDED_

#define SUBSTRING_NOT_FOUND -1

#include <string>
#include <algorithm>

int SubString(std::string str, std::string subString);

bool GetStringBefore(std::string str, std::string keyword, std::string* result);

bool GetStringAfter(std::string str, std::string keyword, std::string * result);

bool GetStringBetween(std::string str, std::string keywordBefore, std::string keywordAfter, std::string * result);

bool GetNextWord(std::string str, std::string keyword, std::string * result);

void ShrinkString(std::string& str);

bool SplitOffOneWord(std::string inputString, std::string * word, std::string * otherString);

#endif
