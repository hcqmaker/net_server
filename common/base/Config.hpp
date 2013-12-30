#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include "Define.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

NETWORK_BEGIN

	class Config
	{
	public:
		Config(){}
		bool loadIni(const std::string& filename)
		{
			boost::property_tree::ini_parser::read_ini(filename.c_str(), m_pTree);
			return true;
		}

		bool getBool(const char* key) { return m_pTree.get<bool>(key); }
		float getFloat(const char* key) { return m_pTree.get<float>(key); }
		int getInt(const char* key) { return m_pTree.get<int>(key); }
		long getLong(const char* key) { return m_pTree.get<long>(key); }
		std::string getString(const char* key) { return m_pTree.get<std::string>(key); }

	private:
		boost::property_tree::ptree m_pTree;
	};
NETWORK_END

#endif // __CONFIG_HPP__