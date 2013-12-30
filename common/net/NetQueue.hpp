#ifndef __NET_QUEUE_HPP__
#define __NET_QUEUE_HPP__

#include "net/ICommon.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <deque>

typedef boost::shared_lock<boost::shared_mutex> ReadLock;  
typedef boost::unique_lock<boost::shared_mutex> WriteLock;

NETWORK_BEGIN
	typedef std::deque<ByteBuffer> ByteBufferQueue;
class NetQueue
{
public:
	bool isEmpty() 
	{ 
		ReadLock ulock(m_mutex);
		return m_queue.empty(); 
	}
	void push(const ByteBuffer& ptr) 
	{ 
		WriteLock ulock(m_mutex);
		m_queue.push_back(ptr); 
	}

	bool pop(ByteBuffer& ptr)
	{ 
		WriteLock ulock(m_mutex);
		if (m_queue.empty())
			return false;
		ptr = m_queue.front(); 
		m_queue.pop_front(); 
		return true;
	}
private:
	ByteBufferQueue m_queue;
	boost::shared_mutex m_mutex;
};
NETWORK_END

#endif // __NET_QUEUE_HPP__