#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

#include "base/Define.hpp"

NETWORK_BEGIN
	template <class T> 
	class Singleton { 
	public: 
		static T& instance() { 
			static T _instance; 
			return _instance; 
		} 
	protected: 
		Singleton(void) {} 
		virtual ~Singleton(void) {} 
		Singleton(const Singleton<T>&);
		Singleton<T>& operator= (const Singleton<T> &);
	}; 

NETWORK_END


#endif // __SINGLETON_HPP__