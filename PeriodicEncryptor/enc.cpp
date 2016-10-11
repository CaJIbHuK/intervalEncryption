#include "enc.h"
const int MESSAGE_LENGTH = 100;
const int NUMBER_OF_POINTS = 20;

void readFile(char *path, std::string & message) {
	std::ifstream is(path, std::ios::binary);
	if (!is) {
		throw  std::exception();
	}

	std::vector<char> buffer(MESSAGE_LENGTH);
	is.read(&buffer[0], MESSAGE_LENGTH);
	message.assign(buffer.begin(), buffer.end());
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Expected filename as argumnt" << std::endl;
		return 1;
	}

	std::string message;
	try
	{
		readFile(argv[1], message);
	}
	catch (const std::exception&)
	{
		std::cout << "NO such file" << std::endl;
	}

	Encryptor encryptor(message, NUMBER_OF_POINTS);
	encryptor.performEncryption();
	for (auto it = encryptor.getCipherText().begin(); it < encryptor.getCipherText().end(); it++)
	{
		std::cout << (*it);
	}
	std::string decryptedMessage = encryptor.getDecryptedMessage();
	for (auto it = decryptedMessage.begin(); it < decryptedMessage.end(); it++)
	{
		std::cout << (*it);
	}

	getchar();
}