#include "recipe.hpp"
#include <sstream>
#include "ingredient.hpp"

using namespace std;

Recipe::Recipe() {}

Recipe::Recipe(const Recipe& other)
    : name(other.name),
      author(other.author),
      prepTime(other.prepTime),
      procedure(other.procedure),
      category(other.category),
      ingredientsList(other.ingredientsList) {}

string Recipe::getName() const {
  return this->name;
}

Name Recipe::getAuthor() const {
  return this->author;
}

Time Recipe::getPrepTime() const {
  return this->prepTime;
}

string Recipe::getProcedure() const {
  return this->procedure;
}

string Recipe::getCategory() const {
  return this->category;
}

SLList<Ingredient>& Recipe::getIngredientsList() {
  return this->ingredientsList;
}

void Recipe::setName(const string& n) {
  this->name = n;
}

void Recipe::setAuthor(const Name& a) {
  this->author = a;
}

void Recipe::setPrepTime(const Time& p) {
  this->prepTime = p;
}

void Recipe::setProcedure(const string& p) {
  this->procedure = p;
}

void Recipe::setCategory(const string& c) {
  this->category = c;
}

void Recipe::setIngredientsList(const SLList<Ingredient>& l) {
  this->ingredientsList = l;
}

string Recipe::toString(const bool& s) const {
  string result;

  result += this->name;
  result += " | ";
  result += this->category;
  result += " | ";
  result += this->author.toString();
  result += " | ";
  result += this->prepTime.toString();

  if (s) {
    result += "\n\nIngredientes:\n";
    result += "------------------------------------------------------------\n";
    result += this->ingredientsList.toString();
    result += "\nProcedimiento:\n";
    result += "------------------------------------------------------------\n";
    result += this->procedure + "\n";
    result += "============================================================\n";
  }

  return result;
}

string Recipe::toStringWithIngredientList(const Recipe& r) {
  return r.toString(true);
}

JSONValue Recipe::toJSON() const {
  JSONValue authorJSON = this->author.toJSON();
  JSONValue prepTimeJSON = this->prepTime.toJSON();

  DLList<JSONValue> ingredientsList;
  SLList<Ingredient>& list =
      const_cast<SLList<Ingredient>&>(this->ingredientsList);
  SLList<Ingredient>::Position pos = list.getFirstPos();

  while (pos != nullptr) {
    Ingredient& ing = list.retrieve(pos);
    ingredientsList.insData(ingredientsList.getLastPos(), ing.toJSON());
    pos = list.getNextPos(pos);
  }

  JSONValue ingredientsJSON(ingredientsList);

  DLList<JSONPair> pair;

  pair.insData(pair.getLastPos(), JSONPair("name", JSONValue(this->name)));
  pair.insData(pair.getLastPos(),
               JSONPair("category", JSONValue(this->category)));
  pair.insData(pair.getLastPos(),
               JSONPair("procedure", JSONValue(this->procedure)));
  pair.insData(pair.getLastPos(), JSONPair("author", authorJSON));
  pair.insData(pair.getLastPos(), JSONPair("prepTime", prepTimeJSON));
  pair.insData(pair.getLastPos(), JSONPair("ingredients", ingredientsJSON));

  return JSONValue(pair);
}

void Recipe::fromJson(const JSONValue& j) {
  this->name = j.get("name").getString();
  this->category = j.get("category").getString();
  this->procedure = j.get("procedure").getString();

  this->author.fromJson(j.get("author"));
  this->prepTime.fromJson(j.get("prepTime"));

  this->ingredientsList.delAll();
  JSONValue ings = j.get("ingredients");
  for (size_t i = 0; i < ings.arraySize(); i++) {
    Ingredient ing;
    ing.fromJson(ings.arrayAt(i));
    this->ingredientsList.insSortedData(ing, Ingredient::compareByName);
  }
}

Recipe& Recipe::operator=(const Recipe& other) {
  this->name = other.name;
  this->author = other.author;
  this->prepTime = other.prepTime;
  this->procedure = other.procedure;
  this->category = other.category;
  this->ingredientsList = other.ingredientsList;
  return *this;
}

bool Recipe::operator==(const Recipe& other) const {
  return this->name == other.name;
}

bool Recipe::operator!=(const Recipe& other) const {
  return !(*this == other);
}

bool Recipe::operator<(const Recipe& other) const {
  return this->name < other.name;
}

bool Recipe::operator<=(const Recipe& other) const {
  return *this < other || *this == other;
}

bool Recipe::operator>(const Recipe& other) const {
  return !(*this <= other);
}

bool Recipe::operator>=(const Recipe& other) const {
  return !(*this < other);
}

int Recipe::compareTo(const Recipe& other) const {
  return this->name.compare(other.name);
}

int Recipe::compare(const Recipe& a, const Recipe& b) {
  return a.name.compare(b.name);
}

int Recipe::compareByName(const Recipe& a, const Recipe& b) {
  return a.name.compare(b.name);
}

int Recipe::compareByAuthor(const Recipe& a, const Recipe& b) {
  return a.author.compareTo(b.author);
}

int Recipe::compareByPrepTime(const Recipe& a, const Recipe& b) {
  return Time::compare(a.prepTime, b.prepTime);
}

int Recipe::compareByProcedure(const Recipe& a, const Recipe& b) {
  return a.procedure.compare(b.procedure);
}

int Recipe::compareByCategory(const Recipe& a, const Recipe& b) {
  return a.category.compare(b.category);
}

string Recipe::serialize() const {
  string result;

  result += this->name + '*';
  result += this->category + '*';
  result += this->procedure + '*';
  result += this->author.serialize() + '*';
  result += this->prepTime.serialize() + '*';
  result += this->ingredientsList.serialize('@');

  return result;
}

void Recipe::deserialize(const string& str) {
  stringstream mySStr(str);
  string myStr;

  getline(mySStr, this->name, '*');
  getline(mySStr, this->category, '*');
  getline(mySStr, this->procedure, '*');
  getline(mySStr, myStr, '*');
  this->author.deserialize(myStr);
  getline(mySStr, myStr, '*');
  this->prepTime.deserialize(myStr);
  getline(mySStr, myStr);
  this->ingredientsList.deserialize(myStr, '@');
}

ostream& operator<<(ostream& os, const Recipe& r) {
  os << r.name << "\n";
  os << r.category << "\n";
  os << r.procedure << "\n";
  os << r.author;
  os << r.prepTime;

  int count = 0;
  SLList<Ingredient>::Position pos = r.ingredientsList.getFirstPos();
  while (pos != nullptr) {
    count++;
    pos = r.ingredientsList.getNextPos(pos);
  }
  os << count << "\n";

  SLList<Ingredient>& list = const_cast<SLList<Ingredient>&>(r.ingredientsList);
  pos = list.getFirstPos();
  while (pos != nullptr) {
    os << list.retrieve(pos);
    pos = list.getNextPos(pos);
  }

  return os;
}

istream& operator>>(istream& is, Recipe& r) {
  string myStr;

  getline(is, r.name);
  if (r.name.empty())
    return is;

  getline(is, r.category);
  getline(is, r.procedure);
  is >> r.author;
  is >> r.prepTime;

  getline(is, myStr);
  if (myStr.empty())
    return is;

  int count = 0;
  try {
    count = stoi(myStr);
  } catch (...) {
    return is;
  }

  r.ingredientsList.delAll();
  for (int i = 0; i < count; i++) {
    Ingredient myIngredient;
    is >> myIngredient;
    r.ingredientsList.insData(r.ingredientsList.getLastPos(), myIngredient);
  }

  getline(is, myStr);

  return is;
}