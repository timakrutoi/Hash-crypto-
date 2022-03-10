#pragma once

#define DEBUG_MESSAGES false

#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include <iomanip>

using namespace std;

#define method 1

#define _A1	0x67452301
#define _B1	0xefcdab89
#define _C1	0x98badcfe
#define _D1	0x10325476

#define _A2	0x01234567
#define _B2	0x89abcdef
#define __C2	0xfedcba98
#define _D2	0x76543210

//             1               2	     	   3		 	   4
#define r1_k { 0,  4,  8, 12,  1,  5,  9, 13,  2,  6, 10, 14,  3,  7, 11, 15}
#define r2_k { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15}
#define r3_k { 0,  1,  2,  3,  8, 10,  9, 11,  4,  5,  6,  7, 12, 14, 13, 15}

//             1   2   3   4
#define r1_s { 3,  7, 11, 19}
#define r2_s { 3,  5,  9, 13}
#define r3_s { 3,  9, 11, 15}

void print_bs(vector<uint32_t> data) {
	if (!data.size())
		return;

	cout << "Total size = " << data.size() * 32 << " bits" << endl;
	for (size_t i = 0; i < data.size(); i++) {
		if (i + 1 < 10)
			cout << " ";
		cout << i + 1 << ") ";
		auto t = bitset<32>{ data[i] };
		for (size_t j = 0; j < 4; ++j) {
			for (size_t k = 0; k < 8; k++)
				cout << t[8 * (j) + (k)];
			cout << " ";
		}

		cout << endl;
	}
}

void print(vector<uint32_t> data) {
	if (!data.size())
		return;
	for (size_t i = 0; i < data.size(); i++) {
		cout << i << ") " << bitset<32>{ data[i] } << endl;
	}
}

uint32_t reverse(uint32_t n) {
	n = (n >> 1) & 0x55555555 | (n << 1) & 0xaaaaaaaa;
	n = (n >> 2) & 0x33333333 | (n << 2) & 0xcccccccc;
	n = (n >> 4) & 0x0f0f0f0f | (n << 4) & 0xf0f0f0f0;
	n = (n >> 8) & 0x00ff00ff | (n << 8) & 0xff00ff00;
	n = (n >> 16) & 0x0000ffff | (n << 16) & 0xffff0000;
	return n;
}

vector<uint32_t> s2int(string s) {
	vector<uint32_t> res;
	uint32_t tmp = 0;

	auto data = s.data();

	size_t len = s.length() - s.length() % 4;

	for (size_t i = 0; i < len; i += 4) {
		tmp = 0;

		for (size_t j = 0; j < 4; ++j) {
			tmp = tmp << 8;
			//tmp += data[4 * i + (j)];
			tmp += data[4 * i + (j)];
		}
		res.push_back(reverse(tmp));
		//res.push_back(tmp);
	}

	tmp = 0;
	for (size_t i = len; i < s.length(); ++i) {
		tmp = tmp << 8;
		//tmp += data[i];
		tmp += data[i];

		//cout << "idx " << len + s.length() - i - 1 << endl;
		//cout << "temp out " << bitset<32>{tmp} << endl;
	}
	//res.push_back(0);
	tmp = tmp << 1;
	tmp += 1;
	tmp = tmp << ((4 - (s.length() - len)) * 8 - 1);
	//tmp = tmp << 7;
	//cout << "temp out " << bitset<32>{tmp} << endl;
	//res.push_back(tmp);
	res.push_back(reverse(tmp));

	size_t add = 448 - (res.size() * 32) % 512;

	//cout << "size = " << res.size() * 32 << endl;
	//cout << "add size = " << add << endl;
	add = add / 32;
	//cout << "add size = " << add << " of 32 byte words" << endl;

	for (size_t i = 0; i < add; ++i) {
		res.push_back(0);
	}

	uint64_t total_size = (uint64_t)s.length() * 8;

	//res.push_back((uint32_t)(total_size >> 32));
	//res.push_back((uint32_t)total_size);

	res.push_back(reverse((uint32_t)(total_size >> 32)));
	res.push_back(reverse((uint32_t)total_size));

	print_bs(res);
	// 1 0 0 0
	// a b c d
	// dcba

	if (DEBUG_MESSAGES)
		cout << endl;

	return res;
}

uint32_t F(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) | (~x & z);
}

uint32_t G(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) | (x & z) | (y & z);
}

uint32_t H(uint32_t x, uint32_t y, uint32_t z) {
	return x ^ y ^ z;
}

