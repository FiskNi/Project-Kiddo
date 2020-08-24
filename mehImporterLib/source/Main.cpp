#include "Loader.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//std::ofstream outfile("test.bin", std::ofstream::binary);

	//int nrOfMeshes = 2;

	//int nrOfVerts = 3;

	//vertex * vertecies = new vertex[nrOfVerts];

	//for (int i = 0; i < nrOfVerts; i++)
	//{
	//	vertecies[i].pos[0] = 0.5 + i;
	//	vertecies[i].pos[1] = 0.6 + i;
	//	vertecies[i].pos[2] = 0.7 + i;

	//	vertecies[i].uv[0] = i;
	//	vertecies[i].uv[1] = i+1;

	//	vertecies[i].normal[0] = 0.1 + i;
	//	vertecies[i].normal[1] = 0.2 + i;
	//	vertecies[i].normal[2] = 0.3 + i;
	//}

	//int mNumVerts = 4;

	//vertex * vertes = new vertex[mNumVerts];

	//for (int i = 0; i < mNumVerts; i++)
	//{
	//	vertes[i].pos[0] = 0.55 + i;
	//	vertes[i].pos[1] = 0.66 + i;
	//	vertes[i].pos[2] = 0.77 + i;

	//	vertes[i].uv[0] = i;
	//	vertes[i].uv[1] = i + 1;

	//	vertes[i].normal[0] = 0.11 + i;
	//	vertes[i].normal[1] = 0.22 + i;
	//	vertes[i].normal[2] = 0.33 + i;
	//}

	//char* outBuff = new char[4];

	//outBuff[0] = 'H';
	//outBuff[1] = 'i';
	//outBuff[2] = 'a';
	//outBuff[3] = '\0';

	//int trying[2] = { 3,42 };

	//float verts[9] = { 1.2,1.2,1.2,2.3,2.3,2.3,3.4,3.4,3.4 };

	//outfile.write((char*)trying, sizeof(int) * 2);
	//outfile.write(outBuff, sizeof(outBuff));
	//outfile.write((char*)verts, sizeof(verts));

	//outfile.close();

	//delete[] outBuff;
	
	//Loader newLoad("multiv2.bin");
	//Loader newLoad("Ambient.bin");

	//Loader newLoad("matnamev3.bin");

	//Loader newLoad("multiMaterial.bin");
	//Loader newLoad("xTestBinary2.bin");
	//Loader newLoad("nakedCube.bin");

	//Loader newLad("hierCubesMat.bin");

	//Loader newLad("hierTanBinormv2.bin");

	//Loader newLad("hierNameswork.bin");

	//Loader newLad("LevelTestBinary.bin");

	//Loader newLad("nrOfTextures.bin");

	//Loader newLad("meshNames.bin");

	Loader newLad("TryCubeBinary.bin");

	system("Pause");

	return 0;
}