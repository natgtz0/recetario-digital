#ifndef __JSONVALUE_HPP__
#define __JSONVALUE_HPP__

#include <sstream>
#include <string>
#include "dllist.hpp"

class JSONPair;

class JSONValue {
 public:
  enum Type { NULLTYPE, BOOL, NUMBER, STRING, ARRAY, OBJECT };

 private:
  Type type;
  bool boolVal;
  double numVal;
  std::string stringVal;
  DLList<JSONValue> arrayVal;
  DLList<JSONPair> objectVal;

  std::string escapeString(const std::string& s) const;

  std::string makeIndent(int level) const;

 public:
  JSONValue();
  JSONValue(bool b);
  JSONValue(int n);
  JSONValue(double n);
  JSONValue(const std::string& s);
  JSONValue(const char* s);
  JSONValue(const DLList<JSONValue> a);
  JSONValue(const DLList<JSONPair> o);

  std::string toString(int indent = 0) const;

  Type getType() const;
  bool isNull() const;
  bool isBool() const;
  bool isNumber() const;
  bool isString() const;
  bool isArray() const;
  bool isObject() const;

  bool getBool() const;
  double getNumber() const;
  std::string getString() const;

  size_t arraySize() const;
  JSONValue arrayAt(size_t i) const;

  bool hasKey(const std::string& key) const;
  JSONValue get(const std::string& key) const;
};

#endif  // __JSONVALUE_HPP__