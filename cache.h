#ifndef CACHE_H
#define CACHE_H

#include <map>
#include <iostream>
#include <mutex>
#include <unordered_map>

using namespace std;

//template <typename IDType>


template<typename IDType,typename T>
class Cache
{
public:
    Cache(uint32_t cachesize){
        SetCacheSize(cachesize);
    }
    Cache(){

    }

    void SetCacheSize(uint32_t cachesize)
    {
        lock_guard<mutex> lock(cache_mutex);
        this->cachesize=cachesize;
    }

    void Add(IDType id,T item){
        lock_guard<mutex> lock(cache_mutex);

        while(cache.size()>=cachesize)
        {
            cout<<"full -> removing an entry"<<endl;
            RemoveOldestEntry();
        }

        {
            cout<<"free -> adde "<<item<<endl;
            //Info info(id,CurrentTimestamp()+1);
            Info info;
            info.object=item;
            info.id=id;
            info.timestamp=CurrentTimestamp()+1;

            cache[id]=info;
        }

    }

    bool Available(IDType id)
    {
        lock_guard<mutex> lock(cache_mutex);
        return cache.find(id)!=cache.end();
    }

    T Get(IDType id){
        lock_guard<mutex> lock(cache_mutex);

        if(cache.find(id)!=cache.end())
        {

            cache[id].timestamp=CurrentTimestamp()+1;
            return cache[id].object;
        }
        else{
            //! exception
            //return nullptr;
        }

    }

    void Touch(IDType id)
    {
        lock_guard<mutex> lock(cache_mutex);
        if(cache.find(id)!=cache.end())
            cache[id].timestamp=CurrentTimestamp()+1;
    }

    void Clear()
    {
        lock_guard<mutex> lock(cache_mutex);
        cache.clear();
    }

    string Status(){
        lock_guard<mutex> lock(cache_mutex);
        //! implement
        string str="";
        for(auto i:cache)
        {
            cout<<i.first<<" ts: "<<i.second.timestamp<<"\n";
        }
        return str;
    }

private:

    class Info{
    public:
//        Info(IDType identifier,uint32_t ts)
//        {
//            id=identifier;
//            timestamp=ts;
//        }


        bool operator < (Info op2)const{
            return id<op2.id;
        }


    //    bool operator < (const Info op2){
    //        return false;
    //    }

        IDType id;
        uint32_t timestamp;
        T object;

    };

    uint32_t cachesize=0;


    mutex cache_mutex;
    unordered_map<IDType,Info> cache; //! vector could be faster



    void RemoveOldestEntry()
    {
        uint32_t ts=CurrentTimestamp();


        for(auto i :cache){

            if(i.second.timestamp<ts)
            {
                ts=i.second.timestamp;
            }
        }

        auto iterator=cache.end();
        for(auto it=cache.begin();it!=cache.end();it++)
        {
            if(it->second.timestamp==ts)
                iterator=it;
        }
        cache.erase(iterator);
        //return ts;
    }

    uint32_t CurrentTimestamp()
    {
        uint32_t ts=0;
        for(auto i :cache){
            if(i.second.timestamp>ts)
            {
                ts=i.second.timestamp;
            }
        }
        return ts;
    }
};

#endif // CACHE_H
