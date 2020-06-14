#include "JsonNode.h"
#include <string>
JsonNode::JsonNode() {
}

JsonNode::JsonNode(const JsonNode& other) {
	this->key = other.key;
	this->value = other.value;
	this->nodes = other.nodes;
	
}

JsonNode::JsonNode(std::string key, std::string value) {
	this->key=key;
	this->value.push_back(value);
}

bool JsonNode::validate(std::string &text, int &counter)
{
	if (text.length() == counter)
	{
		throw "Invalid JSON. Missing brackets";
	}

	if (text[counter] != '{')
	{
		throw "Invalid JSON. First character must be opening bracker -{-";
	}
	if (this->isNextSymbol(text, counter, '}'))
	{
		return true;
	}
	counter++;
	while (true)
	{
		JsonNode json;
		if (!this->isNextSymbol(text, counter, '\"'))
		{
			throw "Invalid JSON. Every key must start with \"(string)";
		}
		counter++;
		int countKeyBrackers = 1;
		int countBrackets = 1;
		std::string currValue = "\"";
		while (countBrackets != 2)
		{
			if (text[counter] == '\n')
			{
				throw "Invalid JSON. The key must end on the same row";
			}
			if (text[counter] == '\"')
			{
				countBrackets++;
			}
			currValue += text[counter];
			counter++;
		}
		json.key = currValue;
		if (!this->isNextSymbol(text,counter, ':'))
		{
			throw "Invalid JSON. Every key must have \":\"(declaration) sign between it and its value";
		}
		counter++;

		if (this->isNextSymbol(text, counter,'['))
		{
			this->GetValues(text, counter, json);
		}
		else if (this->isNextSymbol(text, counter, '\"'))
		{
			this->GetValueString(text, counter, json);
		}
		else if (text[counter] >= 48 && text[counter] <= 57)
		{
			this->GetNumber(text, counter, json);
		}
		else if (text.substr(counter, 5) == "false" || text.substr(counter,4) == "true")
		{
			this->GetBoolean(text, counter, json);
		}
		else if (this->isNextSymbol(text, counter, '{'))
		{
			json.validate(text,counter);
		}
		else
		{
			throw "Invalid value for the object";
		}
		this->nodes.push_back(json);
		if (this->isNextSymbol(text, counter, ','))
		{
			if (this->isNextSymbol(text, counter, '}'))
			{
				throw "Invalid coma before ending bracker";
			}
			else
			{
				counter++;
				continue;
			}
		}
		else if (this->isNextSymbol(text, counter, '}'))
		{
			counter++;
			break;
		}
		else
		{
			throw "Missing ending bracker";
		}
	}
}

void JsonNode::GetValues(std::string& text, int& counter, JsonNode &json)
{
	counter++;
	int countComas =0;
	while (!this->isNextSymbol(text, counter, ']'))
	{
		if (text[counter] == '\n')
		{
			throw "Array must end on the same row";
		}
	    if (this->isNextSymbol(text, counter, '\"'))
		{
			this->GetValueString(text, counter, json);
		}
		else if (text[counter] >= 48 && text[counter] <= 57)
		{
			this->GetNumber(text, counter, json);
		}
		else if (text.substr(counter, 5) == "false" || text.substr(counter, 4) == "true")
		{
			this->GetBoolean(text, counter, json);
		}
		else
		{
			throw "Invalid value for the object";
		}

		if (this->isNextSymbol(text,counter, ','))
		{
			if (this->isNextSymbol(text, counter , ']'))
			{
				throw "Invalid coma before the ending of the array";
			}
			countComas++;
			counter++;
		}
	}
	counter++;
}

void JsonNode::GetValueString(std::string& text, int& counter, JsonNode& json)
{
	counter++;
	int countBrackets = 1;
	std::string currValue = "\"";
	while (countBrackets != 2)
	{
		if (text[counter] == '\n')
		{
			throw "Invalid JSON. The key must end on the same row";
		}
		if (text[counter] == '\"')
		{
			countBrackets++;
		}
		currValue += text[counter];
		counter++;
	}

	json.value.push_back(currValue);
}

void JsonNode::GetNumber(std::string& text, int& counter, JsonNode& json)
{
	std::string currValue;
	while (text[counter] != ',')
	{
		if (this->isNextSymbol(text, counter, '}'))
		{
			break;
		}
		currValue += text[counter];
		counter++;
	}

	if (!CheckIfInt(currValue))
	{
		throw "Invalid JSON. Invalid value";
	}
	json.value.push_back(currValue);
}

void JsonNode::GetBoolean(std::string& text, int& counter, JsonNode& json)
{
	std::string currValue;
	if (text.substr(counter, 5) == "false")
	{
		json.value.push_back("fasle");
		counter += 5;
	}
	else
	{
		json.value.push_back("true");
		counter += 4;
	}
}

void JsonNode::removeEmptyElements(std::string& text, int& counter) {
	while (text[counter] == '\n' || text[counter] == '\t' || text[counter] == 32)
	{
		counter++;
	}
}

bool JsonNode::isNextSymbol(std::string& text, int& counter, char symbol)
{
	this->removeEmptyElements(text, counter);
	if (text[counter] != symbol)
	{
		return false;
	}
	return true;
}

bool JsonNode::CheckIfInt(std::string &input)
{
	for (int i = 0; i < input.length(); i++)
	{
		if (input[i] >= '0' && input[i] <= '9')
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void JsonNode::Print(int spaces)
{
	if (this->key == "")
	{
		for (JsonNode node : this->nodes)
		{
			node.Print(spaces);
		}
	}
	else
	{
		for (int i = 0; i < spaces; i++)
		{
			std::cout << ' ';
		}
		std::cout << "Key: " << this->key;
		if (this->value.size() == 0)
		{
			std::cout << '\n';
			spaces++;
			for (JsonNode node : this->nodes)
			{
				
				node.Print(spaces);
			}
		}
		spaces++;
		if (this->value.size() == 1)
		{
			std::cout << ", Value: " << this->value[0] << std::endl;
		}
		else if (this->value.size() > 1)
		{
			std::cout <<'\n';
			for (int i = 0; i < this->value.size(); i++)
			{
				std::cout << i << "." << this->value[i] << std::endl;
			}
		}
	}
}

void JsonNode::ChangeValueAtKey(std::string& key, JsonNode &json)
{

}

bool JsonNode::DoesKeyExist(std::string key, bool &doesExist)
{

	if (this->key != key)
	{
		if (this->nodes.size() > 0)
		{
			
			for (JsonNode node : this->nodes) {
				if (node.DoesKeyExist(key, doesExist))
				{
					return doesExist;
				}
			}
		}
	}
	else
	{
		doesExist = true;
		return doesExist;
	}

	return false;
}

JsonNode JsonNode::SearchKey(std::string jsonKey, JsonNode &json)
{
		if (this->key != jsonKey)
		{
			if (this->nodes.size() > 0)
			{
				for (int i = 0; i < this->nodes.size(); i++)
				{
					JsonNode temp;
					temp = this->nodes[i].SearchKey(jsonKey, json);
					if (temp.key == "")
					{
						continue;
					}
					json = temp;
				}
			}
			return json;
		}
		else
		{
			return *this;
		}
}
