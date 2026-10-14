#ifndef _DLList_HPP_
#define _DLList_HPP_

#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

template <class T>
class DLList {
  class Node;

 public:
  typedef Node* Position;

  class Exception : public exception {
   private:
    string msg;

   public:
    Exception() noexcept : msg("Error indefinido") {}
    Exception(const Exception& ex) noexcept : msg(ex.msg) {}
    Exception(const string& m) : msg(m) {}
    Exception& operator=(const Exception& ex) noexcept {
      msg = ex.msg;
      return *this;
    }
    virtual ~Exception() {}
    virtual const char* what() const noexcept { return msg.c_str(); }
  };

 private:
  class Node {
   private:
    T* dataPtr = nullptr;

    Position prev = nullptr;

    Position next = nullptr;

   public:
    class Exception : public DLList<T>::Exception {
     public:
      using DLList<T>::Exception::Exception;
    };
    Node();
    ~Node() { delete dataPtr; }
    Node(const T&);

    T* getDataPtr();

    T& getData();

    Position getNext();

    Position getPrev();

    void setDataPtr(T*);

    void setData(const T&);

    void setNext(const Position&);

    void setPrev(const Position&);
  };

  Position header = new (nothrow) Node;

  bool isValidPos(const Position&) const;

  void addData(const DLList<T>&);

  void swap(const Position&, const Position&);

  void quickSort(const Position&, const Position&);
  void quickSort(const Position&,
                 const Position&,
                 const function<int(const T&, const T&)>&);

 public:
  DLList();

  DLList(const DLList<T>&);

  ~DLList();

  bool isEmpty() const;

  void insData(const Position&, const T&);

  void delData(const Position&);

  Position getFirstPos() const;

  Position getLastPos() const;

  Position getPrevPos(const Position&) const;

  Position getNextPos(const Position&) const;

  Position find(const T&) const;
  Position find(const T&, const function<int(const T&, const T&)>&) const;

  T& retrieve(const Position&);

  void sort();
  void sort(const function<int(const T&, const T&)>&);

  string toString() const;
  string toStringFiltered(const T&) const;
  string toStringFiltered(const T&,
                          const function<int(const T&, const T&)>&) const;

  void delAll();

  DLList<T>& operator=(const DLList<T>&);

  std::string serialize(const char& delim = '#') const;
  void deserialize(const std::string&, const char& delim = '#');

  template <class X>
  friend std::ostream& operator<<(std::ostream&, const DLList<X>&);

  template <class X>
  friend std::istream& operator>>(std::istream&, DLList<X>&);
};

// Implementacion

// Node

template <class T>
DLList<T>::Node::Node() {}

template <class T>
DLList<T>::Node::Node(const T& e) : dataPtr(new (nothrow) T(e)) {
  if (this->dataPtr == nullptr)
    throw Exception("Memoria no disponible, Node");
}

template <class T>
T* DLList<T>::Node::getDataPtr() {
  return this->dataPtr;
}

template <class T>
T& DLList<T>::Node::getData() {
  if (this->dataPtr == nullptr) {
    throw Exception("Dato inexistente, getData");
  }
  return *this->dataPtr;
}

template <class T>
typename DLList<T>::Position DLList<T>::Node::getNext() {
  return this->next;
}

template <class T>
typename DLList<T>::Position DLList<T>::Node::getPrev() {
  return this->prev;
}

template <class T>
void DLList<T>::Node::setDataPtr(T* p) {
  this->dataPtr = p;
}

template <class T>
void DLList<T>::Node::setData(const T& e) {
  if (this->dataPtr == nullptr) {
    if ((this->dataPtr = new (nothrow) T(e)) == nullptr) {
      throw Exception("Memoria no disponible, setData");
    }

    return;
  }

  *this->dataPtr = e;
}

template <class T>
void DLList<T>::Node::setNext(const Position& p) {
  this->next = p;
}

template <class T>
void DLList<T>::Node::setPrev(const Position& p) {
  this->prev = p;
}

// DLList

template <class T>
bool DLList<T>::isValidPos(const Position& p) const {
  Position aux(this->header->getNext());
  while (aux != this->header) {
    if (aux == p) {
      return true;
    }

    aux = aux->getNext();
  }

  return false;
}

