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

#include "Output.h"

namespace Ipc
{

std::function<void(int32_t, std::string)>* Output::_log = nullptr;

Output::Output()
{
}

Output::~Output()
{
}

void Output::init(std::function<void(int32_t, std::string)>* logMethod)
{
	_log = logMethod;
}

void Output::printEx(std::string file, uint32_t line, std::string function, std::string what)
{
	std::string error;
	if(!what.empty()) error = "Error in file " + file + " line " + std::to_string(line) + " in function " + function + ": " + what;
	else error = "Unknown error in file " + file + " line " + std::to_string(line) + " in function " + function + ".";
	if(_log && *_log) (*_log)(2, error);
}

void Output::printCritical(std::string errorString, bool errorCallback)
{
	if(_log && *_log) (*_log)(1, errorString);
}

void Output::printError(std::string errorString)
{
	if(_log && *_log) (*_log)(2, errorString);
}

void Output::printWarning(std::string errorString)
{
	if(_log && *_log) (*_log)(3, errorString);
}

void Output::printInfo(std::string message)
{
	if(_log && *_log) (*_log)(4, message);
}

void Output::printDebug(std::string message, int32_t minDebugLevel)
{
	if(_log && *_log) (*_log)(5, message);
}

void Output::printMessage(std::string message, int32_t minDebugLevel)
{
	if(_log && *_log) (*_log)(minDebugLevel, message);
}

}
