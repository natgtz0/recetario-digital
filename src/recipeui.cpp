#include "recipeui.hpp"
#include "ingredientui.hpp"

using namespace std;

RecipeUI::RecipeUI(DLList<Recipe>& l) : myListRef(&l) {
  mainUI();
}

void RecipeUI::mainUI() {
  char op;

  this->splash();

  do {
    system("cls");

    cout << "---------------------- Menu de Recetas ----------------------"
         << endl
         << endl;
    cout << "[A]: Anadir receta" << endl;
    cout << "[E]: Eliminar receta" << endl;
    cout << "[R]: Realizar un cambio" << endl;
    cout << "[M]: Mostrar lista" << endl;
    cout << "[O]: Ordenar" << endl;
    cout << "[S]: Escribir a disco" << endl;
    cout << "[J]: Escribir a disco JSON (extra)*****" << endl;
    cout << "[L]: Leer del disco" << endl;
    cout << "[D]: Leer del disco JSON (extra)*****" << endl;
    cout << "[B]: Buscar" << endl;
    cout << "[N]: Eliminar todo" << endl;
    cout << "[I]: Salir" << endl << endl;

    cout << "Elija una opcion: ";
    cin >> op;
    op = toupper(op);
    cin.ignore();

    switch (op) {
      case 'A':
        this->addRecipe();
        break;
      case 'E':
        this->delRecipe();
        break;
      case 'R':
        this->modRecipe();
        break;
      case 'M':
        this->showList();
        break;
      case 'O':
        this->sortData();
        break;
      case 'S':
        this->writeToDisk();
        break;
      case 'J':
        this->writeToDiskJSON();
        break;
      case 'L':
        this->readFromDisk();
        break;
      case 'D':
        this->readFromDiskJSON();
        break;
      case 'B':
        this->findRecipe();
        break;
      case 'N':
        this->deleteAll();
        break;
      case 'I':
        cout << "Saliendo...";
        return;
      default:
        cout << "Opcion erronea, intenta de nuevo...";
        this->enterToContinue();
        break;
    }
  } while (op != 'I');
}

void RecipeUI::addRecipe() {
  Recipe myRecipe;
  Name myAuthor;
  Time myTime;
  string myStr;
  char op;

  system("cls");

  cout << "---------------------- Anadir Receta ----------------------" << endl
       << endl;

  cout << "Nombre de la receta: ";
  getline(cin, myStr);
  myRecipe.setName(myStr);

  if (this->myListRef->find(myRecipe) != nullptr) {
    cout << "No se puede dar de alta una receta duplicada";
    enterToContinue();
    return;
  }

  cout << "Apellido(s) del autor: ";
  getline(cin, myStr);
  myAuthor.setLast(myStr);

  cout << "Nombre(s) del autor: ";
  getline(cin, myStr);
  myAuthor.setFirst(myStr);
  myRecipe.setAuthor(myAuthor);

  cout << "Tiempo de preparacion (HH:MM): ";
  getline(cin, myStr, ':');
  myTime.setHour(stoi(myStr));
  getline(cin, myStr);
  myTime.setMinute(stoi(myStr));
  myRecipe.setPrepTime(myTime);

  cout << "Categoria (Desayuno/Comida/Cena/Navideno): ";
  getline(cin, myStr);
  myRecipe.setCategory(myStr);

  cout << "Procedimiento: ";
  getline(cin, myStr);
  myRecipe.setProcedure(myStr);

  do {
    cout << "Trabajaremos con la lista de ingredientes? (S/N): ";
    cin >> op;
    cin.ignore();
    op = toupper(op);
  } while (op != 'N' && op != 'S');

  if (op == 'S') {
    IngredientUI ingredientUI(myRecipe.getIngredientsList());
  }

  this->myListRef->insData(this->myListRef->getLastPos(), myRecipe);

  cout << endl << "Receta insertada..." << endl << endl;
  this->enterToContinue();
}

void RecipeUI::delRecipe() {
  string myStr;
  Recipe myRecipe;
  typename DLList<Recipe>::Position pos;

  system("cls");

  cout << "Nombre de la receta a eliminar: ";
  getline(cin, myStr);
  myRecipe.setName(myStr);

  pos = this->myListRef->find(myRecipe);

  cout << endl << endl;

  if (pos == nullptr) {
    cout << "La receta " << myStr << " no existe en la lista";
  } else {
    this->myListRef->delData(pos);
    cout << "Receta " << myStr << " eliminada";
  }

  cout << endl << endl;
  this->enterToContinue();
}

