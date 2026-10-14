#include "jsonvalue.hpp"
#include "jsonpair.hpp"

JSONValue::JSONValue() {
  type = NULLTYPE;
}

JSONValue::JSONValue(bool b) {
  type = BOOL;
  boolVal = b;
}

JSONValue::JSONValue(int n) {
  type = NUMBER;
  numVal = (double)n;
}

JSONValue::JSONValue(double n) {
  type = NUMBER;
  numVal = n;
}

JSONValue::JSONValue(const std::string& s) {
  type = STRING;
  stringVal = s;
}

JSONValue::JSONValue(const char* s) {
  type = STRING;
  stringVal = s;
}

JSONValue::JSONValue(const DLList<JSONValue> a) {
  type = ARRAY;
  arrayVal = a;
}

JSONValue::JSONValue(const DLList<JSONPair> o) {
  type = OBJECT;
  objectVal = o;
}

std::string JSONValue::toString(int indent) const {
  switch (type) {
    case NULLTYPE:
      return "null";
    case BOOL:
      return boolVal ? "true" : "false";
    case NUMBER: {
      std::ostringstream oss;
      oss << numVal;
      return oss.str();
    }
    case STRING:
      return escapeString(stringVal);
    case ARRAY: {
      if (arrayVal.isEmpty())
        return "[]";

      std::string result = "[\n";
      DLList<JSONValue>& list = const_cast<DLList<JSONValue>&>(arrayVal);
      DLList<JSONValue>::Position pos = list.getFirstPos();
      bool first = true;
      while (pos != nullptr) {
        if (!first) {
          result += ",\n";
        }
        result += makeIndent(indent + 1);
        result += list.retrieve(pos).toString(indent + 1);
        first = false;
        pos = list.getNextPos(pos);
      }
      result += "\n" + makeIndent(indent) + "]";
      return result;
    }
    case OBJECT: {
      if (objectVal.isEmpty())
        return "{}";

      std::string result = "{\n";
      DLList<JSONPair>& list = const_cast<DLList<JSONPair>&>(objectVal);
      DLList<JSONPair>::Position pos = list.getFirstPos();
      bool first = true;
      while (pos != nullptr) {
        if (!first) {
          result += ",\n";
        }
        JSONPair& par = list.retrieve(pos);
        result += makeIndent(indent + 1);
        result += escapeString(par.key) + ": " + par.value.toString(indent + 1);
        first = false;
        pos = list.getNextPos(pos);
      }
      result += "\n" + makeIndent(indent) + "}";
      return result;
    }
  }
  return "";
}

JSONValue::Type JSONValue::getType() const {
  return type;
}

bool JSONValue::isNull() const {
  return type == NULLTYPE;
}
bool JSONValue::isBool() const {
  return type == BOOL;
}
bool JSONValue::isNumber() const {
  return type == NUMBER;
}
bool JSONValue::isString() const {
  return type == STRING;
}
bool JSONValue::isArray() const {
  return type == ARRAY;
}
bool JSONValue::isObject() const {
  return type == OBJECT;
}

bool JSONValue::getBool() const {
  return boolVal;
}

double JSONValue::getNumber() const {
  return numVal;
}

std::string JSONValue::getString() const {
  return stringVal;
}

size_t JSONValue::arraySize() const {
  size_t count = 0;
  DLList<JSONValue>& list = const_cast<DLList<JSONValue>&>(arrayVal);
  DLList<JSONValue>::Position pos = list.getFirstPos();
  while (pos != nullptr) {
    count++;
    pos = list.getNextPos(pos);
  }
  return count;
}

JSONValue JSONValue::arrayAt(size_t i) const {
  DLList<JSONValue>& list = const_cast<DLList<JSONValue>&>(arrayVal);
  DLList<JSONValue>::Position pos = list.getFirstPos();
  size_t idx = 0;
  while (pos != nullptr) {
    if (idx == i)
      return list.retrieve(pos);
    idx++;
    pos = list.getNextPos(pos);
  }
  throw std::runtime_error("Indice fuera de rango");
}

bool JSONValue::hasKey(const std::string& key) const {
  if (type != OBJECT)
    return false;
  DLList<JSONPair>& list = const_cast<DLList<JSONPair>&>(objectVal);
  DLList<JSONPair>::Position pos = list.getFirstPos();
  while (pos != nullptr) {
    if (list.retrieve(pos).key == key)
      return true;
    pos = list.getNextPos(pos);
  }
  return false;
}

JSONValue JSONValue::get(const std::string& key) const {
  DLList<JSONPair>& list = const_cast<DLList<JSONPair>&>(objectVal);
  DLList<JSONPair>::Position pos = list.getFirstPos();
  while (pos != nullptr) {
    JSONPair& par = list.retrieve(pos);
    if (par.key == key)
      return par.value;
    pos = list.getNextPos(pos);
  }
  throw std::runtime_error("Clave no encontrada: " + key);
}

std::string JSONValue::escapeString(const std::string& s) const {
  std::string result = "\"";
  for (char c : s) {
    switch (c) {
      case '"':
        result += "\\\"";
        break;
      case '\\':
        result += "\\\\";
        break;
      case '\n':
        result += "\\n";
        break;
      case '\t':
        result += "\\t";
        break;
      case '\r':
        result += "\\r";
        break;
      case '\b':
        result += "\\b";
        break;
      case '\f':
        result += "\\f";
        break;
      default:
        result += c;
    }
  }
  result += "\"";
  return result;
}

std::string JSONValue::makeIndent(int level) const {
  return std::string(level * 2, ' ');
}
