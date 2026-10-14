#include "name.hpp"

using namespace std;

Name::Name() {}

Name::Name(const Name& other) : last(other.last), first(other.first) {}

Name::Name(const string& l, const string& f) : last(l), first(f) {}

string Name::getLast() const {
  return this->last;
}

string Name::getFirst() const {
  return this->first;
}

string Name::toString() const {
  return this->last + ", " + this->first;
}

JSONValue Name::toJSON() const {
  DLList<JSONPair> pair;
  pair.insData(pair.getLastPos(), JSONPair("first", this->first));
  pair.insData(pair.getLastPos(), JSONPair("last", this->last));

  return JSONValue(pair);
}

void Name::fromJson(const JSONValue& j) {
  this->first = j.get("first").getString();
  this->last = j.get("last").getString();
}

void Name::setLast(const string& l) {
  this->last = l;
}

void Name::setFirst(const string& f) {
  this->first = f;
}

Name& Name::operator=(const Name& other) {
  this->last = other.last;
  this->first = other.first;

  return *this;
}

bool Name::operator==(const Name& other) const {
  return this->toString() == other.toString();
}

bool Name::operator!=(const Name& other) const {
  return !(*this == other);
}

bool Name::operator<(const Name& other) const {
  return this->toString() < other.toString();
}

bool Name::operator<=(const Name& other) const {
  return *this < other || *this == other;
}

bool Name::operator>(const Name& other) const {
  return !(*this <= other);
}

bool Name::operator>=(const Name& other) const {
  return !(*this < other);
}

int Name::compareTo(const Name& other) const {
  return this->toString().compare(other.toString());
}

int Name::compare(const Name& a, const Name& b) {
  return a.toString().compare(b.toString());
}

std::string Name::serialize() const {
  return this->last + ',' + this->first;
}

void Name::deserialize(const std::string& str) {
  stringstream mySStr(str);

  getline(mySStr, this->last, ',');
  getline(mySStr, this->first, '*');
}

ostream& operator<<(ostream& os, const Name& n) {
  os << n.last << endl;
  os << n.first;

  return os;
}

istream& operator>>(istream& is, Name& n) {
  getline(is, n.last);
  getline(is, n.first);

  return is;
}
