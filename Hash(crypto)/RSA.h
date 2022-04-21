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

//template <typename T>
//class frac {
//public:
//    T x, y;
//    frac(x_, y_) { this.x = x_; this.y = y_; }
//
//};

//template <typename T>
//vector<frac<T>> get_nice_frac(vector<T> in) {
//    vector<frac<T>> res;
//    T x = 0, y = 0;
//
//    for (size_t i = 0; i < in.size(); ++i) {
//        frac tmp();
//    }
//}

template <typename T>
vector<T> rational2contfrac(T x, T y) {
    T a;
    vector<T> res;
    while (y != 0) {
        T tmp = y;
        y = x % y;
        a = (x - y) / tmp;
        x = tmp;

        res.push_back(a);
    }
    return res;
}

template <typename T>
tuple<vector<T>, vector<T>> nice_frac(vector<T> contfrac) {
    vector<T> p, q;

    q.push_back(0);
    p.push_back(1);

    p.push_back(contfrac[0]);
    q.push_back(1);

    for (size_t i = 1; i < contfrac.size(); ++i) {
        p.push_back(contfrac[i] * p[i] + p[i - 1]);
        q.push_back(contfrac[i] * q[i] + q[i - 1]);
    }

    p.erase(p.begin());
    q.erase(q.begin());

    return tuple<vector<T>, vector<T>>(p, q);
}

template <typename T>
void Wiener_attack(T e, T n) {
    auto contfrac = rational2contfrac(e, n);

    //cout << "contfrac" << endl;
    //for (auto i = 0; i < contfrac.size(); ++i) {
    //    cout << contfrac[i] << ", ";
    //}
    //cout << endl;

    auto [p, q] = nice_frac(contfrac);

    //cout << "nice frac" << endl;
    //for (auto i = 0; i < p.size(); ++i) {
    //    cout << p[i] << " / " << q[i] << ", ";
    //}
    //cout << endl;

    for (size_t i = 1; i < p.size(); ++i) {
        cout << "stage N" << i << endl;
        T fn = (e * q[i] - 1) / p[i];
        // cout << "\t fn = " << fn << endl;
        // cout << "\t p[] = " << p[i] << endl;
        // cout << "\t q[] = " << q[i] << endl;
        auto [x1, x2] = quad_eq((T)1, (T)(-((n - fn) + 1)), n);
        T res = x1 * x2;
        // cout << "\t x1 = " << x1 << endl;
        // cout << "\t x2 = " << x2 << endl;
        // cout << "\t diff " << n - res << endl;
        if (res == n) {
            cout << "KEY FOUND!!" << endl << "\t secret exp = " << q[i] << endl;
            return;
        }
    }
}

template <typename T>
tuple<T, T> quad_eq(T a, T b, T c) {
    T d = b*b - a * c * 4;
    if (d > 0) {
        T x1 = 0, x2 = 0;
        // cout << "\t\td = " << d << endl;
        // cout << "\t\tsqrt(d) = " << d.intSqrt() << endl;
        x1 = (-b + d.intSqrt()) / (a * 2);
        x2 = (-b - d.intSqrt()) / (a * 2);

        return tuple(x1, x2);
    }
    return tuple(0, 0);
}

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
        return ((base)*t);
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

    // cout << dec << "X = " << *x << "; Y = " << *y << endl;

    return gcd;
}

template <typename T>
T gcdExtended_contfrac(T a, T b, T* x, T* y, T n) {
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
