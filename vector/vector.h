#ifndef VECTOR_H
#define VECTOR_H

#include <cstdio>
#include <assert.h>
#include <algorithm>


template<typename T>
struct vector {
    typedef T *iterator;
    typedef T const *const_iterator;

    vector();                                   // O(1) nothrow конструктор по умолчанию
    vector(vector const &other);                // O(N) strong  конструктор от другого вектора
    vector &operator=(vector const &other);     // O(N) strong  оператор присваивания

    ~vector();                                  // O(N) nothrow деструктор

    T &operator[](std::size_t i);               // O(1) nothrow обращение к i-ому элементу как a[i] (можно менять)
    T const &operator[](std::size_t i) const;   // O(1) nothrow обращение к i-ому элементу как a[i] (константу вернёт)

    bool empty() const;                         // O(1) nothrow пустой/нет

    T *data();                                  // O(1) nothrow указатель на массив
    T const *data() const;                      // O(1) nothrow указатель на массив (как константа)
    std::size_t size() const;                   // O(1) nothrow размер массива

    T &front();                                 // O(1) nothrow первый элемент массива (можно менять)
    T const &front() const;                     // O(1) nothrow первый элемент массива (как константа)

    T &back();                                  // O(1) nothrow последний элемент массива (можно менять)
    T const &back() const;                      // O(1) nothrow последний элемент массива (как константа)
    void push_back(const T &val);               // O(1)* strong добавление элемента в конец
    void pop_back();                            // O(1) nothrow удаление последнего элемента

    std::size_t capacity() const;               // O(1) nothrow вместимость вектора на данный момент
    void reserve(std::size_t val);              // O(N) strong  выделяет val памяти, если выделено меньше
    void shrink_to_fit();

    void clear();                      // O(N) nothrow отчищает занимаемую память

    void swap(vector &other);                   // O(1) nothrow меняет местами два вектора

    iterator begin();                           // O(1) nothrow итератор начала (можно менять)
    iterator end();                             // O(1) nothrow итератор конца (можно менять)

    const_iterator begin() const;               // O(1) nothrow итератор конца (как константа)
    const_iterator end() const;                 // O(1) nothrow итератор конца (как константа)

    iterator insert(const_iterator pos, T const &val);          // O(N) weak вставляет элемент на нужную позицию

    iterator erase(const_iterator pos);                         // O(N) weak удаляет элемент

    iterator erase(const_iterator first, const_iterator last);  // O(N) weak удаляет элементы в диапазоне

private:
    void push_back_realloc(T const &val);       // O(N) nothrow добавление нового элемента с расширением размера вектора
    void new_buffer(std::size_t new_capacity);  // O(N) weak изменение вместимости вектора

    static void clear_elements(int32_t from, int32_t count, T* ptr);   // Для устранения копипасты
    T* try_to_copy(T *from, size_t capacity, size_t count);

private:
    T *data_;
    std::size_t size_;
    std::size_t capacity_;
};

// !!!FIXED!!! Верный порядок, изменил у data 0 на nullptr, ибо указатель
template<typename T>
vector<T>::vector() : data_(nullptr), size_(0), capacity_(0) {}  // Применил member initializer list

template<typename T>
bool vector<T>::empty() const {
    return size_ == 0;
}

// Единственная копипаста была в цикле while, заменил на clear_elements
template<typename T>
vector<T>::vector(const vector &other) {  // strong
    if (other.size() == 0) {
        data_ = nullptr;
    } else {
        data_ = try_to_copy(other.data_, other.size(), other.size());
        /*data_ = static_cast<T *>(operator new(other.size() * sizeof(T)));
        size_t i = 0;
        try {
            for (; i < other.size(); i++) {
                new(data_ + i) T(other.data()[i]);
            }
        } catch (...) {
            clear_elements(i - 1, i, data_);
            operator delete(data_);
            throw;
        }*/

    }
    size_ = other.size();
    capacity_ = size_;
}

template<typename T>
vector<T> &vector<T>::operator=(const vector &other) {
    vector<T> temp(other);
    swap(temp);
    return *this;
}

template<typename T>
vector<T>::~vector() {
    clear_elements(size_ - 1, size_, data_);
    operator delete(data_);
}

// Убрал лишний assert
template<typename T>
T &vector<T>::operator[](std::size_t i) {
    return data_[i];
}

// Убрал лишний assert
template<typename T>
T const &vector<T>::operator[](std::size_t i) const {
    return data_[i];
}

template<typename T>
T *vector<T>::data() {
    return data_;
}

template<typename T>
T const *vector<T>::data() const {
    return data_;
}

