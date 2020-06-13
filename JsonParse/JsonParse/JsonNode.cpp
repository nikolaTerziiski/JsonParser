#include "JsonNode.h"

JsonNode::JsonNode() {}

JsonNode::JsonNode(const JsonNode& other) {
	this->key = other.key;
	this->value = other.value;
	this->nodes = other.nodes;
}

JsonNode::JsonNode(std::string key, std::string value) {
	this->key=key;
	this->value=value;
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
		if (!this->isNextSymbol(text,counter, '\"'))
		{
			throw "Invalid JSON. Every key must start with \"(string)";
		}
		counter++;
		while (text[counter] != '\"')
		{
			if (this->isNextSymbol(text ,counter, '\n'))
			{
				throw "Invalid JSON. The key must end on the same row";
			}
			json.key += text[counter];
			counter++;
		}
		counter++;

		if (!this->isNextSymbol(text,counter, ':'))
		{
			throw "Invalid JSON. Every key must have \":\"(declaration) sign between it and its value";
		}
		counter++;

		if (this->isNextSymbol(text, counter, '\"'))
		{
			counter++;
			while (text[counter] != '\"')
			{
				if (text[counter] == '\n')
				{
					throw "Invalid JSON. The key must end on the same row";
				}
				json.value += text[counter];
				counter++;
			}
			counter++;
		}
		else if (text[counter] >= 48 && text[counter] <= 57)
		{
			while (text[counter] < 48 || text[counter] > 57)
			{
				if (this->isNextSymbol(text, counter, '\n'))
				{
					throw "Invalid JSON. The value is not valid";
				}
				json.value += text[counter];
				counter++;
			}
			if (json.value.empty())
			{
				throw "Invalid JSON. The value is not valid";
			}
			counter++;
		}
		this->nodes.push_back(json);
		if (this->isNextSymbol(text, counter, '{'))
		{
			json.validate(text, counter);
		}

		if (this->isNextSymbol(text, counter, ','))
		{
			counter++;
			if (this->isNextSymbol(text, counter, '}'))
			{
				throw "Invalid \",\" before ending bracket \"}\"";
			}
			else
			{
				continue;
			}
		}
		else
		{
			if (this->isNextSymbol(text,counter, '}'))
			{
				counter++;
				break;
			}
			else
			{
				throw "Invalid JSON. Missing brackets or comas";
			}
		}
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
