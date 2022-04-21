#include "md4.h"
#include "RSA.h"
#include "sercantutar-infint-fc767ed/InfInt.h"
#include <random>
#include <algorithm>


std::string random_string(std::size_t length, uint16_t seed=1234) {
	const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,:;";

	std::random_device random_device;
	std::mt19937 generator(random_device());
	//generator.seed(seed);
	std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

	std::string random_string;

	for (std::size_t i = 0; i < length; ++i)
	{
		random_string += CHARACTERS[distribution(generator)];
	}

	return random_string;
}

int test1(string s, int len = 20, const int h_size = 8, int seed = 1234) {
	auto h0 = md4(s).substr(0, h_size);
	string hi = md4(random_string(len, seed)).substr(0, h_size);

	vector<string> y;

	int N = 1;
	int l = 100000;

	while (h0 != hi && N < l) {
		seed++;
		string msg = random_string(len, seed);
		hi = md4(msg).substr(0, h_size);
		auto place = find(y.begin(), y.end(), hi);
		/*
		if (place == y.end())
			y.push_back(hi);
		else {
			cout << "Collision found: h( " << place - y.begin() << " - " << N << " ) = " << hi << endl;
		}
		*/
		N++;
		//cout << hi << endl;
	}

	return N;
}

int test2(string s, int len = 20, const int h_size = 8, int seed = 1234) {
	auto h0 = md4(s).substr(0, h_size);
	string hi = md4(random_string(len, seed)).substr(0, h_size);

	vector<string> y;

	int N = 1;
	int l = 100000;

	while (h0 != hi && N < l) {
		seed++;
		string msg = random_string(len, seed);
		hi = md4(msg).substr(0, h_size);
		auto place = find(y.begin(), y.end(), hi);

		if (place == y.end())
			y.push_back(hi);
		else {
			//cout << "Collision found: h( " << place - y.begin() << " - " << N << " ) = " << hi << endl;
			return N - (place - y.begin());
		}
		
		N += y.size();
		//cout << hi << endl;
	}

	return N;
}

void test_crypt(string m, InfInt q, InfInt p, InfInt e=65537) {
	InfInt mes = m;

	RSA<InfInt> sys(q, p, e);

	InfInt n = sys.get_n(), d = sys.get_private_key();
	cout << "mes: \n\t" << mes << endl;

	InfInt crypted = crypt(mes, e, n);
	cout << "Crypted mes: \n\t" << crypted << endl;

	InfInt decrypted = crypt(crypted, d, n);
	cout << "Decrypted mes: \n\t" << decrypted << endl;
}

void test_signature(string m, InfInt q, InfInt p, InfInt e = 65537) {
	InfInt mes = m;

	RSA<InfInt> sys(q, p, e);

	InfInt n = sys.get_n(), d = sys.get_private_key();
	InfInt hashed = md4(m);
	cout << "mes: \n\t" << mes << endl;
	cout << "hash: \n\t" << hashed << endl;
		
	InfInt crypted = crypt(hashed, d, n);
	cout << "Signature: \n\t" << crypted << endl;

	InfInt decrypted = crypt(crypted, e, n);
	cout << "Decrypted mes: \n\t" << decrypted << endl;
}

void smth() {
	int x = 0, y = 0, n = 12345;

	// auto r = gcdExtended(1071, 462, &x, &y, n);
	auto r = rational2contfrac(1071, 462);
	tuple<vector<int>, vector<int>> d = nice_frac<int>(r);

	for (auto i = 0; i < r.size(); ++i) {
		cout << r[i] << " ";
	}
	cout << endl;

	auto& [p, q] = d;

	for (auto i = 0; i < p.size(); ++i) {
		cout << p[i] << " / " << q[i] << " ; ";
	}

}

void test_Wiener_attack() {
	InfInt q = "849787074319711721459362812091";
	InfInt p = "725486187367325002596395878223";
	InfInt e = "9924420827570144915815808938527800493563956194056715753173";

	RSA r(q, p, e);

	cout << "PRIVATE KEY : " << r.get_private_key() << endl;
	cout << "          N : " << r.get_n() << endl;


	//1779399043 2796304957

	Wiener_attack(e, r.get_n());
	//Wiener_attack((InfInt)1779399043, (InfInt)2796304957);
}

int main() {
	test_Wiener_attack();
	InfInt q = "849787074319711721459362812091";
	InfInt p = "725486187367325002596395878223";
	InfInt e = "9924420827570144915815808938527800493563956194056715753173";


	// test_crypt("14314145435", q, p);
	// test_signature("1234", q, p, e);

}