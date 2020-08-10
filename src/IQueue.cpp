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

#include "IQueue.h"

namespace Ipc {

IQueue::IQueue(uint32_t queueCount, uint32_t bufferSize) : IQueueBase(queueCount) {
  if (bufferSize < 2000000000) _bufferSize = (int32_t)bufferSize;

  _bufferHead.resize(queueCount);
  _bufferTail.resize(queueCount);
  _bufferCount.resize(queueCount);
  _waitWhenFull.resize(queueCount);
  _buffer.resize(queueCount);
  _queueMutex.reset(new std::mutex[queueCount]);
  _processingThread.resize(queueCount);
  _produceConditionVariable.reset(new std::condition_variable[queueCount]);
  _processingConditionVariable.reset(new std::condition_variable[queueCount]);

  for (int32_t i = 0; i < _queueCount; i++) {
    _bufferHead[i] = 0;
    _bufferTail[i] = 0;
    _bufferCount[i] = 0;
    _stopProcessingThread[i] = true;
  }
}

IQueue::~IQueue() {
  for (int32_t i = 0; i < _queueCount; i++) {
    stopQueue(i);
    _buffer[i].clear();
  }
}

bool IQueue::queueEmpty(int32_t index) {
  return _bufferCount[index] > 0;
}

void IQueue::startQueue(int32_t index, bool waitWhenFull, uint32_t processingThreadCount) {
  if (index < 0 || index >= _queueCount) return;
  _stopProcessingThread[index] = false;
  _bufferHead[index] = 0;
  _bufferTail[index] = 0;
  _bufferCount[index] = 0;
  _waitWhenFull[index] = waitWhenFull;
  for (uint32_t i = 0; i < processingThreadCount; i++) {
    std::shared_ptr<std::thread> thread = std::make_shared<std::thread>();
    *thread = std::thread(&IQueue::process, this, index);
    _processingThread[index].push_back(thread);
  }
  _buffer.at(index).resize(_bufferSize);
}

void IQueue::stopQueue(int32_t index) {
  if (index < 0 || index >= _queueCount) return;
  if (_stopProcessingThread[index]) return;
  _stopProcessingThread[index] = true;
  std::unique_lock<std::mutex> lock(_queueMutex[index]);
  lock.unlock();
  _processingConditionVariable[index].notify_all();
  _produceConditionVariable[index].notify_all();
  for (uint32_t i = 0; i < _processingThread[index].size(); i++) {
    if (_processingThread[index][i]->joinable()) _processingThread[index][i]->join();
  }
  _processingThread[index].clear();
  _buffer[index].clear();

}

bool IQueue::enqueue(int32_t index, std::shared_ptr<IQueueEntry> &entry, bool waitWhenFull) {
  if (index < 0 || index >= _queueCount || !entry || _stopProcessingThread[index]) return true;
  std::unique_lock<std::mutex> lock(_queueMutex[index]);
  if (_waitWhenFull[index] || waitWhenFull) {
    _produceConditionVariable[index].wait(lock, [&] { return _bufferCount[index] < _bufferSize || _stopProcessingThread[index]; });
    if (_stopProcessingThread[index]) return true;
  } else if (_bufferCount[index] >= _bufferSize) return false;

  _buffer[index][_bufferTail[index]] = entry;
  _bufferTail[index] = (_bufferTail[index] + 1) % _bufferSize;
  ++(_bufferCount[index]);

  lock.unlock();
  _processingConditionVariable[index].notify_one();
  return true;
}

void IQueue::process(int32_t index) {
  if (index < 0 || index >= _queueCount) return;
  while (!_stopProcessingThread[index]) {
    try {
      std::unique_lock<std::mutex> lock(_queueMutex[index]);

      _processingConditionVariable[index].wait(lock, [&] { return _bufferCount[index] > 0 || _stopProcessingThread[index]; });
      if (_stopProcessingThread[index]) return;

      do {
        std::shared_ptr<IQueueEntry> entry;

        entry = _buffer[index][_bufferHead[index]];
        _buffer[index][_bufferHead[index]].reset();
        _bufferHead[index] = (_bufferHead[index] + 1) % _bufferSize;
        --_bufferCount[index];

        lock.unlock();

        _produceConditionVariable[index].notify_one();

        if (entry) processQueueEntry(index, entry);

        lock.lock();
      } while (_bufferCount[index] > 0 && !_stopProcessingThread[index]);
    }
    catch (const std::exception &ex) {
      std::cerr << "Error in IQueue::process: " << ex.what() << std::endl;
    }
    catch (...) {
      std::cerr << "Unknown error in IQueue::process" << std::endl;
    }
  }
}

}
