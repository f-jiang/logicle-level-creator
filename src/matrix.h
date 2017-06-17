#ifndef MATRIX_H_
#define MATRIX_H_

#include <cstddef>
#include <stdexcept>
#include <iterator>
#include <vector>

template <class T>
class matrix {
public:

    matrix();
    matrix(std::size_t, std::size_t);
    matrix(std::size_t, std::size_t, const T&);
    matrix(const matrix<T>&);   // TODO implicitly declared

    matrix<T>& operator=(const matrix<T>&); // TODO implicitly declared
    bool operator==(const matrix<T>&) const;

    T& at(std::size_t, std::size_t, bool = false);
    const T& at(std::size_t, std::size_t, bool = false) const;

    const std::size_t n_rows() const;
    const std::size_t n_cols() const;
    const std::size_t area() const;
private:
    std::size_t m_n_rows;
    std::size_t m_n_cols;
    std::size_t m_area;
    std::vector<T> m_data;
};

template<class T>
matrix<T>::matrix() :
    m_n_rows(0),
    m_n_cols(0),
    m_area(0),
    m_data(0)
{ }

template<class T>
matrix<T>::matrix(std::size_t n_rows, std::size_t n_cols) :
    m_n_rows(n_rows),
    m_n_cols(n_cols),
    m_area(n_rows * n_cols),
    m_data(m_area, 0)
{ }

template<class T>
matrix<T>::matrix(std::size_t n_rows, std::size_t n_cols, const T& val) :
    m_n_rows(n_rows),
    m_n_cols(n_cols),
    m_area(n_rows * n_cols),
    m_data(m_area, val)
{ }

template<class T>
matrix<T>::matrix(const matrix<T>& other) :
    m_n_rows(other.m_n_rows),
    m_n_cols(other.m_n_cols),
    m_area(other.m_area),
    m_data(other.m_data)
{ }

template<class T>
matrix<T>& matrix<T>::operator=(const matrix<T>& other) {
    if (&other != this) {
        m_n_rows = other.m_n_rows;
        m_n_cols = other.m_n_cols;
        m_area = other.m_area;
        m_data = other.m_data;
    }

    return *this;
}

template<class T>
bool matrix<T>::operator==(const matrix<T>& other) const {
    return m_data == other.m_data;
}

template<class T>
T& matrix<T>::at(std::size_t i, std::size_t j, bool transpose) {
    return transpose ? m_data.at(j * m_n_cols + i) : m_data.at(i * m_n_cols + j);
}

template<class T>
const T& matrix<T>::at(std::size_t i, std::size_t j, bool transpose) const {
    return transpose ? m_data.at(j * m_n_cols + i) : m_data.at(i * m_n_cols + j);
}

template<class T>
const std::size_t matrix<T>::n_rows() const {
    return m_n_rows;
}

template<class T>
const std::size_t matrix<T>::n_cols() const {
    return m_n_cols;
}

template<class T>
const std::size_t matrix<T>::area() const {
    return m_area;;
}

#endif /* MATRIX_H_ */
