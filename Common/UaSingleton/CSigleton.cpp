#include "CSigleton.h"
#include <QUaServer>

//template <class T>
//T &Singleton<T>::instance() {
//  static T _instance;
//  return _instance;
//};

//void instantiate_uaserver() {
//  CSingleton<QUaServer>::GetInstance();
////  GetUaInstance();
//}

template<typename T>
T* CSingleton<T>::m_pInstance = nullptr;


void* Singleton::GetUaServerInstance(){
    return CSingleton<QUaServer>::GetInstance();
}
