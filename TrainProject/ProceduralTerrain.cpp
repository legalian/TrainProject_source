// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralTerrain.h"





TMap<FString, Geom> AProceduralTerrain::Geoms;
TArray<GeoRotator> AProceduralTerrain::Rotations;
TArray<Pattern> AProceduralTerrain::PatternMapper;
TArray<PatternFeature> AProceduralTerrain::PatternFeatures;



// Sets default values
AProceduralTerrain::AProceduralTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;



}


// Called when the game starts or when spawned
void AProceduralTerrain::BeginPlay()
{
	initFromFile();
	initMapper();


	
	sx = 5;
	sy = 5;
	sz = 3;
	grid = new int[sx*sy*sz];
	for (int x = 0;x < sx;x++) {
		for (int y = 0;y < sy;y++) {
			setAt(x,y,0,1);
			for (int z = 1;z < sz;z++) {
				if (x == 0 || y == 0 || (x == 1 && y == 4) || (x == 1 && y == 1 && z == 1)) setAt(x, y, z, 2);
				else setAt(x, y, z, 0);
			}
		}
	}

	Super::BeginPlay();
	patternMatch();
	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	FNavigationSystem::UpdateComponentData(*CustomMesh);
}


void rot_inplace(int& x,int& y,int sx,int sy,int rot) {
	if (rot&2) {
		x=sx-x;
		y=sy-y;
	}
	if (rot&1) {
		int swap = sx-x;
		x = y;
		y = swap;
	}
}
bool AProceduralTerrain::matches(Pattern p,int x,int y,int z,int rot) {
	for (int jx=0;jx<p.xs;jx++) {
		for (int jy=0;jy<p.ys;jy++) {
			for (int jz=0;jz<p.zs;jz++) {
				int ix=jx,iy=jy;
				rot_inplace(ix,iy,p.xs-1,p.ys-1,rot);
				if (0 == (p.getAt(jx, jy, jz) & (1<<getAt(x + ix, y + iy, z + jz)))) return false;
			}
		}
	}
	return true;
}
void AProceduralTerrain::patternMatch() {
	static int rotlookup[] = {
		0,0,0,0,
		1,1,0,1,
		1,0,1,0,
		0,1,0,0,

		0,1,0,0,
		0,0,0,0,
		1,1,0,1,
		1,0,1,0,

		0,2,0,0,
		0,2,0,0,
		0,2,0,0,
		0,2,0,0
	};
	for (int x=-1;x<sx;x++) {
		for (int y=-1;y<sy;y++) {
			for (int z=-1;z<sz;z++) {
				int root = getAt(x,y,z);
				int l[3] = {getAt(x+1,y,z),getAt(x,y+1,z),getAt(x,y,z+1)};
				if (root!=l[0] || root!=l[1] || root!=l[2]) {
					for (int u=0;u<PatternFeatures.Num();u++) {
						PatternFeature t = PatternFeatures[u];
						for (int o=0;o<t.sym;o++) {
							int tx = t.x;
							int ty = t.y;
							rot_inplace(tx, ty, t.xs-1, t.ys-1, o);
							if ((rotlookup[t.dim*16+o*4]==0 && (1<<root)&t.sm && (1<<l[rotlookup[t.dim*16+o*4+1]])&t.lg) || 
								(rotlookup[t.dim*16+o*4]==1 && (1<<root)&t.lg && (1<<l[rotlookup[t.dim*16+o*4+1]])&t.sm)) {
								Pattern p = PatternMapper[u];
								int mtx = x - tx + rotlookup[t.dim * 16 + o * 4 + 2];
								int mty = y - ty + rotlookup[t.dim * 16 + o * 4 + 3];
								if (matches(p,mtx,mty,z-t.z,o)) {
									FTModel m = p.models[0];
									if (u<7) UE_LOG(LogTemp, Warning, TEXT(" MATCH FOUDN %s %d"), *m.ref,o);
									int mx = m.x,my = m.y;
									rot_inplace(mx,my,p.xs,p.ys,o);
									addGeom(Geoms[m.ref],mtx+mx,mty+my,z-t.z+m.z,(m.rot+o)%4);
								}
							}
						}
					}
				}
			}
		}
	}
}
int AProceduralTerrain::getAt(int x,int y,int z) {
	if (x<0 || y<0 || z<0) return 0;
	if (x>=sx || y>=sy || z>=sz) return 0;
	return grid[x+y*sx+z*sy*sx];
}
void AProceduralTerrain::setAt(int x,int y,int z,int s) {
	grid[x+y*sx+z*sy*sx] = s;
}
void AProceduralTerrain::recalcOuterAir() {
	resetOuterAir();
	growOuterAir();
}
void AProceduralTerrain::resetOuterAir() {
	for (int t=0;t<sx*sy*sz;t++) {
		if (grid[t]==7) grid[t]=0;
	}
}
void AProceduralTerrain::growOuterAir() {
	for (int y=0;y<sy;y++) {
		bool found=false;
		for (int x=0;x<sx;x++) {
			for (int z=0;z<sz;z++) {
				if (getAt(x,y,z)==0 && getAt(x,y-1,z)==7 && getAt(x-1,y-1,z)==7 && getAt(x+1,y-1,z)==7 && getAt(x,y-1,z-1)==7 && getAt(x,y-1,z+1)==7) {
					setAt(x,y,z,7);
					found = true;
				}
			}
		}
		if (!found) break;
	}
	for (int y=sy-1;y>=0;y--) {
		bool found=false;
		for (int x=0;x<sx;x++) {
			for (int z=0;z<sz;z++) {
				if (getAt(x,y,z)==0 && getAt(x,y+1,z)==7 && getAt(x-1,y+1,z)==7 && getAt(x+1,y+1,z)==7 && getAt(x,y+1,z-1)==7 && getAt(x,y+1,z+1)==7) {
					setAt(x,y,z,7);
					found = true;
				}
			}
		}
		if (!found) break;
	}
	for (int x=0;x<sx;x++) {
		bool found=false;
		for (int y=0;y<sy;y++) {
			for (int z=0;z<sz;z++) {
				if (getAt(x,y,z)==0 && getAt(x-1,y,z)==7 && getAt(x-1,y-1,z)==7 && getAt(x-1,y+1,z)==7 && getAt(x-1,y,z-1)==7 && getAt(x-1,y,z+1)==7) {
					setAt(x,y,z,7);
					found = true;
				}
			}
		}
		if (!found) break;
	}
	for (int x=sx-1;x>=0;x--) {
		bool found=false;
		for (int y=0;y<sy;y++) {
			for (int z=0;z<sz;z++) {
				if (getAt(x,y,z)==0 && getAt(x+1,y,z)==7 && getAt(x+1,y-1,z)==7 && getAt(x+1,y+1,z)==7 && getAt(x+1,y,z-1)==7 && getAt(x+1,y,z+1)==7) {
					setAt(x,y,z,7);
					found = true;
				}
			}
		}
		if (!found) break;
	}
}



