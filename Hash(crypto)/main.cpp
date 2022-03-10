#include "md4.h"

int main() {
	string s = "The quick brown fox jumps over the lazy dog";
	string s1 = "";

	vector<uint32_t> correct;

	/*
	correct.push_back(0x1bee69a4);
	correct.push_back(0x6ba81118);
	correct.push_back(0x5c194762);
	correct.push_back(0xabaeae90);

	correct.push_back(0x31d6cfe0);
	correct.push_back(0xd16ae931);
	correct.push_back(0xb73c59d7);
	correct.push_back(0xe0c089c0);

	*/

	correct.push_back(0x31d6cfe0);
	correct.push_back(0xd16ae931);
	correct.push_back(0xb73c59d7);
	correct.push_back(0xe0c089c0);

	auto res = md4("");

	for (size_t i = 0; i < correct.size(); ++i) {
		cout << hex << correct[i] << " ";
	}
	cout << endl;
	for (size_t i = 0; i < res.size(); ++i) {
		cout << hex << res[i] << " ";
	}
}