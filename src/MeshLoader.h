#pragma once
#include <string>
#include <ofMain.h>

using namespace std;

class MeshLoader
{
public:
	MeshLoader(string path);
	void loadMesh(string path);
	~MeshLoader(void);
	ofMesh getMesh();
private:
	ofMesh mesh;
};