uint32_t r1(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s) {
	return _rotl((a + F(b, c, d) + x), s);
	//return (a + F(b, c, d) + x) << s;
}

uint32_t r2(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s) {
	auto tmp = 0x5A827999;
	return _rotl((a + G(b, c, d) + x + tmp), s);
	//return (a + G(b, c, d) + x + tmp) << s;
}

uint32_t r3(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s) {
	auto tmp = 0x6ED9EBA1;
	return _rotl((a + H(b, c, d) + x + tmp), s);
	//return (a + H(b, c, d) + x + tmp) << s;
}

vector<uint32_t> md4(string s) {
	auto data = s2int(s);

	size_t blocks_count = data.size() / 16;

	vector<uint32_t> blocks;
	uint32_t A = _A1;
	uint32_t B = _B1;
	uint32_t C = _C1;
	uint32_t D = _D1;

	uint8_t r1k[] = r1_k;
	uint8_t r2k[] = r2_k;
	uint8_t r3k[] = r3_k;

	uint8_t r1s[] = r1_s;
	uint8_t r2s[] = r2_s;
	uint8_t r3s[] = r3_s;

	for (size_t i = 0; i < blocks_count; ++i) {
		blocks.clear();
		for (size_t j = 0; j < 16; ++j) {
			blocks.push_back(data[i * 16 + j]);
		}

		uint32_t AA = A;
		uint32_t BB = B;
		uint32_t CC = C;
		uint32_t DD = D;

		if (method == 0) {
			// Round 1
			A = r1(A, B, C, D, blocks[r1k[0]], r1s[0]);
			A = r1(A, B, C, D, blocks[r1k[1]], r1s[0]);
			A = r1(A, B, C, D, blocks[r1k[2]], r1s[0]);
			A = r1(A, B, C, D, blocks[r1k[3]], r1s[0]);

			D = r1(D, A, B, C, blocks[r1k[4]], r1s[1]);
			D = r1(D, A, B, C, blocks[r1k[5]], r1s[1]);
			D = r1(D, A, B, C, blocks[r1k[6]], r1s[1]);
			D = r1(D, A, B, C, blocks[r1k[7]], r1s[1]);

			C = r1(C, D, A, B, blocks[r1k[8]], r1s[2]);
			C = r1(C, D, A, B, blocks[r1k[9]], r1s[2]);
			C = r1(C, D, A, B, blocks[r1k[10]], r1s[2]);
			C = r1(C, D, A, B, blocks[r1k[11]], r1s[2]);

			B = r1(B, C, D, A, blocks[r1k[12]], r1s[3]);
			B = r1(B, C, D, A, blocks[r1k[13]], r1s[3]);
			B = r1(B, C, D, A, blocks[r1k[14]], r1s[3]);
			B = r1(B, C, D, A, blocks[r1k[15]], r1s[3]);

			// Round 2
			A = r2(A, B, C, D, blocks[r2k[0]], r2s[0]);
			A = r2(A, B, C, D, blocks[r2k[1]], r2s[0]);
			A = r2(A, B, C, D, blocks[r2k[2]], r2s[0]);
			A = r2(A, B, C, D, blocks[r2k[3]], r2s[0]);

			D = r2(D, A, B, C, blocks[r2k[4]], r2s[1]);
			D = r2(D, A, B, C, blocks[r2k[5]], r2s[1]);
			D = r2(D, A, B, C, blocks[r2k[6]], r2s[1]);
			D = r2(D, A, B, C, blocks[r2k[7]], r2s[1]);

			C = r2(C, D, A, B, blocks[r2k[8]], r2s[2]);
			C = r2(C, D, A, B, blocks[r2k[9]], r2s[2]);
			C = r2(C, D, A, B, blocks[r2k[10]], r2s[2]);
			C = r2(C, D, A, B, blocks[r2k[11]], r2s[2]);

			B = r2(B, C, D, A, blocks[r2k[12]], r2s[3]);
			B = r2(B, C, D, A, blocks[r2k[13]], r2s[3]);
			B = r2(B, C, D, A, blocks[r2k[14]], r2s[3]);
			B = r2(B, C, D, A, blocks[r2k[15]], r2s[3]);

			// Round 3
			A = r3(A, B, C, D, blocks[r3k[0]], r3s[0]);
			A = r3(A, B, C, D, blocks[r3k[1]], r3s[0]);
			A = r3(A, B, C, D, blocks[r3k[2]], r3s[0]);
			A = r3(A, B, C, D, blocks[r3k[3]], r3s[0]);

			D = r3(D, A, B, C, blocks[r3k[4]], r3s[1]);
			D = r3(D, A, B, C, blocks[r3k[5]], r3s[1]);
			D = r3(D, A, B, C, blocks[r3k[6]], r3s[1]);
			D = r3(D, A, B, C, blocks[r3k[7]], r3s[1]);

			C = r3(C, D, A, B, blocks[r3k[8]], r3s[2]);
			C = r3(C, D, A, B, blocks[r3k[9]], r3s[2]);
			C = r3(C, D, A, B, blocks[r3k[10]], r3s[2]);
			C = r3(C, D, A, B, blocks[r3k[11]], r3s[2]);

			B = r3(B, C, D, A, blocks[r3k[12]], r3s[3]);
			B = r3(B, C, D, A, blocks[r3k[13]], r3s[3]);
			B = r3(B, C, D, A, blocks[r3k[14]], r3s[3]);
			B = r3(B, C, D, A, blocks[r3k[15]], r3s[3]);
		}
		if (method == 1) {
			// Round 1
			A = r1(A, B, C, D, blocks[r1k[0]], r1s[0]);
			D = r1(D, A, B, C, blocks[r1k[4]], r1s[1]);
			C = r1(C, D, A, B, blocks[r1k[8]], r1s[2]);
			B = r1(B, C, D, A, blocks[r1k[12]], r1s[3]);

			A = r1(A, B, C, D, blocks[r1k[1]], r1s[0]);
			D = r1(D, A, B, C, blocks[r1k[5]], r1s[1]);
			C = r1(C, D, A, B, blocks[r1k[9]], r1s[2]);
			B = r1(B, C, D, A, blocks[r1k[13]], r1s[3]);

			A = r1(A, B, C, D, blocks[r1k[2]], r1s[0]);
			D = r1(D, A, B, C, blocks[r1k[6]], r1s[1]);
			C = r1(C, D, A, B, blocks[r1k[10]], r1s[2]);
			B = r1(B, C, D, A, blocks[r1k[14]], r1s[3]);

			A = r1(A, B, C, D, blocks[r1k[3]], r1s[0]);
			D = r1(D, A, B, C, blocks[r1k[7]], r1s[1]);
			C = r1(C, D, A, B, blocks[r1k[11]], r1s[2]);
			B = r1(B, C, D, A, blocks[r1k[15]], r1s[3]);

			// Round 2
			A = r2(A, B, C, D, blocks[r2k[0]], r2s[0]);
			D = r2(D, A, B, C, blocks[r2k[4]], r2s[1]);
			C = r2(C, D, A, B, blocks[r2k[8]], r2s[2]);
			B = r2(B, C, D, A, blocks[r2k[12]], r2s[3]);

			A = r2(A, B, C, D, blocks[r2k[1]], r2s[0]);
			D = r2(D, A, B, C, blocks[r2k[5]], r2s[1]);
			C = r2(C, D, A, B, blocks[r2k[9]], r2s[2]);
			B = r2(B, C, D, A, blocks[r2k[13]], r2s[3]);

			A = r2(A, B, C, D, blocks[r2k[2]], r2s[0]);
			D = r2(D, A, B, C, blocks[r2k[6]], r2s[1]);
			C = r2(C, D, A, B, blocks[r2k[10]], r2s[2]);
			B = r2(B, C, D, A, blocks[r2k[14]], r2s[3]);

			A = r2(A, B, C, D, blocks[r2k[3]], r2s[0]);
			D = r2(D, A, B, C, blocks[r2k[7]], r2s[1]);
			C = r2(C, D, A, B, blocks[r2k[11]], r2s[2]);
			B = r2(B, C, D, A, blocks[r2k[15]], r2s[3]);

			// Round 3
			A = r3(A, B, C, D, blocks[r3k[0]], r3s[0]);
			D = r3(D, A, B, C, blocks[r3k[4]], r3s[1]);
			C = r3(C, D, A, B, blocks[r3k[8]], r3s[2]);
			B = r3(B, C, D, A, blocks[r3k[12]], r3s[3]);

			A = r3(A, B, C, D, blocks[r3k[1]], r3s[0]);
			D = r3(D, A, B, C, blocks[r3k[5]], r3s[1]);
			C = r3(C, D, A, B, blocks[r3k[9]], r3s[2]);
			B = r3(B, C, D, A, blocks[r3k[13]], r3s[3]);

			A = r3(A, B, C, D, blocks[r3k[2]], r3s[0]);
			D = r3(D, A, B, C, blocks[r3k[6]], r3s[1]);
			C = r3(C, D, A, B, blocks[r3k[10]], r3s[2]);
			B = r3(B, C, D, A, blocks[r3k[14]], r3s[3]);

			A = r3(A, B, C, D, blocks[r3k[3]], r3s[0]);
			D = r3(D, A, B, C, blocks[r3k[7]], r3s[1]);
			C = r3(C, D, A, B, blocks[r3k[11]], r3s[2]);
			B = r3(B, C, D, A, blocks[r3k[15]], r3s[3]);
		}

		A = A + AA;
		B = B + BB;
		C = C + CC;
		D = D + DD;
	}

	vector<uint32_t> res;
	res.push_back(reverse(A));
	res.push_back(reverse(B));
	res.push_back(reverse(C));
	res.push_back(reverse(D));

	return res;
}

