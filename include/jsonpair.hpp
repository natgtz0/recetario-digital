#ifndef __JSONPAIR_HPP__
#define __JSONPAIR_HPP__

#include <string>
#include "jsonvalue.hpp"

class JSONPair {
 public:
  std::string key;
  JSONValue value;

  JSONPair();
  JSONPair(const std::string&, const JSONValue&);
};

#endif  // __JSONPAIR_HPP__