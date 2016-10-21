#ifndef THREAD_SAFE_LIST_H
#define THREAD_SAFE_LIST_H

#include "list.h"

template <class T>
class thread_safe_list {
private:
	list<T> unsafe_list;
	CRITICAL_SECTION lock;
public:
	thread_safe_list() :
		unsafe_list(list<T>()) {
		InitializeCriticalSection(&lock);
	}

};






#endif // THREAD_SAFE_LIST_H
