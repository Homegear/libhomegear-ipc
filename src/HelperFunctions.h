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

#ifndef NODEHELPERFUNCTIONS_H_
#define NODEHELPERFUNCTIONS_H_

#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <chrono>

namespace Ipc
{

/**
 * This class provides functions to make your life easier.
 */
class HelperFunctions
{
public:
	/**
	 * Constructor.
	 * It does nothing. You need to call init() to initialize the object.
	 */
	HelperFunctions();

	/**
	 * Destructor.
	 * Does nothing.
	 */
	virtual ~HelperFunctions();

	/**
	 * Left trims a string.
	 *
	 * @see rtrim()
	 * @see trim()
	 * @param[in,out] s The string to left trim.
	 * @return Returns a reference to the left trimmed string.
	 */
	static inline std::string &ltrim(std::string &s)
	{
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
			return s;
	}

	/**
	 * Right trims a string.
	 *
	 * @see ltrim()
	 * @see trim()
	 * @param[in,out] s The string to right trim.
	 * @return Returns a reference to the right trimmed string.
	 */
	static inline std::string &rtrim(std::string &s)
	{
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
			return s;
	}

	/**
	 * Trims a string.
	 *
	 * @see ltrim()
	 * @see rtrim()
	 * @param[in,out] s The string to trim.
	 * @return Returns a reference to the trimmed string.
	 */
	static inline std::string& trim(std::string& s)
	{
			return ltrim(rtrim(s));
	}

	/**
	 * Converts all characters of a string to lower case.
	 *
	 * @see toUpper()
	 * @param[in,out] s The string to convert all characters to lower case for.
	 * @return Returns a reference to the lower case string.
	 */
	static inline std::string& toLower (std::string& s)
	{
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		return s;
	}

	/**
	 * Converts all characters of a string to upper case.
	 *
	 * @see toLower()
	 * @param[in,out] s The string to convert all characters to upper case for.
	 * @return Returns a reference to the upper case string.
	 */
	static inline std::string& toUpper (std::string& s)
	{
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		return s;
	}

	/**
	 * Converts a byte array to a hex string.
	 *
	 * @param buffer The byte array to convert.
	 * @param size The size of the buffer.
	 * @return Returns the hex string of the byte array.
	 */
	static std::string getHexString(const uint8_t* buffer, uint32_t size);

	/**
	 * Converts a byte array to a hex string.
	 *
	 * @param buffer The byte array to convert.
	 * @param size The size of the buffer.
	 * @return Returns the hex string of the byte array.
	 */
	static std::string getHexString(const char* buffer, uint32_t size);

	/**
	 * Converts a byte array to a hex string.
	 *
	 * @param data The byte array to convert.
	 * @return Returns the hex string of the byte array.
	 */
	static std::string getHexString(const std::vector<char>& data);

	/**
	 * Converts a byte array to a hex string.
	 *
	 * @param data The byte array to convert.
	 * @return Returns the hex string of the byte array.
	 */
	static std::string getHexString(const std::string& data);

	/**
	 * Converts a byte array to a hex string.
	 *
	 * @param data The byte array to convert.
	 * @return Returns the hex string of the byte array.
	 */
	static std::string getHexString(const std::vector<uint8_t>& data);

	/**
	 * Converts an int16 array to a hex string.
	 *
	 * @param data The array to convert.
	 * @return Returns the hex string of the array.
	 */
	static std::string getHexString(const std::vector<uint16_t>& data);

	/**
	 * Converts an integer to a hex string.
	 *
	 * @param number The integer to convert.
	 * @param width The minimal width of the hex string (default -1). If the hex string is smaller, it is prefixed with zeros.
	 * @return Returns the hex string of the integer.
	 */
	static std::string getHexString(int32_t number, int32_t width = -1);

	/**
	 * Gets the current unix time stamp in milliseconds.
	 *
	 * @see getTimeSeconds()
	 * @see getTimeMicroseconds()
	 * @return The current unix time stamp in milliseconds.
	 */
	static int64_t getTime();

	/**
	 * Gets the current unix time stamp in microseconds.
	 *
	 * @see getTimeSeconds()
	 * @see getTime()
	 * @return The current unix time stamp in microseconds.
	 */
	static int64_t getTimeMicroseconds();

	/**
	 * Gets the current unix time stamp in seconds.
	 *
	 * @see getTime()
	 * @see getTimeMicroseconds()
	 * @return The current unix time stamp in seconds.
	 */
	static int32_t getTimeSeconds();

	/**
	 * Gets the current time as a string like "08/27/14 14:13:53.471".
	 *
	 * @param time The unix time stamp in milliseconds to get the time string for. If "0" the current time is returned.
	 * @return Returns a time string like "08/27/14 14:13:53.471".
	 */
	static std::string getTimeString(int64_t time = 0);
};

}
#endif
