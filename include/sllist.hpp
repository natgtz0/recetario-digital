#ifndef _SLList_HPP_
#define _SLList_HPP_

#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Definicion

template <class T>
class SLList {
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
    T data;
    Node* next = nullptr;

   public:
    Node();
    Node(const T&);

    T& getData();
    Position getNext();

    void setData(const T&);
    void setNext(const Position&);
  };
  Position anchor = nullptr;

  bool isValidPos(const Position&) const;

  void addData(const SLList<T>&);

 public:
  SLList();

  SLList(const SLList<T>&);

  ~SLList();

  bool isEmpty() const;

  void insData(const Position&, const T&);

  void insSortedData(const T&);
  void insSortedData(const T&, const function<int(const T&, const T&)>&);

  void delData(const Position&);

  Position getFirstPos() const;

  Position getLastPos() const;

  Position getPrevPos(const Position&) const;

  Position getNextPos(const Position&) const;

  Position find(const T&) const;
  Position find(const T&, const function<int(const T&, const T&)>&) const;

  T& retrieve(const Position&);

  std::string toString() const;

  void delAll();

  SLList<T>& operator=(const SLList<T>&);

  std::string serialize(const char& delim = '#') const;
  void deserialize(const std::string&, const char& delim = '#');

  // Operadores de flujo
  template <class X>
  friend std::ostream& operator<<(std::ostream&, const SLList<X>&);

  template <class X>
  friend std::istream& operator>>(std::istream&, const SLList<X>&);
};

// Implementacion

// Node

template <class T>
SLList<T>::Node::Node() {}

template <class T>
SLList<T>::Node::Node(const T& e) : data(e) {}

template <class T>
T& SLList<T>::Node::getData() {
  return this->data;
}

template <class T>
typename SLList<T>::Position SLList<T>::Node::getNext() {
  return this->next;
}

template <class T>
void SLList<T>::Node::setData(const T& e) {
  this->data = e;
}

template <class T>
void SLList<T>::Node::setNext(const Position& p) {
  this->next = p;
}

// SLList

template <class T>
bool SLList<T>::isValidPos(const Position& p) const {
  Position aux(this->anchor);
  while (aux != nullptr) {
    if (aux == p) {
      return true;
    }
    aux = aux->getNext();
  }
  return false;
}

template <class T>
void SLList<T>::addData(const SLList<T>& other) {
  Position aux(other.anchor), last(this->getLastPos()), newNode;
  try {
    while (aux != nullptr) {
      newNode = new Node(aux->getData());

      if (last == nullptr) {
        this->anchor = newNode;
      } else {
        last->setNext(newNode);
      }

      last = newNode;

      aux = aux->getNext();
    }
  } catch (const bad_alloc& ex) {
    Exception("Memoria no disponible, addData");
  }
}

template <class T>
SLList<T>::SLList() {}

template <class T>
SLList<T>::SLList(const SLList<T>& other) {
  this->addData(other);
}

template <class T>
SLList<T>::~SLList() {
  this->delAll();
}

template <class T>
bool SLList<T>::isEmpty() const {
  return this->anchor == nullptr;
}

template <class T>
void SLList<T>::insData(const Position& p, const T& e) {
  if (p != nullptr and !this->isValidPos(p)) {
    throw Exception("Posición inválida, insData");
  }
  Position newNode(new (nothrow) Node(e));

  if (newNode == nullptr) {
    throw Exception("Memoria no disponible, insData");
  }

  if (p == nullptr) {  // al principio
    newNode->setNext(this->anchor);

    this->anchor = newNode;
  } else {
    newNode->setNext(p->getNext());

    p->setNext(newNode);
  }
}

template <class T>
void SLList<T>::insSortedData(const T& e) {
  Position aux(this->anchor), prev(nullptr);

  while (aux != nullptr and e > aux->getData()) {
    prev = aux;
    aux = aux->getNext();
  }

  Position newNode(new (nothrow) Node(e));

  if (newNode == nullptr) {
    throw Exception("Memoria no disponible, insSortedData");
  }

  if (prev == nullptr) {  // al principio
    newNode->setNext(this->anchor);

    this->anchor = newNode;
  } else {
    newNode->setNext(prev->getNext());

    prev->setNext(newNode);
  }
}

