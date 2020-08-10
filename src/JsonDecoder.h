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

#ifndef IPCJSONDECODER_H_
#define IPCJSONDECODER_H_

#include "Variable.h"
#include "Math.h"
#include "IpcException.h"
#include <cmath>
#if __GNUC__ > 4
#include <codecvt>
#endif

namespace Ipc {

class JsonDecoderException : public IpcException {
 public:
  explicit JsonDecoderException(const std::string &message) : IpcException(message) {}
};

class JsonDecoder {
 public:
  JsonDecoder() = default;
  virtual ~JsonDecoder() = default;

  static PVariable decode(const std::string &json);
  static PVariable decode(const std::string &json, uint32_t &bytesRead);
  static PVariable decode(const std::vector<char> &json);
  static PVariable decode(const std::vector<char> &json, uint32_t &bytesRead);

  static std::string decodeString(const std::string &s);
 private:
  static inline bool posValid(const std::string &json, uint32_t pos);
  static inline bool posValid(const std::vector<char> &json, uint32_t pos);
  static void skipWhitespace(const std::string &json, uint32_t &pos);
  static void skipWhitespace(const std::vector<char> &json, uint32_t &pos);
  static void decodeObject(const std::string &json, uint32_t &pos, PVariable &variable);
  static void decodeObject(const std::vector<char> &json, uint32_t &pos, PVariable &variable);
  static void decodeArray(const std::string &json, uint32_t &pos, PVariable &variable);
  static void decodeArray(const std::vector<char> &json, uint32_t &pos, PVariable &variable);
  static void decodeString(const std::string &json, uint32_t &pos, PVariable &value);
  static void decodeString(const std::vector<char> &json, uint32_t &pos, PVariable &value);
  static void decodeString(const std::string &json, uint32_t &pos, std::string &s);
  static void decodeString(const std::vector<char> &json, uint32_t &pos, std::string &s);
  static bool decodeValue(const std::string &json, uint32_t &pos, PVariable &value);
  static bool decodeValue(const std::vector<char> &json, uint32_t &pos, PVariable &value);
  static void decodeBoolean(const std::string &json, uint32_t &pos, PVariable &value);
  static void decodeBoolean(const std::vector<char> &json, uint32_t &pos, PVariable &value);
  static void decodeNull(const std::string &json, uint32_t &pos, PVariable &value);
  static void decodeNull(const std::vector<char> &json, uint32_t &pos, PVariable &value);
  static bool decodeNumber(const std::string &json, uint32_t &pos, PVariable &value);
  static bool decodeNumber(const std::vector<char> &json, uint32_t &pos, PVariable &value);
};

}
#endif
