#pragma once
#ifndef _UCI_INCLUED_
#define _UCI_INCLUED_

#include <thread>
#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <functional>
#include <fstream>

#include "Commands.h"
#include "ParsingFunctional.h"

#include "Engine.h"

class UciEngine
{
public:
	UciEngine(Engine* engine);
	void start();
private:
	Engine* ptrEngine;

	std::map<const std::string, std::function<void(Engine*, std::string)>> commands;
	std::thread thread;
};

#endif