#include "jsonpair.hpp"

JSONPair::JSONPair() {}

JSONPair::JSONPair(const std::string& k, const JSONValue& v) {
  key = k;
  value = v;
}