void AProceduralTerrain::addGeom(Geom geo,int x,int y,int z,int rot) {
	int32 offset = Vertices.Num();
	for (int u = 0;u < geo.triangles.Num();u += 3) {
		Triangles.Add(offset + geo.triangles[u + 2]);
		Triangles.Add(offset + geo.triangles[u + 1]);
		Triangles.Add(offset + geo.triangles[u]);
	}

	if (rot==0) {
		for (int u=0;u<geo.vertices.Num();u++) {
			Vertices.Add(FVector(x+geo.vertices[u].X,y+geo.vertices[u].Y,z+geo.vertices[u].Z)*100);
			Normals.Add( FVector(  geo.normals[u].X,  geo.normals[u].Y,geo.normals[u].Z));
		}
	} else if (rot==1) {
		for (int u=0;u<geo.vertices.Num();u++) {
			Vertices.Add(FVector(x+geo.vertices[u].Y,y-geo.vertices[u].X,z+geo.vertices[u].Z) * 100);
			Normals.Add( FVector(  geo.normals[u].Y, -geo.normals[u].X,geo.normals[u].Z));
		}
	} else if (rot==2) {
		for (int u=0;u<geo.vertices.Num();u++) {
			Vertices.Add(FVector(x-geo.vertices[u].X,y-geo.vertices[u].Y,z+geo.vertices[u].Z) * 100);
			Normals.Add( FVector( -geo.normals[u].X, -geo.normals[u].Y,geo.normals[u].Z));
		}
	} else if (rot==3) {
		for (int u=0;u<geo.vertices.Num();u++) {
			Vertices.Add(FVector(x-geo.vertices[u].Y,y+geo.vertices[u].X,z+geo.vertices[u].Z) * 100);
			Normals.Add( FVector( -geo.normals[u].Y,  geo.normals[u].X,geo.normals[u].Z));
		}
	}

}


// Called every frame
void AProceduralTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

