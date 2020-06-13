#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
class JsonNode {
private:
	int countKavichki;
	void removeEmptyElements(std::string& text, int& counter);
	bool takingValueAndDotsCheck(std::string& text, int& counter);
public:
	std::string key;
	std::string value;
	std::vector<JsonNode> nodes;

	JsonNode();
	JsonNode(const JsonNode& other);
	JsonNode(std::string key, std::string value);

	bool validate(std::string &text, int &counter);

	bool isNextSymbol(std::string& text, int& counter, char symbol);

	bool takingKeyValue(std::string& text, int& counter);
};