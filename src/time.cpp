#include "time.hpp"

using namespace std;
using namespace chrono;

bool Time::isValid(const int& h, const int& m) const {
  return h >= 0 && h < 24 && m >= 0 && m < 60;
}

int Time::toInt() const {
  return this->hour * 100 + this->minute;
}

Time::Time() {
  system_clock::time_point today(system_clock::now());
  time_t tt(system_clock::to_time_t(today));
  struct tm* timeinfo(localtime(&tt));

  this->hour = timeinfo->tm_hour;
  this->minute = timeinfo->tm_min;
}

Time::Time(const Time& t) : hour(t.hour), minute(t.minute) {}

Time::Time(const int& h, const int& m) {
  if (this->isValid(h, m)) {
    this->hour = h;
    this->minute = m;
  }
}

int Time::getHour() const {
  return this->hour;
}

int Time::getMinute() const {
  return this->minute;
}

string Time::toString() const {
  char myTime[7];

  sprintf(myTime, "%02i:%02i", this->hour, this->minute);

  return myTime;
}

JSONValue Time::toJSON() const {
  DLList<JSONPair> pair;
  pair.insData(pair.getLastPos(), JSONPair("hour", this->hour));
  pair.insData(pair.getLastPos(), JSONPair("minute", this->minute));

  return JSONValue(pair);
}

void Time::fromJson(const JSONValue& j) {
  this->hour = (int)j.get("hour").getNumber();
  this->minute = (int)j.get("minute").getNumber();
}

void Time::setHour(const int& h) {
  if (this->isValid(h, 0)) {
    this->hour = h;
  }
}

void Time::setMinute(const int& m) {
  if (this->isValid(0, m)) {
    this->minute = m;
  }
}

Time& Time::operator=(const Time& t) {
  this->hour = t.hour;
  this->minute = t.minute;

  return *this;
}

bool Time::operator==(const Time& t) const {
  return this->toInt() == t.toInt();
}

bool Time::operator!=(const Time& t) const {
  return !(*this == t);
}

bool Time::operator<(const Time& t) const {
  return this->toInt() < t.toInt();
}

bool Time::operator<=(const Time& t) const {
  return *this < t || *this == t;
}

bool Time::operator>(const Time& t) const {
  return !(*this <= t);
}

bool Time::operator>=(const Time& t) const {
  return !(*this < t);
}

int Time::compare(const Time& t) const {
  return this->toInt() - t.toInt();
}

int Time::compare(const Time& a, const Time& b) {
  return a.toInt() - b.toInt();
}

string Time::serialize() const {
  return this->toString();
}

void Time::deserialize(const string& str) {
  stringstream mySStr(str);
  string myStr;

  getline(mySStr, myStr, ':');
  this->hour = stoi(myStr);

  getline(mySStr, myStr, '*');
  this->minute = stoi(myStr);
}

ostream& operator<<(ostream& os, const Time& t) {
  os << t.hour << endl;
  os << t.minute;

  return os;
}

istream& operator>>(istream& is, Time& t) {
  string myStr;

  getline(is, myStr);
  t.hour = stoi(myStr);
  getline(is, myStr);
  t.minute = stoi(myStr);

  return is;
}
