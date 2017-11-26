/* Copyright 2013-2017 Sathya Laufer
 *
 * Homegear is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Homegear is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Homegear.  If not, see
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

#ifndef IIPCCLIENT_H_
#define IIPCCLIENT_H_

#include "IpcResponse.h"
#include "IQueue.h"
#include "Output.h"
#include "RpcEncoder.h"
#include "RpcDecoder.h"
#include "BinaryRpc.h"

#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

#include <thread>
#include <mutex>
#include <string>
#include <unordered_map>
#include <function>

namespace Ipc
{

class IIpcClient : public IQueue
{
public:
	IIpcClient(std::string socketPath);
	virtual ~IIpcClient();
	virtual void dispose();

	PVariable invoke(std::string methodName, PArray& parameters);
	virtual void start();
	virtual void stop();
protected:
	struct RequestInfo
	{
		std::mutex waitMutex;
		std::condition_variable conditionVariable;
	};
	typedef std::shared_ptr<RequestInfo> PRequestInfo;

	class QueueEntry : public IQueueEntry
	{
	public:
		QueueEntry() {}
		QueueEntry(std::vector<char>& packet) { this->packet = packet; }
		virtual ~QueueEntry() {}

		std::vector<char> packet;
	};

	std::mutex _disposeMutex;
	bool _disposing = false;
	std::string _socketPath;
	int32_t _fileDescriptor = -1;
	int64_t _lastGargabeCollection = 0;
	std::atomic_bool _stopped;
	std::atomic_bool _closed;
	std::mutex _sendMutex;
	std::mutex _rpcResponsesMutex;
	std::unordered_map<int64_t, std::unordered_map<int32_t, PIpcResponse>> _rpcResponses;
	std::map<std::string, std::function<PVariable(PArray& parameters)>> _localRpcMethods;
	std::thread _mainThread;
	std::thread _maintenanceThread;
	std::mutex _requestInfoMutex;
	std::map<int64_t, PRequestInfo> _requestInfo;
	std::mutex _packetIdMutex;
	int32_t _currentPacketId = 0;

	std::unique_ptr<BinaryRpc> _binaryRpc;
	std::unique_ptr<RpcDecoder> _rpcDecoder;
	std::unique_ptr<RpcEncoder> _rpcEncoder;

	void connect();
	void mainThread();
	void sendResponse(PVariable& packetId, PVariable& variable);

	void processQueueEntry(int32_t index, std::shared_ptr<IQueueEntry>& entry);
	PVariable send(std::vector<char>& data);

	virtual void onConnect() = 0;

	// {{{ RPC methods
		virtual Ipc::PVariable broadcastEvent(Ipc::PArray& parameters) { return Ipc::PVariable(new Ipc::Variable()); }
		virtual Ipc::PVariable broadcastNewDevices(Ipc::PArray& parameters) { return Ipc::PVariable(new Ipc::Variable()); }
		virtual Ipc::PVariable broadcastDeleteDevices(Ipc::PArray& parameters) { return Ipc::PVariable(new Ipc::Variable()); }
		virtual Ipc::PVariable broadcastUpdateDevice(Ipc::PArray& parameters) { return Ipc::PVariable(new Ipc::Variable()); }
	// }}}
};

}
#endif
