#ifndef ORDERED_LIST_H
#define ORDERED_LIST_H
#include "list.h"

template<class T>
struct orderedListItem{
	T item;
	int index;
	/*store the relative offset from the beginning of the
	array, since the array can be moved in memory, direct
	pointers to the next item cannot be stored, only the
	relative position from the beginning of the array.*/
	int *nextIndex;
	int *prevIndex;

	void print(){
		if(prevIndex) cout << "-previous index [" << *prevIndex << "]\n";
					  cout << "      memory index[" << index << "] = \"" << *item << "\"\n";
		if(nextIndex) cout << "-    next index [" << *nextIndex << "]\n";
	}
};

/**\brief a list of items that maintains the order in which
items are added or removed.

The orderedList class uses a linked list of the class orderedListItem
to keep track of when items are added to the beginning (shift()) or
end (push()) of the list, or when items are removed from the beginning
(unshift()) or end (pop()) of the list.**/
template<class T>
class orderedList{
private:

	orderedListItem<T> *item;

	int itemCount;
	int nextIndex;
	int maxIndex;
	int listSize;

	list<int> freeIndex;

	/*need to keep track of the actual array position of the
	first and last orderedListItem pointers because the actual
	pointer information is lost when the array is moved after
	being resized.*/
	int firstOffset;
	int lastOffset;

	void grow(){
		int add = listSize + 1;
		//cout << " - change list size " << listSize << " -> " << listSize+add << endl;
		orderedListItem<T> *newItem = new orderedListItem<T>[listSize+add]();
		memcpy(&newItem[0],&item[0],sizeof(orderedListItem<T>)*listSize);
		delete item;
		item = newItem;
		listSize += add;
	}

	int nextFreeIndex(){
		if(freeIndex.getCount() > 0){
			return freeIndex.pop();
		}
		if(maxIndex == listSize) grow();
		return maxIndex++;
	}

public:
	orderedList() :
		item(new orderedListItem<T>[1]()),
		itemCount(0),
		nextIndex(0),
		maxIndex(0),
		listSize(0),
		freeIndex(list<int>()),
		firstOffset(-1),
		lastOffset(-1){
		//grow();
	}

	/**\brief inserts a new item at the end of the list.

	to retrieve the last item that was push()ed onto the list,
	call the pop() function.**/
	T push(T newItem){
		//cout << "[push]\n";
		nextIndex = nextFreeIndex();
		//cout << " - previous last memory index : " << lastOffset << endl;
		//cout << " -      store at memory index : " << nextIndex << endl;
		item[nextIndex] = orderedListItem<T>();
		item[nextIndex].item = newItem;
		item[nextIndex].index = nextIndex;
		item[nextIndex].nextIndex = NULL;

		/*if there is already an object or more set, thus the lastOffset
		has a not -1 value, set the next index for that object as the new
		index, and set the previous index of the next object as the index
		of the last offset.*/
		if(lastOffset != -1){
			item[nextIndex].prevIndex = new int(item[lastOffset].index);
			item[lastOffset].nextIndex = new int(nextIndex);
		} else {
			item[nextIndex].prevIndex = NULL;
		}
		lastOffset = nextIndex;
		if(firstOffset == -1) firstOffset = nextIndex;
		++itemCount;
		return newItem;
	}

	/**\brief inserts a new item at the front of the list.

	Every item in the list is shifted backwards and the new item
	is inserted in the front. To retrieve the last shift()ed item,
	called the unshift() function.**/
	T shift(T newItem){
		nextIndex = nextFreeIndex();
		item[nextIndex] = orderedListItem<T>();
		item[nextIndex].item = newItem;
		item[nextIndex].index = nextIndex;
		if(firstOffset != -1){
			item[firstOffset].prevIndex = new int(nextIndex);
			item[nextIndex].nextIndex = new int(firstOffset);
		} else{
			item[nextIndex].nextIndex = NULL;
		}
		firstOffset = nextIndex;
		if(lastOffset == -1) lastOffset = nextIndex;
		++itemCount;
		return newItem;
	}

