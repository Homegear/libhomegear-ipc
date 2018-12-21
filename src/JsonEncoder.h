/* Copyright 2013-2017 Sathya Laufer
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

#ifndef IPCJSONENCODER_H_
#define IPCJSONENCODER_H_

#include "Variable.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <codecvt>

namespace Ipc
{

class JsonEncoder
{
public:
	JsonEncoder();
	virtual ~JsonEncoder() {}

	std::string getString(const PVariable variable);
	std::vector<char> getVector(const PVariable variable);
private:
	int32_t _requestId = 1;

	std::string toString(double number);
	void encodeValue(const PVariable& variable, std::ostringstream& s);
	void encodeValue(const PVariable& variable, std::vector<char>& s);
	void encodeArray(const PVariable& variable, std::ostringstream& s);
	void encodeArray(const PVariable& variable, std::vector<char>& s);
	void encodeStruct(const PVariable& variable, std::ostringstream& s);
	void encodeStruct(const PVariable& variable, std::vector<char>& s);
	void encodeBoolean(const PVariable& variable, std::ostringstream& s);
	void encodeBoolean(const PVariable& variable, std::vector<char>& s);
	void encodeInteger( const PVariable& variable, std::ostringstream& s);
	void encodeInteger( const PVariable& variable, std::vector<char>& s);
	void encodeInteger64( const PVariable& variable, std::ostringstream& s);
	void encodeInteger64( const PVariable& variable, std::vector<char>& s);
	void encodeFloat(const PVariable& variable, std::ostringstream& s);
	void encodeFloat(const PVariable& variable, std::vector<char>& s);
	void encodeString(const PVariable& variable, std::ostringstream& s);
	void encodeString(const PVariable& variable, std::vector<char>& s);
	void encodeVoid(const PVariable& variable, std::ostringstream& s);
	void encodeVoid(const PVariable& variable, std::vector<char>& s);
};

}

#endif