template <class T>
void DLList<T>::addData(const DLList<T>& other) {
  Position aux(other.header->getNext());
  Position last(this->getLastPos());
  Position newNode;
  try {
    while (aux != other.header) {
      newNode = new Node(aux->getData());

      if (last == nullptr) {
        this->header->setNext(newNode);
        newNode->setPrev(this->header);
      } else {
        newNode->setPrev(last);
        last->setNext(newNode);
      }
      newNode->setNext(this->header);
      this->header->setPrev(newNode);
      last = newNode;
      aux = aux->getNext();
    }
  } catch (const bad_alloc& ex) {
    throw Exception("Memoria no disponible, addData");
  }
}

template <class T>
DLList<T>::DLList() {
  if (this->header == nullptr) {
    throw Exception("Memoria no disponible, DLList");
  }

  this->header->setPrev(header);
  this->header->setNext(header);
}

template <class T>
DLList<T>::DLList(const DLList<T>& other) : DLList() {
  this->addData(other);
}

template <class T>
DLList<T>::~DLList() {
  this->delAll();
  delete this->header;
}

template <class T>
bool DLList<T>::isEmpty() const {
  return this->header->getNext() == header;
}

template <class T>
void DLList<T>::insData(const Position& p, const T& e) {
  if (p != nullptr && !this->isValidPos(p))
    throw Exception("Posición inválida, insData");

  Position newNode;
  try {
    if ((newNode = new (nothrow) Node(e)) == nullptr)
      throw Exception("Memoria no disponible, insData");
  } catch (const typename Node::Exception& ex) {
    throw Exception(string("Error en el nodo: ") + ex.what());
  }

  Position insPos(p == nullptr ? this->header : p);
  newNode->setPrev(insPos);
  newNode->setNext(insPos->getNext());
  insPos->getNext()->setPrev(newNode);
  insPos->setNext(newNode);
}

template <class T>
void DLList<T>::delData(const Position& p) {
  if (!isValidPos(p))
    throw Exception("Posición inválida, delData");
  p->getPrev()->setNext(p->getNext());
  p->getNext()->setPrev(p->getPrev());
  delete p;
}

template <class T>
typename DLList<T>::Position DLList<T>::getFirstPos() const {
  return this->isEmpty() ? nullptr : this->header->getNext();
}

template <class T>
typename DLList<T>::Position DLList<T>::getLastPos() const {
  return this->isEmpty() ? nullptr : this->header->getPrev();
}

template <class T>
typename DLList<T>::Position DLList<T>::getPrevPos(const Position& p) const {
  return (p->getPrev() == this->header || !this->isValidPos(p)) ? nullptr
                                                                : p->getPrev();
}

template <class T>
typename DLList<T>::Position DLList<T>::getNextPos(const Position& p) const {
  return (p->getNext() == this->header || !this->isValidPos(p)) ? nullptr
                                                                : p->getNext();
}

template <class T>
typename DLList<T>::Position DLList<T>::find(const T& e) const {
  Position aux(this->header->getNext());
  while (aux != this->header) {
    if (aux->getData() == e)
      return aux;
    aux = aux->getNext();
  }
  return nullptr;
}

template <class T>
typename DLList<T>::Position DLList<T>::find(
    const T& e,
    const function<int(const T&, const T&)>& cmp) const {
  Position aux(this->header->getNext());
  while (aux != this->header) {
    if (cmp(aux->getData(), e) == 0) {
      return aux;
    }

    aux = aux->getNext();
  }
  return nullptr;
}

template <class T>
T& DLList<T>::retrieve(const Position& p) {
  if (!this->isValidPos(p)) {
    throw Exception("Posición inválida, retrieve");
  }
  return p->getData();
}

template <class T>
std::string DLList<T>::toString() const {
  string result;
  Position aux(this->header->getNext());

  while (aux != this->header) {
    result += aux->getData().toString() + "\n";

    aux = aux->getNext();
  }
  return result;
}

template <class T>
std::string DLList<T>::toStringFiltered(const T& e) const {
  string result;
  Position aux(this->header->getNext());

  while (aux != this->header) {
    if (aux->getData() == e) {
      result += aux->getData().toString() + "\n";
    }

    aux = aux->getNext();
  }
  return result;
}

template <class T>
std::string DLList<T>::toStringFiltered(
    const T& e,
    const function<int(const T&, const T&)>& cmp) const {
  string result;
  Position aux(this->header->getNext());

  while (aux != this->header) {
    if (cmp(aux->getData(), e) == 0) {
      result += aux->getData().toString() + "\n";
    }

    aux = aux->getNext();
  }
  return result;
}

