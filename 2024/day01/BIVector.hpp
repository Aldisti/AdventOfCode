
#pragma once

# include <iostream>
# include <vector>

template <typename T>
class BIVector {

    private:
        std::vector<T>  vector;

    public:
        BIVector();
		BIVector(BIVector const &biv);
		~BIVector(void);
		BIVector &operator=(BIVector const &biv);
        // constant methods
        std::vector<T>  getVector() const;
        size_t          size() const;
        // non-constant methods
        void            insert(T element);
        T               pop(size_t index);
};

template <typename T>
std::ostream    &operator << (std::ostream &out, BIVector<T> const &biv) {
    std::vector<T> v = biv.getVector();

	out << "[";
	for (size_t i = 0; i < v.size() - 1; i++)
        out << v[i] << ", ";
	out << v.back() << "]";
	return (out);
}

template <typename T>
BIVector<T>::BIVector() {
    return;
};

template <typename T>
BIVector<T>::BIVector(BIVector<T> const &biv) {
    *this = biv;
}

template <typename T>
BIVector<T>::~BIVector() {
    return;
}

template <typename T>
BIVector<T>& BIVector<T>::operator = (BIVector<T> const &biv) {
    if (this == &biv)
        return *this;
    this->vector.clear();
    for (const T &i : biv.getVector()) {
        this->vector.push_back(i);
    }
    return *this;
}

template <typename T>
std::vector<T> BIVector<T>::getVector() const {
    return this->vector;
}

template <typename T>
size_t BIVector<T>::size() const {
    return this->vector.size();
}

template <typename T>
void BIVector<T>::insert(T element) {
    if (this->vector.size() == 0) {
        this->vector.push_back(element);
        return;
    }
    if (this->vector.size() == 1) {
        if (this->vector.front() < element)
            this->vector.push_back(element);
        else
            this->vector.insert(this->vector.begin(), element);
        return;
    }

    size_t  left = 0;
    size_t  right = this->vector.size() - 1;
    size_t  mid = right / 2;
    while (left < right and left + 1 != right) {
        if (this->vector[mid] < element)
            left = mid;
        else
            right = mid;
        mid = (left + right) / 2;
    }

    right += (element > this->vector[right]);
    if (element < this->vector[left])
        right = left;

    this->vector.insert(this->vector.begin() + right, element);
}

template <typename T>
T BIVector<T>::pop(size_t index) {
    T   ret;

    ret = this->vector.at(index);
    this->vector.erase(this->vector.begin() + index);
    return ret;
}
