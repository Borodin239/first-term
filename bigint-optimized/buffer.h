//
// Created by Евгений Бородин on 07.07.2020.
//

#ifndef BIGINT_BUFFER_H
#define BIGINT_BUFFER_H

#include <vector>
#include <algorithm>
#include <cstdint>
#include <my_vector.h>
#include <assert.h>

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
            return vector_->digits_[ind];
        }
        return digits_[ind];
    }

    uint32_t const &operator[](size_t ind) const {  // OK
        if (large_) {
            return vector_->digits_[ind];
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
            if (size_ == CAPACITY) {
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
    }

    // UPD:: исправил тот факт, что у large иногда не вызывался resize_, так же убрал лишний код
    void resize(size_t size) {
        if (size > CAPACITY) {
            if (!large_) {
                vector_ = new my_vector(digits_, size_);
                large_ = true;
                resize(size);
            } else {
                vector_->digits_.resize(size);
            }
        } else if (large_) {
            vector_->digits_.resize(size);
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
        if (vector_->link_counter != 1) {
            vector_->link_counter--;
            vector_ = new my_vector(*vector_);
        }
    }

    friend bool operator==(buffer const &first, buffer const &second) {
        if (first.size_ != second.size_) {
            return false;
        }
        for (size_t i = 0; i < first.size(); i++) {
            if (first[i] != second[i]) {
                return false;
            }
        }
        return true;
    }

private:
    bool large_;
    size_t size_;
    static constexpr size_t CAPACITY = 2;
    union {
        uint32_t digits_[CAPACITY];
        my_vector *vector_;
    };
};

#endif //BIGINT_BUFFER_H
