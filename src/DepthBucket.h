#pragma once
#include <iostream>
#include <sstream>

using namespace std;

class DepthBucket
{
public:
	DepthBucket(void);
	DepthBucket(int,int);
	~DepthBucket(void);
	stringstream printBucket();
	int addDepthPixel(unsigned char);
private:
	int start, end;
	int *bucket;
	void init();
};

