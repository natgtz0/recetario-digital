#ifndef __NAME_HPP__
#define __NAME_HPP__

#include "jsonpair.hpp"   // no forma parte de los requerimientos del proyecto
#include "jsonvalue.hpp"  // no forma parte de los requerimientos del proyecto

#include <iostream>
#include <sstream>
#include <string>

class Name {
 private:
  std::string last;
  std::string first;

 public:
  Name();
  Name(const Name&);
  Name(const std::string&, const std::string&);

  std::string getLast() const;
  std::string getFirst() const;

  std::string toString() const;
  JSONValue toJSON() const;         // extra
  void fromJson(const JSONValue&);  // extra

  void setLast(const std::string&);
  void setFirst(const std::string&);

  Name& operator=(const Name&);

  bool operator==(const Name&) const;
  bool operator!=(const Name&) const;
  bool operator<(const Name&) const;
  bool operator<=(const Name&) const;
  bool operator>(const Name&) const;
  bool operator>=(const Name&) const;

  int compareTo(const Name&) const;
  static int compare(const Name&, const Name&);

  friend std::istream& operator>>(std::istream&, Name&);
  friend std::ostream& operator<<(std::ostream&, const Name&);

  std::string serialize() const;
  void deserialize(const std::string&);
};

#endif  // __NAME_HPP__