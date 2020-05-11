#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <map>
#include <mutex>
#include <stdexcept>
#include <unordered_map>

using namespace std;

// template <typename IDType>

template<typename IDType, typename T>
class Cache
{
public:
	Cache(const uint32_t cachesize) : m_cachesize(cachesize) {}
	Cache() {}

	void setCacheSize(const uint32_t cachesize)
	{
		lock_guard<mutex> lock(m_cache_mutex);
		this->m_cachesize = cachesize;
	}

	//! is it good to use a copy here?
	void add(const IDType id, const T item)
	{
		lock_guard<mutex> lock(m_cache_mutex);

		while (m_cache.size() >= m_cachesize)
		{
			cout << "full -> removing an entry" << endl;
			removeOldestEntry();
		}

		{
			// cout<<"free -> adde "<<item<<endl;
			// Info info(id,CurrentTimestamp()+1);
			Info info;
			info.m_object    = item;
			info.m_id        = id;
			info.m_timestamp = currentTimestamp() + 1;

			m_cache[id] = info;
		}
	}

	bool available(const IDType id)
	{
		lock_guard<mutex> lock(m_cache_mutex);
		return m_cache.find(id) != m_cache.end();
	}

	T get(const IDType id)
	{
		lock_guard<mutex> lock(m_cache_mutex);

		if (m_cache.find(id) != m_cache.end())
		{
			m_cache[id].m_timestamp = currentTimestamp() + 1;
			return m_cache[id].m_object;
		}
		else
		{
			throw std::runtime_error("Couldn't find element in cache.");
		}
	}

	void touch(const IDType id)
	{
		lock_guard<mutex> lock(m_cache_mutex);
		if (m_cache.find(id) != m_cache.end())
			m_cache[id].m_timestamp = currentTimestamp() + 1;
	}

	void clear()
	{
		m_extern_lock.lock();
		lock_guard<mutex> lock(m_cache_mutex);
		m_cache.clear();
	}

	string status()
	{
		m_extern_lock.lock();
		lock_guard<mutex> lock(m_cache_mutex);
		//! implement
		string str = "";
		for (auto i : m_cache)
		{
			cout << i.first << " ts: " << i.second.timestamp << "\n";
		}
		return str;
	}

	void lock()
	{
		cout << "LOCK" << endl;
		m_extern_lock.lock();
	}

	void unlock()
	{
		cout << "UNLOCK" << endl;
		m_extern_lock.unlock();
	}

private:
	class Info
	{
	public:
		//        Info(IDType identifier,uint32_t ts)
		//        {
		//            id=identifier;
		//            timestamp=ts;
		//        }

		bool operator<(const Info op2) const { return m_id < op2.m_id; }

		//    bool operator < (const Info op2){
		//        return false;
		//    }

		IDType m_id;
		uint32_t m_timestamp;
		T m_object;
	};

	uint32_t m_cachesize = 0;

	mutex m_cache_mutex;
	mutex m_extern_lock;
	unordered_map<IDType, Info> m_cache;  //! vector could be faster

	void removeOldestEntry()
	{
		uint32_t ts = currentTimestamp();

		for (auto entry : m_cache)
		{
			if (entry.second.m_timestamp < ts)
			{
				ts = entry.second.m_timestamp;
			}
		}

		auto iterator = m_cache.end();
		for (auto it = m_cache.begin(); it != m_cache.end(); ++it)
		{
			if (it->second.m_timestamp == ts)
			{
				iterator = it;
			}
		}
		m_cache.erase(iterator);
		// return ts;
	}

	uint32_t currentTimestamp()
	{
		uint32_t ts = 0;
		for (auto entry : m_cache)
		{
			if (entry.second.m_timestamp > ts)
			{
				ts = entry.second.m_timestamp;
			}
		}
		return ts;
	}
};

#endif  // CACHE_H
