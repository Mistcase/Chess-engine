#include "uci.h"

UciEngine::UciEngine(Engine * engine)
{
	this->ptrEngine = engine;

    this->commands.emplace("clear", uci_execution::Clear);
    
	this->commands.emplace("uci", uci_execution::Uci);
	this->commands.emplace("isready", uci_execution::IsReady);
	this->commands.emplace("setoption", uci_execution::SetOption);
	this->commands.emplace("position", uci_execution::SetPosition);

	this->commands.emplace("go", uci_execution::Go);
	this->commands.emplace("stop", uci_execution::Stop);

	this->commands.emplace("quit", uci_execution::Quit);




	this->commands.emplace("eval", custom_commands::EvalPos);
}

void UciEngine::start()
{
	std::string a, b;
	this->thread = std::thread([&]()
	{
		std::ofstream log;
		//log.open("log.txt");
		while (true)
		{
			std::string str, command, args;
			std::getline(std::cin, str);

			//log << str << std::endl;

			SplitOffOneWord(str, &command, &args);
			ShrinkString(command);
			if (command != "position")
				ShrinkString(str);
			try
			{
				this->commands.at(command)(this->ptrEngine, args);
			}
			catch (const std::exception&)
			{
				printf("Unknown command: %s\n", str.c_str());
			}
		}
	});

	this->thread.detach();
}
