#include <iostream>
#include <cstring>
#include <fstream>
#include "JsonNode.h"

std::string openedFile;
void CalculatePaths(std::vector<std::string>& path) {
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
void ReplaceKeyValue(std::string& keyToSearch, JsonNode& jsonToReplace, JsonNode& json)
{
	if (json.key == keyToSearch)
	{
		json.nodes = jsonToReplace.nodes;
		json.value = std::vector<std::string>();
	}
	else
	{
		for (int i = 0; i < json.nodes.size(); i++)
		{
			ReplaceKeyValue(keyToSearch, jsonToReplace, json.nodes[i]);
		}
	}
}
void SaveInformation(JsonNode& json, std::string& pathToSave) {
	std::vector<std::string> path;
	CalculatePaths(path);

	if (path.size() != 0)
	{
		if (!DoesInputIsValid(path, json, path.size()))
		{
			std::cout << "Invalid path!" << std::endl;
			return;
		}
	}
	else
	{
		//TODO - just save the file in the given file
		std::ifstream in;
		in.open(pathToSave, std::ios::out);
		//Write the file down
	}
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
			CalculatePaths(path);

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

				ReplaceKeyValue(path[path.size() - 1], jsonToReplace, json);
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
			CalculatePaths(path);

			std::string newJsonString;
			std::cin >> newJsonString;

			//Proverka za putq do predposledniq element
			if (DoesInputIsValid(path,json, path.size() -1) && !DoesInputIsValid(path,json, path.size()))
			{
				/*try
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
				}*/
			}
			else
			{
				std::cout << "Invalid path to object!" << std::endl;
				std::cout << "The last path must not exists, so it can be created" << std::endl;
				continue;
			}
		}
		else if (command == "delete")
		{
			std::vector<std::string> path;
			CalculatePaths(path);

			std::string newJsonString;
			std::cin >> newJsonString;

			if (!DoesInputIsValid(path, json, path.size()))
			{
				std::cout << "Invalid path!" << std::endl;
				continue;
			}

		}
		else if (command == "move")
		{
			std::vector<std::string> fromPath;
			CalculatePaths(fromPath);

			if (!DoesInputIsValid(fromPath, json, fromPath.size()))
			{
				std::cout << "Invalid path!" << std::endl;
				continue;
			}

			std::vector<std::string> toPath;
			CalculatePaths(toPath);

			if (!DoesInputIsValid(toPath, json, toPath.size()))
			{
				std::cout << "Invalid path!" << std::endl;
				continue;
			}

			JsonNode jsonToMove;
			json.SearchKey(fromPath[fromPath.size() - 1], jsonToMove);


		}
		else if (command == "save" || command == "saveas")
		{
			if (command == "saveas")
			{
				std::string newDirectory;
				std::cin >> newDirectory;
				SaveInformation(json, newDirectory);
			}
			else
			{
				SaveInformation(json, openedFile);
			}
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
			std::cin >> openedFile;
			std::ifstream in;
			in.open(openedFile, std::ios::in);
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