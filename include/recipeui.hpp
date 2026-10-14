#ifndef __RECIPEUI_HPP__
#define __RECIPEUI_HPP__

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "dllist.hpp"
#include "ingredientui.hpp"
#include "recipe.hpp"

#include "jsonpair.hpp"    // no forma parte de los requerimientos del proyecto
#include "jsonparser.hpp"  // no forma parte de los requerimientos del proyecto
#include "jsonvalue.hpp"   // no forma parte de los requerimientos del proyecto

class RecipeUI {
 private:
  DLList<Recipe>* myListRef;

  void mainUI();

  void addRecipe();

  void delRecipe();

  void modRecipe();

  void showList();

  void sortData();

  void deleteAll();

  void writeToDisk();

  void writeToDiskJSON();

  void readFromDisk();

  void readFromDiskJSON();

  void findRecipe();

  void enterToContinue();

  void splash();

 public:
  RecipeUI(DLList<Recipe>&);
};

#endif  // __RECIPEUI_HPP__