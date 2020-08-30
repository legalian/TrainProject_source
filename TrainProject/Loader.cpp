




#include "ProceduralTerrain.h"





int32 readInt32(std::ifstream& f) {
	int u = 0;
	if (f.fail()) throw 22;
	f.read((char*)&u, sizeof(int));
	if (f.fail()) throw 22;
	return u;
}
FString readString(std::ifstream& f) {
	unsigned int namesize;
	if (f.fail()) throw 22;
	f.read((char*)&namesize, sizeof(unsigned int));
	if (f.fail()) throw 22;
	char* name = new char[namesize + 1];
	name[namesize] = '\0';
	if (f.fail()) throw 22;
	f.read(name, namesize);
	if (f.fail()) throw 22;
	return FString(name);
}
void AProceduralTerrain::initFromFile() {
	static bool pulledFromFile = false;
	if (pulledFromFile) return;
	pulledFromFile = true;
	std::ifstream f;
	FString tilesetfile = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir() + "tileset");
	f.open(TCHAR_TO_ANSI(*tilesetfile), std::ios::in | std::ios::binary);
	if (!f.is_open()) throw 22;
	int32 n = 0;
	n = readInt32(f);
	UE_LOG(LogTemp, Warning, TEXT("point hit %d %d %d"), n, sizeof(int), sizeof(int32));
	//UE_LOG(LogTemp, Warning, n);
	//return;
	for (int32 m = 0;m < n;m++) {
		Geom tgeo;
		FString fkey = readString(f);
		int32 vertcount = readInt32(f);
		tgeo.vertices.Reserve(vertcount);
		tgeo.normals.Reserve(vertcount);
		for (int32 i = 0;i < vertcount;i++) {
			float vert[6];
			f.read((char*)vert, sizeof(float) * 6);
			tgeo.vertices.Add(FVector(vert[0], vert[1], vert[2]));
			tgeo.normals.Add(FVector(vert[3], vert[4], vert[5]));
		}
		int32 tricount = readInt32(f);
		tgeo.triangles.Reserve(tricount);
		for (int32 i = 0;i < tricount * 3;i++) tgeo.triangles.Add(readInt32(f));
		int32 interfacecount = readInt32(f);
		tgeo.interfaces.Reserve(interfacecount);
		for (int32 i = 0;i < interfacecount;i++) {
			FTGeoInterface gi;
			f.read((char*)&gi.x, sizeof(int));
			f.read((char*)&gi.y, sizeof(int));
			f.read((char*)&gi.z, sizeof(int));
			f.read((char*)&gi.id, sizeof(int));
			int32 remapcount = readInt32(f);
			gi.trefs.Reserve(remapcount);
			for (int32 j = 0;j < remapcount;j++) {
				gi.trefs.Add(readInt32(f));
			}
			tgeo.interfaces.Add(gi);
		}
		Geoms.Add(fkey, tgeo);
	}
	int32 r = readInt32(f);
	Rotations.Reserve(r);
	for (int32 s = 0;s < r;s++) {
		GeoRotator gr;
		int32 remapcount;
		gr.r90.id = readInt32(f);
		remapcount = readInt32(f);
		gr.r90.trefs.Reserve(remapcount);
		for (int32 j = 0;j < remapcount;j++) gr.r90.trefs.Add(readInt32(f));
		gr.r180.id = readInt32(f);
		remapcount = readInt32(f);
		gr.r180.trefs.Reserve(remapcount);
		for (int32 j = 0;j < remapcount;j++) gr.r180.trefs.Add(readInt32(f));
		gr.r270.id = readInt32(f);
		remapcount = readInt32(f);
		gr.r270.trefs.Reserve(remapcount);
		for (int32 j = 0;j < remapcount;j++) gr.r270.trefs.Add(readInt32(f));
		Rotations.Add(gr);
	}
}



