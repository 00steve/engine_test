#ifndef MAP_H
#define MAP_H
#include <string>
#include "list.h"
#include "string_util.h"


/*create a template class that can store an
item of any type along with sorting information
that is needed to keep track of all of the items
in the list. The mapNode class stores the information
to sort it using a binary tree.

The key is stored as a 64 bit int for faster comparisons.
*/
template <class T>
class mapNode {
    public:
        mapNode(T item,string key,unsigned long long int hashKey) :
            item ( item ),
            parentNode(NULL),
            leftNode(NULL),
            rightNode(NULL),
            key(key),
            hsh(hashKey) {
        }

        /*store a reference to the item that was
        passed into the push function of the map*/
        T item;
        /*store the index of the list item that is
        above the current item in the b-tree.*/
        mapNode<T> *parentNode;
        /*stores the intex of the list item that is
        stored to the left of the current item. The
        item to the left is more than the current
        item.*/
        mapNode<T> *leftNode;
        /*store the index of the list item that is
        stored to the right of the item. The item
        to the right is an item that has a lower
        value than the current item.*/
        mapNode<T> *rightNode;
        /*store a reference to the key that is
        associated with the item when an item is
        searched for, the item with this key is
        returned.*/
        string key;
        /*store a hshed value of the key. It is
        a long long (64bit, 8 byte) version of the
        key. It should be much more efficient for
        searching for the proper value key.*/
        unsigned long long int hsh;

};


/*sort, store and return objects based on a string key.*/
template <class T>
class map {
private:

    /*store the root of the binary tree that stores
    all of the items in the map.*/
    mapNode<T> *root;
    /*store a reference to the last item that was
    returned. Storing a pointer can give a huge performance
    boost if the programmer is checking to see if a key
    exists before trying to return it. When an item at
    a key index is called, it will check to see if the
    last checked item's key matches the search key.*/
    mapNode<T> *lastCheck;

    /*store the current node being returned so that nodes
    can be iterated over.*/
    mapNode<T> *current;

	mapNode<T> *smallest;
	mapNode<T> *largest;


    /*store an int to keep track of which directon the cursor
    is moving between nodes.*/
    int iterateMode;
    bool iterateSuccess;

    /*store the number of items in the map. Assuming that
    the count() function is called more often than the
    tree is updated, the size is stored and when a new
    node is added, the count is set as null*/
    int count;

    /*convert a string to a unique integer.
    It takes the char value of the each char, from
    left to right and before adding the next char to
    the value, it does a bit shift up one bit.*/
    unsigned long long int generateHash(string key) {
        unsigned long long int hsh;
        int keyLength = key.length();
        hsh = key[0];
        for(int i=1;i<keyLength;i++)
        {
            hsh <<= 3;
            hsh += key[i];
            //scout << pad_string(binary(hsh),64,string("0"),-1) << endl;
        }
        return hsh;
    }

    /*recursively traverse the tree until
    a node that has not been assigned yet and assign
    the tempNode to that position. If a node existts,
    check to see if the key is smaller or larger and
    run the function again until an empty node is
    reached.*/
    bool placeItem(mapNode<T> * &node, mapNode<T> *tempNode) {
        if(!node) {
            node = tempNode;
            ++count;
            return true;
        }
        if(node->hsh > tempNode->hsh) {
			if(smallest == node) smallest = tempNode;
            if(placeItem(node->leftNode,tempNode))
				node->leftNode->parentNode = node;
            return false;
        }
        else {
        	if(largest == node) largest = tempNode;
            if(placeItem(node->rightNode,tempNode))
				node->rightNode->parentNode = node;
            return false;
        }
    }

	bool iterateTree(T& item){
		/*if the end of a branch is reached, call
		the iterate function again*/
		if(current){
			//item =
		} else {

			//current = current.parentNode;
			iterateMode *= 2;

			switch(iterateMode){
			case -1:
				current = current.leftNode;
				return iterateTree(item);
				break;
			case -2:
				current = current.rightNode;
				iterateMode = 1;
				return iterateTree(item);
			case 2:
				current = current.parentNode;
				iterateMode = 1;
				return iterateTree(item);
			}


			return iterateTree(item);
		}

	}