void RecipeUI::modRecipe() {
  string myStr;
  Recipe myRecipe;
  DLList<Recipe>::Position pos;
  char op;

  system("cls");

  cout << "---------------------- Modificar Receta ----------------------"
       << endl
       << endl;

  cout << "Nombre de la receta a modificar: ";
  getline(cin, myStr);
  myRecipe.setName(myStr);

  pos = this->myListRef->find(myRecipe);

  if (pos == nullptr) {
    cout << "No existe una receta con ese nombre";
    enterToContinue();
    return;
  }

  Recipe& r = this->myListRef->retrieve(pos);

  cout << endl << "Receta encontrada:" << endl;
  cout << r.toString() << endl << endl;
  cout << "Ingrese los nuevos datos (ENTER para conservar el valor actual):"
       << endl
       << endl;

  cout << "Nuevo nombre [" << r.getName() << "]: ";
  getline(cin, myStr);
  if (!myStr.empty())
    r.setName(myStr);

  cout << "Nueva categoria [" << r.getCategory() << "]: ";
  getline(cin, myStr);
  if (!myStr.empty())
    r.setCategory(myStr);

  cout << "Nuevo procedimiento [" << r.getProcedure() << "]: ";
  getline(cin, myStr);
  if (!myStr.empty())
    r.setProcedure(myStr);

  do {
    cout << "Modificar lista de ingredientes? (S/N): ";
    cin >> op;
    cin.ignore();
    op = toupper(op);
  } while (op != 'N' && op != 'S');

  if (op == 'S') {
    IngredientUI ingredientUI(r.getIngredientsList());
  }

  cout << endl << "Receta modificada correctamente." << endl << endl;
  this->enterToContinue();
}

void RecipeUI::showList() {
  char op;
  Recipe myRecipe;
  DLList<Recipe>::Position pos;

  system("cls");

  cout << "Listado de recetas" << endl << endl;

  do {
    cout << "[T]odas" << endl;
    cout << "Solo [D]esayuno" << endl;
    cout << "Solo [C]omida" << endl;
    cout << "Solo c[E]na" << endl;
    cout << "Solo [N]avideno" << endl << endl;

    cout << "Elija una opcion (T, D, C, E, N): ";
    cin >> op;
    cin.ignore();
    op = toupper(op);
  } while (op != 'T' and op != 'D' and op != 'C' and op != 'E' and op != 'N');

  switch (op) {
    case 'T':
      cout << "Incluir lista de ingredientes? (S/N): ";
      cin >> op;
      cin.ignore();
      op = toupper(op);

      if (op == 'S') {
        pos = myListRef->getFirstPos();
        while (pos != nullptr) {
          cout << myListRef->retrieve(pos).toString(true) << endl;
          pos = myListRef->getNextPos(pos);
        }
      } else {
        cout << this->myListRef->toString();
      }
      break;

    case 'D':
      myRecipe.setCategory("Desayuno");
      cout << this->myListRef->toStringFiltered(myRecipe,
                                                Recipe::compareByCategory);
      break;

    case 'C':
      myRecipe.setCategory("Comida");
      cout << this->myListRef->toStringFiltered(myRecipe,
                                                Recipe::compareByCategory);
      break;

    case 'E':
      myRecipe.setCategory("Cena");
      cout << this->myListRef->toStringFiltered(myRecipe,
                                                Recipe::compareByCategory);
      break;

    case 'N':
      myRecipe.setCategory("Navideno");
      cout << this->myListRef->toStringFiltered(myRecipe,
                                                Recipe::compareByCategory);
      break;

    default:
      break;
  }
  cout << endl << endl;

  this->enterToContinue();
}

void RecipeUI::sortData() {
  char op;

  system("cls");

  cout << "---------------------- Ordenar Recetas ----------------------"
       << endl
       << endl;
  cout << "1. Por nombre" << endl;
  cout << "2. Por tiempo de preparacion" << endl;
  cout << "3. Por categoria" << endl;
  cout << "4. Por autor" << endl << endl;

  cout << "Elija criterio: ";
  cin >> op;
  cin.ignore();

  switch (op) {
    case '1':
      this->myListRef->sort(Recipe::compareByName);
      break;
    case '2':
      this->myListRef->sort(Recipe::compareByPrepTime);
      break;
    case '3':
      this->myListRef->sort(Recipe::compareByCategory);
      break;
    case '4':
      this->myListRef->sort(Recipe::compareByAuthor);
      break;
    default:
      cout << "Opcion invalida";
      this->enterToContinue();
      return;
  }

  cout << endl << "Lista ordenada!" << endl << endl;
  cout << this->myListRef->toString() << endl;
  this->enterToContinue();
}

