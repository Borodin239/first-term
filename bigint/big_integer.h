//
// Created by Евгений Бородин on 16.06.2020.
//

#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <vector>
#include <string>

struct big_integer {
public:
    big_integer();  // OK
    big_integer(big_integer const& other);  // OK
    big_integer(int32_t a);  // OK
    explicit big_integer(std::string const& str);  // OK
    ~big_integer();  // OK

    big_integer& operator=(big_integer const& other);  // OK

    big_integer& operator+=(big_integer const& rhs);  // OK
    big_integer& operator-=(big_integer const& rhs);  // OK
    big_integer& operator*=(big_integer const& rhs);  // OK
    big_integer& operator/=(big_integer const& rhs);  // OK
    big_integer& operator%=(big_integer const& rhs);  // OK

    big_integer& operator&=(big_integer const& rhs);  // OK
    big_integer& operator|=(big_integer const& rhs);  // OK
    big_integer& operator^=(big_integer const& rhs);  // OK

    big_integer& operator<<=(int32_t rhs);  // OK
    big_integer& operator>>=(int32_t rhs);  // OK

    big_integer operator+() const;  // OK
    big_integer operator-() const;  // OK
    big_integer operator~() const;  // OK

    big_integer& operator++();  // OK
    big_integer operator++(int);  // OK

    big_integer& operator--();  // OK
    big_integer operator--(int);  // OK

    friend bool operator==(big_integer const& a, big_integer const& b);  // OK
    friend bool operator!=(big_integer const& a, big_integer const& b);  // OK
    friend bool operator<(big_integer const& a, big_integer const& b);  // OK
    friend bool operator>(big_integer const& a, big_integer const& b);  // OK
    friend bool operator<=(big_integer const& a, big_integer const& b);  // OK
    friend bool operator>=(big_integer const& a, big_integer const& b);  // OK

    friend std::string to_string(big_integer const& a);  // OK

    friend big_integer operator+(big_integer a, big_integer const& b);  // OK
    friend big_integer operator-(big_integer a, big_integer const& b);  // OK
    friend big_integer operator*(big_integer a, big_integer const& b);  // OK
    friend big_integer operator/(big_integer a, big_integer const& b);  // OK
    friend big_integer operator%(big_integer a, big_integer const& b);  // OK

    friend big_integer operator&(big_integer a, big_integer const& b);  // OK
    friend big_integer operator|(big_integer a, big_integer const& b);  // OK
    friend big_integer operator^(big_integer a, big_integer const& b);  // OK

    friend big_integer operator<<(big_integer a, int32_t b);  // OK
    friend big_integer operator>>(big_integer a, int32_t b);  // OK

private:
    std::vector<uint32_t> digits_;
    bool is_negative_{};

    big_integer& trim();
    friend uint32_t trial(big_integer &first, big_integer const &second);
    friend void difference(big_integer &first, big_integer const &second, size_t m);
    friend bool smaller(big_integer const &first, big_integer const &second, size_t m);
    friend big_integer binary_operations(big_integer &first, big_integer &second, char operation);
};

std::ostream& operator<<(std::ostream& s, big_integer const& a);  // OK

#endif // BIG_INTEGER_H