/*
		// Round 1
		A = r1(A, B, C, D, blocks[r1k[0]], r1s[0]);
		D = r1(D, A, B, C, blocks[r1k[4]], r1s[1]);
		C = r1(C, D, A, B, blocks[r1k[8]], r1s[2]);
		B = r1(B, C, D, A, blocks[r1k[12]], r1s[3]);

		A = r1(A, B, C, D, blocks[r1k[1]], r1s[0]);
		D = r1(D, A, B, C, blocks[r1k[5]], r1s[1]);
		C = r1(C, D, A, B, blocks[r1k[9]], r1s[2]);
		B = r1(B, C, D, A, blocks[r1k[13]], r1s[3]);

		A = r1(A, B, C, D, blocks[r1k[2]], r1s[0]);
		D = r1(D, A, B, C, blocks[r1k[6]], r1s[1]);
		C = r1(C, D, A, B, blocks[r1k[10]], r1s[2]);
		B = r1(B, C, D, A, blocks[r1k[14]], r1s[3]);

		A = r1(A, B, C, D, blocks[r1k[3]], r1s[0]);
		D = r1(D, A, B, C, blocks[r1k[7]], r1s[1]);
		C = r1(C, D, A, B, blocks[r1k[11]], r1s[2]);
		B = r1(B, C, D, A, blocks[r1k[15]], r1s[3]);

		// Round 2
		A = r2(A, B, C, D, blocks[r2k[0]], r2s[0]);
		D = r2(D, A, B, C, blocks[r2k[4]], r2s[1]);
		C = r2(C, D, A, B, blocks[r2k[8]], r2s[2]);
		B = r2(B, C, D, A, blocks[r2k[12]], r2s[3]);

		A = r2(A, B, C, D, blocks[r2k[1]], r2s[0]);
		D = r2(D, A, B, C, blocks[r2k[5]], r2s[1]);
		C = r2(C, D, A, B, blocks[r2k[9]], r2s[2]);
		B = r2(B, C, D, A, blocks[r2k[13]], r2s[3]);

		A = r2(A, B, C, D, blocks[r2k[2]], r2s[0]);
		D = r2(D, A, B, C, blocks[r2k[6]], r2s[1]);
		C = r2(C, D, A, B, blocks[r2k[10]], r2s[2]);
		B = r2(B, C, D, A, blocks[r2k[14]], r2s[3]);

		A = r2(A, B, C, D, blocks[r2k[3]], r2s[0]);
		D = r2(D, A, B, C, blocks[r2k[7]], r2s[1]);
		C = r2(C, D, A, B, blocks[r2k[11]], r2s[2]);
		B = r2(B, C, D, A, blocks[r2k[15]], r2s[3]);

		// Round 3
		A = r3(A, B, C, D, blocks[r3k[0]], r3s[0]);
		D = r3(D, A, B, C, blocks[r3k[4]], r3s[1]);
		C = r3(C, D, A, B, blocks[r3k[8]], r3s[2]);
		B = r3(B, C, D, A, blocks[r3k[12]], r3s[3]);

		A = r3(A, B, C, D, blocks[r3k[1]], r3s[0]);
		D = r3(D, A, B, C, blocks[r3k[5]], r3s[1]);
		C = r3(C, D, A, B, blocks[r3k[9]], r3s[2]);
		B = r3(B, C, D, A, blocks[r3k[13]], r3s[3]);

		A = r3(A, B, C, D, blocks[r3k[2]], r3s[0]);
		D = r3(D, A, B, C, blocks[r3k[6]], r3s[1]);
		C = r3(C, D, A, B, blocks[r3k[10]], r3s[2]);
		B = r3(B, C, D, A, blocks[r3k[14]], r3s[3]);

		A = r3(A, B, C, D, blocks[r3k[3]], r3s[0]);
		D = r3(D, A, B, C, blocks[r3k[7]], r3s[1]);
		C = r3(C, D, A, B, blocks[r3k[11]], r3s[2]);
		B = r3(B, C, D, A, blocks[r3k[15]], r3s[3]);
*/

