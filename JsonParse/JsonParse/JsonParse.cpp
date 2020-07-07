#include <iostream>
#include <cstring>
#include <fstream>
#include "JsonNode.h"
void CalculatePaths(JsonNode& json, std::vector<std::string>& path) {
	   std::string input;
	   std::cin >> input;
	   
	   int counter = 0;
	   std::string localPath;
	   
	   //Taking every key. We can only search by keys
	   while (input.length() != counter)
	   {
	   	if (input[counter] == '/')
	   	{
	   		path.push_back(localPath);
	   		localPath = "";
	   		counter++;
	   	}
	   	else
	   	{
	   		localPath += input[counter];
	   		counter++;
	   	}
	   }
	   path.push_back(localPath);
}
bool DoesInputIsValid(std::vector<std::string>& paths, JsonNode json, int size) {
	for (int i = 0; i < size; i++)
	{
		JsonNode tempInside;
		if (!json.DoesKeyExist(paths[i]))
		{
			return false;
		}
		json.SearchKey(paths[i], tempInside);

		json = tempInside;
	}
	return true;
}
void Engine(JsonNode& json) {
	std::string command;
	do
	{
		std::cin >> command;
		if (command == "print")
		{
			int spaces = 0;
			json.Print(spaces);
		}
		else if (command == "search")
		{
			std::string key;
			std::cin >> key;
			JsonNode result = json;
			bool doesExist = false;
			if (json.DoesKeyExist(key))
			{
				json.SearchKey(key, result);

				//Returning values
				std::cout << "Printing values:" << std::endl;
				int spaces = 0;
				result.Print(0);
				continue;
			}
			std::cout << "The key does not exists!" << std::endl;
		}
		else if (command == "set")
		{
			std::vector<std::string> path;
			CalculatePaths(json, path);

			std::string newJsonString;
			std::cin >> newJsonString;

			if (!DoesInputIsValid(path,json, path.size()))
			{
				std::cout << "Invalid path!" << std::endl;
				continue;
			}

			try
			{
				int counter = 0;
				JsonNode jsonToReplace;
				jsonToReplace.validate(newJsonString, counter);
				jsonToReplace.isValid = true;

				json.ReplaceKeyValue(path[path.size() - 1], jsonToReplace);
			}
			catch (const char* er)
			{
				std::cout << "Invalid input! ";
				std::cout << er << std::endl;
			}
		}
		else if (command == "validate")
		{
			std::cout << (json.isValid ? "Yes" : "No") << std::endl;
		}
		else if (command == "create")
		{
			
			std::vector<std::string> path;
			CalculatePaths(json, path);

			std::string newJsonString;
			std::cin >> newJsonString;

			for (int i = 0; i < path.size(); i++)
			{
				//Proverka dali e posleden element. Ako e posleden i ima key => ima value => trqbva da grumne
				if (i < path.size() - 1)
				{
					if (!json.DoesKeyExist(path[i]))
					{
						std::cout << "Invalid path!" << std::endl;
						continue;
					}
				}
				else
				{
					if (json.DoesKeyExist(path[i]))
					{
						std::cout << "There is element on this path" << std::endl;
						continue;
					}
				}
			}
			try
			{
				int counter = 0;
				JsonNode jsonToCheck;
				jsonToCheck.validate(newJsonString, counter);
				jsonToCheck.isValid = true;
			}
			catch (const char* er)
			{
				std::cout << "Invalid input! ";
				std::cout << er << std::endl;
			}
		}
		else if (command == "delete")
		{
			std::vector<std::string> path;
			CalculatePaths(json, path);

			for (int i = 0; i < path.size(); i++)
			{
				bool doesExist = false;
				if (!json.DoesKeyExist(path[i]))
				{
					std::cout << "Invalid path!" << std::endl;
					continue;
				}
			}

			//Delete element...

		}
		else if (command == "move")
		{

		}
		else if (command == "save" || command == "saveas")
		{
			if (command == "saveas")
			{
				//Da pazim elementa po daden string
			}
			//Da pazim elementa po tekushtiq put
		}
		else
		{
			std::cout << "Invalid command";
		}
	} while (!(command == "close"));
}

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
				json.isValid = true;
				Engine(json);
			}
			catch (const char* er)
			{
				std::cout << er << std::endl;
				continue;
			}
		
		}

		else
		{
			std::cout << "Invalid command write: help - for more information";
		}
	}
}