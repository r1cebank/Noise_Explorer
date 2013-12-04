#pragma once
#include "ofMain.h"

class ThreadFilter : public ofThread
{
public:
	ThreadFilter(void);
	~ThreadFilter(void);
	void threadedFunction(void);
};

