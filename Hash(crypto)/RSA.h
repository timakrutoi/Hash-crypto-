#pragma once

#include <iomanip>
#include "bigint-2010.04.30/BigInteger.hh"
// #include <bits/stdc++.h>

using namespace std;

template <typename T>
class RSA {
public:
    RSA(T q, T p, T e) {
        this->n = q * p;
        this->f = (q - 1) * (p - 1);
        this->e = e;
        T d=0, y=0;
        T sd = gcdExtended(this->e, this->f, &d, &y, this->n);
        this->d = d;
        // cout << "DDDD = " << d % n << endl;
    }

    T get_n() { return this->n; };
    T get_public_key() { return this->e; };
    T get_private_key() { return this->d; };

private:
    T q, p, f;  // private info
    T n, e;     // public key
    T d;        // private key
};

template <typename T>
T crypt(T mes, T e, T n) {
    return exp_(mes, e, n) % n;
}

template <typename T>
T exp_(T base, T exp, T n) {
    if (exp == 0)
        return 1;

    if (exp == 1)
        return base;

    T t = exp_(base, exp / 2, n) % n;
    t = (t * t);

    // if exponent is even value
    if (exp % 2 == 0)
        return t;

    // if exponent is odd value
    else
        return ((base) * t);
}

template <typename T>
T gcdExtended(T a, T b, T* x, T* y, T n) {
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

    return gcd;
}
