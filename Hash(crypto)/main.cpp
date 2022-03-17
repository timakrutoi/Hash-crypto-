#include "md4.h"
#include "md5.h"

int main() {
	string s = "The quick brown fox jumps over the lazy dog";
	string s1 = "55555555555555555555555";

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

	auto res = md4(s);
	
	for (size_t i = 0; i < res.size(); ++i) {
		cout << hex << res[i] << " ";
		if (i == 3) cout << endl;
	}
}