	/**\brief removes an item at a specific index.

	The pointer of the preceeding item is routed to the
	next item after the item being deleted. It is a linked
	list, so it should be nice and easy.

	The item deleted is deleted reguardless of the actual
	position in the linked list. It is selected and deleted
	solely based on its index in the array that holds the
	linked list items.**/
	bool cut(int index){
		//cout << "[cut]";
		//cout << " - order # " << index;
		if(itemCount <= index) return false;
		/*get the real index of the item to cut. the "index" implied
		from the index argument is really the 3rd item from the front
		of the line.*/
		int* ind = new int(firstOffset);
		while(index --> 0){
			ind = item[*ind].nextIndex;
		}
		index = *ind;
		//cout << " - remove index " << index << endl;
		freeIndex.push(index);

		if(item[index].nextIndex){
			if(item[index].prevIndex){
				//both previous and next index
				item[*item[index].prevIndex].nextIndex = item[index].nextIndex;
				item[*item[index].nextIndex].prevIndex = item[index].prevIndex;
			}else{
				//only next index
				item[*item[index].nextIndex].prevIndex = NULL;
				firstOffset = *item[index].nextIndex;
			}
		}else if(item[index].prevIndex){
			//only previous index
			item[*item[index].prevIndex].nextIndex = NULL;
			lastOffset = *item[index].prevIndex;
		}

		--itemCount;
		return true;
	}

	int getItemIndex(T checkItem){
		for(int i=0;i<itemCount;i++){
			if(item[i].item != checkItem) continue;
			return i;
		}
		return -1;
	}


	/**\brief return the last item from the ordered list.

	The item returned is at the end of the list. The last item
	pushed onto the list will be returned, followed by the item
	that was pushed before it, etc.**/
	T pop(){
		//cout << "[pop]\n";
		//cout << " - remove index " << lastOffset << endl;
        if(lastOffset == -1) return T();
        T val = item[lastOffset].item;
        /*push the index of the item at the last offset before
        the offset might be changed by the sonofabitch code from
        below that stumped me for I don't know how many hours.*/
        freeIndex.push(item[lastOffset].index);
        if(item[lastOffset].prevIndex){
			lastOffset = *item[lastOffset].prevIndex;
			//cout << " - new last index " << lastOffset << endl;
			item[lastOffset].nextIndex = NULL;
        } else {
			lastOffset = -1;
			firstOffset = -1;
        }

        --itemCount;
		return val;
	}

	/**\brief return the first item from the order list.

	The next item in the list becomes the first item.**/
	T unshift(){
		//cout << "[unshift]\n";
		//cout << " - firstOffset: " << firstOffset << endl;
		if(firstOffset == -1) return T();
		T val = item[firstOffset].item;
		freeIndex.push(item[firstOffset].index);
		if(item[firstOffset].nextIndex){
			firstOffset = *item[firstOffset].nextIndex;
			item[firstOffset].prevIndex = NULL;
		} else {
			firstOffset = -1;
			lastOffset = -1;
			//cout << "[nothing left]\n";
		}
		--itemCount;
		return val;
	}

	void print(){
		//cout << "first( " << firstListItem << "): " << firstListItem->item << endl;
		//cout << "last ( " << lastListItem << "): " << lastListItem->item << endl;
		orderedListItem<T> *current = &item[firstOffset];
		while(current){
			cout << "[" << current->index << "](" << current << "): " << current->item << endl;
			cout << "\t\t - index         : " << current->index << endl;
			if(current->prevIndex)
			cout << "\t\t - previous index: " << *current->prevIndex << endl;
			if(current->nextIndex)
			cout << "\t\t - next index    : " << *current->nextIndex << endl;
			if(current->nextIndex){
				current = &item[*current->nextIndex];
			} else {
				current=NULL;
			}
		}
	}

	int getSize(){ return listSize; }
	int getCount(){ return itemCount; }

    /**\brief Allows direct access to items in the list, using it like an array.

    The list is treversed from first node, until the index is reached. If the index
    is greater than the number of items in the linked list, null is returned.**/
    T& operator[](int index) {
    	orderedListItem<T> *ret = &item[firstOffset];
    	while(index > 0){
			if(!ret->nextIndex) return ret->item;
			ret = &item[*ret->nextIndex];
			--index;
    	}
        return ret->item;
    }


	/**\brief Prints everything about the ordered list.

	I added this for debugging because I suck asshole at figuring out how a simple linked list should
	work so I needed all of the help I could get to see what the monkey shit balls was going on.*/
	void betterprint(){
		cout << "\n\n---------------------------------------------\n";
		cout << "-------------ordered list, bitches-----------\n";
		cout << "---------------------------------------------\n";
		cout << "list size : " << listSize << endl;
		cout << "items in list : " << itemCount << endl;
		cout << "---------------------------------------------\n";
		cout << "first offset : " << firstOffset << endl;
		if(firstOffset==-1) return;
		int *i = (firstOffset != -1 ? new int(firstOffset):NULL);
		do{
			cout << *i << endl;
			item[*i].print();
		}while((i = item[*i].nextIndex));
		cout << "last offset : " << lastOffset << endl;
		cout << "---------------------------------------------\n";
	}
};




#endif // ORDERED_LIST_H
