#ifndef _TITANS_CORE_COMMON_Singleton_H_
#define _TITANS_CORE_COMMON_Singleton_H_

#include <cstddef>

namespace TITANS {

template<class T>  class CreateNew
{
public:
    static T* Instance(T* Proto) {
        return new T;
    }
    static void Destroy(T* obj) {
        delete obj;
    }
};


template<class T>  class CreateByProto
{
public:
    static T* Instance(T* Proto) {
        return Proto;
    }

    static void Destroy(T* obj) {
        return;
    }
};


template < class T, template<class> class CreatePolicy = CreateNew > class  Singleton
{
public:

    static T* Instance() {
        if (Instance_ == NULL) {
            Instance_ = CreatePolicy<T>::Instance(ProtoInstance_);
        }

        return  Instance_;
    }

    static void Destroy() {
        CreatePolicy<T>::Destroy(Instance_);
    }

    static void SetProto(T* proto) {
        Instance_ = NULL;
        ProtoInstance_ = proto;
        //	Instance_=NULL;
    }

    static T*  Instance_;
    static T*  ProtoInstance_;
private:
    Singleton(void);
    Singleton(const Singleton&);
    Singleton& operator= (const Singleton&);
};


template <class T, template <class> class CreationPolicy>
T* Singleton <T, CreationPolicy >::Instance_ = NULL;

template <class T, template <class> class CreationPolicy>
T* Singleton <T, CreationPolicy >::ProtoInstance_ = NULL;

#define INS(sSinglton) sSinglton::Instance()

}//namespace TITANS


#endif //_TITANS_CORE_COMMON_Singleton_H_
