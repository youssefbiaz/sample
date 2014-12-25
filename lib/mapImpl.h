#include <cstddef>

template <class keyType, class valueType>
Map<keyType, valueType>::Map() { // constructor automatically sets head and tail to NULL
  head = NULL;
  tail = NULL;
  traversor = NULL;
}

template <class keyType, class valueType>
Map<keyType, valueType>::Map(const Map<keyType, valueType> &other) {
  //deep copy constructor
  this->head = NULL;
  this->tail = NULL;
  this->traversor = NULL;
  this->merge(other);
}

//operator overload for assignment operator
template <class keyType, class valueType>
const Map<keyType, valueType> &Map<keyType,valueType>::operator=(const Map<keyType, valueType> &other) {
  //if they're the same Map, just return this map and don't worry bout it
  if (this == &other) {
    return *this;
  }

  //otherwise, delete the contents of this map
  MapItem<keyType, valueType> *ptr1 = this->head, *ptr2 = NULL;
  while(ptr1 != NULL) {
    ptr2 = ptr1->next;
    delete ptr1;
    ptr1 = ptr2;
  }

  //re-initialize this map's head and tail ptrs, then merge it with the other map and return
  this->head = NULL;
  this->tail = NULL;
  this->merge(other);
  return *this;
}

template <class keyType, class valueType>
Map<keyType, valueType>::~Map() { // destructor iterates through and deletes everything
  MapItem<keyType, valueType> *ptr1 = head, *ptr2 = NULL;
  while (ptr1 != NULL) {
    ptr2 = ptr1->next;
    delete ptr1;
    ptr1 = ptr2;
  }
}

template <class keyType, class valueType> 
int Map<keyType, valueType>::size() const {  // iterate and return size
  int sizeOfList = 0;
  MapItem<keyType, valueType> *ptr = head; 
  while (ptr != NULL) {
    sizeOfList++;
    ptr = ptr->next;
  }
  return sizeOfList;
}

template <class keyType, class valueType>
void Map<keyType, valueType>::add(const keyType &key, const valueType &value) { // add an element
  bool keyTypeExists = true;
  try {
    get(key);
  } catch (NoSuchElementException &e) {
    keyTypeExists = false;
  }
  if (!keyTypeExists) {
    MapItem<keyType, valueType> *newElement = new MapItem<keyType, valueType>(key, value, NULL, NULL);
    if (head == NULL) {
      head = newElement;
      tail = newElement;
    } else {
      newElement->prev = tail;
      tail->next = newElement;
      tail = newElement; 
      newElement->next = NULL;
    }
  }
}

template <class keyType, class valueType>
void Map<keyType, valueType>::remove(const keyType &key) {
  //check for empty list before doing anything, then remove only if it exists
  if (head != NULL) {
    if (get(key)) {
      MapItem<keyType, valueType> *ptr = head;
      while(ptr != NULL) {
	if (ptr->key == key) {
	  //removing first item in the list
	  if (ptr == head) {
	    //removing first item of a non-one-tem list
	    if (head != tail) {
	      ptr->next->prev = NULL;
	      head = ptr->next;
	    } else { //removing first item of a one-item list
	      head = NULL;
	      tail = NULL;
	    }
	  } else if (ptr == tail) { // remove last item of a lsit
	    tail = ptr->prev;
	    tail->next = NULL;
	  } else { // remove something in the middle of a list
	    ptr->prev->next = ptr->next;
	    ptr->next->prev = ptr->prev;
	  }
	  delete ptr; // delete the item and break
	  break;
	}
	ptr = ptr->next;
      }
    }
  }
}

//this f'n gets one item and returns it and changes bool value to tell
// user if element was found
template <class keyType, class valueType>
const valueType & Map<keyType, valueType>::get(const keyType &key)  const {
  MapItem<keyType, valueType> *ptr;
  ptr = head;
  while (ptr != NULL) {
    if (ptr->key == key) {
      return ptr->value;
    }
    ptr = ptr->next;
  }
  throw NoSuchElementException("key does not exist");
}

//merges two lists but only changes this list w/o changing the other
template <class keyType, class valueType>
void Map<keyType, valueType>::merge(const Map<keyType, valueType> &other) {
  MapItem<keyType, valueType> *ptr = other.head;
  while (ptr != NULL) {
    this->add(ptr->key, ptr->value);
    ptr = ptr->next;
  }
}

//resets traversal to the first element (at the head ptr)
template <class keyType, class valueType>
void Map<keyType, valueType>::first() {
  if (head == NULL) {
    throw NoSuchElementException("There are no elements");
  }
  traversor = head;
}

//moves traversor to next element in the list using next ptr
template <class keyType, class valueType>
void Map<keyType, valueType>::next() {
  if (head == NULL) {
    throw NoSuchElementException("There are no elements");
  } else if (traversor == tail) {
    throw NoSuchElementException("There is no next element.");
  } else if (traversor == NULL) {
    throw NoSuchElementException("Traversor doesn't point to anything yet");
  }
  traversor = traversor->next;
}

template <class keyType, class valueType>
const keyType & Map<keyType, valueType>::getCurrentKey() {
  return traversor->key;
}

template <class keyType, class valueType>
const valueType & Map<keyType, valueType>::getCurrentValue() {
  return traversor->value;
}
