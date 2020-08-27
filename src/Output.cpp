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

#include "Output.h"

namespace Ipc {

int32_t Output::_logLevel = 3;
std::mutex Output::_outputMutex;

Output::Output() {
}

Output::~Output() {
}

void Output::setLogLevel(int32_t value) {
  _logLevel = value;
}

std::string Output::getTimeString() {
  const char timeFormat[] = "%x %X";
  std::time_t t;
  int32_t milliseconds;
  const auto timePoint = std::chrono::system_clock::now();
  t = std::chrono::system_clock::to_time_t(timePoint);
  milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timePoint.time_since_epoch()).count() % 1000;
  char timeString[50];
  std::tm localTime;
  localtime_r(&t, &localTime);
  strftime(&timeString[0], 50, &timeFormat[0], &localTime);
  std::ostringstream timeStream;
  timeStream << timeString << "." << std::setw(3) << std::setfill('0') << milliseconds;
  return timeStream.str();
}

void Output::printEx(std::string file, uint32_t line, std::string function, std::string what) {
  if (_logLevel < 2) return;
  std::string error;
  if (!what.empty()) error = "Error in file " + file + " line " + std::to_string(line) + " in function " + function + ": " + what;
  else error = "Unknown error in file " + file + " line " + std::to_string(line) + " in function " + function + ".";
  std::lock_guard<std::mutex> outputGuard(_outputMutex);
  std::cout << getTimeString() << " " << error << std::endl;
  std::cerr << getTimeString() << " " << error << std::endl;
}

void Output::printCritical(std::string errorString, bool errorCallback) {
  if (_logLevel < 1) return;
  std::lock_guard<std::mutex> outputGuard(_outputMutex);
  std::cout << getTimeString() << " " << errorString << std::endl;
  std::cerr << getTimeString() << " " << errorString << std::endl;
}

void Output::printError(std::string errorString) {
  if (_logLevel < 2) return;
  std::lock_guard<std::mutex> outputGuard(_outputMutex);
  std::cout << getTimeString() << " " << errorString << std::endl;
  std::cerr << getTimeString() << " " << errorString << std::endl;
}

void Output::printWarning(std::string errorString) {
  if (_logLevel < 3) return;
  std::lock_guard<std::mutex> outputGuard(_outputMutex);
  std::cout << getTimeString() << " " << errorString << std::endl;
  std::cerr << getTimeString() << " " << errorString << std::endl;
}

void Output::printInfo(std::string message) {
  if (_logLevel < 4) return;
  std::lock_guard<std::mutex> outputGuard(_outputMutex);
  std::cout << getTimeString() << " " << message << std::endl;
}

void Output::printDebug(std::string message, int32_t minDebugLevel) {
  if (_logLevel < minDebugLevel) return;
  std::lock_guard<std::mutex> outputGuard(_outputMutex);
  std::cout << getTimeString() << " " << message << std::endl;
}

void Output::printMessage(std::string message, int32_t minDebugLevel) {
  if (_logLevel < minDebugLevel) return;
  std::lock_guard<std::mutex> outputGuard(_outputMutex);
  std::cout << getTimeString() << " " << message << std::endl;
  if (minDebugLevel <= 3) std::cerr << getTimeString() << " " << message << std::endl;
}

}
