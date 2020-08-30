// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include <fstream>
#include "ProceduralTerrain.generated.h"


struct GeoTransform {
public:
	int32 id;
	TArray<int32> trefs;
};

struct GeoRotator {
public:
	GeoTransform r90;
	GeoTransform r180;
	GeoTransform r270;
};


struct FTGeoInterface {
public:
	int x;
	int y;
	int z;
	int32 id;
	TArray<int32> trefs;
};

struct Geom {
public:
	TArray<FVector> vertices;
	TArray<FVector> normals;
	TArray<int32> triangles;
	TArray<FTGeoInterface> interfaces;
};


struct FTModel {
public:
	FString ref;
	int rot;
	int x;
	int y;
	int z;
};

struct PatternFeature {
public:
	int x;
	int y;
	int z;
	int dim;
	int sm;
	int lg;
	int sym;
	int xs;
	int ys;
};
struct Pattern {
public:
	int xs;
	int ys;
	int zs;
	int sym;
	int* flagfield;
	TArray<FTModel> models;
	int getAt(int x,int y,int z);
	PatternFeature extractFeature();
	bool symmetrical_180();
	bool symmetrical_90();
};

//structure



//s

//choose variant, then display faces if needed.
//patternms for choosing variants

//geometry shown inherently
//geometry shown for each reactor block exposed to air.12



UCLASS()
class TRAINPROJECT_API AProceduralTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralTerrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* CustomMesh;

	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<int32> Triangles;
	static TMap<FString,Geom> Geoms;
	static TArray<GeoRotator> Rotations;
	static TArray<Pattern> PatternMapper;
	static TArray<PatternFeature> PatternFeatures;
	static void initMapper();
	static void initFromFile();

	void recalcOuterAir();
	void resetOuterAir();
	void growOuterAir();

	void patternMatch();
	//void generateMesh();

	void addGeom(Geom geo,int x,int y,int z,int rot);

	//void initializeGrid(int xs, int ys, int zs);
	//void uninitializeGrid();
	int getAt(int x,int y,int z);
	void setAt(int x,int y,int z,int s);
	bool matches(Pattern p,int x,int y,int z,int rot);
	int* grid;
	int sx;
	int sy;
	int sz;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
