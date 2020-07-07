//
// Created by Евгений Бородин on 16.06.2020.
//
#include "big_integer.h"
#include <string>
#include <algorithm>
#include <iostream>

typedef unsigned long long int uint128_t __attribute__ ((mode (TI)));

const uint64_t POW2 = static_cast<uint64_t>(UINT32_MAX) + 1;

big_integer::big_integer() : digits_(1, 0), is_negative_(false) {}

big_integer::big_integer(const big_integer &arg) = default;

big_integer::big_integer(int32_t arg) : digits_(1), is_negative_(arg < 0) {
    if (arg == INT32_MIN) {
        digits_[0] = static_cast<uint32_t>(INT32_MAX) + 1;
    }
    else {
        digits_[0] = static_cast<uint32_t>(abs(arg));
    }
}

big_integer::big_integer(std::string const &str) : digits_(1, 0) {
    if (str.empty() || str == "0" || str == "-0") {
        return;
    }
    size_t pos = 0;
    is_negative_ = false;
    if (str[0] == '-') {
        pos++;
    }
    while (pos < str.size()) {
        *this *= 10;
        *this += (str[pos] - '0');
        pos++;
    }
    if (str[0] == '-') {
        is_negative_ = true;
    }
}

big_integer::~big_integer() = default;

big_integer &big_integer::operator=(const big_integer &number) {
    digits_ = number.digits_;
    is_negative_ = number.is_negative_;
    return *this;
}

big_integer &big_integer::operator+=(const big_integer &rhs) {
    *this = *this + rhs;
    return *this;
}

big_integer &big_integer::operator-=(const big_integer &rhs) {
    *this = *this - rhs;
    return *this;
}

big_integer &big_integer::operator*=(const big_integer &rhs) {
    *this = *this * rhs;
    return *this;
}

big_integer &big_integer::operator/=(const big_integer &rhs) {
    *this = *this / rhs;
    return *this;
}

big_integer &big_integer::operator%=(const big_integer &rhs) {
    *this = *this % rhs;
    return *this;
}

big_integer &big_integer::operator&=(const big_integer &rhs) {
    *this = *this & rhs;
    return *this;
}

big_integer &big_integer::operator|=(const big_integer &rhs) {
    *this = *this | rhs;
    return *this;
}

big_integer &big_integer::operator^=(const big_integer &rhs) {
    *this = *this ^ rhs;
    return *this;
}

big_integer &big_integer::operator<<=(int32_t rhs) {
    *this = *this << rhs;
    return *this;
}

big_integer &big_integer::operator>>=(int32_t rhs) {
    *this = *this >> rhs;
    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer temp = *this;
    temp.trim();
    if (temp != 0) {
        temp.is_negative_ = !temp.is_negative_;
    }
    return temp;
}

big_integer big_integer::operator~() const {
    return -*this - 1;
}


big_integer &big_integer::operator++() {
    *this += 1;
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer prev = *this;
    *this += 1;
    return prev;
}

big_integer &big_integer::operator--() {
    *this -= 1;
    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer prev = *this;
    *this -= 1;
    return prev;
}

bool operator>(const big_integer &first, const big_integer &second) {
    // если разный знак, то если first < 0 second.is_negative = false (возвращаем что надо) и наоборот
    if (first.is_negative_ != second.is_negative_) {
        return second.is_negative_;
    }
    // рассмотрим разное кол-во цифр
    if (second.digits_.size() > first.digits_.size()) {
        return first.is_negative_;
    } else if (second.digits_.size() < first.digits_.size()) {
        return !first.is_negative_;
    }
    // Одинаковое кол-во цифр и одинаковые знаки
    if (second.is_negative_) {
        for (size_t i = first.digits_.size(); i != 0; i--) {
            if (first.digits_[i - 1] < second.digits_[i - 1]) {
                return true;
            }
            if (first.digits_[i - 1] > second.digits_[i - 1]) {
                return false;
            }
        }
        return false;
    }
    for (size_t i = first.digits_.size(); i != 0; i--) {
        if (first.digits_[i - 1] > second.digits_[i - 1]) {
            return true;
        }
        if (first.digits_[i - 1] < second.digits_[i - 1]) {
            return false;
        }
    }
    return false;
}

bool operator<(big_integer const &first, big_integer const &second) {
    return second > first;
}

bool operator>=(big_integer const &first, big_integer const &second) {
    return !(first < second);
}

bool operator<=(big_integer const &first, big_integer const &second) {
    return !(first > second);
}

bool operator==(big_integer const &first, big_integer const &second) {
    return first.is_negative_ == second.is_negative_ && first.digits_ == second.digits_;
}

bool operator!=(big_integer const &first, big_integer const &second) {
    return !(first == second);
}



// Убирает лишние 0 вначале числа
big_integer &big_integer::trim() {
    size_t i = digits_.size();
    while (i != 1) {
        if (digits_[i - 1] == 0) {
            i--;
            digits_.pop_back();
        } else {
            break;
        }
    }
    if (i == 1 && digits_[0] == 0) {
        is_negative_ = false;
    }
    return *this;
}



