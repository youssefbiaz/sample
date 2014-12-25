
template <class T>
Set<T>::Set() {} //default constructor

template <class T>
Set<T>::Set(const Set<T> &other) {
  //deep copy constructor using a set passed in by reference
  internalStorage.merge(other.internalStorage);
}

template<class T>
Set<T>::~Set() {} // default destructor

template<class T>
const Set<T> &Set<T>::operator=(const Set<T> &other) { // operator overload for assignment
  //if they're the same set, just return this set and don't bother
  if (this == &other) {
    return *this;
  }
  //calls Map's operator overload
  this->internalStorage = other.internalStorage;
  return *this;
}

// the rest of these call Map's equiv f'n to implement Set's f'ns

template<class T>
int Set<T>::size() const { 
  return internalStorage.size();
}

template<class T>
void Set<T>::add(const T &item) {
  internalStorage.add(item, 0);
}

template<class T>
void Set<T>::remove(const T &item) {
  internalStorage.remove(item);
}

template<class T>
bool Set<T>::contains(T item) const {
  try {
    internalStorage.get(item);
  } catch (NoSuchElementException &e) {
    return false;
  }
  return true;
}

template<class T>
void Set<T>::merge(const Set<T> &other) {
  internalStorage.merge(other.internalStorage);
}

template<class T>
void Set<T>::first() {
  internalStorage.first();
}

template<class T>
void Set<T>::next() {
  internalStorage.next();
}

template<class T>
const T & Set<T>::getCurrent() {
  return internalStorage.getCurrentKey();
}

template<class T>
Set<T> Set<T>::setUnion(const Set<T> &other) const {
  Set<T> newSet;
  newSet.merge(*this);
  newSet.merge(other);
  return newSet;
}

//change all this to something that iterates w/o changing traversor
template<class T>
Set<T> Set<T>::setIntersection(const Set<T> &other) const {
  Set<T> setA, setB, setIntersect;
  setA.merge(*this);
  try {
    setA.first();
  } catch (NoSuchElementException &e) {
    return setIntersect;
  }
  setB.merge(other);
  for (int i = 0; i < setA.size(); i++) {
    try {
      setB.first();
    } catch (NoSuchElementException &e) {
      return setIntersect;
    }
    for (int j = 0; j < setB.size(); j++) {
      if (setA.getCurrent() == setB.getCurrent()) {
	setIntersect.add(setA.getCurrent());
      }
      try {
	setB.next();
      } catch (NoSuchElementException &e) {
	break;
      }
    }
    try {
      setA.next();
    }catch (NoSuchElementException &e) {
      break;
    } 
  }
  return setIntersect;
}
