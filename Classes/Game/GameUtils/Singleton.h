/********************************************************************
	date  :	2016/01/30   13:26

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>
#include <memory>

/**
 *	µ¥ÀýÄ£Ê½
 */
template<class T>
class Singleton
{
public:
	static T* getInstance()
	{
		if (m_pInstance.get() == nullptr)
			m_pInstance.reset(new T());
		return m_pInstance.get();
	}
protected:
	Singleton(){}
	virtual ~Singleton(){}

	Singleton(const Singleton&);
	void operator=(const Singleton&);
private:
	static std::auto_ptr<T> m_pInstance;
};

template<class T> std::auto_ptr<T> Singleton<T>::m_pInstance(nullptr);

#endif // !SINGLETON_H