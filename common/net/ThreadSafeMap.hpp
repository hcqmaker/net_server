#ifndef __THREADSAFE_MAP_HPP__
#define __THREADSAFE_MAP_HPP__

#include "ICommon.hpp"
#include <map>


NETWORK_BEGIN
	template<typename K, typename V>
	class tsmap
	{
	public:
		tsmap() {}
		void add(const K& k, const V& v)
		{
			WriteLock lock(m_mutex);
			m_map.insert(std::map<K,V>::value_type(k, v));
		}
		bool remove(const K& k)
		{
			WriteLock lock(m_mutex);
			std::map<K,V>::iterator i = m_map.find(k);
			if (i == m_map.end())
				return false;
			m_map.erase(i);
			return true;
		}
		bool container(const K& k)
		{
			ReadLock lock(m_mutex);
			std::map<K,V>::iterator i = m_map.find(k);
			if (i == m_map.end())
				return false;
			return true;
		}

		bool value(const K& k, V& v)
		{
			std::map<K,V>::iterator i = m_map.find(k);
			if (i == m_map.end())
				return false;
			v = i->second;
			return true;
		}

	protected:
		std::map<K,V> m_map;
		boost::shared_mutex m_mutex;
	};
NETWORK_END



#endif // __THREADSAFE_MAP_HPP__