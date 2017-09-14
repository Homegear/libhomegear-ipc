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

#include "BinaryRpc.h"

namespace Ipc
{

BinaryRpc::BinaryRpc()
{
	_data.reserve(1024);
	checkEndianness();
}

BinaryRpc::~BinaryRpc()
{

}

void BinaryRpc::checkEndianness()
{
	union {
		uint32_t i;
		char c[4];
	} bint = {0x01020304};

	_isBigEndian = bint.c[0] == 1;
}

void BinaryRpc::memcpyBigEndian(char* to, const char* from, const uint32_t& length)
{
	if(_isBigEndian) memcpy(to, from, length);
	else
	{
		uint32_t last = length - 1;
		for(uint32_t i = 0; i < length; i++)
		{
			to[i] = from[last - i];
		}
	}
}

int32_t BinaryRpc::process(char* buffer, int32_t bufferLength)
{
	int32_t initialBufferLength = bufferLength;
	if(bufferLength <= 0 || _finished) return 0;
	_processingStarted = true;
	if(_data.size() + bufferLength < 8)
	{
		_data.insert(_data.end(), buffer, buffer + bufferLength);
		return initialBufferLength;
	}
	else if(_data.size() < 8)
	{
		int32_t sizeToInsert = 8 - _data.size();
		_data.insert(_data.end(), buffer, buffer + sizeToInsert);
		buffer += sizeToInsert;
		bufferLength -= sizeToInsert;
	}
	if(strncmp(_data.data(), "Bin", 3) != 0)
	{
		_finished = true;
		throw BinaryRpcException("Packet does not start with \"Bin\".");
	}
	_type = (_data[3] & 1) ? Type::response : Type::request;
	if(_data[3] & 0x40)
	{
		_hasHeader = true;
		memcpyBigEndian((char*)&_headerSize, _data.data() + 4, 4);
		if(_headerSize > 10485760) throw BinaryRpcException("Header is larger than 10 MiB.");
	}
	else
	{
		memcpyBigEndian((char*)&_dataSize, _data.data() + 4, 4);
		if(_dataSize > 104857600) throw BinaryRpcException("Data is data larger than 100 MiB.");
	}
	if(_dataSize == 0 && _headerSize == 0)
	{
		_finished = true;
		throw BinaryRpcException("Invalid packet format.");
	}
	if(_dataSize == 0) //Has header
	{
		if(_data.size() + bufferLength < 8 + _headerSize + 4)
		{
			if(_headerSize + 8 + 100 > _data.capacity()) _data.reserve(_headerSize + 8 + 1024);
			_data.insert(_data.end(), buffer, buffer + bufferLength);
			return initialBufferLength;
		}
		int32_t sizeToInsert = (8 + _headerSize + 4) - _data.size();
		_data.insert(_data.end(), buffer, buffer + sizeToInsert);
		buffer += sizeToInsert;
		bufferLength -= sizeToInsert;
		memcpyBigEndian((char*)&_dataSize, _data.data() + 8 + _headerSize, 4);
		_dataSize += _headerSize + 4;
		if(_dataSize > 104857600) throw BinaryRpcException("Data is data larger than 100 MiB.");
	}
	_data.reserve(8 + _dataSize);
	if(_data.size() + bufferLength < _dataSize + 8)
	{
		_data.insert(_data.end(), buffer, buffer + bufferLength);
		return initialBufferLength;
	}
	int32_t sizeToInsert = (8 + _dataSize) - _data.size();
	_data.insert(_data.end(), buffer, buffer + sizeToInsert);
	buffer += sizeToInsert;
	bufferLength -= sizeToInsert;
	_finished = true;
	return initialBufferLength - bufferLength;
}

void BinaryRpc::reset()
{
	_data.clear();
	_data.reserve(1024);
	_type = Type::unknown;
	_processingStarted = false;
	_finished = false;
	_hasHeader = false;
	_headerSize = 0;
	_dataSize = 0;
}

}
