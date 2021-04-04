#pragma once

template<typename T>
class Listnode
{
private:
	T data;
	Listnode<T> * next = 0;

	Listnode(T data, Listnode<T> * next) : data(data), next(next) {}
public:
	template<typename U> friend class List;
	Listnode() {}
};