    /*gets the pointer to an item by treversing the
    tree. If it doesn't find it, it returns null*/
    mapNode<T>* retrieveItem(mapNode<T> *&node, string key,unsigned long long int hsh) {
        if(!node) {
            return NULL;
        }
        if(node->hsh == hsh && node->key == key) {
            return node;
        }
        if(node->hsh > hsh) {
            return retrieveItem(node->leftNode,key,hsh);
        }
        else {
            return retrieveItem(node->rightNode,key,hsh);
        }
    }

	/*a function that treverses the entire tree, and looks for any items
	that match the key, they are returned in a list of items that match.*/
	bool retrieveAllItems(mapNode<T> *&node,list<mapNode<T>*> &matches,string key,unsigned long long int hsh){
		/*if the current node is null, return.*/
        if(!node) {
            return false;
        }
        /*the hash is pretty much an early out, so numbers are being
        compared against each other vs. strings, which could take much
        longer.*/
        if(node->hsh == hsh && node->key == key) {
			matches.push(node);
            //return true;
        }
        /*if the current node has a larger hash than the hash being searched
        for, it means that the target hash has to be in the left node, or one
		of the left node's children.*/
        if(node->hsh > hsh) {
            return retrieveAllItems(node->leftNode,matches,key,hsh);
        }
        else {
            return retrieveAllItems(node->rightNode,matches,key,hsh);
        }
        return true;
	}

	/*removes a node from the map. When a node is removed, if there are no children nodes,
	the node is set to null. If there is only a left or right node, the left or right node
	replaces the node. If there is a left and a right node, the left node replaces it and
	the right node is inserted into its replacement.*/
	void removeNode(mapNode<T> *&node){
		/*if the node being removed has no children*/
		if(!node->leftNode && !node->rightNode){

			/*first disconnect the node from its parent, if it has one.*/
			if(node->parentNode){
				if(node->parentNode->leftNode == node){
					node->parentNode->leftNode = NULL;
				} else {
					node->parentNode->rightNode = NULL;
				}
			}
			node = NULL;
			return;
		}
		if(node->leftNode && node->rightNode){
			placeItem(node->leftNode,node->rightNode);
			node = node->leftNode;
			return;
		}
		if(node->leftNode && !node->rightNode){
			node = node->leftNode;
		}else{
			node = node->rightNode;
		}

	}


	/*DONT USE THIS FUNCTION IN A TIME INTENSIVE OPERATION. <-- upfront warning. Right
	now it does a string compare between every single string in the entire map, which
	is probably crazy slow. If at all possible, use retrieveAllItems(), which will return
	all exact matches. This function returns all items that match the key, but might have
	more junk at the end.*/
	bool retrieveAllChildren(mapNode<T> *&node,map<T> &matches,string key){
		/*if the current node is null, return.*/
        if(!node) {
            return false;
        }
        /*the hash is pretty much an early out, so numbers are being
        compared against each other vs. strings, which could take much
        longer.*/
        if(stringMatchUntil(node->key,key) == key.length()) {
			string nkey = node->key.substr((node->key[key.length()] =='.' ? key.length()+1:key.length()));
			matches.push(node->item,nkey);
        }
        /*Check all of the child nodes for the string match. Right now I can't
        think of a more efficient way, so hopefully this isn't utilized in the
        middle of a time intensive operation*/
        retrieveAllChildren(node->leftNode,matches,key);
        retrieveAllChildren(node->rightNode,matches,key);
        return true;
	}

	/*DONT USE THIS FUNCTION IN A TIME INTENSIVE OPERATION. <-- upfront warning. Right
	now it does a string compare between every single string in the entire map, which
	is probably crazy slow. If at all possible, use retrieveAllItems(), which will return
	all exact matches. This function returns all items that match the key, but might have
	more junk at the end.*/
	bool retrieveAllChildren(mapNode<T> *&node,list<mapNode<T>*> &matches,string key){
		/*if the current node is null, return.*/
        if(!node) {
            return false;
        }
        /*the hash is pretty much an early out, so numbers are being
        compared against each other vs. strings, which could take much
        longer.*/
        if(stringMatchUntil(node->key,key) == key.length()) {
			string nkey = node->key.substr((node->key[key.length()] =='.' ? key.length()+1:key.length()));
			matches.push(node);
        }
        /*Check all of the child nodes for the string match. Right now I can't
        think of a more efficient way, so hopefully this isn't utilized in the
        middle of a time intensive operation*/
        retrieveAllChildren(node->leftNode,matches,key);
        retrieveAllChildren(node->rightNode,matches,key);
        return true;
	}

