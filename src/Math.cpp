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

#include "Math.h"

namespace Ipc {

Math::Math() {
  _hexMap['0'] = 0x0;
  _hexMap['1'] = 0x1;
  _hexMap['2'] = 0x2;
  _hexMap['3'] = 0x3;
  _hexMap['4'] = 0x4;
  _hexMap['5'] = 0x5;
  _hexMap['6'] = 0x6;
  _hexMap['7'] = 0x7;
  _hexMap['8'] = 0x8;
  _hexMap['9'] = 0x9;
  _hexMap['A'] = 0xA;
  _hexMap['B'] = 0xB;
  _hexMap['C'] = 0xC;
  _hexMap['D'] = 0xD;
  _hexMap['E'] = 0xE;
  _hexMap['F'] = 0xF;
  _hexMap['a'] = 0xA;
  _hexMap['b'] = 0xB;
  _hexMap['c'] = 0xC;
  _hexMap['d'] = 0xD;
  _hexMap['e'] = 0xE;
  _hexMap['f'] = 0xF;
}

Math::~Math() {
}

bool Math::isNumber(const std::string &s, bool hex) {
  if (!hex) hex = ((signed)s.find('x') > -1);
  if (!hex) try { std::stoll(s, 0, 10); } catch (...) { return false; }
  else try { std::stoll(s, 0, 16); } catch (...) { return false; }
  return true;
}

int32_t Math::getNumber(std::string &s, bool isHex) {
  int32_t xpos = s.find('x');
  int32_t number = 0;
  if (xpos == -1 && !isHex) try { number = std::stoll(s, 0, 10); } catch (...) {}
  else try { number = std::stoll(s, 0, 16); } catch (...) {}
  return number;
}

int64_t Math::getNumber64(std::string &s, bool isHex) {
  int32_t xpos = s.find('x');
  int64_t number = 0;
  if (xpos == -1 && !isHex) try { number = std::stoll(s, 0, 10); } catch (...) {}
  else try { number = std::stoll(s, 0, 16); } catch (...) {}
  return number;
}

int32_t Math::getNumber(char hexChar) {
  if (_hexMap.find(hexChar) == _hexMap.end()) return 0;
  return _hexMap.at(hexChar);
}

uint32_t Math::getUnsignedNumber(std::string &s, bool isHex) {
  int32_t xpos = s.find('x');
  uint32_t number = 0;
  if (xpos == -1 && !isHex) try { number = std::stoull(s, 0, 10); } catch (...) {}
  else try { number = std::stoull(s, 0, 16); } catch (...) {}
  return number;
}

int32_t Math::getOctalNumber(std::string &s) {
  int32_t number = 0;
  try { number = std::stoll(s, 0, 8); } catch (...) {}
  return number;
}

double Math::getDouble(const std::string &s) {
  double number = 0;
  try { number = std::stod(s); } catch (...) {}
  return number;
}

uint32_t Math::getIeee754Binary32(float value) {
  int32_t sign = 0;
  int32_t integer;
  int32_t exponent = 127;
  int32_t fraction = 0;

  if (value < 0) {
    sign = 0x80000000;
    value = -value;
  }

  integer = floor(value);
  value -= integer;

  for (int32_t i = 22; i >= 0; i--) {
    value += value;
    fraction += floor(value) * pow(2, i);
    value -= floor(value);
  }

  while ((integer != 1) && (exponent > 0) && (exponent < 255)) {
    if (integer > 1) {
      fraction = ((integer & 1) << 22) + (fraction >> 1);
      integer = integer >> 1;
      exponent++;
    } else {
      integer = (fraction & 0x400000) >> 22;
      fraction = (fraction & 0x3FFFFF) << 1;
      value += value;
      fraction += floor(value);
      value -= floor(value);
      exponent--;
    }
  }

  return sign + (exponent << 23) + fraction;
}

uint64_t Math::getIeee754Binary64(double value) {
  int64_t sign = 0;
  int64_t integer;
  int64_t exponent = 1023;
  int64_t fraction = 0;

  if (value < 0) {
    sign = 0x8000000000000000ll;
    value = -value;
  }

  integer = floor(value);
  value -= integer;

  for (int32_t i = 51; i >= 0; i--) {
    value += value;
    fraction += floor(value) * pow(2, i);
    value -= floor(value);
  }

  while ((integer != 1) && (exponent > 0) && (exponent < 2047)) {
    if (integer > 1) {
      fraction = ((integer & 1) << 51) + (fraction >> 1);
      integer = integer >> 1;
      exponent++;
    } else {
      integer = (fraction & 0x8000000000000ll) >> 51;
      fraction = (fraction & 0x7FFFFFFFFFFFFll) << 1;
      value += value;
      fraction += floor(value);
      value -= floor(value);
      exponent--;
    }
  }

  return sign + (exponent << 52) + fraction;
}

float Math::getFloatFromIeee754Binary32(uint32_t binary32) {
  int32_t sign = (binary32 & 0x80000000) ? -1 : 1;
  int32_t exponent = ((binary32 & 0x7F800000) >> 23) - 127;
  float fraction = (binary32 & 0x7FFFFF) + 0x800000;
  fraction = fraction / 0x800000;
  float result = sign * fraction * (float)pow(2, exponent);

  return result;
}

std::string Math::toString(double number) {
  std::stringstream out;
  out << number;
  std::string string = out.str();
  std::string::size_type pos = string.find('.');
  if (pos == std::string::npos) return string;
  int32_t decimalPlaces = string.size() - pos - 1;
  if (decimalPlaces > 3 && string[string.size() - 2] == string.back() && string[string.size() - 3] == string.back()) {
    double factor = std::pow(10, decimalPlaces - 1);
    //Round to 9 digits
    number = std::floor(number * factor + 0.5) / factor;
    out.str(std::string());
    out.clear();
    out << number;
  }
  return out.str();
}

std::string Math::toString(double number, int32_t precision) {
  std::ostringstream out;
  out << std::setiosflags(std::ios_base::fixed | std::ios_base::dec) << std::setprecision(precision) << number;
  return out.str();
}

int32_t Math::clamp(int32_t value, int32_t min, int32_t max) {
  if (value > max) return max;
  if (value < min) return min;
  return value;
}

double Math::clamp(double value, double min, double max) {
  if (value > max) return max;
  if (value < min) return min;
  return value;
}

int32_t Math::scale(int32_t value, int32_t valueMin, int32_t valueMax, int32_t scaleMin, int32_t scaleMax) {
  double vPerc = ((double)(value - valueMin)) / (valueMax - valueMin);
  double bigSpan = vPerc * (scaleMax - scaleMin);

  return std::lround(scaleMin + bigSpan);
}

double Math::scale(double value, double valueMin, double valueMax, double scaleMin, double scaleMax) {
  double vPerc = (value - valueMin) / (valueMax - valueMin);
  double bigSpan = vPerc * (scaleMax - scaleMin);

  return scaleMin + bigSpan;
}
}