uint32_t get_operation(uint32_t first, uint32_t second, char operation) {
    if (operation == '&') {
        return first & second;
    }
    if (operation == '|') {
        return first | second;
    }
    return first ^ second;
}

big_integer binary_operations(big_integer first, const big_integer &second, char operation) {
    size_t max_size = std::max(first.digits_.size(), second.digits_.size());
    big_integer a = first;
    big_integer b = second;
    while (a.digits_.size() != max_size) {
        a.digits_.push_back(0);
    }
    while (b.digits_.size() != max_size) {
        b.digits_.push_back(0);
    }
    a = a.is_negative_ ? convert(a) : a;
    b = b.is_negative_ ? convert(b) : b;

    big_integer answer;
    answer.digits_.resize(max_size);

    for (size_t i = 0; i < max_size; i++) {
        answer.digits_[i] = get_operation(a.digits_[i], b.digits_[i], operation);
    }

    bool check_sign = get_operation(first.is_negative_, second.is_negative_, operation);
    if (check_sign) {
        answer.is_negative_ = true;
        answer = convert(answer);
        answer.is_negative_ = true;
    }

    return answer.trim();
}

big_integer convert(big_integer number) {
    number.is_negative_ = false;
    for (size_t i = 0; i < number.digits_.size(); i++) {
        number.digits_[i] = ~number.digits_[i];
    }
    number += 1;
    return number;
}


big_integer operator^(big_integer first, const big_integer &second) {
    return binary_operations(first, second, '^');
}

big_integer operator&(big_integer first, const big_integer &second) {
    return binary_operations(first, second, '&');
}


big_integer operator|(big_integer first, const big_integer &second) {
    return binary_operations(first, second, '|');
}

big_integer operator<<(big_integer first, int32_t second) {
    bool sign = first.is_negative_;
    first = first * static_cast<uint32_t>(1 << (second % 32));
    if (second > 32 && first != 0) {
        second /= 32;
        first.digits_.insert(first.digits_.begin(), second, 0);
    }
    first.is_negative_ = sign;
    return first;
}

big_integer operator>>(big_integer first, int32_t second) {
    if (first == 0) {
        return first;
    }

    uint32_t shift_digits = second / 32;
    uint32_t mod = 1 << (second % 32);

    big_integer temp = first / mod;

    if (temp.digits_.size() - shift_digits <= 0) {
        return 0;
    } else {
        size_t len = temp.digits_.size() - shift_digits;
        std::vector<uint32_t> res(len);
        for (size_t i = 0; i < len; ++i) {
            res[i] = temp.digits_[i + shift_digits];
        }
        big_integer answer;
        answer.digits_ = res;
        answer.is_negative_ = first.is_negative_;
        if (first.is_negative_) {
            --answer;
        }
        return answer;
    }
}


big_integer operator+(big_integer first, const big_integer &second) {

    if (first.is_negative_ != second.is_negative_) {
        if (first.is_negative_) {
            return second - (-first);
        }
        return first - (-second);
    }

    size_t max_size = std::max(first.digits_.size(), second.digits_.size());
    big_integer answer;
    answer.digits_.resize(max_size);
    uint64_t res = 0;
    uint64_t shift = 0;
    for (size_t i = 0; i < max_size; i++) {
        res = shift;
        if (i < first.digits_.size()) {
            res += static_cast<uint64_t>(first.digits_[i]);
        }
        if (i < second.digits_.size()) {
            res += static_cast<uint64_t>(second.digits_[i]);
        }
        answer.digits_[i] = static_cast<uint32_t> (res % POW2);
        shift = res / POW2;
    }
    answer.digits_.push_back(shift);
    answer.is_negative_ = first.is_negative_;
    return answer.trim();
}

big_integer operator-(big_integer first, const big_integer &second) {

    // оба отрицательные -- вычитаем из модуля второго модуль первого
    if (first.is_negative_ && second.is_negative_) {
        return -(-first - -second);
    }
    // первое отрицательное, второе - положительное => сложение отрицательных чисел
    if (first.is_negative_ && !second.is_negative_) {
        return -(-first + second);
    }
    // второе отрицательное, первое - положительное => сложение первого и модуля второго
    if (second.is_negative_ && !first.is_negative_) {
        return first + (-second);
    }
    // оба положительные, второе больше первого
    if (first < second) {
        return -(second - first);
    }

    uint32_t shift = 0;
    for (size_t i = 0; i < first.digits_.size(); i++) {
        uint32_t first_digit = first.digits_[i];
        uint32_t second_digit = 0;
        if (second.digits_.size() > i) {
            second_digit = second.digits_[i];
        }
        first.digits_[i] = first_digit - second_digit - shift;
        if (second_digit > first_digit || shift > first_digit - second_digit) {
            shift = 1;
        } else {
            shift = 0;
        }
    }

    return first.trim();
}

