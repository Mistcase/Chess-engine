#pragma once
#ifndef _COMMANDS_INCLUDED_
#define _COMMANDS_INCLUDED_

#include <map>
#include <string>
#include <functional>
#include <stdio.h>
#include <assert.h>

#include "types.h"
#include "ConstAttacks.h"
#include "ParsingFunctional.h"
#include "Coods.h"
#include "Moves.h"
#include "Engine.h"
#include "Parser.h"

const std::string STARTPOS_FEN_NOTATION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const int FEN_NOTATION_FIELDS_COUNT = 6;

namespace uci_execution
{
    void Clear(Engine*, std::string);

	void Uci(Engine*, std::string);
	void IsReady(Engine*, std::string);
	void Quit(Engine*, std::string);

	void SetOption(Engine* eng, std::string args);

	void SetPosition(Engine* eng, std::string args);
	void Go(Engine* eng, std::string args);
	void Stop(Engine* eng, std::string args);
}

namespace custom_commands
{
	void EvalPos(Engine*, std::string);
}

#endif
