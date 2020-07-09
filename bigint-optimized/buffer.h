//
// Created by Евгений Бородин on 07.07.2020.
//

#ifndef BIGINT_BUFFER_H
#define BIGINT_BUFFER_H

#include <vector>
#include <algorithm>
#include <cstdint>

struct my_vector {
    std::vector<uint32_t> digits_;
    size_t link_counter;

    ~my_vector() = default;

    my_vector() = default;

    my_vector(my_vector &vector_) : digits_(vector_.digits_), link_counter(1) {}

    my_vector(std::vector<uint32_t> vector_) : digits_(vector_), link_counter(1) {}

    my_vector(uint32_t digits[], size_t size) : digits_(digits, digits + size), link_counter(1) {}
};

struct buffer {

    explicit buffer(uint32_t arg) : large_(false), size_(1) {  // OK
        digits_[0] = arg;
    }

    buffer(buffer const &arg) : large_(arg.large_), size_(arg.size_) {
        if (large_) {
            vector_ = arg.vector_;
            vector_->link_counter++;
        } else {
            std::copy(arg.digits_, arg.digits_ + arg.size_, digits_);
        }
    }

    ~buffer() {  // OK
        if (large_) {
            if (vector_->link_counter == 1) {
                delete vector_;
            } else {
                vector_->link_counter--;
            }
        }
    }

    uint32_t &operator[](size_t ind) {  // OK
        if (large_) {
            single();
            return (*vector_).digits_[ind];
        }
        return digits_[ind];;
    }

    uint32_t const &operator[](size_t ind) const {  // OK
        if (large_) {
            return (*vector_).digits_[ind];
        }
        return digits_[ind];
    }

    uint32_t const &back() const {  // OK
        return (*this)[size_ - 1];
    }

    size_t size() const {  // OK
        return size_;
    }

    void push_back(uint32_t a) {  // OK
        if (large_) {
            single();
            vector_->digits_.push_back(a);
        } else {
            if (size_ == 2) {
                large_ = true;
                vector_ = new my_vector(digits_, 2);
                vector_->digits_.push_back(a);
            } else {
                digits_[size_] = a;
            }
        }
        size_++;
    }

    void pop_back() {  // OK
        if (large_) {
            single();
            vector_->digits_.pop_back();
        }
        size_--;
        if (large_ && size_ == 2) {
            single();
            uint32_t first = vector_->digits_[0];
            uint32_t second = vector_->digits_[1];
            delete vector_;
            digits_[0] = first;
            digits_[1] = second;
            large_ = false;
        }
    }

    void resize(size_t size) {
        if (size > 2) {
            large_ = true;
            if (size_ <= 2) {
                std::vector<uint32_t> curr(digits_, digits_ + size_);
                curr.resize(size);
                vector_ = new my_vector(curr);
            } else {
                vector_->digits_.resize(size);
            }
        }
        size_ = size;
    }

    buffer &operator=(buffer const &arg) {
        this->~buffer();
        size_ = arg.size_;
        large_ = arg.large_;
        if (large_) {
            vector_ = arg.vector_;
            vector_->link_counter++;
        } else {
            std::copy(arg.digits_, arg.digits_ + arg.size_, digits_);
        }
        return *this;
    }

    void reverse() {
        if (large_) {
            single();
            std::reverse(vector_->digits_.begin(), vector_->digits_.end());
        } else {
            std::reverse(digits_, digits_ + size_);
        }
    }

    void single() {
        //vector_ = vector_->make_unique_data();
        if (vector_->link_counter != 1) {
            vector_->link_counter--;
            vector_ = new my_vector(*vector_);
        }
    }

    // TODO:: подумай что с этим можно сделать
    friend bool operator==(buffer const &first, buffer const &second) {
        if (first.size_ != second.size_) {
            return false;
        }
        if (first.large_) {
            return first.vector_->digits_ == second.vector_->digits_;
        }
        if (first.size_ == 1) {
            return first.digits_[0] == second.digits_[0];
        }
        return first.digits_[0] == second.digits_[0] && first.digits_[1] == second.digits_[1];
    }

private:
    bool large_;
    size_t size_;
    union {
        uint32_t digits_[2];
        my_vector *vector_;
    };
};

#endif //BIGINT_BUFFER_H
