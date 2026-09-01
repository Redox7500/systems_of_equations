#include <algorithm>
#include <iostream>
#include <optional>
#include <string>

unsigned long long ll_abs(long long x)
{
    return (x < 0)? -static_cast<unsigned long long>(x) : x;
}

unsigned long long gcd(long long _a, long long _b)
{
    unsigned long long a = ll_abs(_a);
    unsigned long long b = ll_abs(_b);

    char a_tz = __builtin_ctz(a);
    char b_tz = __builtin_ctz(b);
    char shifts = std::min(a_tz, b_tz);
    a >>= a_tz;
    b >>= b_tz;

    while (a && b)
    {
        if (a > b)
        {
            a -= b;
            a >>= __builtin_ctzll(a);
        }
        else
        {
            b -= a;
            b >>= __builtin_ctzll(b);
        }
    }

    return (a + b) << shifts;
}

struct Fraction
{
    int numerator   = 1;
    int denominator = 1;

    Fraction operator+(const Fraction& other_fraction) const {return Fraction{this->numerator + other_fraction.numerator, this->denominator + other_fraction.denominator};}
    Fraction operator-(const Fraction& other_fraction) const {return Fraction{this->numerator - other_fraction.numerator, this->denominator - other_fraction.denominator};}
    Fraction operator*(const Fraction& other_fraction) const {return Fraction{this->numerator * other_fraction.numerator, this->denominator * other_fraction.denominator}.simplify();}
    Fraction operator/(const Fraction& other_fraction) const {return Fraction{this->numerator * other_fraction.denominator, this->denominator * other_fraction.numerator}.simplify();}

    friend std::ostream& operator<<(std::ostream& os, const Fraction& fraction) {os << (double)fraction.numerator / fraction.denominator; return os;}

    Fraction simplify()
    {


        return *this;
    }
};

std::optional<int> int_from_string(std::string string)
{
    try
    {
        return std::stoi(string);
    }
    catch (...)
    {
        return std::nullopt;
    }
}



int main()
{
    std::cout << gcd(-5, 15);
    // std::optional<int>
    // std::string yes;
    // std::cin >> yes;
    // std::cout << yes;
}