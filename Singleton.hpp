#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__


template<class _Target>
class Singleton {
public:
    static _Target& Instance() 
    {
        if(_ptr == nullptr ) 
            _ptr = new _Target;
        return *_ptr;
    }
private:
    static _Target * _ptr;

};

template<class _Target>
_Target * Singleton<_Target>::_ptr = nullptr;

#endif