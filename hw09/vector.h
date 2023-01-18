#pragma once

#include <memory>
#include <ostream>
#include <stdexcept>

template <typename T>
class Vector {
public:
    Vector() = default;

    /**
     * Creates a vector of size n with values default_val.
     */
    Vector(size_t n, const T& default_val){
        _size = n;
        _capacity = _size;
        auto new_data = std::make_unique<T[]>(_capacity);
        for (size_t i = 0; i < _size; i++)
            new_data[i] = default_val;
        _data = std::move(new_data);
    }

    /**
     * Creates a vector containing the elements in l.
     */
    Vector(std::initializer_list<T> l){
        _size = l.size();
        _capacity = _size;
        auto new_data = std::make_unique<T[]>(_capacity);
        for (size_t i = 0; i < _size; i++)
            new_data[i] = l.begin()[i];
        _data = std::move(new_data);
    }

    Vector(const Vector& copy) : _size(copy._size), _capacity(copy._capacity) {
        //_data = std::move(copy._data);

        auto new_data = std::make_unique<T[]>(_capacity);
        for (size_t i = 0; i < _size; ++i)
            new_data[i] = copy[i];
        _data = std::move(new_data);
    }

    Vector(Vector&& move) : _size(move._size), _capacity(move._capacity) {
        _data = std::move(move._data);


        move._size = 0;
        move._capacity = 0;
        move._data = nullptr;
    }

    /**
     * Replaces the contents of the vector.
     */
    Vector& operator=(const Vector& copy){
        _size = copy._size;
        _capacity = copy._capacity;
        auto new_data = std::make_unique<T[]>(copy.capacity());
        for (size_t i = 0; i < copy.size(); i++)
            new_data[i] = copy[i];
        _data = std::move(new_data);
        return *this;
    }


    /**
     * Replaces the contents of the vector.
     */
    Vector& operator=(Vector&& move) noexcept{
        _size = move._size;
        _capacity = move._capacity;
        _data = std::move(move._data);

        move._size = 0;
        move._capacity = 0;
        move._data = nullptr;

        return *this;
    }

    size_t size() const noexcept { return _size; }

    size_t capacity() const noexcept { return _capacity; }

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(const T& value){
        _size++;
        _capacity = calculate_capacity(_size); 

        auto new_data = std::make_unique<T[]>(_capacity);
        for (size_t i = 0; i < _size-1; i++)
            new_data[i] = std::move(_data[i]);
        new_data[_size-1] = value;
        _data = std::move(new_data);
    }

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(T&& value){
        _size++;
        _capacity = calculate_capacity(_size); 

        auto new_data = std::make_unique<T[]>(_capacity);
        for (size_t i = 0; i < _size-1; i++)
            new_data[i] = std::move(_data[i]);
        new_data[_size-1] = std::move(value);
        _data = std::move(new_data);
    }

    /**
     * Removes the last element of the vector.
     */
    void pop_back(){
        _size--;
        auto new_data = std::make_unique<T[]>(_capacity);
        for (size_t i=0; i<_size; i++){
            new_data[i] = _data[i];
        }
        _data = std::move(new_data);
    }

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    const T& at(const size_t pos) const {
        if (pos >= _size)
            throw std::out_of_range("index out of bound");

        return _data[pos];
    }

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    T& at(const size_t pos) {
        if (pos >= _size)
            throw std::out_of_range("index out of bound");

        return _data[pos];
    }

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    const T& operator[](const size_t index) const {
        return _data[index];
    }

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    T& operator[](const size_t index) {
        return _data[index];
    }

    friend std::ostream& operator<<(std::ostream& o, Vector<T> v) {
        o << "Size: " << v._size << ", Capacity: " << v._capacity << std::endl;
        for (size_t i = 0; i < v._size; ++i) {
            if (i > 0)
                o << ", ";
            o << v._data[i];
        }
        o << std::endl;
        return o;
    }

    ~Vector() = default;

private:
    // Defines how the `_capacity` is increased.
    size_t growth_factor = 2;
    size_t _size;
    size_t _capacity;

    // Holds vector's data.
    std::unique_ptr<T[]> _data;

    /**
     * Calculates the necessary capacity for new_size.
     * If necessary, double `_capacity` using `growth_factor`.
     */
    size_t calculate_capacity(size_t new_size) {
        if (_capacity == 0) {
            _capacity = new_size;
            resize(_capacity);
        }
        if (_capacity < new_size){
            _capacity *= growth_factor;
            resize(_capacity);
        }

        return _capacity; 
    }

    /**
    * Resizes the vector to new_capacity.
    * If the current capacity is less than new_capacity
    * the vector moves all elements to a new array.
    */
    void resize(size_t new_capacity) {
        auto new_data = std::make_unique<T[]>(new_capacity);
        for (size_t i=0; i<_size-1; i++){
            new_data[i] = _data[i]; 
        }

        _data = std::move(new_data);
    }
};
