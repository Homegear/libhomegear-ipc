/* Copyright 2013-2019 Homegear GmbH
 *
 * libhomegear-base is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * libhomegear-base is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libhomegear-base.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU Lesser General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
*/

#include "Variable.h"
#include "HelperFunctions.h"
#include "Math.h"

namespace Ipc {

Variable::Variable() {
  type = VariableType::tVoid;
  arrayValue = std::make_shared<Array>();
  structValue = std::make_shared<Struct>();
}

Variable::Variable(Variable const &rhs) {
  errorStruct = rhs.errorStruct;
  type = rhs.type;
  stringValue = rhs.stringValue;
  integerValue = rhs.integerValue;
  integerValue64 = rhs.integerValue64;
  floatValue = rhs.floatValue;
  booleanValue = rhs.booleanValue;
  binaryValue = rhs.binaryValue;
  for (Array::const_iterator i = rhs.arrayValue->begin(); i != rhs.arrayValue->end(); ++i) {
    PVariable lhs = std::make_shared<Variable>();
    *lhs = *(*i);
    arrayValue->push_back(lhs);
  }
  for (Struct::const_iterator i = rhs.structValue->begin(); i != rhs.structValue->end(); ++i) {
    PVariable lhs = std::make_shared<Variable>();
    *lhs = *(i->second);
    structValue->insert(std::pair<std::string, PVariable>(i->first, lhs));
  }
}

Variable::Variable(VariableType variableType) : Variable() {
  type = variableType;
  if (type == VariableType::tVariant) type = VariableType::tVoid;
}

Variable::Variable(uint8_t integer) : Variable() {
  type = VariableType::tInteger;
  integerValue = (int32_t)integer;
  integerValue64 = (int64_t)integer;
  floatValue = (double)integer;
  booleanValue = (bool)integer;
}

Variable::Variable(int32_t integer) : Variable() {
  type = VariableType::tInteger;
  integerValue = (int32_t)integer;
  integerValue64 = (int64_t)integer;
  floatValue = (double)integer;
  booleanValue = (bool)integer;
}

Variable::Variable(uint32_t integer) : Variable() {
  type = VariableType::tInteger;
  integerValue = (int32_t)integer;
  integerValue64 = (int64_t)integer;
  floatValue = (double)integer;
  booleanValue = (bool)integer;
}

Variable::Variable(int64_t integer) : Variable() {
  type = VariableType::tInteger64;
  integerValue = (int32_t)integer;
  integerValue64 = (int64_t)integer;
  floatValue = (double)integer;
  booleanValue = (bool)integer;
}

Variable::Variable(uint64_t integer) : Variable() {
  type = VariableType::tInteger64;
  integerValue = (int32_t)integer;
  integerValue64 = (int64_t)integer;
  floatValue = (double)integer;
  booleanValue = (bool)integer;
}

Variable::Variable(const std::string &string) : Variable() {
  type = VariableType::tString;
  stringValue = string;
  integerValue64 = Math::getNumber64(stringValue);
  integerValue = (int32_t)integerValue64;
  booleanValue = !stringValue.empty() && stringValue != "0" && stringValue != "false" && stringValue != "f";
}

Variable::Variable(const char *string) : Variable(std::string(string)) {
}

Variable::Variable(bool boolean) : Variable() {
  type = VariableType::tBoolean;
  booleanValue = boolean;
  integerValue = booleanValue;
  integerValue64 = booleanValue;
}

Variable::Variable(double floatVal) : Variable() {
  type = VariableType::tFloat;
  floatValue = floatVal;
  integerValue = (int32_t)std::lround(floatVal);
  integerValue64 = std::llround(floatVal);
  booleanValue = (bool)floatVal;
}

Variable::Variable(const PArray &arrayVal) : Variable() {
  type = VariableType::tArray;
  arrayValue = arrayVal;
}

Variable::Variable(const std::vector<std::string> &arrayVal) : Variable() {
  type = VariableType::tArray;
  arrayValue->reserve(arrayVal.size());
  for (auto &element : arrayVal) {
    arrayValue->push_back(std::make_shared<Variable>(element));
  }
}

Variable::Variable(const PStruct &structVal) : Variable() {
  type = VariableType::tStruct;
  structValue = structVal;
}

Variable::Variable(const std::vector<uint8_t> &binaryVal) : Variable() {
  type = VariableType::tBinary;
  binaryValue = binaryVal;
}

Variable::Variable(const uint8_t *binaryVal, size_t binaryValSize) : Variable() {
  type = VariableType::tBinary;
  binaryValue = std::vector<uint8_t>(binaryVal, binaryVal + binaryValSize);
}

Variable::Variable(const std::vector<char> &binaryVal) : Variable() {
  type = VariableType::tBinary;
  binaryValue.clear();
  binaryValue.insert(binaryValue.end(), binaryVal.begin(), binaryVal.end());
}

Variable::Variable(const char *binaryVal, size_t binaryValSize) : Variable() {
  type = VariableType::tBinary;
  binaryValue = std::vector<uint8_t>(binaryVal, binaryVal + binaryValSize);
}

Variable::~Variable() {
}

std::shared_ptr<Variable> Variable::createError(int32_t faultCode, std::string faultString) {
  std::shared_ptr<Variable> error = std::make_shared<Variable>(VariableType::tStruct);
  error->errorStruct = true;
  error->structValue->insert(StructElement("faultCode", std::make_shared<Variable>(faultCode)));
  error->structValue->insert(StructElement("faultString", std::make_shared<Variable>(faultString)));
  return error;
}

Variable &Variable::operator=(const Variable &rhs) {
  if (&rhs == this) return *this;
  errorStruct = rhs.errorStruct;
  type = rhs.type;
  stringValue = rhs.stringValue;
  integerValue = rhs.integerValue;
  integerValue64 = rhs.integerValue64;
  floatValue = rhs.floatValue;
  booleanValue = rhs.booleanValue;
  binaryValue = rhs.binaryValue;
  for (Array::const_iterator i = rhs.arrayValue->begin(); i != rhs.arrayValue->end(); ++i) {
    PVariable lhs = std::make_shared<Variable>();
    *lhs = *(*i);
    arrayValue->push_back(lhs);
  }
  for (Struct::const_iterator i = rhs.structValue->begin(); i != rhs.structValue->end(); ++i) {
    PVariable lhs = std::make_shared<Variable>();
    *lhs = *(i->second);
    structValue->insert(std::pair<std::string, PVariable>(i->first, lhs));
  }
  return *this;
}

bool Variable::operator==(const Variable &rhs) {
  if (type != rhs.type) return false;
  if (type == VariableType::tBoolean) return booleanValue == rhs.booleanValue;
  if (type == VariableType::tInteger) return integerValue == rhs.integerValue;
  if (type == VariableType::tInteger64) return integerValue64 == rhs.integerValue64;
  if (type == VariableType::tString) return stringValue == rhs.stringValue;
  if (type == VariableType::tFloat) return floatValue == rhs.floatValue;
  if (type == VariableType::tArray) {
    if (arrayValue->size() != rhs.arrayValue->size()) return false;
    for (std::pair<Array::iterator, Array::iterator> i(arrayValue->begin(), rhs.arrayValue->begin()); i.first != arrayValue->end(); ++i.first, ++i.second) {
      if (**(i.first) != **(i.second)) return false;
    }
    return true;
  }
  if (type == VariableType::tStruct) {
    if (structValue->size() != rhs.structValue->size()) return false;
    for (auto &element : *structValue) {
      auto rhsIterator = rhs.structValue->find(element.first);
      if (rhsIterator == rhs.structValue->end() || *element.second != *rhsIterator->second) return false;
    }
    return true;
  }
  if (type == VariableType::tBase64) return stringValue == rhs.stringValue;
  if (type == VariableType::tBinary) {
    if (binaryValue.size() != rhs.binaryValue.size()) return false;
    if (binaryValue.empty()) return true;
    uint8_t *i = &binaryValue[0];
    uint8_t *j = (uint8_t *)&rhs.binaryValue[0];
    for (; i < &binaryValue[0] + binaryValue.size(); i++, j++) {
      if (*i != *j) return false;
    }
    return true;
  }
  return false;
}

bool Variable::operator<(const Variable &rhs) {
  if (type == VariableType::tBoolean) return booleanValue < rhs.booleanValue;
  if (type == VariableType::tInteger) return integerValue < rhs.integerValue;
  if (type == VariableType::tInteger64) return integerValue64 < rhs.integerValue64;
  if (type == VariableType::tString) return stringValue < rhs.stringValue;
  if (type == VariableType::tFloat) return floatValue < rhs.floatValue;
  if (type == VariableType::tArray) {
    if (arrayValue->size() < rhs.arrayValue->size()) return true; else return false;
  }
  if (type == VariableType::tStruct) {
    if (structValue->size() < rhs.structValue->size()) return true; else return false;
  }
  if (type == VariableType::tBase64) return stringValue < rhs.stringValue;
  return false;
}

bool Variable::operator<=(const Variable &rhs) {
  if (type == VariableType::tBoolean) return booleanValue <= rhs.booleanValue;
  if (type == VariableType::tInteger) return integerValue <= rhs.integerValue;
  if (type == VariableType::tInteger64) return integerValue64 <= rhs.integerValue64;
  if (type == VariableType::tString) return stringValue <= rhs.stringValue;
  if (type == VariableType::tFloat) return floatValue <= rhs.floatValue;
  if (type == VariableType::tArray) {
    if (arrayValue->size() <= rhs.arrayValue->size()) return true; else return false;
  }
  if (type == VariableType::tStruct) {
    if (structValue->size() <= rhs.structValue->size()) return true; else return false;
  }
  if (type == VariableType::tBase64) return stringValue <= rhs.stringValue;
  return false;
}

bool Variable::operator>(const Variable &rhs) {
  if (type == VariableType::tBoolean) return booleanValue > rhs.booleanValue;
  if (type == VariableType::tInteger) return integerValue > rhs.integerValue;
  if (type == VariableType::tInteger64) return integerValue64 > rhs.integerValue64;
  if (type == VariableType::tString) return stringValue > rhs.stringValue;
  if (type == VariableType::tFloat) return floatValue > rhs.floatValue;
  if (type == VariableType::tArray) {
    if (arrayValue->size() > rhs.arrayValue->size()) return true; else return false;
  }
  if (type == VariableType::tStruct) {
    if (structValue->size() > rhs.structValue->size()) return true; else return false;
  }
  if (type == VariableType::tBase64) return stringValue > rhs.stringValue;
  return false;
}

bool Variable::operator>=(const Variable &rhs) {
  if (type == VariableType::tBoolean) return booleanValue >= rhs.booleanValue;
  if (type == VariableType::tInteger) return integerValue >= rhs.integerValue;
  if (type == VariableType::tInteger64) return integerValue64 >= rhs.integerValue64;
  if (type == VariableType::tString) return stringValue >= rhs.stringValue;
  if (type == VariableType::tFloat) return floatValue >= rhs.floatValue;
  if (type == VariableType::tArray) {
    if (arrayValue->size() >= rhs.arrayValue->size()) return true; else return false;
  }
  if (type == VariableType::tStruct) {
    if (structValue->size() >= rhs.structValue->size()) return true; else return false;
  }
  if (type == VariableType::tBase64) return stringValue >= rhs.stringValue;
  return false;
}

bool Variable::operator!=(const Variable &rhs) {
  return !(operator==(rhs));
}

Variable::operator Variable::bool_type() const {
  bool result = false;
  if (type != VariableType::tBoolean) {
    switch (type) {
      case VariableType::tArray: result = !arrayValue->empty();
        break;
      case VariableType::tBase64: result = !stringValue.empty();
        break;
      case VariableType::tBinary: result = !binaryValue.empty();
        break;
      case VariableType::tBoolean: break;
      case VariableType::tFloat: result = (bool)floatValue;
        break;
      case VariableType::tInteger: result = (bool)integerValue;
        break;
      case VariableType::tInteger64: result = (bool)integerValue64;
        break;
      case VariableType::tString: result = !stringValue.empty() && stringValue != "0" && stringValue != "false" && stringValue != "f";
        break;
      case VariableType::tStruct: result = !structValue->empty();
        break;
      case VariableType::tVariant: break;
      case VariableType::tVoid: result = false;
        break;
    }
  } else result = booleanValue;
  return result ? &Variable::this_type_does_not_support_comparisons : 0;
}

std::string Variable::print(bool stdout, bool stderr, bool oneLine) {
  std::ostringstream result;
  if (type == VariableType::tVoid) {
    result << "(void)" << (oneLine ? " " : "\n");
  } else if (type == VariableType::tBoolean) {
    result << "(Boolean) " << booleanValue << (oneLine ? " " : "\n");
  } else if (type == VariableType::tInteger) {
    result << "(Integer) " << integerValue << (oneLine ? " " : "\n");
  } else if (type == VariableType::tInteger64) {
    result << "(Integer64) " << integerValue64 << (oneLine ? " " : "\n");
  } else if (type == VariableType::tFloat) {
    result << "(Float) " << floatValue << (oneLine ? " " : "\n");
  } else if (type == VariableType::tString) {
    result << "(String) " << stringValue << (oneLine ? " " : "\n");
  } else if (type == VariableType::tBase64) {
    result << "(Base64) " << stringValue << (oneLine ? " " : "\n");
  } else if (type == VariableType::tArray) {
    std::string indent("");
    result << printArray(arrayValue, indent, false, oneLine);
  } else if (type == VariableType::tStruct) {
    std::string indent("");
    result << printStruct(structValue, indent, false, oneLine);
  } else if (type == VariableType::tBinary) {
    result << "(Binary) " << HelperFunctions::getHexString(binaryValue) << (oneLine ? " " : "\n");
  } else {
    result << "(unknown)" << (oneLine ? " " : "\n");
  }
  std::string resultString = result.str();
  if (stdout) std::cout << resultString;
  if (stderr) std::cerr << resultString;
  return resultString;
}

std::string Variable::print(PVariable variable, std::string indent, bool ignoreIndentOnFirstLine, bool oneLine) {
  if (!variable) return "";
  std::ostringstream result;
  if (variable->type == VariableType::tVoid) {
    result << (ignoreIndentOnFirstLine ? "" : indent) << "(void)" << (oneLine ? " " : "\n");
  } else if (variable->type == VariableType::tInteger) {
    result << (ignoreIndentOnFirstLine ? "" : indent) << "(Integer) " << variable->integerValue << (oneLine ? " " : "\n");
  } else if (variable->type == VariableType::tInteger64) {
    result << (ignoreIndentOnFirstLine ? "" : indent) << "(Integer64) " << variable->integerValue64 << (oneLine ? " " : "\n");
  } else if (variable->type == VariableType::tFloat) {
    result << (ignoreIndentOnFirstLine ? "" : indent) << "(Float) " << variable->floatValue << (oneLine ? " " : "\n");
  } else if (variable->type == VariableType::tBoolean) {
    result << (ignoreIndentOnFirstLine ? "" : indent) << "(Boolean) " << variable->booleanValue << (oneLine ? " " : "\n");
  } else if (variable->type == VariableType::tString) {
    result << (ignoreIndentOnFirstLine ? "" : indent) << "(String) " << variable->stringValue << (oneLine ? " " : "\n");
  } else if (type == VariableType::tBase64) {
    result << (ignoreIndentOnFirstLine ? "" : indent) << "(Base64) " << variable->stringValue << (oneLine ? " " : "\n");
  } else if (variable->type == VariableType::tArray) {
    return printArray(variable->arrayValue, indent, ignoreIndentOnFirstLine, oneLine);
  } else if (variable->type == VariableType::tStruct) {
    return printStruct(variable->structValue, indent, ignoreIndentOnFirstLine, oneLine);
  } else if (variable->type == VariableType::tBinary) {
    result << (ignoreIndentOnFirstLine ? "" : indent) << "(Binary) " << HelperFunctions::getHexString(variable->binaryValue) << (oneLine ? " " : "\n");
  } else {
    result << (ignoreIndentOnFirstLine ? "" : indent) << "(Unknown)" << (oneLine ? " " : "\n");
  }
  return result.str();
}

std::string Variable::printArray(PArray array, std::string indent, bool ignoreIndentOnFirstLine, bool oneLine) {
  std::ostringstream result;
  result << (ignoreIndentOnFirstLine ? "" : indent) << "(Array length=" << array->size() << ")" << (oneLine ? " " : "\n" + indent) << "[" << (oneLine ? " " : "\n");
  std::string currentIndent = indent;
  if (!oneLine) {
    currentIndent.push_back(' ');
    currentIndent.push_back(' ');
  }
  for (auto &element : *array) {
    result << print(element, currentIndent, false, oneLine);
  }
  result << (oneLine ? " ] " : indent + "]\n");
  return result.str();
}

std::string Variable::printStruct(PStruct tStruct, std::string indent, bool ignoreIndentOnFirstLine, bool oneLine) {
  std::ostringstream result;
  result << (ignoreIndentOnFirstLine ? "" : indent) << "(Struct length=" << tStruct->size() << ")" << (oneLine ? " " : "\n" + indent) << "{" << (oneLine ? " " : "\n");
  std::string currentIndent = indent;
  if (!oneLine) {
    currentIndent.push_back(' ');
    currentIndent.push_back(' ');
  }
  for (auto &element : *tStruct) {
    result << currentIndent << "[" << element.first << "]" << " ";
    result << print(element.second, currentIndent, true, oneLine);
  }
  result << (oneLine ? " } " : indent + "}\n");
  return result.str();
}

std::string Variable::toString() {
  switch (type) {
    case VariableType::tArray: return "array";
    case VariableType::tBase64: return stringValue;
    case VariableType::tBoolean: if (booleanValue) return "true"; else return "false";
    case VariableType::tFloat: return std::to_string(floatValue);
    case VariableType::tInteger: return std::to_string(integerValue);
    case VariableType::tInteger64: return std::to_string(integerValue64);
    case VariableType::tString: return stringValue;
    case VariableType::tStruct: return "struct";
    case VariableType::tBinary: return HelperFunctions::getHexString(binaryValue);
    case VariableType::tVoid: return "";
    case VariableType::tVariant: return "valuetype";
  }
  return "";
}

std::string Variable::getTypeString(VariableType type) {
  switch (type) {
    case VariableType::tArray: return "array";
    case VariableType::tBase64: return "base64";
    case VariableType::tBoolean: return "boolean";
      //case VariableType::rpcDate:
      //	return "dateTime.iso8601";
    case VariableType::tFloat: return "double";
    case VariableType::tInteger: return "i4";
    case VariableType::tInteger64: return "i8";
    case VariableType::tString: return "string";
    case VariableType::tStruct: return "struct";
    case VariableType::tBinary: return "binary";
    case VariableType::tVoid: return "void";
    case VariableType::tVariant: return "valuetype";
  }
  return "string";
}

}