big_integer operator*(big_integer first, const big_integer &second) {
    if (first == 0 || second == 0) {
        return big_integer(0);
    }
    big_integer answer;
    answer.is_negative_ = first.is_negative_ != second.is_negative_;
    answer.digits_.resize(first.digits_.size() + second.digits_.size() + 1);
    for (size_t i = 0; i < first.digits_.size(); i++) {
        uint64_t shift = 0;
        uint64_t res;
        for (size_t j = 0; j < second.digits_.size(); j++) {
            res = static_cast<uint64_t> (first.digits_[i]) * second.digits_[j];
            res += shift + static_cast<uint64_t>(answer.digits_[i + j]);
            answer.digits_[i + j] = static_cast<uint32_t> (res % POW2);
            shift = res / POW2;
        }
        answer.digits_[i + second.digits_.size()] = shift;
    }
    return answer.trim();
}

big_integer operator/(big_integer first, const big_integer &second) {

    // Банальные случаи
    if (second == 1) {
        return first;
    }
    if (second == -1) {
        return -first;
    }
    if (first == 0) {
        return 0;
    }
    big_integer a = first;
    a.is_negative_ = false;
    big_integer b = second;
    b.is_negative_ = false;
    if (b > a) {
        return 0;
    }

    big_integer answer;
    answer.is_negative_ = (first.is_negative_ != second.is_negative_);
    // Дальнейший алгоритм и методы, которые там исользуются были взяты из статьи в условиях дз
    // https://surface.syr.edu/cgi/viewcontent.cgi?referer=&httpsredir=1&article=1162&context=eecs_techreports
    if (second.digits_.size() == 1) {
        uint64_t shift = 0;
        uint64_t temp;
        for (size_t i = 0; i < first.digits_.size(); i++) {
            temp = POW2 * shift + first.digits_[first.digits_.size() - i - 1];
            answer.digits_.push_back(static_cast<uint32_t>((temp / second.digits_[0]) % POW2));
            shift = temp % second.digits_[0];
        }
        std::reverse(answer.digits_.begin(), answer.digits_.end());
    } else {
        big_integer dq;
        a.digits_.push_back(0);
        size_t m = b.digits_.size();
        size_t n = a.digits_.size();
        answer.digits_.resize(n - m);
        uint32_t qt = 0;
        size_t j = answer.digits_.size() - 1;
        for (size_t i = m + 1; i <= n; i++) {
            qt = trial(a, b);
            dq = b * qt;
            if (!smaller(a, dq, m + 1)) {
                qt--;
                dq = b * qt;
            }
            answer.digits_[j] = qt;
            difference(a, dq, m + 1);
            if (a.digits_.size() != 0) {
                a.digits_.pop_back();
            }
            j--;
        }
    }
    return answer.trim();
}

uint32_t trial(big_integer &first, big_integer const &second) {  // OK
    uint128_t a = ((static_cast<uint128_t>(first.digits_[first.digits_.size() - 1]) << 64) +
                   (static_cast<uint128_t>(first.digits_[first.digits_.size() - 2]) << 32) +
                   static_cast<uint128_t>(first.digits_[first.digits_.size() - 3]));
    uint128_t b = ((static_cast<uint128_t>(second.digits_[second.digits_.size() - 1]) << 32) +
                   static_cast<uint128_t>(second.digits_[second.digits_.size() - 2]));
    return std::min(static_cast<uint32_t>(a / b), UINT32_MAX);
}


bool smaller(big_integer const &first, big_integer const &second, size_t id) {
    for (size_t i = 1; i <= first.digits_.size(); i++) {
        if (id - i >= second.digits_.size()) {
            if (first.digits_[first.digits_.size() - i] != 0) {
                return first.digits_[first.digits_.size() - i] > 0;
            }
        } else {
            if (first.digits_[first.digits_.size() - i] != second.digits_[id - i]) {
                return first.digits_[first.digits_.size() - i] > second.digits_[id - i];
            }
        }
    }
    return true;
}

void difference(big_integer &first, big_integer const &second, size_t m) {  // OK
    int64_t borrow = 0;
    size_t k = first.digits_.size() - m;
    for (size_t i = 0; i < m; i++) {
        uint32_t val = 0;
        if (i < second.digits_.size()) {
            val = second.digits_[i];
        }
        uint64_t diff = static_cast<uint64_t>(first.digits_[k + i]) - val + POW2 - borrow;
        first.digits_[k + i] = diff % POW2;
        borrow = 1 - diff / POW2;
    }
}

big_integer operator%(big_integer first, const big_integer &second) {
    return first - (first / second) * second;
}

std::string to_string(const big_integer &number) {
    big_integer temp(number);
    std::string answer;
    if (temp == 0) {
        return "0";
    }
    while (temp != 0) {
        answer += static_cast<char>('0' + (temp % 10).digits_[0]);
        temp /= 10;
    }
    if (number.is_negative_) {
        answer += '-';
    }
    std::reverse(answer.begin(), answer.end());
    return answer;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}