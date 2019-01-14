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

#ifndef IPCRPCDECODER_H_
#define IPCRPCDECODER_H_

#include "Variable.h"
#include "BinaryDecoder.h"
#include "RpcHeader.h"
#include "HelperFunctions.h"

#include <memory>
#include <vector>
#include <cmath>

namespace Ipc
{
class RpcDecoder
{
public:
	RpcDecoder();
	virtual ~RpcDecoder() {}

	virtual std::shared_ptr<RpcHeader> decodeHeader(std::vector<char>& packet);
	virtual std::shared_ptr<RpcHeader> decodeHeader(std::vector<uint8_t>& packet);
	virtual std::shared_ptr<std::vector<std::shared_ptr<Variable>>> decodeRequest(std::vector<char>& packet, std::string& methodName);
	virtual std::shared_ptr<std::vector<std::shared_ptr<Variable>>> decodeRequest(std::vector<uint8_t>& packet, std::string& methodName);
	virtual std::shared_ptr<Variable> decodeResponse(std::vector<char>& packet, uint32_t offset = 0);
	virtual std::shared_ptr<Variable> decodeResponse(std::vector<uint8_t>& packet, uint32_t offset = 0);
	virtual void decodeResponse(PVariable& variable, uint32_t offset = 0);
private:
	std::unique_ptr<BinaryDecoder> _decoder;

	std::shared_ptr<Variable> decodeParameter(std::vector<char>& packet, uint32_t& position);
	std::shared_ptr<Variable> decodeParameter(std::vector<uint8_t>& packet, uint32_t& position);
	void decodeParameter(PVariable& variable, uint32_t& position);
	VariableType decodeType(std::vector<char>& packet, uint32_t& position);
	VariableType decodeType(std::vector<uint8_t>& packet, uint32_t& position);
	std::shared_ptr<Array> decodeArray(std::vector<char>& packet, uint32_t& position);
	std::shared_ptr<Array> decodeArray(std::vector<uint8_t>& packet, uint32_t& position);
	std::shared_ptr<Struct> decodeStruct(std::vector<char>& packet, uint32_t& position);
	std::shared_ptr<Struct> decodeStruct(std::vector<uint8_t>& packet, uint32_t& position);
};
}
#endif
