#include "Commands.h"

void uci_execution::Uci(Engine*, std::string)
{
	printf("id name Andromeda_None\n\rid author I. Kindjzal\n\n");

	printf("option name Debug Log File type string default\n");
	printf("option name Threads type spin default 1 min 1 max 32\n");
	//printf("option name Hash type spin default 16 min 1 max 131072\n");
	//printf("option name Clear Hash type button\n");
	printf("option name Ponder type check default false\n");
	printf("option name MultiPV type spin default 1 min 1 max 50\n");
	printf("option name Skill Level type spin default 10 min 0 max 10\n");
	printf("option name Minimum Thinking Time type spin default 20 min 0 max 5000\n");

	printf("uciok\n");
}

void uci_execution::Clear(Engine*, std::string)
{
    //Only OS X
    system("clear");
}

void uci_execution::IsReady(Engine*, std::string)
{
	printf("readyok\n");
}

void uci_execution::Quit(Engine*, std::string)
{
	std::exit(0);
}

void uci_execution::SetOption(Engine* engine, std::string args)
{
	if (SubString(args, "name") != SUBSTRING_NOT_FOUND)
	{
		std::string optionName;
		if (!GetStringBetween(args, "name", "value", &optionName))
			return;

		ShrinkString(optionName);

		if (optionName == "debug log file")
		{
			std::string fileName;
			GetStringAfter(args, "value", &fileName);
			ShrinkString(fileName);
			engine->setOption(ENGINE_OPTION_DEBUG_FILE_NAME, &fileName);
		}
		else if (optionName == "ponder")
		{
			bool value;
			std::string result;
			GetNextWord(args, "value", &result);

			if (result == "true" || result == "1")
			{
				value = true;
			}
			else if (result == "false" || result == "0")
			{
				value = false;
			}
			else
			{
				printf("Error: uncorrect value");
			}

			engine->setOption(ENGINE_OPTION_PONDER, &value);
		}
		else if (optionName == "multipv" || optionName == "skill level" || optionName == "minimum thinking time" || optionName == "threads")
		{
			int intValue;
			std::string countValue;
			GetNextWord(args, "value", &countValue);
			intValue = atoi(countValue.c_str());

			if (optionName == "multipv")
				engine->setOption(ENGINE_OPTION_MULTIPV, &intValue);
			else if (optionName == "skill level")
				engine->setOption(ENGINE_OPTION_SKILL_LEVEL, &intValue);
			else if (optionName == "minimum thinking time")
				engine->setOption(ENGINE_OPTION_MINIMUM_THINKING_TIME, &intValue);
			else if (optionName == "threads")
				engine->setOption(ENGINE_OPTION_THREADS, &intValue);
		}
		else
		{
			printf("No such option\n");
		}

	}
	else
	{
		printf("No such option\n");
	}
}

void uci_execution::SetPosition(Engine * engine, std::string args)
{
	Parser parser(args);
	std::string word, fenNotation("");
	bool flagMoves = false, flagFen = false;
	unsigned int fenFieldsCounter = 0;

	while ((word = parser.getNextWord()) != std::string(""))
	{
		if (word == "startpos")
		{
			engine->setPosition(STARTPOS_FEN_NOTATION);
		}
		else if (word == "moves")
		{
			flagFen = false;
			flagMoves = true;
		}
		else if (word == "fen")
		{
			flagMoves = false;
			flagFen = true;
		}
		else
		{
			//Check correctness
			if (flagFen && flagMoves)
				abort();

			if (flagMoves)
			{
				engine->makeMove(word); //Get move and make it
			}
			if (flagFen)
			{
				fenFieldsCounter++;
				fenNotation += word;
				if (fenFieldsCounter == FEN_NOTATION_FIELDS_COUNT)
					engine->setPosition(fenNotation); //FenString
				else fenNotation += " ";
			}
		}
	}
	std::cout << "";
}

void uci_execution::Go(Engine * eng, std::string args)
{
	Parser parser(args);
	std::string mode = parser.getNextWord();

	if (mode == "depth")
	{
		Depth depth = std::atoi(parser.getNextWord().c_str());
		eng->startCalculations(depth);
	}
	else
		eng->startCalculations(6);
}

void uci_execution::Stop(Engine * eng, std::string args)
{
	eng->stopCalculations();
}

void custom_commands::EvalPos(Engine *engine, std::string)
{
	sync_cout << Evaluate(engine->getPosition(), EVALUATION_SIDE_WHITE) / 10 << sync_endl;
}
