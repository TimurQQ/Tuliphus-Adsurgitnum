#pragma once
#ifndef _TABLE_H_INCLUDED_
#define _TABLE_H_INCLUDED_

//<------------------Pair--------------------->
template <class KeyType, class ValType>
struct Pair {
	KeyType key;
	ValType value;
	Pair()
		: key(KeyType()),
		value(ValType())
	{}

	Pair(const KeyType& key)
		: key(key),
		value(ValType())
	{}
};

//<---------------------TableIterator----------------------->

template <class KeyType, class ValType>
class TableIt {
private:
	Pair<KeyType, ValType>* current_;
public:
	TableIt()
		: current_(nullptr)
	{}

	TableIt(Pair<KeyType, ValType>* a)
		: current_(a)
	{}

	int operator !=(const TableIt<KeyType, ValType>&) const;
	int operator ==(const TableIt<KeyType, ValType>&) const;
	Pair<KeyType, ValType>& operator *();
	Pair<KeyType, ValType>* operator ->();
	TableIt<KeyType, ValType>& operator ++();
	TableIt<KeyType, ValType> operator ++(int);
};

//<--------------------------------Table---------------------------------->

template <class KeyType, class ValType>
class Table {
	friend class TableIt<KeyType, ValType>;
public:
	Table();
	Table(const Table<KeyType, ValType>&); //  опирующий конструктор
	~Table() { delete[] array_; }

	Table<KeyType, ValType>& operator = (
		const Table<KeyType, ValType>&);
	ValType& operator[](const KeyType&); // l-value
	const ValType& operator[](const KeyType&) const; // r-value
	typedef TableIt <KeyType, ValType> Iterator;

	Iterator begin();
	Iterator end();
	Iterator find(const KeyType&) const;

	int length() { return length_; } // кол-во ресурсных точек
protected:
	// pass
private:
	static const int kQuota = 10;
	int cur_max_size_ = 0;
	Pair<KeyType, ValType>* array_ = nullptr;
	int length_ = 0;
	int GetPos(const KeyType&) const;
};

// <------------------PublicImplementation(Table)------------------>

template<class KeyType, class ValType>
Table<KeyType, ValType>::Table()
	: cur_max_size_(Table::kQuota),
	length_(0),
	array_(new Pair<KeyType, ValType>[Table::kQuota])
{}

template<class KeyType, class ValType>
Table<KeyType, ValType>::Table(const Table<KeyType, ValType>& a)
	: cur_max_size_(a.cur_max_size_),
	length_(a.length_),
	array_(new Pair<KeyType, ValType>[a.cur_max_size_])
{
	for (int i = 0; i < cur_max_size_; ++i) {
		array_[i] = a.array_[i]
	}
}

template<class KeyType, class ValType>
Table<KeyType, ValType>& Table<KeyType, ValType>::operator =(
	const Table<KeyType, ValType>& a)
{
	if (this != &a) {
		delete[] array_;
		array_ = new Pair<KeyType, ValType>[a.cur_max_size_];
		cur_max_size_ = a.cur_max_size_;
		length_ = a.length_;
		for (int i = 0; i < cur_max_size_; ++i) {
			array_[i] = a.array_[i]
		}
	}
	return *this;
}

template<class KeyType, class ValType> // l-value method
ValType& Table<KeyType, ValType>::operator[](const KeyType& key) {
	int i = GetPos(key);
	if (i < 0) {
		i = length_;
		if (length_ >= cur_max_size_) {
			Pair<KeyType, ValType>* old = array_;
			array_ = new Pair<KeyType, ValType>[cur_max_size_ += kQuota];
			for (i = 0; i < length_; ++i) {
				array_[i] = old[i];
			}
			delete[] old;
		}
		array_[length_].key = key;
		++length_;
	}
	return array_[i].value;
}

template<class KeyType, class ValType> // r-value method
const ValType& Table<KeyType, ValType>::operator[](const KeyType& key) const {
	int i = GetPos(key);
	if (i < 0) {
		throw std::exception("Illegal index");
	}
	return array_[i].value;
}

template<class KeyType, class ValType>
TableIt<KeyType, ValType> Table<KeyType, ValType>::begin() {
	return TableIt<KeyType, ValType>(this->array_);
}

template<class KeyType, class ValType>
TableIt<KeyType, ValType> Table<KeyType, ValType>::end() {
	return TableIt<KeyType, ValType>(this->array_ + length_);
}

template<class KeyType, class ValType>
TableIt<KeyType, ValType> Table<KeyType, ValType>::find(const KeyType& key)
	const
{
	int i = GetPos(key);
	if (i < 0) {
		i = length_;
	}
	return TableIt<KeyType, ValType>(this->array_ + i);
}

// <------------------PublicImplementation(TableIterator)------------------>

template<class KeyType, class ValType>
int TableIt<KeyType, ValType>::operator !=(
	const TableIt<KeyType, ValType>& it) const 
{
	return current_ != it.current_;
}

template<class KeyType, class ValType>
int TableIt<KeyType, ValType>::operator ==(
	const TableIt<KeyType, ValType>& it) const
{
	return current_ == it.current_;
}

template<class KeyType, class ValType>
Pair<KeyType, ValType>& TableIt<KeyType, ValType>::operator *() {
	return *current_;
}

template<class KeyType, class ValType>
Pair<KeyType, ValType>* TableIt<KeyType, ValType>::operator ->() {
	return current_;
}

template<class KeyType, class ValType>
TableIt<KeyType, ValType>& TableIt<KeyType, ValType>::operator ++() {
	++current_;
	return *this;
}

template<class KeyType, class ValType>
TableIt<KeyType, ValType> TableIt<KeyType, ValType>::operator ++(int) {
	TableIt<KeyType, ValType> res(*this); // make a copy
	++current_;
	return res;
}

//<----------------------------PrivateImplementation(Table)----------------------------->
template <class KeyType, class ValType>
int Table<KeyType, ValType>::GetPos(const KeyType& key) const {
	for (int i = 0; i < length_; ++i) {
		if (array_[i].key == key) {
			return i;
		}
	}
	return -1;
}

#endif // _TABLE_H_INCLUDED_
