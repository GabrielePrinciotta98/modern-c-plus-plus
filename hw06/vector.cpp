#include "vector.h"

#include <iterator>
#include <stdexcept>
#include <numeric>
#include <cmath>


namespace linalg {

Vector::Vector(std::size_t n)
    : data_{std::vector<float>(n)}{}

Vector::Vector(std::size_t n, float val)
    : data_{std::vector<float>(n, val)}{}

Vector::Vector(std::initializer_list<float> list) 
    : data_{list} {}


void Vector::assign(float val)
{
    data_.assign(data_.size(), val);
}

void Vector::assign(Vector v)
{
    data_.assign(v.begin(), v.end());
}

Vector& Vector::operator=(float val) 
{
    assign(val);
    return *this;
}

Vector& Vector::operator+=(float val)
{
    std::transform(data_.begin(), data_.end(), data_.begin(), 
        [val](float i){ return i + val; });
    return *this;
}

Vector& Vector::operator-=(float val)
{
    std::transform(data_.begin(), data_.end(), data_.begin(), 
        [val](float i){ return i - val; });
    return *this;
}

Vector& Vector::operator*=(float val)
{
    std::transform(data_.begin(), data_.end(), data_.begin(), 
        [val](float i){ return i * val; });
    return *this;
}

Vector& Vector::operator/=(float val)
{
    if (val != 0)
    {
        std::transform(data_.begin(), data_.end(), data_.begin(), 
            [val](float i){ return i / val; });
    }
    return *this;
}

Vector& Vector::operator+=(const Vector& y)
{
    if(this -> size() != y.size()) throw std::invalid_argument("");
    std::transform(data_.begin(), data_.end(), y.data_.begin(), data_.begin(), 
        [](float i, float j){ return i + j; });
    return *this;
}

Vector& Vector::operator-=(const Vector& y)
{
    if(this -> size() != y.size()) throw std::invalid_argument("");
    std::transform(data_.begin(), data_.end(), y.data_.begin(), data_.begin(), 
        [](float i, float j){ return i - j; });
    return *this;
}

std::size_t Vector::size() const
{
    return data_.size();
}

Vector::iterator Vector::begin()
{
    return data_.begin();
}

Vector::iterator Vector::end()
{
    return data_.end();
}

Vector::const_iterator Vector::begin() const
{
    return data_.cbegin();
}

Vector::const_iterator Vector::end() const
{
    return data_.cend();
}

Vector::const_iterator Vector::cbegin() const
{
    return data_.cbegin();
}

Vector::const_iterator Vector::cend() const
{
    return data_.cend();
}

float& Vector::operator[](int idx)
{
    size_t size = data_.size();
    if (idx < 0 && idx*-1 < size)
        return data_.at(idx + size);
    return data_.at(idx);
}

const float& Vector::operator[](int idx) const
{
    auto size = data_.size();
    if (idx < 0 && idx*-1 < size)
        return data_.at(idx + size);
    return data_.at(idx);
}

float& Vector::coeff(int idx)
{
    return data_.at(idx);
}

const float& Vector::coeff(int idx) const
{
    return data_.at(idx);
}

auto operator<<(std::ostream& ostr, const Vector& x) -> std::ostream& {
    ostr << "[ ";
    std::copy(x.begin(), x.end(), std::ostream_iterator<float>(ostr, " "));
    ostr << "]";
    return ostr;
}

float min(const Vector& x)
{
    if (x.size() == 0) throw std::invalid_argument("");
    return *std::min_element(x.begin(), x.end());
}

float max(const Vector& x)
{
    if (x.size() == 0) throw std::invalid_argument("");
    return *std::max_element(x.begin(), x.end());
}

std::size_t argmin(const Vector& x)
{
    if (x.size() == 0) throw std::invalid_argument("");
    return std::distance(x.begin(), std::min_element(x.begin(), x.end()));
}

std::size_t argmax(const Vector& x)
{
    if (x.size() == 0) throw std::invalid_argument("");
    return std::distance(x.begin(), std::max_element(x.begin(), x.end()));
}

std::size_t non_zeros(const Vector& x)
{
    int n_zeros = std::count(x.begin(), x.end(), 0);
    return x.size() - n_zeros;
}

float sum(const Vector& x)
{
    return std::accumulate(x.begin(), x.end(), 0.0f);
}

float prod(const Vector& x)
{
    return std::accumulate(x.begin(), x.end(), 1.0f, std::multiplies<float>());
}

float dot(const Vector &x, const Vector &y)
{
    if(x.size() != y.size()) throw std::invalid_argument("");
    
    std::vector<float> sums;
    std::transform(x.begin(), x.end(), y.begin(), std::back_inserter(sums), 
        [](float i, float j){ return i * j; });
    return std::accumulate(sums.begin(), sums.end(), 0.0f);
}

float norm(const Vector &x)
{
    return sqrtf(dot(x, x));
}

void normalize(Vector& x)
{
    x /= norm(x);
}

Vector normalized(const Vector& x)
{
    Vector result = x;
    normalize(result);
    return result;
}

Vector floor(const Vector& x)
{
    std::vector<float> output;
    std::transform(x.begin(), x.end(), std::back_inserter(output), 
        [](float i){ return std::floor(i); });
    Vector result = Vector(output.size());
    for (int i = 0; i < output.size(); i++)
    {
        result[i] = output[i];
    }

    return result;
}

Vector ceil(const Vector& x)
{
    std::vector<float> output;
    std::transform(x.begin(), x.end(), std::back_inserter(output), 
        [](float i){ return std::ceil(i); });
    Vector result = Vector(output.size());
    for (int i = 0; i < output.size(); i++)
    {
        result[i] = output[i];
    }

    return result;
}

Vector operator+(const Vector& x)
{
    Vector result = x;
    return result;
}

Vector operator-(const Vector& x)
{
    Vector result = x;
    return result *= -1;
}

Vector operator+(const Vector& x, const Vector& y)
{
    Vector result = x;
    result += y;
    return result;
}

Vector operator-(const Vector& x, const Vector& y)
{
    Vector result = x;
    result -= y;
    return result;
}

Vector operator+(const Vector& x, float val)
{
    Vector result = x;
    result += val;
    return result;
}

Vector operator-(const Vector& x, float val)
{
    Vector result = x;
    result -= val;
    return result;
}

Vector operator*(const Vector& x, float val)
{
    Vector result = x;
    result *= val;
    return result;
}

Vector operator/(const Vector& x, float val)
{
    Vector result = x;
    result /= val;
    return result;
}

Vector operator+(float val, const Vector& x)
{
    Vector result = x;
    result += val;
    return result;
}

Vector operator-(float val, const Vector& x)
{
    Vector result = x;
    result = -result + val;
    return result;
}

Vector operator*(float val, const Vector& x)
{
    Vector result = x;
    result *= val;
    return result;
}


}



/*
 * Please Implement all functions from the header file here!#
 * Have fun! :-) And don't forget maybe you can use standard algorithms ;-)
 */