void RecipeUI::deleteAll() {
  char op;

  system("cls");

  cout << "---------------------- Eliminar Todas las Recetas "
          "----------------------"
       << endl
       << endl;

  if (this->myListRef->isEmpty()) {
    cout << "La lista ya esta vacia.";
    enterToContinue();
    return;
  }

  int count = 0;
  DLList<Recipe>::Position p = this->myListRef->getFirstPos();
  while (p != nullptr) {
    count++;
    p = this->myListRef->getNextPos(p);
  }

  cout << "Hay " << count << " receta(s) en la lista." << endl << endl;

  do {
    cout << "Estas seguro de que quieres eliminar TODAS las recetas? (S/N): ";
    cin >> op;
    cin.ignore();
    op = toupper(op);
  } while (op != 'S' && op != 'N');

  if (op == 'S') {
    this->myListRef->delAll();
    cout << endl << "Todas las recetas han sido eliminadas." << endl << endl;
  }

  this->enterToContinue();
}

void RecipeUI::writeToDisk() {
  string fileName;

  system("cls");

  cout << "**** *** ** * Escribiendo al disco * ** *** ****" << endl << endl;

  cout << "Nombre del archivo: ";
  getline(cin, fileName);

  cout << endl << endl;

  ofstream myFile(fileName, ios_base::trunc);

  if (!myFile.is_open()) {
    cout << "Error, el archivo " << fileName
         << " no se pudo abrir para escribir" << endl;
    this->enterToContinue();
    return;
  }

  cout << "Escribiendo al archivo " << fileName << "..." << endl;

  myFile << this->myListRef->serialize();

  myFile.close();

  cout << "Operacion completada" << endl;
  this->enterToContinue();
}

void RecipeUI::writeToDiskJSON() {
  string fileName;

  system("cls");

  cout << "**** *** ** * Escribiendo a JSON * ** *** ****" << endl << endl;

  cout << "Nombre del archivo: ";
  getline(cin, fileName);

  cout << endl << endl;

  ofstream myFile(fileName, ios_base::trunc);

  if (!myFile.is_open()) {
    cout << "Error, el archivo " << fileName
         << " no se pudo abrir para escribir" << endl;
    this->enterToContinue();
    return;
  }

  cout << "Escribiendo al archivo " << fileName << "..." << endl;

  DLList<JSONValue> recipesArray;
  DLList<Recipe>::Position pos = this->myListRef->getFirstPos();
  while (pos != nullptr) {
    Recipe& r = this->myListRef->retrieve(pos);
    recipesArray.insData(recipesArray.getLastPos(), r.toJSON());
    pos = this->myListRef->getNextPos(pos);
  }

  DLList<JSONPair> rootPairs;
  rootPairs.insData(rootPairs.getLastPos(),
                    JSONPair("recipes", JSONValue(recipesArray)));
  JSONValue root(rootPairs);

  myFile << root.toString();
  myFile.close();

  cout << "Operacion completada" << endl;
  this->enterToContinue();
}

void RecipeUI::readFromDisk() {
  string fileName, fileStr;

  system("cls");

  cout << "**** *** ** * Leyendo del disco * ** *** ****" << endl << endl;

  cout << "Nombre del archivo: ";
  getline(cin, fileName);

  cout << endl << endl;

  ifstream myFile(fileName);

  if (!myFile.is_open()) {
    cout << "Error, el archivo " << fileName << " no se pudo abrir para lectura"
         << endl;
    this->enterToContinue();
    return;
  }

  cout << "Leyendo del archivo " << fileName << "..." << endl;

  getline(myFile, fileStr);

  myFile.close();

  this->myListRef->deserialize(fileStr);

  cout << "Proceso terminado" << endl;
  this->enterToContinue();
}

void RecipeUI::readFromDiskJSON() {
  std::string fileName;

  system("cls");

  std::cout << "**** *** ** * Leyendo de JSON * ** *** ****" << std::endl
            << std::endl;

  std::cout << "Nombre del archivo: ";
  std::getline(std::cin, fileName);

  std::cout << std::endl << std::endl;

  std::ifstream myFile(fileName);

  if (!myFile.is_open()) {
    std::cout << "Error, el archivo " << fileName
              << " no se pudo abrir para lectura" << std::endl;
    this->enterToContinue();
    return;
  }

  std::cout << "Leyendo del archivo " << fileName << "..." << std::endl;

  std::stringstream ss;
  ss << myFile.rdbuf();
  std::string content = ss.str();
  myFile.close();

  try {
    JSONParser parser(content);
    JSONValue root = parser.parse();

    JSONValue recipesArray = root.get("recipes");

    this->myListRef->delAll();
    for (size_t i = 0; i < recipesArray.arraySize(); i++) {
      Recipe r;
      r.fromJson(recipesArray.arrayAt(i));
      this->myListRef->insData(this->myListRef->getLastPos(), r);
    }

    std::cout << "Lectura completada: " << recipesArray.arraySize()
              << " receta(s) cargada(s)" << std::endl;

  } catch (const JSONParser::Exception& ex) {
    std::cout << "Error parseando JSON: " << ex.what() << std::endl;
  } catch (const std::exception& ex) {
    std::cout << "Error procesando datos: " << ex.what() << std::endl;
  }

  this->enterToContinue();
}

