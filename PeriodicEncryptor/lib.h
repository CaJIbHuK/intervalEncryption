#pragma once
#include <random>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <fstream>
#include <exception>

class PointsGenerator {
private:
		std::vector<int> _points;
		int _bound;
		int _count;
public:
	PointsGenerator(int count, int bound);
	~PointsGenerator() {};
	void generatePoints();
	const std::vector<int> & getPoints();
};

class Encryptor
{
private:
	std::string _message;
	std::vector<unsigned char> _cipherText;
	std::vector<std::vector<unsigned char>> _keys;
	PointsGenerator* _pg;
	std::vector<std::vector<unsigned char>> _generateKeys();
	std::vector<unsigned char> _generateKey();
	void _encdec(std::vector<unsigned char> & inputContainer, std::vector<unsigned char> & outputContainer);
public:
	Encryptor(std::string m, int numberOfPoints);
	~Encryptor() { delete this->_pg;};
	const std::vector<std::vector<unsigned char>> & getKeys();
	const std::vector<unsigned char> &  getCipherText();
	const std::vector<int> & getPoints();
	void performEncryption();
	std::string getDecryptedMessage();
};

