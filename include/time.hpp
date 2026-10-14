#ifndef __TIME_HPP__
#define __TIME_HPP__

#include "jsonpair.hpp"   // no forma parte de los requerimientos del proyecto
#include "jsonvalue.hpp"  // no forma parte de los requerimientos del proyecto

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

class Time {
 private:
  int hour;
  int minute;

  bool isValid(const int&, const int&) const;

  int toInt() const;

 public:
  Time();
  Time(const Time&);
  Time(const int&, const int&);

  int getHour() const;
  int getMinute() const;

  std::string toString() const;
  JSONValue toJSON() const;         // extra
  void fromJson(const JSONValue&);  // extra

  void setHour(const int&);
  void setMinute(const int&);

  Time& operator=(const Time&);

  bool operator==(const Time&) const;
  bool operator!=(const Time&) const;
  bool operator<(const Time&) const;
  bool operator<=(const Time&) const;
  bool operator>(const Time&) const;
  bool operator>=(const Time&) const;

  int compare(const Time&) const;
  static int compare(const Time&, const Time&);

  friend std::istream& operator>>(std::istream&, Time&);
  friend std::ostream& operator<<(std::ostream&, const Time&);

  std::string serialize() const;
  void deserialize(const std::string&);
};

#endif  // __TIME_HPP__