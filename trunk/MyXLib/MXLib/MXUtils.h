#ifndef MXLIB_UTILS
#define MXLIB_UTILS

#include <windows.h>
#include <memory>
#include <list>
#include <boost/signal.hpp>
#include <D3dx9math.h>

namespace MX
{
	using namespace std;

template <class T>
void safe_delete (T *&a) {
 if (a)
 {
	 delete a;
	 a = NULL;
 }
}

template <class T>
void safe_release (T *&a) {
 if (a)
 {
	 a->Release();
	 a = NULL;
 }
}

template <class T>
class DataFolder
{
public:
	DataFolder(){};
	~DataFolder()
	{
		Deinitialize();
	}

	void Deinitialize()
	{
		for (auto it = data.begin(); it != data.end(); it ++)
			(*it)->Deinitialize();
		data.clear();
	}
	

protected:
	list<shared_ptr<T>> data;

};

RECT MakeR(int x, int y, int w, int h);

float GetDistance(float x1, float y1, float x2, float y2);

/*
class AutoConnection
{
public:
	AutoConnection(){};
	AutoConnection(boost::signals::connection _c) : c(_c) {}
	~AutoConnection(){ c.disconnect(); }

	AutoConnection& operator=(const AutoConnection& other)
	{
		c.disconnect();
		c = other.c;
		return *this;
	}

	AutoConnection& operator=(const boost::signals::connection& other)
	{
		c.disconnect();
		c = other;
		return *this;
	}

	boost::signals::connection& GetConnection() { return c; }
protected:
	boost::signals::connection c;
};*/

class Utils
{
public:
static D3DXMATRIX Identity;
};


};

#endif