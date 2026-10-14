#include "jsonparser.hpp"

JSONParser::JSONParser(const std::string& s) : source(s), pos(0) {}

char JSONParser::peek() {
  if (atEnd()) {
    throw Exception("fin del JSON inesperado, peek");
  }

  return source[pos];
}

char JSONParser::advance() {
  if (atEnd()) {
    throw Exception("fin del JSON inesperado, advance");
  }

  char c = source[pos];
  pos++;
  return c;
}

void JSONParser::skipWhitespace() {
  while (!atEnd()) {
    char c = source[pos];
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
      pos++;
    } else {
      return;
    }
  }
}

void JSONParser::expect(char c) {
  skipWhitespace();
  if (atEnd() || source[pos] != c) {
    throw Exception(("Se esperaba ") + c);
  }

  pos++;
}

bool JSONParser::atEnd() {
  return pos >= source.size();
}

bool JSONParser::match(const std::string& w) {
  if (pos + w.size() > source.size()) {
    return false;
  }

  for (size_t i = 0; i < w.size(); i++) {
    if (source[pos + i] != w[i])
      return false;
  }

  pos += w.size();

  return true;
}

JSONValue JSONParser::parseNull() {
  skipWhitespace();

  if (match("null")) {
    return JSONValue();
  }

  throw Exception("Se esperaba null, parseNull");
}

JSONValue JSONParser::parseBool() {
  skipWhitespace();

  if (match("true")) {
    return JSONValue(true);
  }
  if (match("false")) {
    return JSONValue(false);
  }

  throw Exception("Se esperaba true o false, parseBool");
}

JSONValue JSONParser::parseNumber() {
  skipWhitespace();

  size_t start = pos;

  if (!atEnd() && source[pos] == '-') {
    pos++;
  }

  while (!atEnd()) {
    char c = source[pos];
    if ((c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E' ||
        c == '+' || c == '-') {
      pos++;
    } else {
      break;
    }
  }

  if (start == pos) {
    throw Exception("Se esperaba un numero, parseNumber");
  }

  std::string numStr = source.substr(start, pos - start);

  try {
    double value = std::stod(numStr);
    return JSONValue(value);
  } catch (...) {
    throw Exception("Numero invalido: " + numStr);
  }
}

std::string JSONParser::parseString() {
  skipWhitespace();

  if (atEnd() || source[pos] != '"') {
    throw Exception("Se esperaba un string (comilla \"), parseString");
  }
  pos++;

  std::string result;

  while (!atEnd() && source[pos] != '"') {
    char c = source[pos];

    if (c == '\\') {
      pos++;
      if (atEnd()) {
        throw Exception("Escape incompleto en string, parseString");
      }

      char escaped = source[pos];

      switch (escaped) {
        case '"':
          result += '"';
          break;
        case '\\':
          result += '\\';
          break;
        case '/':
          result += '/';
          break;
        case 'n':
          result += '\n';
          break;
        case 't':
          result += '\t';
          break;
        case 'r':
          result += '\r';
          break;
        case 'b':
          result += '\b';
          break;
        case 'f':
          result += '\f';
          break;
        default:
          throw Exception(std::string("Escape desconocido: \\") + escaped);
      }
      pos++;
    } else {
      result += c;
      pos++;
    }
  }

  if (atEnd()) {
    throw Exception("String sin cerrar, parseString");
  }
  pos++;

  return result;
}

JSONValue JSONParser::parseValue() {
  skipWhitespace();

  if (atEnd()) {
    throw Exception("Se esperaba un valor, parseValue");
  }

  char c = source[pos];

  if (c == '{') {
    return parseObject();
  }
  if (c == '[') {
    return parseArray();
  }
  if (c == '"') {
    return JSONValue(parseString());
  }
  if (c == 'n') {
    return parseNull();
  }
  if (c == 't' || c == 'f') {
    return parseBool();
  }
  if (c == '-' || (c >= '0' && c <= '9')) {
    return parseNumber();
  }

  throw Exception("Caracter inesperado, parseValue");
}

JSONValue JSONParser::parseObject() {
  expect('{');

  DLList<JSONPair> pair;
  skipWhitespace();

  if (!atEnd() && source[pos] == '}') {
    pos++;
    return JSONValue(pair);
  }

  while (true) {
    skipWhitespace();

    std::string key = parseString();
    expect(':');
    JSONValue value = parseValue();

    pair.insData(pair.getLastPos(), JSONPair(key, value));

    skipWhitespace();

    if (atEnd()) {
      throw Exception("Objeto sin cerrar, parseObject");
    }

    char c = source[pos];
    if (c == ',') {
      pos++;
      continue;
    }
    if (c == '}') {
      pos++;
      return JSONValue(pair);
    }

    throw Exception(std::string("Se esperaba ',' o '}', se encontro '") + c +
                    "', parseObject");
  }
}

JSONValue JSONParser::parseArray() {
  expect('[');

  DLList<JSONValue> elements;
  skipWhitespace();

  if (!atEnd() && source[pos] == ']') {
    pos++;
    return JSONValue(elements);
  }

  while (true) {
    JSONValue v = parseValue();
    elements.insData(elements.getLastPos(), v);

    skipWhitespace();

    if (atEnd()) {
      throw Exception("Arreglo sin cerrar, parseArray");
    }

    char c = source[pos];
    if (c == ',') {
      pos++;
      continue;
    }
    if (c == ']') {
      pos++;
      return JSONValue(elements);
    }

    throw Exception(std::string("Se esperaba ',' o ']', se encontro '") + c +
                    "', parseArray");
  }
}