template<typename T>
std::size_t vector<T>::size() const {
    return size_;
}

template<typename T>
T &vector<T>::front() {
    return data_[0];
}

template<typename T>
T const &vector<T>::front() const {
    return data_[0];
}

template<typename T>
T &vector<T>::back() {
    return data_[size_ - 1];
}

template<typename T>
T const &vector<T>::back() const {
    return data_[size_ - 1];
}

template<typename T>
void vector<T>::push_back(const T &val) {
    if (size_ != capacity_) {
        new(data_ + size_) T(val);
    } else {
        push_back_realloc(val);
    }
    ++size_;
}

// Убрал лишний assert
template<typename T>
void vector<T>::pop_back() {
    data_[size_ - 1].~T();
    --size_;
}

template<typename T>
std::size_t vector<T>::capacity() const {
    return capacity_;
}

template<typename T>
void vector<T>::reserve(std::size_t val) {
    if (val > capacity_) {
        new_buffer(val);
    }
}

template<typename T>
void vector<T>::shrink_to_fit() {
    if (empty()) {
        vector<T> temp;
        swap(temp);
    }
    if (size_ != capacity_) {
        new_buffer(size_);
    }
}

template<typename T>
void vector<T>::clear() {
    clear_elements(size_ - 1, size_, data_);
    size_ = 0;
}

template<typename T>
void vector<T>::swap(vector &other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template<typename T>
typename vector<T>::iterator vector<T>::begin() {
    return data_;
}

template<typename T>
typename vector<T>::iterator vector<T>::end() {
    return data_ + size_;
}

template<typename T>
typename vector<T>::const_iterator vector<T>::begin() const {
    return data_;
}

template<typename T>
typename vector<T>::const_iterator vector<T>::end() const {
    return data_ + size_;
}

// Убрал лишний assert и int32_t на ptrdiff_t поменял
template<typename T>
typename vector<T>::iterator vector<T>::insert(vector::const_iterator pos, const T &val) {
    ptrdiff_t index = pos - data_;

    // !!!FIXED!!! заменил в цикле копирование на swap, убрал new(data_ + size_) T()
    // Избавился от копипасты (метод push_back_realloc)
    push_back(val);
    for (int32_t i = size_ - 1; i != index; i--) {
        std::swap(data_[i], data_[i - 1]);
    }
    return data_ + index;
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator pos) {
    return erase(pos, pos + 1);
}

// Убрал лишние assert, заменил int32_t на ptrdiff_t, ибо операция с указателями
template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator first, vector::const_iterator last) {
    ptrdiff_t to = last - data_;
    ptrdiff_t delta = last - first;

    for (int32_t i = first - data_; i < size_ - delta; i++) {
        std::swap(data_[i], data_[i + delta]);
    }

    clear_elements(size_ - 1, delta, data_);
    size_ -= delta;
    return data_ + to;
}

template<typename T>
void vector<T>::push_back_realloc(const T &val) {
    T temp = val;
    if (capacity_ == 0) {
        new_buffer(2);
    } else {
        new_buffer(capacity_ * 2);
    }
    new(data_ + size_) T(temp);
}

template<typename T>
void vector<T>::new_buffer(std::size_t new_capacity) {

    assert(new_capacity >= size_);

    T *new_data = try_to_copy(data_, new_capacity, size_);
    /*T *new_data = static_cast<T *>(operator new(new_capacity * sizeof(T)));
    size_t i = 0;
    try {
        for (; i < size_; i++) {
            new(new_data + i) T(data_[i]);
        }
    }
    catch (...) {
        clear_elements(i - 1, i, new_data);
        operator delete(new_data);
        throw;
    }*/

    // !!!FIXED!!! убрал вызов своего дуструктора, весь необходимый функционал был вынесен в clear_elements()
    clear_elements(size_ - 1, size_, data_);
    operator delete(data_);
    data_ = new_data;
    capacity_ = new_capacity;
}

// !!!FIXED!!! убрана вся копипаста за счёт этого метода
template<typename T>
static void vector<T>::clear_elements(int32_t from, int32_t count, T* ptr) {
    for (int32_t i = from; i > from - count; i--) {
        ptr[i].~T();
    }
}

template<typename T>
T* vector<T>::try_to_copy(T *from, size_t capacity, size_t count) {
    T* new_data_ = static_cast<T *>(operator new(capacity * sizeof(T)));
    size_t i = 0;
    try {
        for (; i < count; i++) {
            new(new_data_ + i) T(from[i]);
        }
    } catch (...) {
        clear_elements(i - 1, i, new_data_);
        operator delete(new_data_);
        throw;
    }

    return new_data_;
}

#endif
