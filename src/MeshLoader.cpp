#include "MeshLoader.h"


MeshLoader::MeshLoader(string path)
{
	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.load(path);
}

void MeshLoader::loadMesh(string path)
{
	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.load(path);
}


MeshLoader::~MeshLoader(void)
{
}


ofMesh MeshLoader::getMesh()
{
	return mesh;
}