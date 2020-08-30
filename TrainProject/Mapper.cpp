



#include "ProceduralTerrain.h"




int air = 1;
int simple_floor = 2;
int simple_wall = 4;
int invis_wall = 8;
int outer_air = 128;

// int none = 0;
int any = ~0;
int solid = simple_floor | simple_wall | invis_wall;

int notstructure = air | outer_air;
int some_air = air | outer_air;

int wallair = simple_wall | notstructure;

void AProceduralTerrain::initMapper() {
	static bool pulledFromFile = false;
	if (pulledFromFile) return;
	pulledFromFile = true;
	int tryrot = 2;
	Pattern pm[] = {
		{
			1,1,2,4,new int[] {
				simple_wall,notstructure,
			},{{"Stan Walltop 1x1x1",0,0,0,0}}
		},{
			2,1,2,2,new int[] {
				simple_wall,wallair,
				simple_wall,notstructure
			},{{"Stan Walltop Edge 1x1x1",0,0,0,0}}
		},{
			2,1,2,2,new int[] {
				simple_wall,notstructure,
				simple_wall,simple_wall
			},{{"Stan Walltop Edge 1x1x1",0,0,0,0}}
		},{
			2,2,2,1,new int[] {
				simple_wall,wallair,simple_wall,wallair,
				simple_wall,wallair,simple_wall,notstructure
			},{{"Stan Walltop Corner 1x1x1",0,1,1,0}}
		},{
			2,2,2,1,new int[] {
				simple_wall,wallair,simple_wall,notstructure,
				simple_wall,wallair,simple_wall,simple_wall
			},{{"Stan Walltop Corner 1x1x1",0,1,1,0}}
		},{
			2,2,2,1,new int[] {
				simple_wall,wallair,simple_wall,simple_wall,
				simple_wall,notstructure,simple_wall,simple_wall
			},{{"Stan Walltop Corner 1x1x1",0,1,1,0}}
		},{
			2,2,2,1,new int[] {
				simple_wall,notstructure,simple_wall,simple_wall,
				simple_wall,simple_wall,simple_wall,simple_wall
			},{{"Stan Walltop Corner 1x1x1",0,1,1,0}}
		},





		{
			3,2,2,4,new int[] {
				any,notstructure,any,notstructure,
				any,notstructure,simple_wall,simple_wall,
				any,notstructure,any,notstructure
			},{{"Stan Upperwall 2outward 1x1x1",tryrot,2,2,1}}
		},{
			3,2,2,4,new int[] {
				any,notstructure,any,simple_wall,
				any,notstructure,simple_wall,simple_wall,
				any,notstructure,any,notstructure
			},{{"Stan Upperwall 1outward 1x1x1",tryrot,2,2,1}}
		},{
			3,2,2,4,new int[] {
				any,simple_wall,any,simple_wall,
				any,notstructure,simple_wall,simple_wall,
				any,notstructure,any,notstructure
			},{{"Stan Upperwall 1inward 1outward Inv 1x1x1",tryrot,2,2,1}}


		},{
			3,2,2,4,new int[] {
				any,notstructure,any,notstructure,
				any,notstructure,simple_wall,simple_wall,
				any,notstructure,any,simple_wall
			},{{"Stan Upperwall 1outward Inv 1x1x1",tryrot,2,2,1}}
		},{
			3,2,2,4,new int[] {
				any,notstructure,any,simple_wall,
				any,notstructure,simple_wall,simple_wall,
				any,notstructure,any,simple_wall
			},{{"Stan Upperwall 1x1x1",tryrot,2,2,1}}
		},{
			3,2,2,4,new int[] {
				any,simple_wall,any,simple_wall,
				any,notstructure,simple_wall,simple_wall,
				any,notstructure,any,simple_wall
			},{{"Stan Upperwall 1inward 1x1x1",tryrot,2,2,1}}
		},{


			3,2,2,4,new int[] {
				any,notstructure,any,notstructure,
				any,notstructure,simple_wall,simple_wall,
				any,simple_wall,any,simple_wall
			},{{"Stan Upperwall 1inward 1outward 1x1x1",tryrot,2,2,1}}
		},{
			3,2,2,4,new int[] {
				any,notstructure,any,simple_wall,
				any,notstructure,simple_wall,simple_wall,
				any,simple_wall,any,simple_wall
			},{{"Stan Upperwall 1inward Inv 1x1x1",tryrot,2,2,1}}
		},{
			3,2,2,4,new int[] {
				any,simple_wall,any,simple_wall,
				any,notstructure,simple_wall,simple_wall,
				any,simple_wall,any,simple_wall
			},{{"Stan Upperwall 2inward 1x1x1",tryrot,2,2,1}}
		},








		{
			3,2,2,4,new int[] {
				any,notstructure,any,notstructure,
				any,notstructure,simple_floor,simple_wall,
				any,notstructure,any,notstructure
			},{{"Stan Wall 2outward 1x1x1",tryrot,2,2,1}}
		}, {
			3,2,2,4,new int[] {
				any,notstructure,any,simple_wall,
				any,notstructure,simple_floor,simple_wall,
				any,notstructure,any,notstructure
			},{{"Stan Wall 1outward 1x1x1",tryrot,2,2,1}}
		}, {
			3,2,2,4,new int[] {
				any,simple_wall,any,simple_wall,
				any,notstructure,simple_floor,simple_wall,
				any,notstructure,any,notstructure
			},{{"Stan Wall 1inward 1outward Inv 1x1x1",tryrot,2,2,1}}


		}, {
			3,2,2,4,new int[] {
				any,notstructure,any,notstructure,
				any,notstructure,simple_floor,simple_wall,
				any,notstructure,any,simple_wall
			},{{"Stan Wall 1outward Inv 1x1x1",tryrot,2,2,1}}
		}, {
			3,2,2,4,new int[] {
				any,notstructure,any,simple_wall,
				any,notstructure,simple_floor,simple_wall,
				any,notstructure,any,simple_wall
			},{{"Stan Wall 1x1x1",tryrot,2,2,1}}
		}, {
			3,2,2,4,new int[] {
				any,simple_wall,any,simple_wall,
				any,notstructure,simple_floor,simple_wall,
				any,notstructure,any,simple_wall
			},{{"Stan Wall 1inward 1x1x1",tryrot,2,2,1}}
		}, {


			3,2,2,4,new int[] {
				any,notstructure,any,notstructure,
				any,notstructure,simple_floor,simple_wall,
				any,simple_wall,any,simple_wall
			},{{"Stan Wall 1inward 1outward 1x1x1",tryrot,2,2,1}}
		}, {
			3,2,2,4,new int[] {
				any,notstructure,any,simple_wall,
				any,notstructure,simple_floor,simple_wall,
				any,simple_wall,any,simple_wall
			},{{"Stan Wall 1inward Inv 1x1x1",tryrot,2,2,1}}
		}, {
			3,2,2,4,new int[] {
				any,simple_wall,any,simple_wall,
				any,notstructure,simple_floor,simple_wall,
				any,simple_wall,any,simple_wall
			},{{"Stan Wall 2inward 1x1x1",tryrot,2,2,1}}
		},






		{
			1,1,2,4,new int[] {
				simple_floor,notstructure
			},{{"Stan Floor 1x1x1",0,0,0,0}}
		},{
			2,2,1,4,new int[] {
				some_air,simple_floor,
				simple_floor,simple_floor
			},{{"Stan Invcorner 1x1x1",0,1,1,0}}
		},{
			2,2,1,4,new int[] {
				some_air,some_air,
				some_air,simple_floor
			},{{"Stan Corner 1x1x1",0,1,1,0}}
		},{
			3,2,1,4,new int[] {
				any,simple_floor,
				some_air,simple_floor,
				any,simple_floor
			},{{"Stan Edge 1x1x1",0,1,1,0},{"Stan Edge widget1 1x1x1",0,1,1,0},{"Stan Edge widget2 1x1x1",0,1,1,0}}
		}

	};


	PatternMapper.Append(pm, UE_ARRAY_COUNT(pm));
	PatternFeatures.Reserve(PatternMapper.Num());
	for (int feat = 0;feat < PatternMapper.Num();feat++) {
		PatternFeature pf = PatternMapper[feat].extractFeature();
		UE_LOG(LogTemp, Warning, TEXT("feature %d %d %d %d %d %d"), pf.x, pf.y, pf.x, pf.dim, pf.sm, pf.lg);
		PatternFeatures.Add(pf);
	}

}







