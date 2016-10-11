#pragma once
#include <random>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <fstream>
#include <exception>
#include <tuple>

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

class Analyzer {
private:
	Encryptor* _encryptor;
	std::vector<unsigned char> _result;
	std::vector<std::tuple<int, int, std::vector<unsigned char>&>> _periodsOfOverlap;
	std::vector<std::vector<unsigned char>> _cipherTexts;
public:
	Analyzer(Encryptor& encryptor) { this->_encryptor = &encryptor; };
	~Analyzer() { delete this->_encryptor; };
	
};