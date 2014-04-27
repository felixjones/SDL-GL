#include "Object.h"

#include <new>

xiObject * xiObject::Alloc() {
	return ( xiObject * )malloc( sizeof( xiObject ) );
}

void xiObject::Dealloc() {
	free( this );
}

xiObject * xiObject::Init() {
	if ( !this ) {
		return nullptr;
	}
	memset( this, 0, sizeof( xiObject ) );
	new ( this )xiObject();

	Retain();

	return this;
}

void xiObject::Retain() {
	retainCount++;
}

void xiObject::Release() {
	retainCount--;

	if ( retainCount == 0 ) {
		Dealloc();
	}
}
