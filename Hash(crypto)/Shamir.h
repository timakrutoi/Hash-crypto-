#pragma once

#include <vector>
#include <random>

using namespace std;

template <typename T>
vector<T> get_random_coefs(T k, T p) {
	vector<T> res;

	//res.push_back(7);
	//res.push_back(8);
	//return res;

	srand(123);

	for (auto i = 0; (k > i); ++i) {
		res.push_back(T(rand()) % p);
	}

	return res;
}

template <typename T>
T get_pol_value(vector<T> vec, T x, T p) {
	T res = 0;

	for (auto i = 0; i < vec.size(); ++i) {
		res += (exp_(x, T(i), p) * vec[vec.size() - 1 - i]) % p;
		// cout << "\t\t" << exp_(x, T(i), p) << " * " << vec[vec.size() - 1 - i] << " + ";
	}
	//cout << endl;

	return res % p;
}

template <typename T>
vector<tuple<T, T>> get_projections(T k, T n, T M, T p) {
	vector<T> pol = get_random_coefs(k-1, p);
	pol.push_back(M);

	/*
	cout << "POLY " << endl;
	for (auto i = 0; i < pol.size(); ++i) {
		cout << "\t " << pol[i];
	}
	cout << endl << endl;
	*/

	vector<tuple<T, T>> projections;

	for (auto i = 0; (n > i); ++i) {
		tuple<T, T> tmp = make_tuple(i+1, get_pol_value<T>(pol, i + 1, p));
		projections.push_back(tmp);
	}
	//cout << projections << endl;

	return projections;
}

template <typename T>
T get_mes(vector<tuple<T, T>> projections, T p) {
	T result = 0;
	T xxx = 0;

	for (int i = 0; i < projections.size(); i++) {
		T term = get<1>(projections[i]);
		for (auto j = 0; j < projections.size(); j++) {
			if (j != i) {
				T low, y;
				T eh = (get<0>(projections[i]) - get<0>(projections[j])) % p;
				if (eh < 0) eh += p;

				T high = xxx + get<0>(projections[j]);
				T gsa = gcdExtended(eh, p, &low, &y, p);

				term = (term * (high * low)) % p;
			}
		}
		result += term;
	}

	result %= p;

	return result;
}

template <typename T>
T gcdExtended__(T a, T b, T* x, T* y, T n) {
	// Base Case
	if (a == 0) {
		*x = 0;
		*y = 1;
		return b;
	}

	T x1, y1; // To store results of recursive call
	T gcd = gcdExtended(b % a, a, &x1, &y1, n);

	// Update x and y using results of
	// recursive call
	*x = (y1 - (b / a) * x1) % n;
	*y = x1 % n;

	if (*x < 0)
		*x += n;
	if (*y < 0)
		*y += n;

	// cout << dec << "X = " << *x << "; Y = " << *y << endl;

	return gcd;
}