	void printNode(mapNode<T> *&node){
		if(node->leftNode) printNode(node->leftNode);
		cout << "NODE: " << node->key << endl;
		if(node->rightNode) printNode(node->rightNode);
	}

public:

    map() :
        root (NULL),
        lastCheck (NULL),
        current(NULL),
        smallest(NULL),
        largest(NULL),
        iterateMode(-1),
        count(0) {
    }

	T push(mapNode<T> *node) {
		placeItem(root,node);
		return node->item;
	}

    T push(T newItem,string newKey) {
        /*build a new associative item that will
        be inserted into the tree. Set it's value,
        key and set the other item references to
        null and put the item in the tree wherever it makes
        sense*/
        placeItem(root,new mapNode<T>(newItem,newKey,generateHash(newKey)));
        return newItem;
    }

    /*searches the array for the item with the
    given key. If a matching item does not exist,
    a new item is returned.*/
    bool exists(const string key) {
        /*precheck to see if the search key matches
        the last returned or isset() checked item.*/
        if(lastCheck && lastCheck->key == key) return true;
        /*run standard recursive search for a matching key and
        return the result*/
        unsigned long long int hsh = generateHash(key);
        mapNode<T> *tmp = retrieveItem(root,key,hsh);
        /*if a matching key was returned, store it as the
        last check variable.*/
        return (tmp!=NULL);
    }

    /*searches the array for the item with the
    given key. If a matching item does not exist,
    a new item is returned.*/
    T& operator[](const string key) {
        /*precheck to see if the search key matches
        the last returned or isset() checked item.*/
        if(lastCheck && lastCheck->key == key) return lastCheck->item;
        /*run standard recursive search for a matching key and
        return the result*/
        unsigned long long int hsh = generateHash(key);
        mapNode<T> *tmp = retrieveItem(root,key,hsh);
        /*if a matching key was returned, store it as the
        last check variable.*/
        if(tmp) {lastCheck = tmp;

        } else {
			tmp = new mapNode<T>(T(),"",0);
			//tmp->item = NULL;
        }
		return tmp->item;
    }

	/*iterates over all of the nodes in the
	tree until it has reached the end, then
	returns false.*/
	bool iterate(T &item){
		iterateMode = -1;
		current = root;
        return iterateTree(item);
	}

	bool removeLastNode(){
		if(!lastCheck) return false;
		removeNode(lastCheck);
		return true;
	}

	list<T*> all(const string key) {
		list<mapNode<T>*> nodes = list<mapNode<T>*>();
		list<T*> items = list<T*>();
		retrieveAllItems(root,nodes,key,generateHash(key));
		for(int i=0;i<nodes.getCount();i++){
			items.push(&nodes[i]->item);
		}
		return items;
	}

	/*returns a list of all nodes who share the same beginning
	to their key. For example, the key "root.window" would find
	"root.window.height, root.window.type, and root.window.myFavoriteSomething,
	and return all of them.*/
	map<T> children(const string key){
		map<T> nodes = map<T>();
		if(!root) {

			return nodes;
		}
		retrieveAllChildren(root,nodes,key);
		return nodes;
	}

	list<mapNode<T>*> childrenList(const string key){
		list<mapNode<T>*> nodes = list<mapNode<T>*>();
		retrieveAllChildren(root,nodes,key);
		return nodes;
	}

    bool isSet(const string key) {
        unsigned long long int hsh = generateHash(key);
        return (lastCheck = retrieveItem(root,key,hsh));
    }

	void print(){
		printNode(root);
	}

    T getLastCheck(){
        return lastCheck->item;
    }

	int getCount(){ return count; }

};


#endif // MAP_H
