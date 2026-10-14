#ifndef __RECIPES_HPP__
#define __RECIPES_HPP__

#include "ingredient.hpp"
#include "name.hpp"
#include "sllist.hpp"
#include "time.hpp"

#include "jsonpair.hpp"   // no forma parte de los requerimientos del proyecto
#include "jsonvalue.hpp"  // no forma parte de los requerimientos del proyecto

#include <string>

class Recipe {
 private:
  std::string name;
  Name author;
  Time prepTime;
  std::string procedure;
  std::string category;
  SLList<Ingredient> ingredientsList;

 public:
  Recipe();
  Recipe(const Recipe&);

  std::string getName() const;
  Name getAuthor() const;
  Time getPrepTime() const;
  std::string getProcedure() const;
  std::string getCategory() const;
  SLList<Ingredient>& getIngredientsList();

  void setName(const std::string&);
  void setAuthor(const Name&);
  void setPrepTime(const Time&);
  void setProcedure(const std::string&);
  void setCategory(const std::string&);
  void setIngredientsList(const SLList<Ingredient>&);

  std::string toString(const bool& s = false) const;
  static std::string toStringWithIngredientList(const Recipe&);

  JSONValue toJSON() const;         // extra
  void fromJson(const JSONValue&);  // extra

  Recipe& operator=(const Recipe&);

  bool operator==(const Recipe&) const;
  bool operator!=(const Recipe&) const;
  bool operator<(const Recipe&) const;
  bool operator<=(const Recipe&) const;
  bool operator>(const Recipe&) const;
  bool operator>=(const Recipe&) const;

  int compareTo(const Recipe&) const;
  static int compare(const Recipe&, const Recipe&);

  static int compareByName(const Recipe&, const Recipe&);
  static int compareByAuthor(const Recipe&, const Recipe&);
  static int compareByPrepTime(const Recipe&, const Recipe&);
  static int compareByProcedure(const Recipe&, const Recipe&);
  static int compareByCategory(const Recipe&, const Recipe&);

  friend std::istream& operator>>(std::istream&, Recipe&);
  friend std::ostream& operator<<(std::ostream&, const Recipe&);

  std::string serialize() const;
  void deserialize(const std::string&);
};

#endif  // __RECIPES_HPP__