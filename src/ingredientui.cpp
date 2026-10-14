#include "ingredientui.hpp"

using namespace std;

IngredientUI::IngredientUI(SLList<Ingredient>& l) : listRef(&l) {
  this->mainUI();
}

void IngredientUI::mainUI() {
  char op;

  do {
    system("cls");

    cout << "***** **** *** ** * Menu de Ingredientes * ** *** **** ******"
         << endl
         << endl;

    cout << "[A]ltas" << endl;
    cout << "[B]ajas" << endl;
    cout << "[C]ambios" << endl;
    cout << "[D]espliegue" << endl;
    cout << "[E]liminar todos" << endl;
    cout << "[S]alir" << endl;

    cout << "Elija una opcion (A,B,C,D,E,S): ";
    cin >> op;
    cin.ignore();

    op = toupper(op);

    switch (op) {
      case 'A':
        this->addIngredient();
        break;
      case 'B':
        this->delIngredient();
        break;
      case 'C':
        this->modIngredient();
        break;
      case 'D':
        this->showList();
        break;
      case 'E':
        this->deleteAll();
        break;
      case 'S':
        cout << "Gracias, adios!!!" << endl;
        this->enterToContinue();
        break;
      default:
        cout << "Opcion erronea, intenta de nuevo" << endl;
        this->enterToContinue();
    }
  } while (op != 'S');
}

void IngredientUI::addIngredient() {
  Ingredient myIngredient;
  string myStr;
  float myFloat;

  system("cls");

  cout << "***** **** *** ** * Anadir nuevo Ingrediente * ** *** **** ******"
       << endl
       << endl;

  cout << "Nombre: ";
  getline(cin, myStr);
  myIngredient.setName(myStr);

  if (this->listRef->find(myIngredient) != nullptr) {
    cout << "No se puede dar de alta un ingrediente duplicado";
    this->enterToContinue();
    return;
  }

  cout << "Cantidad: ";
  cin >> myFloat;
  cin.ignore();
  myIngredient.setQuantity(myFloat);

  cout << "Unidad de medida: ";
  getline(cin, myStr);
  myIngredient.setUnit(myStr);

  try {
    this->listRef->insSortedData(myIngredient, Ingredient::compareByName);
  } catch (const SLList<Ingredient>::Exception& ex) {
    cout << "Error al tratar de insertar el nuevo ingrediente" << endl;
    cout << "Mensaje de error: " << ex.what() << endl;
    enterToContinue();
    return;
  }

  cout << endl << endl;
  cout << "Ingrediente anadido correctamente" << endl << endl;
  enterToContinue();
}

void IngredientUI::delIngredient() {
  string myStr;
  Ingredient myIngredient;
  SLList<Ingredient>::Position pos;
  char op;

  system("cls");

  cout << "***** **** *** ** * Eliminar Ingrediente * ** *** **** ******"
       << endl
       << endl;

  cout << "Nombre: ";
  getline(cin, myStr);
  myIngredient.setName(myStr);

  pos = this->listRef->find(myIngredient);

  if (pos == nullptr) {
    cout << "No se puede eliminar un ingrediente inexistente";
    enterToContinue();
    return;
  }

  cout << "Ingrediente localizado" << endl;
  cout << "Detalle del ingrediente:" << endl;
  cout << this->listRef->retrieve(pos).toString() << endl << endl;

  do {
    cout << "Estas seguro? (S/N): ";
    cin >> op;
    cin.ignore();
    op = toupper(op);
  } while (op != 'N' && op != 'S');

  if (op == 'S') {
    this->listRef->delData(pos);
    cout << "Ingrediente eliminado!!!" << endl << endl;
    this->enterToContinue();
  }
}

