#ifndef __JSONPARSER_HPP__
#define __JSONPARSER_HPP__

#include <exception>
#include <string>
#include "jsonpair.hpp"
#include "jsonvalue.hpp"

class JSONParser {
 public:
  class Exception : public std::exception {
   private:
    std::string msg;

   public:
    Exception() noexcept : msg("Error indefinido") {}
    Exception(const Exception& ex) noexcept : msg(ex.msg) {}
    Exception(const std::string& m) : msg(m) {}
    Exception& operator=(const Exception& ex) noexcept {
      msg = ex.msg;
      return *this;
    }
    virtual ~Exception() {}
    virtual const char* what() const noexcept { return msg.c_str(); }
  };

 private:
  std::string source;
  size_t pos;

  char peek();
  char advance();
  void skipWhitespace();
  void expect(char c);
  bool atEnd();

  bool match(const std::string&);

  JSONValue parseNull();
  JSONValue parseBool();
  JSONValue parseNumber();
  std::string parseString();

  JSONValue parseValue();

  JSONValue parseObject();
  JSONValue parseArray();

 public:
  JSONParser(const std::string& s);

  // En public:
  JSONValue parse() { return parseValue(); }
};
#endif  // __JSONPARSER_HPP__