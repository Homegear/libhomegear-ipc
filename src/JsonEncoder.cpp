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

#include "JsonEncoder.h"

namespace Ipc
{

JsonEncoder::JsonEncoder()
{
}

std::string JsonEncoder::getString(const PVariable variable)
{
	if(!variable) return "";
	std::ostringstream s;
	switch(variable->type)
	{
	case VariableType::tStruct:
		encodeStruct(variable, s);
		break;
	case VariableType::tArray:
		encodeArray(variable, s);
		break;
	default:
		s << '[';
		encodeValue(variable, s);
		s << ']';
		break;
	}
	return s.str();
}

std::vector<char> JsonEncoder::getVector(const PVariable variable)
{
	std::vector<char> json;
	if(!variable) return json;
	json.reserve(1024);
	switch(variable->type)
	{
	case VariableType::tStruct:
		encodeStruct(variable, json);
		break;
	case VariableType::tArray:
		encodeArray(variable, json);
		break;
	default:
		json.push_back('[');
		encodeValue(variable, json);
		json.push_back(']');
		break;
	}
	return json;
}

void JsonEncoder::encodeValue(const PVariable& variable, std::ostringstream& s)
{
	switch(variable->type)
	{
	case VariableType::tArray:
		encodeArray(variable, s);
		break;
	case VariableType::tStruct:
		encodeStruct(variable, s);
		break;
	case VariableType::tBoolean:
		encodeBoolean(variable, s);
		break;
	case VariableType::tInteger:
		encodeInteger(variable, s);
		break;
	case VariableType::tInteger64:
		encodeInteger64(variable, s);
		break;
	case VariableType::tFloat:
		encodeFloat(variable, s);
		break;
	case VariableType::tBase64:
		encodeString(variable, s);
		break;
	case VariableType::tString:
		encodeString(variable, s);
		break;
	case VariableType::tVoid:
		encodeVoid(variable, s);
		break;
	case VariableType::tVariant:
		encodeVoid(variable, s);
		break;
	case VariableType::tBinary:
		encodeVoid(variable, s);
		break;
	}
}

void JsonEncoder::encodeValue(const PVariable& variable, std::vector<char>& s)
{
	if(s.size() + 128 > s.capacity()) s.reserve(s.capacity() + 1024);
	switch(variable->type)
	{
	case VariableType::tArray:
		encodeArray(variable, s);
		break;
	case VariableType::tStruct:
		encodeStruct(variable, s);
		break;
	case VariableType::tBoolean:
		encodeBoolean(variable, s);
		break;
	case VariableType::tInteger:
		encodeInteger(variable, s);
		break;
	case VariableType::tInteger64:
		encodeInteger64(variable, s);
		break;
	case VariableType::tFloat:
		encodeFloat(variable, s);
		break;
	case VariableType::tBase64:
		encodeString(variable, s);
		break;
	case VariableType::tString:
		encodeString(variable, s);
		break;
	case VariableType::tVoid:
		encodeVoid(variable, s);
		break;
	case VariableType::tVariant:
		encodeVoid(variable, s);
		break;
	case VariableType::tBinary:
		encodeVoid(variable, s);
		break;
	}
}

void JsonEncoder::encodeArray(const PVariable& variable, std::ostringstream& s)
{
	s << '[';
	if(!variable->arrayValue->empty())
	{
		encodeValue(variable->arrayValue->at(0), s);
		for(std::vector<PVariable>::iterator i = ++variable->arrayValue->begin(); i != variable->arrayValue->end(); ++i)
		{
			s << ',';
			encodeValue(*i, s);
		}
	}
	s << ']';
}

void JsonEncoder::encodeArray(const PVariable& variable, std::vector<char>& s)
{
	s.push_back('[');
	if(!variable->arrayValue->empty())
	{
		encodeValue(variable->arrayValue->at(0), s);
		for(std::vector<PVariable>::iterator i = ++variable->arrayValue->begin(); i != variable->arrayValue->end(); ++i)
		{
			s.push_back(',');
			encodeValue(*i, s);
		}
	}
	s.push_back(']');
}

void JsonEncoder::encodeStruct(const PVariable& variable, std::ostringstream& s)
{
	s << '{';
	if(!variable->structValue->empty())
	{
		s << '"';
		s << variable->structValue->begin()->first;
		s << "\":";
		encodeValue(variable->structValue->begin()->second, s);
		for(std::map<std::string, PVariable>::iterator i = ++variable->structValue->begin(); i != variable->structValue->end(); ++i)
		{
			s << ',';
			s << '"';
			s << encodeString(i->first);
			s << "\":";
			encodeValue(i->second, s);
		}
	}
	s << '}';
}

void JsonEncoder::encodeStruct(const PVariable& variable, std::vector<char>& s)
{
	s.push_back('{');
	if(!variable->structValue->empty())
	{
		s.push_back('"');
		s.insert(s.end(), variable->structValue->begin()->first.begin(), variable->structValue->begin()->first.end());
		s.push_back('"');
		s.push_back(':');
		encodeValue(variable->structValue->begin()->second, s);
		for(std::map<std::string, PVariable>::iterator i = ++variable->structValue->begin(); i != variable->structValue->end(); ++i)
		{
			s.push_back(',');
			s.push_back('"');
			std::string key = encodeString(i->first);
			s.insert(s.end(), key.begin(), key.end());
			s.push_back('"');
			s.push_back(':');
			encodeValue(i->second, s);
		}
	}
	s.push_back('}');
}

void JsonEncoder::encodeBoolean(const PVariable& variable, std::ostringstream& s)
{
	s << ((variable->booleanValue) ? "true" : "false");
}

void JsonEncoder::encodeBoolean(const PVariable& variable, std::vector<char>& s)
{
	if(variable->booleanValue)
	{
		s.push_back('t');
		s.push_back('r');
		s.push_back('u');
		s.push_back('e');
	}
	else
	{
		s.push_back('f');
		s.push_back('a');
		s.push_back('l');
		s.push_back('s');
		s.push_back('e');
	}
}

void JsonEncoder::encodeInteger(const PVariable& variable, std::ostringstream& s)
{
	s << std::to_string(variable->integerValue);
}

void JsonEncoder::encodeInteger(const PVariable& variable, std::vector<char>& s)
{
	std::string value(std::to_string(variable->integerValue));
	s.insert(s.end(), value.begin(), value.end());
}

void JsonEncoder::encodeInteger64(const PVariable& variable, std::ostringstream& s)
{
	s << std::to_string(variable->integerValue64);
}

void JsonEncoder::encodeInteger64(const PVariable& variable, std::vector<char>& s)
{
	std::string value(std::to_string(variable->integerValue64));
	s.insert(s.end(), value.begin(), value.end());
}

void JsonEncoder::encodeFloat(const PVariable& variable, std::ostringstream& s)
{
	s << std::fixed << std::setprecision(15) << variable->floatValue << std::setprecision(6);
	s.unsetf(std::ios_base::floatfield);
}

void JsonEncoder::encodeFloat(const PVariable& variable, std::vector<char>& s)
{
	std::string value(toString(variable->floatValue));
	s.insert(s.end(), value.begin(), value.end());
}

#if __GNUC__ > 4

std::string JsonEncoder::encodeString(const std::string& s)
{
	std::string result;
	std::u16string utf16 = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(s.data());
	result.reserve(utf16.size() * 2);

	if(result.size() + utf16.size() + 128 > result.capacity())
	{
		auto factor = utf16.size() / 1024;
		auto neededSize = result.size() + (factor * 1024) + 1024;
		if(neededSize > result.capacity()) result.reserve(neededSize);
	}

	//The RFC says: "All Unicode characters may be placed within the quotation marks except for the characters that must
	//be escaped: quotation mark, reverse solidus, and the control characters (U+0000 through U+001F)."

	//Source: https://github.com/miloyip/rapidjson/blob/master/include/rapidjson/writer.h
	static const char hexDigits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	static const char escape[256] =
			{
					//0 1 2 3 4 5 6 7 8 9 A B C D E F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'b', 't', 'n', 'u', 'f', 'r', 'u', 'u', // 00-0F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // 10-1F
					0, 0, '"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20-2F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 30-4F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'\\', 0, 0, 0, // 50-5F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60-7F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // 80-9F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // A0-BF
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // C0-DF
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u'  // E0-FF
			};
	for(const char16_t& c : utf16)
	{
		if((uint16_t)c < 256 && escape[(uint8_t)c])
		{
			result.push_back('\\');
			result.push_back(escape[(uint8_t)c]);
			if (escape[c] == 'u')
			{
				result.push_back('0');
				result.push_back('0');
				result.push_back(hexDigits[(uint8_t)c >> 4]);
				result.push_back(hexDigits[(uint8_t)c & 0xF]);
			}
		}
		else
		{
			if((uint16_t)c < 256) result.push_back((char)(uint8_t)c);
			else
			{
				result.push_back('\\');
				result.push_back('u');
				result.push_back(hexDigits[(uint8_t)(c >> 12)]);
				result.push_back(hexDigits[(uint8_t)((c >> 8) & 0x0F)]);
				result.push_back(hexDigits[(uint8_t)((c >> 4) & 0x0F)]);
				result.push_back(hexDigits[(uint8_t) (c & 0x0F)]);
			}
		}
	}
	return result;
}

void JsonEncoder::encodeString(const std::shared_ptr<Variable>& variable, std::ostringstream& s)
{
	std::u16string utf16 = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(variable->stringValue.data());

	//The RFC says: "All Unicode characters may be placed within the quotation marks except for the characters that must
	//be escaped: quotation mark, reverse solidus, and the control characters (U+0000 through U+001F)."

	//Source: https://github.com/miloyip/rapidjson/blob/master/include/rapidjson/writer.h
	static const char hexDigits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	static const char escape[256] =
			{
					//0 1 2 3 4 5 6 7 8 9 A B C D E F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'b', 't', 'n', 'u', 'f', 'r', 'u', 'u', // 00-0F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // 10-1F
					0, 0, '"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20-2F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 30-4F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'\\', 0, 0, 0, // 50-5F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60-7F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // 80-9F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // A0-BF
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // C0-DF
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u'  // E0-FF
			};
	s << "\"";
	for(std::u16string::iterator i = utf16.begin(); i != utf16.end(); ++i)
	{
		if((uint16_t)*i < 256 && escape[(uint8_t)*i])
		{
			if (escape[(uint8_t)*i] == 'u')
			{
				s << '\\' << escape[(uint8_t)*i];
				s << '0' << '0' << hexDigits[((uint8_t)*i) >> 4] << hexDigits[((uint8_t)*i) & 0xF];
			}
		}
		else
		{
			if((uint16_t)*i < 256) s << (char)(uint8_t)*i;
			else
			{
				s << '\\' << 'u' << hexDigits[(uint8_t)(*i >> 12)] << hexDigits[(uint8_t)((*i >> 8) & 0x0F)] << hexDigits[(uint8_t)((*i >> 4) & 0x0F)] << hexDigits[(uint8_t) (*i & 0x0F)];
			}
		}
	}
	s << "\"";
}

void JsonEncoder::encodeString(const std::shared_ptr<Variable>& variable, std::vector<char>& s)
{
	std::u16string utf16 = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(variable->stringValue.data());
	if(s.size() + utf16.size() + 128 > s.capacity())

	{
		int32_t factor = utf16.size() / 1024;
		uint32_t neededSize = s.size() + (factor * 1024) + 1024;
		if(neededSize > s.capacity()) s.reserve(neededSize);
	}

	//The RFC says: "All Unicode characters may be placed within the quotation marks except for the characters that must
	//be escaped: quotation mark, reverse solidus, and the control characters (U+0000 through U+001F)."

	//Source: https://github.com/miloyip/rapidjson/blob/master/include/rapidjson/writer.h
	static const char hexDigits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	static const char escape[256] =
			{
					//0 1 2 3 4 5 6 7 8 9 A B C D E F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'b', 't', 'n', 'u', 'f', 'r', 'u', 'u', // 00-0F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // 10-1F
					0, 0, '"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20-2F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 30-4F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'\\', 0, 0, 0, // 50-5F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60-7F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // 80-9F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // A0-BF
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // C0-DF
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u'  // E0-FF
			};
	s.push_back('"');
	for(const char16_t c : utf16)
	{
		if((uint16_t)c < 256 && escape[(uint8_t)c])
		{
			s.push_back('\\');
			s.push_back(escape[(uint8_t)c]);
			if (escape[(uint8_t)c] == 'u')
			{
				s.push_back('0');
				s.push_back('0');
				s.push_back(hexDigits[(uint8_t)c >> 4]);
				s.push_back(hexDigits[(uint8_t)c & 0xF]);
			}
		}
		else
		{
			if((uint16_t)c < 256) s.push_back((char)(uint8_t)c);
			else
			{
				s.push_back('\\');
				s.push_back('u');
				s.push_back(hexDigits[(uint8_t)(c >> 12)]);
				s.push_back(hexDigits[(uint8_t)((c >> 8) & 0x0F)]);
				s.push_back(hexDigits[(uint8_t)((c >> 4) & 0x0F)]);
				s.push_back(hexDigits[(uint8_t) (c & 0x0F)]);
			}
		}
	}
	s.push_back('"');
}

#else

std::string JsonEncoder::encodeString(const std::string& s)
{
	std::string result;
	result.reserve(s.size() * 2);

	if(result.size() + s.size() + 128 > result.capacity())
	{
		auto factor = s.size() / 1024;
		auto neededSize = result.size() + (factor * 1024) + 1024;
		if(neededSize > result.capacity()) result.reserve(neededSize);
	}

	//Source: https://github.com/miloyip/rapidjson/blob/master/include/rapidjson/writer.h
	static const char hexDigits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	static const char escape[256] =
			{
					//0 1 2 3 4 5 6 7 8 9 A B C D E F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'b', 't', 'n', 'u', 'f', 'r', 'u', 'u', // 00-0F
					'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // 10-1F
					0, 0, '"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20-2F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 30-4F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'\\', 0, 0, 0, // 50-5F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60-7F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80-9F
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A0-BF
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // C0-DF
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // E0-FF
			};
	for(const uint8_t& c : s)
	{
		if(escape[c])
		{
			result.push_back('\\');
			result.push_back(escape[c]);
			if (escape[c] == 'u')
			{
				result.push_back('0');
				result.push_back('0');
				result.push_back(hexDigits[c >> 4]);
				result.push_back(hexDigits[c & 0xF]);
			}
		}
		else result.push_back(c);
	}
	return result;
}

void JsonEncoder::encodeString(const PVariable& variable, std::ostringstream& s)
{
	//Source: https://github.com/miloyip/rapidjson/blob/master/include/rapidjson/writer.h
	static const char hexDigits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	static const char escape[256] =
	{
		//0 1 2 3 4 5 6 7 8 9 A B C D E F
		'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'b', 't', 'n', 'u', 'f', 'r', 'u', 'u', // 00-0F
		'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // 10-1F
		0, 0, '"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20-2F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 30-4F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'\\', 0, 0, 0, // 50-5F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60-7F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80-9F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A0-BF
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // C0-DF
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // E0-FF
	};
	s << "\"";
	for(std::string::iterator i = variable->stringValue.begin(); i != variable->stringValue.end(); ++i)
	{
		if(escape[(uint8_t)*i])
		{
			s << '\\' << escape[(uint8_t)*i];
			if (escape[(uint8_t)*i] == 'u')
			{
				s << '0' << '0' << hexDigits[((uint8_t)*i) >> 4] << hexDigits[((uint8_t)*i) & 0xF];
			}
		}
		else s << *i;
	}
	s << "\"";
}

void JsonEncoder::encodeString(const PVariable& variable, std::vector<char>& s)
{
	if(s.size() + variable->stringValue.size() + 128 > s.capacity())

	{
		int32_t factor = variable->stringValue.size() / 1024;
		uint32_t neededSize = s.size() + (factor * 1024) + 1024;
		if(neededSize > s.capacity()) s.reserve(neededSize);
	}

	//Source: https://github.com/miloyip/rapidjson/blob/master/include/rapidjson/writer.h
	static const char hexDigits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	static const char escape[256] =
	{
		//0 1 2 3 4 5 6 7 8 9 A B C D E F
		'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'b', 't', 'n', 'u', 'f', 'r', 'u', 'u', // 00-0F
		'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', // 10-1F
		0, 0, '"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20-2F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 30-4F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'\\', 0, 0, 0, // 50-5F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60-7F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80-9F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A0-BF
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // C0-DF
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // E0-FF
	};
	s.push_back('"');
	for(const uint8_t& c : variable->stringValue)
	{
		if(escape[c])
		{
			s.push_back('\\');
			s.push_back(escape[c]);
			if (escape[c] == 'u')
			{
				s.push_back('0');
				s.push_back('0');
				s.push_back(hexDigits[c >> 4]);
				s.push_back(hexDigits[c & 0xF]);
			}
		}
		else s.push_back(c);
	}
	s.push_back('"');
}

#endif

void JsonEncoder::encodeVoid(const PVariable& variable, std::ostringstream& s)
{
	s << "null";
}

void JsonEncoder::encodeVoid(const PVariable& variable, std::vector<char>& s)
{
	s.push_back('n');
	s.push_back('u');
	s.push_back('l');
	s.push_back('l');
}

std::string JsonEncoder::toString(double number)
{
	std::stringstream out;
    out << number;
    std::string string = out.str();
    std::string::size_type pos = string.find('.');
    if(pos == std::string::npos) return string;
    int32_t decimalPlaces = string.size() - pos - 1;
    if(decimalPlaces > 3 && string[string.size() - 2] == string.back() && string[string.size() - 3] == string.back())
    {
    	double factor = std::pow(10, decimalPlaces - 1);
		//Round to 9 digits
		number = std::floor(number * factor + 0.5) / factor;
		out.str(std::string());
		out.clear();
		out << number;
    }
    return out.str();
}

}
