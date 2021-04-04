#pragma once
#include "Listnode.hpp"

template<typename T>
class List
{
private:
	Listnode<T> * first = 0;
	Listnode<T> * last = 0;
public:
	List() {}
	void PushBack(const T & data);
	void PopFront();
	void Replace(Listnode<T> * iterator, const T & data);
	T & GetDataByIt(Listnode<T> * it) const;
	Listnode<T> * GetNextByIt(Listnode<T> * it) const;
	Listnode<T> * GetFirst() const;
	Listnode<T> * GetLast() const;
	~List();
};

template<typename T>
inline void List<T>::PushBack(const T & data)
{
	if (!first)
	{
		first = last = new Listnode<T>(data, 0);
		return;
	}

	last->next = new Listnode<T>(data, 0);
	last = last->next;
}

template<typename T>
inline void List<T>::PopFront()
{
	if (!first)
		return;

	Listnode<T> * temp = first;
	if (first == last) // For one item.
	{
		delete temp;
		first = 0;
		last = 0;
		return;
	}

	if (first->next == last) // For two items.
	{
		delete first;
		first = last;
		return;
	}

	first = first->next;
	delete temp;
}

template<typename T>
inline void List<T>::Replace(Listnode<T>* iterator, const T & data)
{
	if (!iterator)
		return;
	iterator->data = T(data);
}

template<typename T>
inline T & List<T>::GetDataByIt(Listnode<T>* it) const
{
	return it->data;
}

template<typename T>
inline Listnode<T>* List<T>::GetNextByIt(Listnode<T>* it) const
{
	if (!it)
		return 0;
	return it->next;
}

template<typename T>
inline Listnode<T> * List<T>::GetFirst() const
{
	return first;
}

template<typename T>
inline Listnode<T> * List<T>::GetLast() const
{
	return last;
}

template<typename T>
inline List<T>::~List()
{
	while (first)
		PopFront();
}
