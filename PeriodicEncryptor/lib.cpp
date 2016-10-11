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

const std::vector<int> & PointsGenerator::getPoints() {
	return this->_points;
}

//------------Encryptor-----------------
Encryptor::Encryptor(std::string m, int numberOfPoints)
{
	this->_message = m;
	this->_pg = new PointsGenerator(numberOfPoints, m.size());
	this->_keys = this->_generateKeys();
}

void Encryptor::_encdec(std::vector<unsigned char> & inputContainer, std::vector<unsigned char> & outputContainer) {
	outputContainer.clear();
	int length = inputContainer.size();
	std::vector<int> points = this->getPoints();
	int currInterval = 0;
	bool lastInterval = false;
	std::vector<unsigned char> currKey = this->_keys[currInterval];
	for (int i = 0; i < length; i++)
	{
		if (!lastInterval)
			if (i > points[currInterval]) {
				currKey = this->_keys[++currInterval];
				lastInterval = currInterval == points.size();
			};
		outputContainer.push_back(inputContainer[i] ^ currKey[i]);
	}
}

void Encryptor::performEncryption() {
	std::vector<unsigned char> messageVec(this->_message.begin(), this->_message.end());
	this->_encdec(messageVec, this->_cipherText);
}

std::string Encryptor::getDecryptedMessage() {	
	std::vector<unsigned char> decryptedVec;
	this->_encdec(this->_cipherText, decryptedVec);
	return std::string(decryptedVec.begin(), decryptedVec.end());
}

const std::vector<unsigned char> & Encryptor::getCipherText() {
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

const std::vector<std::vector<unsigned char>> & Encryptor::getKeys() {
	return this->_keys;
}

const std::vector<int>& Encryptor::getPoints() {
	return this->_pg->getPoints();
}

//----------------------Analyzer--------------------