/*
		// Round 1
		A = r1(A, B, C, D, blocks[r1k[0]], r1s[0]);
		A = r1(A, B, C, D, blocks[r1k[1]], r1s[0]);
		A = r1(A, B, C, D, blocks[r1k[2]], r1s[0]);
		A = r1(A, B, C, D, blocks[r1k[3]], r1s[0]);

		D = r1(D, A, B, C, blocks[r1k[4]], r1s[1]);
		D = r1(D, A, B, C, blocks[r1k[5]], r1s[1]);
		D = r1(D, A, B, C, blocks[r1k[6]], r1s[1]);
		D = r1(D, A, B, C, blocks[r1k[7]], r1s[1]);

		C = r1(C, D, A, B, blocks[r1k[8]], r1s[2]);
		C = r1(C, D, A, B, blocks[r1k[9]], r1s[2]);
		C = r1(C, D, A, B, blocks[r1k[10]], r1s[2]);
		C = r1(C, D, A, B, blocks[r1k[11]], r1s[2]);

		B = r1(B, C, D, A, blocks[r1k[12]], r1s[3]);
		B = r1(B, C, D, A, blocks[r1k[13]], r1s[3]);
		B = r1(B, C, D, A, blocks[r1k[14]], r1s[3]);
		B = r1(B, C, D, A, blocks[r1k[15]], r1s[3]);

		// Round 2
		A = r2(A, B, C, D, blocks[r2k[0]], r2s[0]);
		A = r2(A, B, C, D, blocks[r2k[1]], r2s[0]);
		A = r2(A, B, C, D, blocks[r2k[2]], r2s[0]);
		A = r2(A, B, C, D, blocks[r2k[3]], r2s[0]);

		D = r2(D, A, B, C, blocks[r2k[4]], r2s[1]);
		D = r2(D, A, B, C, blocks[r2k[5]], r2s[1]);
		D = r2(D, A, B, C, blocks[r2k[6]], r2s[1]);
		D = r2(D, A, B, C, blocks[r2k[7]], r2s[1]);

		C = r2(C, D, A, B, blocks[r2k[8]], r2s[2]);
		C = r2(C, D, A, B, blocks[r2k[9]], r2s[2]);
		C = r2(C, D, A, B, blocks[r2k[10]], r2s[2]);
		C = r2(C, D, A, B, blocks[r2k[11]], r2s[2]);

		B = r2(B, C, D, A, blocks[r2k[12]], r2s[3]);
		B = r2(B, C, D, A, blocks[r2k[13]], r2s[3]);
		B = r2(B, C, D, A, blocks[r2k[14]], r2s[3]);
		B = r2(B, C, D, A, blocks[r2k[15]], r2s[3]);

		// Round 3
		A = r3(A, B, C, D, blocks[r3k[0]], r3s[0]);
		A = r3(A, B, C, D, blocks[r3k[1]], r3s[0]);
		A = r3(A, B, C, D, blocks[r3k[2]], r3s[0]);
		A = r3(A, B, C, D, blocks[r3k[3]], r3s[0]);

		D = r3(D, A, B, C, blocks[r3k[4]], r3s[1]);
		D = r3(D, A, B, C, blocks[r3k[5]], r3s[1]);
		D = r3(D, A, B, C, blocks[r3k[6]], r3s[1]);
		D = r3(D, A, B, C, blocks[r3k[7]], r3s[1]);

		C = r3(C, D, A, B, blocks[r3k[8]], r3s[2]);
		C = r3(C, D, A, B, blocks[r3k[9]], r3s[2]);
		C = r3(C, D, A, B, blocks[r3k[10]], r3s[2]);
		C = r3(C, D, A, B, blocks[r3k[11]], r3s[2]);

		B = r3(B, C, D, A, blocks[r3k[12]], r3s[3]);
		B = r3(B, C, D, A, blocks[r3k[13]], r3s[3]);
		B = r3(B, C, D, A, blocks[r3k[14]], r3s[3]);
		B = r3(B, C, D, A, blocks[r3k[15]], r3s[3]);
*/
