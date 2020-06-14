#include <iostream>
#include <cstring>
#include <fstream>
#include "JsonNode.h"
int main()
{

	std::string command;
	while (command != "exit")
	{
		std::cin >> command;
		if (command == "help")
		{
			
		}
		else if (command == "open")
		{
			//Validating the json
			std::string path;
			std::cin >> path;
			std::ifstream in;
			in.open(path, std::ios::in);
			if (!in.is_open())
			{
				std::cout << "Invalid path! Try opening the file again."<<std::endl;
				continue;
			}

			std::string text { std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>() };

			try
			{

				JsonNode json; int counter = 0;
				json.validate(text, counter);
			}
			catch (const char* er)
			{
				std::cout << er << std::endl;
				continue;
			}
			std::cout << "Json valid!" << std::endl;
		}

		else
		{
			std::cout << "Invalid command write: help - for more information";
		}
	}
}