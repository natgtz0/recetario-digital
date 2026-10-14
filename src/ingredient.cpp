#include "ingredient.hpp"
#include <sstream>

using namespace std;

Ingredient::Ingredient() : quantity(0.0f) {}

Ingredient::Ingredient(const Ingredient& other)
    : name(other.name), quantity(other.quantity), unit(other.unit) {}

Ingredient::Ingredient(const string& n, const float& q, const string& u)
    : name(n), quantity(q), unit(u) {}

string Ingredient::getName() const {
  return this->name;
}

float Ingredient::getQuantity() const {
  return this->quantity;
}

string Ingredient::getUnit() const {
  return this->unit;
}

void Ingredient::setName(const string& n) {
  this->name = n;
}

void Ingredient::setQuantity(const float& q) {
  this->quantity = q;
}

void Ingredient::setUnit(const string& u) {
  this->unit = u;
}

string Ingredient::toString() const {
  char qty[8];

  sprintf(qty, "%6.2f", this->quantity);

  string namePadded = this->name;
  namePadded.resize(15, ' ');

  return namePadded + " | " + qty + " " + this->unit;
}

JSONValue Ingredient::toJSON() const {
  DLList<JSONPair> pair;
  pair.insData(pair.getLastPos(), JSONPair("name", JSONValue(this->name)));
  pair.insData(pair.getLastPos(),
               JSONPair("quantity", JSONValue(this->quantity)));
  pair.insData(pair.getLastPos(), JSONPair("unit", JSONValue(this->unit)));

  return JSONValue(pair);
}

void Ingredient::fromJson(const JSONValue& j) {
  this->name = j.get("name").getString();
  this->quantity = (float)j.get("quantity").getNumber();
  this->unit = j.get("unit").getString();
}

Ingredient& Ingredient::operator=(const Ingredient& other) {
  this->name = other.name;
  this->quantity = other.quantity;
  this->unit = other.unit;

  return *this;
}

bool Ingredient::operator==(const Ingredient& other) const {
  return this->name == other.name;
}

bool Ingredient::operator!=(const Ingredient& other) const {
  return !(*this == other);
}

bool Ingredient::operator<(const Ingredient& other) const {
  return this->name < other.name;
}

bool Ingredient::operator<=(const Ingredient& other) const {
  return *this < other || *this == other;
}

bool Ingredient::operator>(const Ingredient& other) const {
  return !(*this <= other);
}

bool Ingredient::operator>=(const Ingredient& other) const {
  return !(*this < other);
}

int Ingredient::compareTo(const Ingredient& other) const {
  return this->name.compare(other.name);
}

int Ingredient::compare(const Ingredient& other) const {
  return this->name.compare(other.name);
}

int Ingredient::compare(const Ingredient& a, const Ingredient& b) {
  return a.name.compare(b.name);
}

int Ingredient::compareByName(const Ingredient& a, const Ingredient& b) {
  return a.name.compare(b.name);
}

int Ingredient::compareByQuantity(const Ingredient& a, const Ingredient& b) {
  if (a.quantity < b.quantity)
    return -1;
  if (a.quantity > b.quantity)
    return 1;
  return 0;
}

int Ingredient::compareByUnit(const Ingredient& a, const Ingredient& b) {
  return a.unit.compare(b.unit);
}

ostream& operator<<(ostream& os, const Ingredient& i) {
  os << i.name << endl;
  os << i.quantity << endl;
  os << i.unit;
  return os;
}

istream& operator>>(istream& is, Ingredient& i) {
  string myStr;

  if (!getline(is, i.name))
    return is;
  if (!getline(is, myStr))
    return is;
  i.quantity = stof(myStr);
  getline(is, i.unit);

  return is;
}

string Ingredient::serialize() const {
  string result;
  char qty[16];

  sprintf(qty, "%.2f", this->quantity);

  result += this->name + ',';
  result += string(qty) + ',';
  result += this->unit;

  return result;
}

void Ingredient::deserialize(const string& str) {
  stringstream mySStr(str);
  string myStr;

  getline(mySStr, this->name, ',');
  getline(mySStr, myStr, ',');
  this->quantity = stof(myStr);
  getline(mySStr, this->unit);
}