template <class T>
void DLList<T>::delAll() {
  Position aux;

  while (this->header->getNext() != this->header) {
    aux = this->header->getNext();

    this->header->setNext(aux->getNext());
    aux->getNext()->setPrev(header);

    delete aux;
  }

  this->header->setPrev(this->header);
}

template <class T>
DLList<T>& DLList<T>::operator=(const DLList<T>& other) {
  this->delAll();

  this->addData(other);

  return *this;
}

template <class T>
std::string DLList<T>::serialize(const char& delim) const {
  std::string result;
  Position aux(this->header->getNext());

  while (aux != this->header) {
    result += aux->getData().serialize() + delim;

    aux = aux->getNext();
  }

  return result;
}

template <class T>
void DLList<T>::deserialize(const std::string& str, const char& delim) {
  std::stringstream mySStr(str);
  std::string myStr;
  T myObj;
  Position newNode;

  this->delAll();

  while (!mySStr.eof()) {
    getline(mySStr, myStr, delim);

    if (!myStr.empty()) {
      myObj.deserialize(myStr);

      try {
        if ((newNode = new (std::nothrow) Node(myObj)) == nullptr) {
          throw Exception("Memoria no disponible, deserialize");
        }
      } catch (const typename Node::Exception& ex) {
        throw Exception(ex.what());
      }

      newNode->setPrev(this->header->getPrev());
      newNode->setNext(this->header);

      this->header->getPrev()->setNext(newNode);
      this->header->setPrev(newNode);
    }
  }
}

template <class X>
std::ostream& operator<<(std::ostream& os, const DLList<X>& l) {
  os << l.toString();
  return os;
}

template <class X>
std::istream& operator>>(std::istream& is, DLList<X>& l) {
  return is;
}

// Ordenamiento

template <class T>
void DLList<T>::swap(const Position& a, const Position& b) {
  if (a != b) {
    T* aux(a->getDataPtr());
    a->setDataPtr(b->getDataPtr());
    b->setDataPtr(aux);
  }
}

template <class T>
void DLList<T>::sort() {
  this->quickSort(this->header->getNext(), this->header->getPrev());
}

template <class T>
void DLList<T>::quickSort(const Position& left, const Position& right) {
  if (left == right) {  // Caso de 1 elemento, paro
    return;
  }

  if (left->getNext() == right) {  // Caso 2 elementos, paro
    if (left->getData() > right->getData()) {
      this->swap(left, right);
    }

    return;
  }

  // Particion

  const T& pivot(right->getData());
  Position i(left), j(right);

  while (i != j) {
    while (i != j and i->getData() <= pivot) {
      i = i->getNext();
    }

    while (i != j and j->getData() >= pivot) {
      j = j->getPrev();
    }

    this->swap(i, j);
  }

  this->swap(i, right);

  // Divide y venceras
  if (i != left) {
    this->quickSort(left, i->getPrev());
  }

  if (j != right) {
    this->quickSort(j->getNext(), right);
  }
}

template <class T>
void DLList<T>::sort(const function<int(const T&, const T&)>& cmp) {
  this->quickSort(this->header->getNext(), this->header->getPrev(), cmp);
}

template <class T>
void DLList<T>::quickSort(const Position& left,
                          const Position& right,
                          const function<int(const T&, const T&)>& cmp) {
  if (left == right) {  // Caso de 1 elemento, paro
    return;
  }

  if (left->getNext() == right) {  // Caso 2 elementos, paro
    if (cmp(left->getData(), right->getData()) > 0) {
      this->swap(left, right);
    }

    return;
  }

  // Particion

  const T& pivot(right->getData());
  Position i(left), j(right);

  while (i != j) {
    while (i != j and cmp(i->getData(), pivot) <= 0) {
      i = i->getNext();
    }

    while (i != j and cmp(j->getData(), pivot) >= 0) {
      j = j->getPrev();
    }

    this->swap(i, j);
  }

  this->swap(i, right);

  // Divide y venceras
  if (i != left) {
    this->quickSort(left, i->getPrev(), cmp);
  }

  if (j != right) {
    this->quickSort(j->getNext(), right, cmp);
  }
}

#endif  // _DLList_HPP_