#ifndef __THREADSAFE_QUEUE_HPP__
#define __THREADSAFE_QUEUE_HPP__

#include "ICommon.hpp"
#include <deque>

NETWORK_BEGIN
	template<typename V>
	class tsqueue
	{
	public:
		tsqueue(){}
		tsqueue(size_t size):m_queue(size) {}
		
		void push(const T& v) 
		{ 
			WriteLock lock(m_mutex);
			m_queue.push_back(v); 
		}

		bool pop(T& v) 
		{
			if (empty())
				return false;

			WriteLock lock(m_mutex);
			v = m_queue.front();
			m_queue.pop_front();
			return true;
		}
		bool empty()
		{
			ReadLock lock(m_mutex);
			return m_queue.empty();
		}
		size_t size()
		{
			ReadLock lock(m_mutex);
			return m_queue.size();
		}
	protected:
		std::deque<T> m_queue;
		boost::shared_mutex m_mutex;
	};

NETWORK_END


#endif // __THREADSAFE_QUEUE_HPP__