void RecipeUI::findRecipe() {
  string myStr;
  Recipe myRecipe;
  DLList<Recipe>::Position pos;
  char op;

  system("cls");

  cout << "---------------------- Buscar Receta ----------------------" << endl
       << endl;
  cout << "1. Buscar por nombre" << endl;
  cout << "2. Buscar por categoria" << endl << endl;

  cout << "Elija opcion: ";
  cin >> op;
  cin.ignore();

  if (op == '1') {
    cout << "Nombre de la receta: ";
    getline(cin, myStr);
    myRecipe.setName(myStr);
    pos = this->myListRef->find(myRecipe);

    if (pos == nullptr) {
      cout << endl << "No existe una receta con el nombre " << myStr;
    } else {
      cout << endl << "Receta encontrada:" << endl << endl;
      cout << this->myListRef->retrieve(pos).toString(true);
    }
  } else if (op == '2') {
    cout << "Categoria (Desayuno/Comida/Cena/Navideno): ";
    getline(cin, myStr);
    myRecipe.setCategory(myStr);
    pos = this->myListRef->find(myRecipe, Recipe::compareByCategory);

    if (pos == nullptr) {
      cout << endl << "No existe una receta con la categoria " << myStr;
    } else {
      cout << endl << "Primera receta encontrada:" << endl << endl;
      cout << this->myListRef->retrieve(pos).toString(true);
    }
  } else {
    cout << "Opcion invalida";
  }

  cout << endl << endl;
  this->enterToContinue();
}

void RecipeUI::enterToContinue() {
  cout << "[ENTER] para continuar...";
  cin.get();
}

void RecipeUI::splash() {
  system("cls");
  cout << endl << endl;
  cout << "                                                                  "
       << endl;
  cout << "                            .-+#@@@%##%@@%*++++**#%@%%*=.         "
       << endl;
  cout << "                     .=%@@%=:.       ....            ...:%#       "
       << endl;
  cout << "                .+@@%:               ......       .........@.     "
       << endl;
  cout << "             -@@+                    ....@:.     .....-+...-#     "
       << endl;
  cout << "          -%%-     -#...             ...:@...   ....:%=.... @.    "
       << endl;
  cout << "        #@-         :@:...           ..-@-...  ....=@:....  %:    "
       << endl;
  cout << "      +@.           .-@:...          ..@+...   ...+@:...    @     "
       << endl;
  cout << "     @=             ..@=....         .+%...    ..-@:...    +-     "
       << endl;
  cout << "    @-              ..-@....          @+..     .-@-..     .*        "
          "           --- RECETARIO DIGITAL ---"
       << endl;
  cout << "   %=#.              ..*%..           @:       .%+..     -*       "
       << endl;
  cout << "  -#.+*               ..-@..          @        :%...    #.          "
          "            [ENTER] para continuar."
       << endl;
  cout << "  @ ..#%                ..@-.      .-+%###*=.  =-... .@.          "
       << endl;
  cout << " :%  ...+#@@@@#-.      :-*@@@%##*-.            :*#*--             "
       << endl;
  cout << "  @.   .........-#.*@*=:    -**%@@@@@@%####%@@@*=  ==             "
       << endl;
  cout << "   =@=          :%.  .%@@@-...                   ..#=             "
       << endl;
  cout << "         .-+##*-:+=.                               :#             "
       << endl;
  cout << "                 :@                                 @:            "
       << endl;
  cout << "                  @:                                +%            "
       << endl;
  cout << "                 .@   .:-#@@@%%%#*+-::::::::-=*#%%%--@            "
       << endl;
  cout << "                 -+=+-:..............................-            "
       << endl;
  cout << "                  #:...............................=%.            "
       << endl;
  cout << "                    +@#-:....................:-%@#.               "
       << endl;
  cout << "                        -+#@@@@%%##%%%@@@@#+-.                    "
       << endl;
  cout << "                                                                  "
       << endl;
  cin.get();
}