void IngredientUI::modIngredient() {
  string myStr;
  Ingredient myIngredient;
  Ingredient original;
  SLList<Ingredient>::Position pos;
  float myFloat;

  system("cls");

  cout << "-------------------------- Modificar Ingrediente "
          "--------------------------"
       << endl
       << endl;

  cout << "Nombre del ingrediente a modificar: ";
  getline(cin, myStr);
  myIngredient.setName(myStr);

  pos = this->listRef->find(myIngredient);

  if (pos == nullptr) {
    cout << "No se encontro el ingrediente especificado." << endl;
    enterToContinue();
    return;
  }

  original = this->listRef->retrieve(pos);

  cout << endl << "Ingrediente encontrado:" << endl;
  cout << original.toString() << endl << endl;
  cout << "Ingrese los nuevos datos (ENTER para conservar el valor actual):"
       << endl
       << endl;

  cout << "Nuevo nombre [" << original.getName() << "]: ";
  getline(cin, myStr);
  if (!myStr.empty()) {
    Ingredient tempCheck;
    tempCheck.setName(myStr);
    SLList<Ingredient>::Position dupPos = this->listRef->find(tempCheck);
    if (dupPos != nullptr && dupPos != pos) {
      cout << "Ya existe un ingrediente con ese nombre. No se realizaron "
              "cambios."
           << endl;
      enterToContinue();
      return;
    }
    myIngredient.setName(myStr);
  } else {
    myIngredient.setName(original.getName());
  }

  cout << "Nueva cantidad [" << original.getQuantity() << "]: ";
  getline(cin, myStr);
  if (!myStr.empty()) {
    try {
      myFloat = stof(myStr);
    } catch (...) {
      cout << "Cantidad invalida. Se conserva el valor anterior." << endl;
      myFloat = original.getQuantity();
    }
    myIngredient.setQuantity(myFloat);
  } else {
    myIngredient.setQuantity(original.getQuantity());
  }

  cout << "Nueva unidad de medida [" << original.getUnit() << "]: ";
  getline(cin, myStr);
  if (!myStr.empty()) {
    myIngredient.setUnit(myStr);
  } else {
    myIngredient.setUnit(original.getUnit());
  }

  try {
    this->listRef->delData(pos);
    this->listRef->insSortedData(myIngredient, Ingredient::compareByName);
  } catch (const SLList<Ingredient>::Exception& ex) {
    cout << "Error al modificar el ingrediente." << endl;
    cout << "Mensaje de error: " << ex.what() << endl;
    enterToContinue();
    return;
  }

  cout << endl << "Ingrediente modificado correctamente." << endl << endl;
  enterToContinue();
}

void IngredientUI::showList() {
  system("cls");

  cout << "**** *** ** * Listado de Ingredientes * ** *** ****" << endl << endl;
  cout << "Nombre                    Cantidad  Unidad" << endl;
  cout << "-----------------------------------------------------------" << endl;

  cout << this->listRef->toString();

  cout << endl << endl;
  enterToContinue();
}

void IngredientUI::deleteAll() {
  char op;

  system("cls");

  cout << "**** *** ** * Eliminar Todos los Ingredientes * ** *** ****" << endl
       << endl;

  if (this->listRef->isEmpty()) {
    cout << "La lista de ingredientes ya esta vacia.";
    enterToContinue();
    return;
  }

  int count = 0;
  SLList<Ingredient>::Position p = this->listRef->getFirstPos();
  while (p != nullptr) {
    count++;
    p = this->listRef->getNextPos(p);
  }

  cout << "Hay " << count << " ingrediente(s) en la lista." << endl << endl;

  do {
    cout << "Estas seguro de que quieres eliminar TODOS los ingredientes? "
            "(S/N): ";
    cin >> op;
    cin.ignore();
    op = toupper(op);
  } while (op != 'S' && op != 'N');

  if (op == 'S') {
    this->listRef->delAll();
    cout << endl
         << "Todos los ingredientes han sido eliminados." << endl
         << endl;
  }

  this->enterToContinue();
}

void IngredientUI::enterToContinue() {
  cout << "[ENTER] para continuar...";
  cin.get();
}