#ifndef __INGREDIENT_HPP__
#define __INGREDIENT_HPP__

#include "jsonpair.hpp"   // no forma parte de los requerimientos del proyecto
#include "jsonvalue.hpp"  // no forma parte de los requerimientos del proyecto

#include <string>

class Ingredient {
 private:
  std::string name;
  float quantity;
  std::string unit;

 public:
  Ingredient();
  Ingredient(const Ingredient&);
  Ingredient(const std::string&, const float&, const std::string&);

  std::string getName() const;
  float getQuantity() const;
  std::string getUnit() const;

  void setName(const std::string&);
  void setQuantity(const float&);
  void setUnit(const std::string&);

  std::string toString() const;
  JSONValue toJSON() const;
  void fromJson(const JSONValue&);

  Ingredient& operator=(const Ingredient&);

  bool operator==(const Ingredient&) const;
  bool operator!=(const Ingredient&) const;
  bool operator<(const Ingredient&) const;
  bool operator<=(const Ingredient&) const;
  bool operator>(const Ingredient&) const;
  bool operator>=(const Ingredient&) const;

  int compareTo(const Ingredient&) const;
  int compare(const Ingredient&) const;
  static int compare(const Ingredient&, const Ingredient&);

  static int compareByName(const Ingredient&, const Ingredient&);
  static int compareByQuantity(const Ingredient&, const Ingredient&);
  static int compareByUnit(const Ingredient&, const Ingredient&);

  friend std::istream& operator>>(std::istream&, Ingredient&);
  friend std::ostream& operator<<(std::ostream&, const Ingredient&);

  std::string serialize() const;
  void deserialize(const std::string&);
};

#endif  // __INGREDIENT_HPP__