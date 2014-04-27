#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <stdint.h>

class xiObject {
public:
	static xiObject *	Alloc();
	xiObject *			Init();
	virtual void		Dealloc();
	void				Retain();
	void				Release();
protected:
	xiObject() {};
private:
	int32_t	retainCount;
};

#endif