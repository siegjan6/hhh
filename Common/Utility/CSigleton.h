#ifndef CSIGLETON_H
#define CSIGLETON_H

using namespace std;

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

private:
    explicit CSingleton() {}
    ~CSingleton() {}
};
#define DECL_SINGLETON(T) friend class CSingleton<T>;

#endif // CSIGLETON_H