bool Pattern::symmetrical_180() {
	for (int z = 0;z < zs;z++) {
		for (int x = 0;x < (xs + 1) / 2;x++) {
			for (int y = 0;y < (ys + 1) / 2;y++) {
				if (getAt(x, y, z) != getAt(xs - 1 - x, ys - 1 - y, z)) return false;
			}
		}
	}
	return true;
}
bool Pattern::symmetrical_90() {
	if (xs != ys) return false;
	for (int z = 0;z < zs;z++) {
		for (int x = 0;x < (xs + 1) / 2;x++) {
			for (int y = 0;y < x;y++) {
				if (getAt(x, y, z) != getAt(y, xs - 1 - x, z)) return false;
			}
		}
	}
	return true;
}



int Pattern::getAt(int x, int y, int z) {
	return flagfield[x * ys * zs + y * zs + z];
}


PatternFeature Pattern::extractFeature() {
	PatternFeature out;
	out.sym = std::min(sym,symmetrical_180() ? (symmetrical_90() ? 1 : 2) : 4);
	out.xs = xs;
	out.ys = ys;
	for (int x = 0;x < xs;x++) {
		for (int y = 0;y < ys;y++) {
			for (int z = 0;z < zs;z++) {
				if (x && !(getAt(x - 1, y, z) & getAt(x, y, z))) {
					out.x = x - 1;out.y = y;out.z = z;out.sm = getAt(x - 1, y, z);out.lg = getAt(x, y, z);out.dim = 0;
					return out;
				}
				if (y && !(getAt(x, y - 1, z) & getAt(x, y, z))) {
					out.x = x;out.y = y - 1;out.z = z;out.sm = getAt(x, y - 1, z);out.lg = getAt(x, y, z);out.dim = 1;
					return out;
				}
				if (z && !(getAt(x, y, z - 1) & getAt(x, y, z))) {
					out.x = x;out.y = y;out.z = z - 1;out.sm = getAt(x, y, z - 1);out.lg = getAt(x, y, z);out.dim = 2;
					return out;
				}
			}
		}
	}
	throw 22;
}





//dudnmatter
//air
//simple_floor
//displayable wall
//undisplayable wall





