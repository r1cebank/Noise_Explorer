#include "DepthBucket.h"

DepthBucket::DepthBucket(int s,int e)
{
	bucket = new int[e];
	start = s;
	end = e;
	init();
}

int DepthBucket::addDepthPixel(unsigned char pixel)
{
	int intValue = (int)pixel;
	if(intValue < start && intValue < end)
		return -1;
	bucket[intValue]++;
}

stringstream DepthBucket::printBucket()
{
	stringstream str;
	cout << "<Printing Bucket>" << endl;
	for (int i = start; i < end; i++)
	{
		cout << "<" << i << ">" << ": " << bucket[i] << endl; 
	}
	return str;
}
void DepthBucket::init()
{
	for(int i = 0; i < end; i++)
		bucket[i] = 0;
}

DepthBucket::DepthBucket(void)
{
	DepthBucket(0, 255);
}


DepthBucket::~DepthBucket(void)
{
}
