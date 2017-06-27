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

#ifndef IPCBINARYDECODER_H_
#define IPCBINARYDECODER_H_

#include <iostream>
#include <memory>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>

namespace Ipc
{

class BinaryDecoder
{
public:
	BinaryDecoder();
	virtual ~BinaryDecoder() {}

	virtual int32_t decodeInteger(std::vector<char>& encodedData, uint32_t& position);
	virtual int32_t decodeInteger(std::vector<uint8_t>& encodedData, uint32_t& position);
	virtual int64_t decodeInteger64(std::vector<char>& encodedData, uint32_t& position);
	virtual int64_t decodeInteger64(std::vector<uint8_t>& encodedData, uint32_t& position);
	virtual uint8_t decodeByte(std::vector<char>& encodedData, uint32_t& position);
	virtual uint8_t decodeByte(std::vector<uint8_t>& encodedData, uint32_t& position);
	virtual std::string decodeString(std::vector<char>& encodedData, uint32_t& position);
	virtual std::string decodeString(std::vector<uint8_t>& encodedData, uint32_t& position);
	virtual std::vector<uint8_t> decodeBinary(std::vector<char>& encodedData, uint32_t& position);
	virtual std::vector<uint8_t> decodeBinary(std::vector<uint8_t>& encodedData, uint32_t& position);
	virtual bool decodeBoolean(std::vector<char>& encodedData, uint32_t& position);
	virtual bool decodeBoolean(std::vector<uint8_t>& encodedData, uint32_t& position);
	virtual double decodeFloat(std::vector<char>& encodedData, uint32_t& position);
	virtual double decodeFloat(std::vector<uint8_t>& encodedData, uint32_t& position);
private:
	/**
	 * The result of checkEndianness() is stored in this variable. This is done through calling "init".
	 */
	bool _isBigEndian = true;

	/**
	 * Checks if the system is little or big endian.
	 */
	void checkEndianness();

	/**
	 * Copies binary values from one memory location to another reversing the byte order when the system is little endian.
	 *
	 * @param[out] to The destination array. No memory is allocated, so make sure, the array is large enough.
	 * @param[in] from The source array.
	 * @param length The number of bytes to copy.
	 */
	void memcpyBigEndian(char* to, const char* from, const uint32_t& length);

	/**
	 * Copies binary values from one memory location to another reversing the byte order when the system is little endian.
	 *
	 * @param[out] to The destination array. No memory is allocated, so make sure, the array is large enough.
	 * @param[in] from The source array.
	 * @param length The number of bytes to copy.
	 */
	void memcpyBigEndian(uint8_t* to, const uint8_t* from, const uint32_t& length);
};

}
#endif
