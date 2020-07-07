#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
class JsonNode {
private:
	int countKavichki;
	void removeEmptyElements(std::string& text, int& counter);
public:
	std::string key;
	std::vector<std::string> value;
	std::vector<JsonNode> nodes;
	bool isValid;

	JsonNode();
	JsonNode(const JsonNode& other);
	JsonNode(std::string key, std::string value);

	bool validate(std::string &text, int &counter);
	void GetValues(std::string& text, int& counter, JsonNode& json);
	void GetValueString(std::string& text, int& counter, JsonNode& json);
	void GetNumber(std::string& text, int& counter, JsonNode &json);
	void GetBoolean(std::string& text, int& counter, JsonNode &json);

	bool isNextSymbol(std::string& text, int& counter, char symbol);
	bool DoesKeyExist(std::string key);
	JsonNode& SearchKey(std::string jsonKey, JsonNode &json);


	bool CheckIfInt(std::string& text);
	void Print(int spaces);

	void ChangeValueAtKey(std::string& key, JsonNode& json);
};