/*The list class is a template class that lets the programmer
make an unordered list of any type that they choose.

This list is useful for speed intensive operations that do not
mind wasted ram. If only the Push and Pop functions are used
for adding and retrieving items, they will be returned in a
LIFO manner.

The Cut function lets you remove any specific item from the list
based on its index. The item that is removed is returned and the
last item on the list takes its place.

This list does not clear the indexes that are no longer used. If
the list is being used to hold pointers, it should not be that
big of a deal, since pointers have a small memory footprint, but
it can add up if tens or hundreds of thousands of pointers are
being used.
*/

#ifndef SKUTILS_LIST_H
#define SKUTILS_LIST_H
#include <string.h>

#include <iostream>
using namespace std;

template <class T>
class List {
private:

    /*the number of items in the list*/
    int count;

    /*the number of indexes in the list*/
    int size;

    /*create a pointer of the template item to be used to
    hold an array*/
    T *items;

    /*the iterator used to loop over all of the items in the
    list*/
    int it;

    /*if an item was cut and the item at the end of the list
    was shifted into its place and the iterator is already
    past that index, set it as the tmpIt variable so that the
    iterator knows to still check these values*/
    int tmpIt;

    /*Store the index of the last item that was searched.*/
    unsigned int lastSearchIndex;

public:
    List() :
        count(0),
        size(0),
        items(new T[0]),
        it(0),
        tmpIt(-1),
        lastSearchIndex(-1) {
        items = new T[size];
        count = 0;
        this->size = size;
        it = 0;
        tmpIt = -1;
        //cout << "create list\n";
    }

    List(int size) {
        items = new T[size];
        count = 0;
        this->size = size;
        it = 0;
        tmpIt = -1;
    }

    List(int size,T *initArray){
        items = initArray;
        count = size;
        this->size = size;
        it = 0;
        tmpIt = -1;
    }


    ~List() {
		//cout << "delete list\n";
    }

    T Push(T item) {
        //check to see if there are still empty spaces in the list
        if(count == size) {
            /*if the list needs to be incremented, calculate a new size
            for the list, create a temporary pointer, copy all of the items
            from the current pointer,make the current pointer point to the
            new pointer and then delete the temporary pointer.*/
            size = size * 2 + 1;//calculate new size of list
            T *temp = new T[size];//create temporary list of new size
            memcpy(temp,items,sizeof(T)*count);//copy info from the old list to the new one
            items = temp;//store the temp list as the new list
        }

        //add the item to the list and increment count after the item has been added
        items[count++] = item;
        return item;
    }

	bool PushIfNew(T item){
		if(Exists(item)) return false;
		Push(item);
		return true;
	}

    /*removes the last item from the list and returns it. This function can only be used
    if the item being passed to it is a pointer. If it is not a pointer, it will fail when it trys
    to return NULL if nothing is found*/
    T Pop() {
        if(count > 0) return items[--count];
        else return T();
    }

    /*The cut function removes a specific item from the list. The last item
    on the list replaces it so that all of the other items don't need to be
    shifted. If this function is used, the list loses it's order's integrity.
    If the order of the list is important, do not use this function.

    When an item is cut, the iterator should be checked to see if it is
    past the point where the item was cut. If it is, the temp iterator should
    be set to the index of the cut item so that that item is iterated over
    as well.
    */
    T Cut(int index) {
        if(index < count) {
            if(it>index+1) tmpIt = index;
            else if(it == index+1) --it;
            T item = items[index];
            items[index] = items[--count];
            return item;
        }
        return NULL;
    }


    T CutLastSearch(){
        return Cut(lastSearchIndex);
    }

    /*the remove function searches for an item that matches the one
    searched on and if it does, it calls the cut function to remove
    the matching item.*/
    bool Remove(T searchItem){
        for(int i=0;i<count;i++){
            if(items[i] != searchItem) continue;
            Cut(i);
            return true;
        }
        return false;
    }

    bool RemoveAt(int index) {
        if(index < count) {
            if(it>index+1) tmpIt = index;
            else if(it == index+1) --it;
            T item = items[index];
            items[index] = items[--count];
            return true;
        }
        return false;
    }




    /*clear simply resets array's counting variables without clearing any
    memory or doing anything else that can eat up time. The point is to be
    quick n' dirty. This function pretty much assumes that you will be
    refilling this list back up again, overwriting everything that was in it.*/
    void Clear() {
        count = 0;
        tmpIt = -1;
    }

    /*given a specific index, and a new object, it replaces what was
    held at the index with the new object*/
    void ReplaceAt(int index,T item) {
        items[index] = item;
    }

    /*iterate allows the list to return every item in the list 1 time
    and when there are no more items to return, the iterator returns
    NULL and resets the pointer to the beginning of the list.*/
    T* Iterate() {
        /*if an item has been cut, and the iterator was already past it,
        return the item that was swapped into its place.*/
        if(tmpIt != -1)
        {
            int tt = tmpIt;
            tmpIt = -1;
            return &items[tt];
        }
        if(count > it)
        {
            return &items[it++];
        }
        it = 0;
        return NULL;
    }


    /*The exists function checks to see if a value is currently
    stored in the list. If it is, it returns true, if it doesn't,
    it returns false.*/
    bool Exists(T test) {
        for(int i=0;i < count;i++){
            //check to see if the passed in item is the same as the
            //item at a specific index in the list
            if(items[i] != test) continue;
            lastSearchIndex = i;
            return true;
        }//end of each loop through the items

        //if it made it here, no items were found.
        return false;
    }


    /*Returns the size of list, this is how many items can be stored before
    the list needs to increase in size*/
    int GetSize() {
        return size;
    }

    /*Returns the number of items in the list*/
    int GetCount() {
        return count;
    }

    /*Allows direct access to items in the list, using it like an array.*/
    T& operator[](const int index) {
        return items[index];
    }

    /*get the pointer to an object in the array*/
    T* RefAt(const int &index) {
        return &items[index];
    }

    /*get the last item in the list*/
    T& Last(){
        return items[count-1];
    }

    T LastSearch(){
        return items[lastSearchIndex];
    }

    int CursorIndex(){return it-1;}

    void ResetCursor(){it=0;tmpIt=-1;}

};

#endif // SKUTILS_LIST_H
