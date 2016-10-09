#include "lib.h"
//------------PointsGenerator------------
PointsGenerator::PointsGenerator(int count, int bound) {	
	this->_points.resize(count);
	this->_bound = bound;
	this->_count = count;
	this->generatePoints();
};

void PointsGenerator::generatePoints() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0, this->_bound-1);

	auto gen = std::bind(distribution, generator);
	auto begin = this->_points.begin();
	auto end = this->_points.end();

	for (auto it = begin; it < end; it++)
	{
		int value = gen();
		while (std::find(begin, end, value) != end)
		{
			value = gen();
		}
		(*it) = value;
	};

	std::sort(begin, end);	
}

std::vector<int> PointsGenerator::getPoints() {
	return this->_points;
}

//------------Encryptor-----------------
Encryptor::Encryptor(std::string m, int numberOfPoints)
{
	this->_message = m;
	this->_pg = new PointsGenerator(numberOfPoints, m.size());
	this->_keys = this->_generateKeys();
}

void Encryptor::performEncryption() {
	this->_cipherText.clear();
	int messageLength = this->_message.size();
	std::vector<int> points = this->getPoints();
	int currInterval = 0;
	std::vector<unsigned char> currKey = this->_keys[currInterval];
	for (int i = 0; i < messageLength; i++)
	{
		if (i > points[currInterval]) 
			currKey = this->_keys[++currInterval];
		this->_cipherText.push_back(this->_message[i] ^ currKey[i]);
	}
}

//just decrypton. for reasons of tests
std::string Encryptor::getDecryptedMessage() {	
	std::string decryptedMessage;
	int messageLength = this->_cipherText.size();
	std::vector<int> points = this->getPoints();
	int currInterval = 0;
	std::vector<unsigned char> currKey = this->_keys[currInterval];
	for (int i = 0; i < messageLength; i++)
	{
		if (i > points[currInterval])
			currKey = this->_keys[++currInterval];
		decryptedMessage.push_back(this->_cipherText[i] ^ currKey[i]);
	}
	return decryptedMessage;
}

std::vector<unsigned char> Encryptor::getCipherText() {
	return this->_cipherText;
}

std::vector<unsigned char> Encryptor::_generateKey() {
	std::vector<unsigned char> key(this->_message.size());
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen(seed);
	std::uniform_int_distribution<int> dist(0, 255);
	std::generate(key.begin(), key.end(), [&dist, &gen] { return dist(gen); });
	return key;
}

std::vector<std::vector<unsigned char>> Encryptor::_generateKeys() {
	int numberOfKeys = this->_pg->getPoints().size() + 1;
	std::vector<std::vector<unsigned char>> keys(numberOfKeys);
	std::generate(keys.begin(), keys.end(), [this] {return this->_generateKey();});
	return keys;
}

std::vector<std::vector<unsigned char>> Encryptor::getKeys() {
	return this->_keys;
}

std::vector<int> Encryptor::getPoints() {
	return this->_pg->getPoints();
}