template <class T>
void SLList<T>::insSortedData(const T& e,
                              const function<int(const T&, const T&)>& cmp) {
  Position aux(this->anchor), prev(nullptr);

  while (aux != nullptr and cmp(e, aux->getData()) > 0) {
    prev = aux;
    aux = aux->getNext();
  }

  Position newNode(new (nothrow) Node(e));

  if (newNode == nullptr) {
    throw Exception("Memoria no disponible, insSortedData");
  }

  if (prev == nullptr) {  // al principio
    newNode->setNext(this->anchor);

    this->anchor = newNode;
  } else {
    newNode->setNext(prev->getNext());

    prev->setNext(newNode);
  }
}

template <class T>
void SLList<T>::delData(const Position& p) {
  if (!isValidPos(p)) {
    throw Exception("Posición inválida, delData");
  }
  if (p == this->anchor) {  // el primero
    this->anchor = p->getNext();
  } else {
    this->getPrevPos(p)->setNext(p->getNext());
  }
  delete p;
}

template <class T>
typename SLList<T>::Position SLList<T>::getFirstPos() const {
  return this->anchor;
}

template <class T>
typename SLList<T>::Position SLList<T>::getLastPos() const {
  if (this->isEmpty()) {
    return nullptr;
  }
  Position aux(this->anchor);
  while (aux->getNext() != nullptr) {
    aux = aux->getNext();
  }
  return aux;
}

template <class T>
typename SLList<T>::Position SLList<T>::getPrevPos(const Position& p) const {
  Position aux(this->anchor);
  while (aux != nullptr and aux->getNext() != p) {
    aux = aux->getNext();
  }
  return aux;
}

template <class T>
typename SLList<T>::Position SLList<T>::getNextPos(const Position& p) const {
  return this->isValidPos(p) ? p->getNext() : nullptr;
}

template <class T>
typename SLList<T>::Position SLList<T>::find(const T& e) const {
  Position aux(this->anchor);
  while (aux != nullptr and aux->getData() != e) {
    aux = aux->getNext();
  }
  return aux;
}

template <class T>
typename SLList<T>::Position SLList<T>::find(
    const T& e,
    const function<int(const T&, const T&)>& cmp) const {
  Position aux(this->anchor);
  while (aux != nullptr and cmp(aux->getData(), e) != 0) {
    aux = aux->getNext();
  }
  return aux;
}

template <class T>
T& SLList<T>::retrieve(const Position& p) {
  if (!this->isValidPos(p)) {
    throw Exception("Posición inválida, retrieve");
  }
  return p->getData();
}

template <class T>
std::string SLList<T>::toString() const {
  string result;
  Position aux(this->anchor);
  while (aux != nullptr) {
    result += aux->getData().toString() + "\n";

    aux = aux->getNext();
  }
  return result;
}

template <class T>
void SLList<T>::delAll() {
  Position aux;
  while (this->anchor != nullptr) {
    aux = this->anchor;
    this->anchor = this->anchor->getNext();
    delete aux;
  }
}

template <class T>
SLList<T>& SLList<T>::operator=(const SLList<T>& other) {
  this->delAll();
  this->addData(other);
  return *this;
}

template <class T>
std::string SLList<T>::serialize(const char& delim) const {
  std::string result;
  Position aux(this->anchor);

  while (aux != nullptr) {
    result += aux->getData().serialize() + delim;

    aux = aux->getNext();
  }

  return result;
}

template <class T>
void SLList<T>::deserialize(const std::string& str, const char& delim) {
  std::stringstream mySStr(str);
  std::string myStr;
  T myObj;

  this->delAll();

  while (!mySStr.eof()) {
    getline(mySStr, myStr, delim);

    if (!myStr.empty()) {
      myObj.deserialize(myStr);

      this->insData(this->getLastPos(), myObj);
    }
  }
}

template <class X>
std::ostream& operator<<(std::ostream&, const SLList<X>&) {}

template <class X>
std::istream& operator>>(std::ostream&, const SLList<X>&) {}

#endif  // _SLList_HPP_