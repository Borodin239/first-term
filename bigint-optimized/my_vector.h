//
// Created by Евгений Бородин on 09.07.2020.
//

#ifndef BIGINT_MY_VECTOR_H
#define BIGINT_MY_VECTOR_H

struct my_vector {
    std::vector<uint32_t> digits_;
    size_t link_counter;

    ~my_vector() = default;

    my_vector() = default;

    my_vector(my_vector const &vector_) : digits_(vector_.digits_), link_counter(1) {}

    my_vector(uint32_t digits[], size_t size) : digits_(digits, digits + size), link_counter(1) {}
};

#endif //BIGINT_MY_VECTOR_H
