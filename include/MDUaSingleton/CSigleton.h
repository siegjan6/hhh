#ifndef CSIGLETON_H
#define CSIGLETON_H

#include "../../include/MDUaSingleton/MDUaSingleton_Global.h"

using namespace std;

#include <mutex>

//#define SINGLETON_CTOR(x) \
//     private:\
//            x() = default;\
//            x(const x&)=delete;\
//            x& operator=(const x&)=delete;\
//            ~x()=default;

//#define SINGLETON_INSTACNCE(x) SINGLETON_CTOR(x)\
//    public:\
//    static x* Instance()\
//    {static x _instance; return &_instance;}

////采用模板再实现一次,
////使用方法 class YourSingleton: public SingletonBase<YourSingleton>
//template<typename T>  //T 是子类
//class SingletonBase
//{
//    SINGLETON_CTOR(SingletonBase);  //这个还是可以用的
//public:
//    static T&  Instance()
//    {
//        static T t;   //饿汉式
//        return t;
//    }
//};

////再加上今天的学习的std::once_call实现懒汉式
//template<typename T>  //T 是子类
//class SingletonBaseLazy
//{
//    SINGLETON_CTOR(SingletonBaseLazy);  //这个还是可以用的
//public:
//    static T&  Instance()
//    {
//        static std::once_flag flag;
//        std::call_once(flag, [&](){_ptr.reset(new T); });
//        return *_ptr;
//    }
//    static std::unique_ptr<T> _ptr;
//};
//template<typename T>
//std::unique_ptr<T> SingletonBaseLazy<T>::_ptr;


template <typename T>
class CSingleton
{
public:
    static T* GetInstance()
    {
        static std::once_flag of;
        std::call_once(of, [](){
            m_pInstance = new T();
        });
        return m_pInstance;
    }
    static T* m_pInstance;
private:

    explicit CSingleton() {}
    ~CSingleton() {}
};
#define DECL_SINGLETON(T) \
    friend class CSingleton<T>; \
    public: \
        static T* GetInstance(){return CSingleton<T>::GetInstance();}


#define G_UA \
    (QUaServer*)(Singleton::GetInstance()->GetUaServerInstance());


//template <class T>
class MDUASINGLETONSHARED_EXPORT Singleton {
    DECL_SINGLETON(Singleton)
public:

    void* GetUaServerInstance();

private:
    explicit Singleton() {}
    ~Singleton() {}

};

#endif // CSIGLETON_H
