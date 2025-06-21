#pragma once

#define SHAPE_COUNT 7
#define ROTATION_COUNT 4

typedef enum {
	SHAPE_I,
	SHAPE_J,
	SHAPE_L,
	SHAPE_O,
	SHAPE_S,
	SHAPE_T,
	SHAPE_Z,
} ShapeType;
typedef enum {
	ROTATION_0,
	ROTATION_1,
	ROTATION_2,
	ROTATION_3,
} RotationState;

typedef char mat2x2[2][2];
typedef char mat3x3[3][3];
typedef char mat4x4[4][4];

extern mat2x2 blockModel2x2_[1][4];
extern mat3x3 blockModel3x3_[5][4];
extern mat4x4 blockModel4x4_[1][4];

typedef union {
	const mat2x2* mat2;
	const mat3x3* mat3;
	const mat4x4* mat4;
} ModelPointer;

extern ModelPointer modelPointers[SHAPE_COUNT][ROTATION_COUNT];

void InitializeModelPointers();

//////////////////////////////
/// Super Rotation System
/// See: https://github.com/HongLabInc/HongLabTetris/issues/62
//////////////////////////////

constexpr int NumOfKickTest = 5;

struct KickOffset
{
	int x, y;
};

struct SRSKickData {
	KickOffset kicks[NumOfKickTest];
};

enum RotationTransition {
	TRANS_0_TO_1,
	TRANS_1_TO_0,
	TRANS_1_TO_2,
	TRANS_2_TO_1,
	TRANS_2_TO_3,
	TRANS_3_TO_2,
	TRANS_3_TO_0,
	TRANS_0_TO_3,
	TRANS_COUNT
};

extern const SRSKickData SRS_KICK_DATA_JLSTZ[TRANS_COUNT];
extern const SRSKickData SRS_KICK_DATA_I[TRANS_COUNT];
