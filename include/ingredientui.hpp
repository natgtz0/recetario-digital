#ifndef __INGREDIENTUI_HPP__
#define __INGREDIENTUI_HPP__

#include "recipe.hpp"
#include "sllist.hpp"

class IngredientUI {
 private:
  SLList<Ingredient>* listRef;

  void mainUI();

  void addIngredient();

  void delIngredient();

  void modIngredient();

  void showList();

  void deleteAll();

  void enterToContinue();

 public:
  IngredientUI(SLList<Ingredient>&);
};

#endif  // __INGREDIENTUI_HPP__