// ChaoCam.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SA2ModLoader.h"


struct AL_NormalCameraExecutor_Data1
{
	EntityData1 EntityData1;
	int type;
	NJS_POINT3 velo;
	NJS_VECTOR AimPos;
	Rotation AngSpd;
	NJS_VECTOR target;
	NJS_POINT3 AimTarget;
	float dist;
	int ViewAngle;
	float TargetNormalOffsetY;
	float TargetTreeOffsetY;
	float PosOffsetY;
	float PosItpRatio;
	int StopFlag;
};

const int sub_53E020Ptr = 0x53E020;
void sub_53E020(int a1)
{
	__asm
	{
		mov esi, a1
		call sub_53E020Ptr
	}
}
const int AddToCollisionListPtr = 0x0047E750;
void AddToCollisionList(ObjectMaster* a1)
{
	__asm
	{
		mov esi, a1
		call AddToCollisionListPtr
	}
}

const int GetChaoObjectPtr = 0x530410;
ObjectMaster* GetChaoObject(int a1, int a2)
{
	ObjectMaster* retu;
	__asm
	{
		mov eax, a1
		mov edi, a2
		call GetChaoObjectPtr
		mov retu, eax
	}
	return retu;
}

void  njCalcVector(NJS_VECTOR* a1, NJS_VECTOR* a2, NJS_MATRIX_PTR a3)
{
	Float v3; // ST00_4
	Float v4; // ST04_4

	v3 = a3[M10] * a1->x + a3[M11] * a1->y + a3[M12] * a1->z;
	v4 = a3[M20] * a1->x + a3[M21] * a1->y + a3[M22] * a1->z;
	a2->x = a3[M01] * a1->y + *a3 * a1->x + a3[M02] * a1->z;
	a2->y = v3;
	a2->z = v4;
}

const int njTranslateVPtr = 0x00428A30;
void  njTranslateV(NJS_VECTOR* a1)
{
	__asm
	{
		mov eax, a1
		call njTranslateVPtr
	}
}

DataPointer(char, ControlEnabled, 0x0174AFFE);
FastcallFunctionPointer(double, __fastcall njCos, (signed int a1), 0x042AC30);
FastcallFunctionPointer(double, __fastcall njSin, (signed int a1), 0x0042AAB0);
FunctionPointer(signed int, njPushMatrixEx, (), 0x00429710);
FunctionPointer(signed int, njPopMatrixEx, (), 0x00429000);
DataPointer(float*, MatrixBase, 0x01A557FC);
DataPointer(ObjectMaster*, OdekakeMachinePtr, 0x01AED318);
DataPointer(int, SomeOdekakeFlag, 0x01AED31C);
DataPointer(int, dword_1AED310, 0x01AED310);
DataPointer(NJS_MATRIX, flt_25F02A0, 0x25F02A0);
DataArray(int, ChaoObjectCount, 0x1DC0F80, 10);

NJS_TEXNAME AL_CAMERA_TEXNAME[2];
NJS_TEXLIST AL_CAMERA_TEXLIST = { arrayptrandlength(AL_CAMERA_TEXNAME) };

DataPointer(ObjectMaster*, PointerToNormalCamera, 0x01AED2E8);

int IsChaoCameraActive;
int ToggleDisplay = 0;
int PauseEnabled = 0;

//1.panning(just tried and it glitches out so im not sure if i can do that one but ill look into it)
//2.shadows
//3.visuals

void __cdecl AL_ChaoCameraExecutor(ObjectMaster* a1)
{
	AL_NormalCameraExecutor_Data1* camData1; // edi
	NJS_VECTOR* playerPos1; // ebx
	signed int v3; // ecx
	signed int v4; // ecx
	NJS_MATRIX_PTR v5; // esi
	float* v6; // ebx
	float* v7; // eax
	float* v8; // ebx
	int v9; // edx
	ObjectMaster* v10; // ecx
	NJS_VECTOR* v11; // eax
	float v12; // st7
	float v13; // st7
	float v14; // st6
	ObjectMaster* heldChao; // eax
	NJS_VECTOR* lookAtPosMaybe; // eax
	EntityData1* v17; // esi
	signed int v18; // ebx
	Data1Ptr v19; // ebx
	signed int v20; // esi
	float v21; // st7
	ObjectMaster* v22; // esi
	double v23; // st7
	UnknownData2* v24; // ebx
	float v25; // ST20_4
	float v26; // st7
	float v27; // st7
	float v28; // ST1C_4
	float v29; // st7
	NJS_VECTOR* v30; // [esp+10h] [ebp-28h]
	__int16 v31; // [esp+14h] [ebp-24h]
	NJS_VECTOR resultPosition; // [esp+18h] [ebp-20h]
	NJS_VECTOR v33; // [esp+2Ch] [ebp-Ch]
	camData1 = (AL_NormalCameraExecutor_Data1*)a1->Data1.Undefined;

	if ((MenuButtons_Pressed[0] & Buttons_L) && (MenuButtons_Pressed[0] & Buttons_R))
		IsChaoCameraActive = !IsChaoCameraActive;

	if (MenuButtons_Pressed[0] & Buttons_X) ToggleDisplay = !ToggleDisplay;
	if (MenuButtons_Pressed[0] & Buttons_Y) PauseEnabled = !PauseEnabled;

	if (!IsChaoCameraActive || ChaoObjectCount[0] <= 0)
	{
		ControlEnabled = 1;
		*(int*)0x01DBE634 = 1; //chaoenabled
		playerPos1 = &MainCharObj1[0]->Position;
		v30 = &MainCharObj1[0]->Position;
		camData1->dist = 25.0f;
	}
	else
	{
		ControlEnabled = 0;
		*(int*)0x01DBE634 = PauseEnabled; //chaoenabled
		GetChaoObject(0, a1->Data1.Entity->Index)->Data1.Chao->field_38 = 1; //NestFlag
		playerPos1 = &GetChaoObject(0, a1->Data1.Entity->Index)->Data1.Entity->Position;
		v30 = &GetChaoObject(0, a1->Data1.Entity->Index)->Data1.Entity->Position;
		if (MenuButtons_Held[0] & Buttons_Down)
		{
			if (camData1->dist > 7.5f) camData1->dist -= 0.25f;
		}
		if (MenuButtons_Held[0] & Buttons_Up)
		{
			if (camData1->dist < 50.0f) camData1->dist += 0.25f;
		}
	}

	v31 = MainCharObj1[0]->Action;
	if (!camData1->EntityData1.Action)        // InitCam
	{
		v3 = 0x4000 - MainCharObj1[0]->Rotation.y;
		resultPosition.x = playerPos1->x;
		camData1->EntityData1.Position.x = resultPosition.x - njSin(v3) * 10.0;
		camData1->EntityData1.Position.y = camData1->PosOffsetY + playerPos1->y;
		v4 = 0x4000 - MainCharObj1[0]->Rotation.y;
		resultPosition.x = playerPos1->z;
		camData1->EntityData1.Position.z = resultPosition.x - njCos(v4) * 10.0;
		camData1->AimPos.x = camData1->EntityData1.Position.x;
		camData1->AimPos.y = camData1->EntityData1.Position.y;
		camData1->AimPos.z = camData1->EntityData1.Position.z;
		++camData1->EntityData1.Action;
	}
	resultPosition.x = camData1->AimPos.x - playerPos1->x;
	resultPosition.y = 0.0;
	resultPosition.z = camData1->AimPos.z - playerPos1->z;
	njUnitVector(&resultPosition);
	v5 = MatrixBase;

	resultPosition.x = resultPosition.x * camData1->dist;
	resultPosition.z = camData1->dist * resultPosition.z;

	if (MenuButtons_Pressed[0] & Buttons_A)
	{
		a1->Data1.Entity->Index++;
		if (a1->Data1.Entity->Index >= ChaoObjectCount[0])
			a1->Data1.Entity->Index = 0;
	}
	if (MenuButtons_Pressed[0] & Buttons_B)
	{
		a1->Data1.Entity->Index--;
		if (a1->Data1.Entity->Index < 0)
			a1->Data1.Entity->Index = ChaoObjectCount[0] - 1;
	}

	if (MenuButtons_Held[0] & Buttons_L)
	{
		njPushMatrix(flt_25F02A0);
		njRotateY(MatrixBase, -512);
		njCalcVector(&resultPosition, &resultPosition, MatrixBase);
		njPopMatrixEx();
	}
	if (MenuButtons_Held[0] & Buttons_R)
	{
		njPushMatrix(flt_25F02A0);
		njRotateY(MatrixBase, 512);
		njCalcVector(&resultPosition, &resultPosition, MatrixBase);
		njPopMatrixEx();
	}


	v9 = dword_1AED310;
	v10 = OdekakeMachinePtr;
	if (v31 == 23 || v31 == 34)
	{
		heldChao = MainCharObj2[0]->HoldTarget;
		if (heldChao)
		{
			lookAtPosMaybe = &heldChao->Data1.Entity->Position;
		}
		else
		{
			lookAtPosMaybe = playerPos1;
		}
		v13 = 0.04f;
		camData1->target.x = (lookAtPosMaybe->x - camData1->target.x) * 0.04f
			+ camData1->target.x;
		camData1->target.y = (lookAtPosMaybe->y + camData1->TargetTreeOffsetY - camData1->target.y)
			* 0.04f
			+ camData1->target.y;
		v14 = lookAtPosMaybe->z;
		goto LABEL_25;
	}
	if (!SomeOdekakeFlag || !OdekakeMachinePtr)
	{
		v13 = 0.4f;
		camData1->target.x = (playerPos1->x - camData1->target.x) * 0.4f
			+ camData1->target.x;
		camData1->target.y = (camData1->TargetNormalOffsetY + playerPos1->y - camData1->target.y)
			* 0.2f
			+ camData1->target.y;
		v14 = playerPos1->z;
		goto LABEL_25;
	}
	v11 = &OdekakeMachinePtr->Data1.Entity->Position;
	v33.x = v11->x;
	v33.y = v11->y + 10.0f;
	v33.z = v11->z;
	if (!dword_1AED310)
	{
		v13 = 0.04f;
		camData1->target.x = (v33.x - camData1->target.x) * v13
			+ camData1->target.x;
		camData1->target.y = (v33.y - camData1->target.y) * v13
			+ camData1->target.y;
		v14 = v33.z;
	LABEL_25:
		camData1->target.z = v13 * (v14 - camData1->target.z) + camData1->target.z;
		goto LABEL_26;
	}
	camData1->target = v33;
LABEL_26:
	if (SomeOdekakeFlag && v10)
	{
		if (v9)
		{
			v17 = (EntityData1*)v10->Data1.Entity; //oof
			v18 = v17->Rotation.y + 22528;
			camData1->AimPos.x = njSin(v18) * 45.0 + v17->Position.x;
			camData1->AimPos.y = v17->Position.y + 16.0;
			camData1->AimPos.z = njCos(v18) * 45.0 + v17->Position.z;
			camData1->EntityData1.Position.x = camData1->AimPos.x;
			camData1->EntityData1.Position.y = camData1->AimPos.y;
			camData1->EntityData1.Position.z = camData1->AimPos.z;
			goto LABEL_33;
		}
		v19.Undefined = (EntityData1*)v10->Data1.Entity;
		v20 = v19.Entity->Rotation.y + 22528;
		camData1->AimPos.x = njSin(v20) * 37.0 + v19.Entity->Position.x;
		camData1->AimPos.y = v19.Entity->Position.y + 12.0;
		camData1->AimPos.z = njCos(v20) * 37.0 + v19.Entity->Position.z;
	}
	else
	{
		camData1->AimPos.x = playerPos1->x + resultPosition.x;
		camData1->AimPos.y = camData1->PosOffsetY + playerPos1->y;
		camData1->AimPos.z = playerPos1->z + resultPosition.z;
	}
LABEL_33:
	v22 = a1;
	v23 = a1->EntityData2->field_DC + 2.5;
	if (camData1->AimPos.y < v23)
	{
		camData1->AimPos.y = v23;
	}
	v24 = a1->EntityData2;
	if (v24->field_40 & 0x800)
	{
		camData1->PosItpRatio = 0.02;
	}
	else if (SomeOdekakeFlag)
	{
		camData1->PosItpRatio = 0.05;
	}
	else
	{
		v25 = camData1->PosItpRatio + 0.01f;
		camData1->PosItpRatio = v25;
		if (0.13 < (double)v25)
		{
			camData1->PosItpRatio = 0.13;
		}
	}
	resultPosition.x = (camData1->AimPos.x - camData1->EntityData1.Position.x) * camData1->PosItpRatio;
	if (v24->field_40 & 0x400 && (v26 = camData1->AimPos.y - camData1->EntityData1.Position.y, v26 < 30.0))
	{
		v27 = v26 * 0.1f * 0.1f;
	}
	else
	{
		v27 = (camData1->AimPos.y - camData1->EntityData1.Position.y) * 0.1f;
	}
	resultPosition.y = v27;
	resultPosition.z = (camData1->AimPos.z - camData1->EntityData1.Position.z) * camData1->PosItpRatio;
	v28 = njScalor(&resultPosition);
	v29 = v28;
	if (v28 > 0.0 && v29 > 3.0)
	{
		njUnitVector(&resultPosition);
		v22 = a1;
		resultPosition.x = resultPosition.x * 3.0;
		resultPosition.y = resultPosition.y * 3.0;
		resultPosition.z = 3.0 * resultPosition.z;
		v29 = (float)3.0;
	}
	camData1->EntityData1.Position.x = camData1->EntityData1.Position.x + resultPosition.x;
	camData1->EntityData1.Position.y = resultPosition.y + camData1->EntityData1.Position.y;
	camData1->EntityData1.Position.z = camData1->EntityData1.Position.z + resultPosition.z;
	if (camData1->StopFlag)
	{
		if (v29 > 0.01f || v24->field_40 & 0x800)
		{
			camData1->StopFlag = 0;
			AddToCollisionList(v22);
			return;
		}
	}
	else
	{
		if (!(v24->field_40 & 0x800) && v29 < 0.005f)
		{
			camData1->StopFlag = 1;
			sub_53E020((int)v22);
			AddToCollisionList(a1);
			return;
		}
		sub_53E020((int)v22);
		v22 = a1;
	}
	AddToCollisionList(a1);
}

NJS_TEXANIM texanim[] = {
	{256,256,128,128,0,0,256,256,1,0},
	{256,256,128,128,0,0,256,256,0,0}
};
NJS_SPRITE chaoCamEdge_sprite = { {320,220,0}, 1,1, 0, &AL_CAMERA_TEXLIST, texanim };
NJS_SPRITE chaoCamCenter_sprite = { {320,220,0}, 0.5f,0.5f, 0, &AL_CAMERA_TEXLIST, texanim };

void AL_ChaoCameraDisplayer(ObjectMaster* a1)
{
	if (IsChaoCameraActive && ToggleDisplay)
	{
		*(char*)0x25EFFCC = 0;
		//njDrawSprite2D(&chaoCamEdge_sprite, 1, 0, 0x60);
		njDrawSprite2D(&chaoCamCenter_sprite, 0, 1, 0x60);
		*(char*)0x25EFFCC = 1;
	}
}

ObjectMaster* AL_NormalCameraExecutor_LoadReplace()
{
	ObjectMaster* v1; // esi
	MemManFuncs* v2; // eax
	int v3; // eax
	AL_NormalCameraExecutor_Data1* v4; // edi
	double v5; // st7
	UnknownData2* v6; // eax
	UnknownData2* v7; // edx

	if (PointerToNormalCamera)
	{
		return 0;
	}
	chaoCamEdge_sprite.sx = HorizontalResolution / 576; //3.33
	chaoCamEdge_sprite.sy = VerticalResolution / 324;
	LoadTextureList((char*)"al_camera", &AL_CAMERA_TEXLIST);
	v1 = AllocateObjectMaster(AL_ChaoCameraExecutor, 2, "AL_ChaoCameraExecutor");
	v2 = MemoryManager;
	v1->DeleteSub = (ObjectFuncPtr)0x0057D2D0;
	v3 = (int)v2->Allocate(144, (char*)"..\\..\\src\\Chao\\al_camera.c", 410);
	v4 = (AL_NormalCameraExecutor_Data1*)(v3 + 4);
	*(int*)v3 = 305419896;
	memset((void*)(v3 + 4), 0, 0x8Cu);
	v1->field_1C = AL_ChaoCameraDisplayer;
	v1->Data1.Undefined = &v4->EntityData1;
	InitCollision(v1, (CollisionData*)0x8A57B0, 2, 5u);
	if (MainCharObj2[0] && MainCharObj2[0]->CharID == Characters_Eggman)
	{
		v4->TargetNormalOffsetY = 10.0;
		v4->TargetTreeOffsetY = 24.0;
		v4->PosOffsetY = 11.5;
	}
	else
	{
		v4->TargetNormalOffsetY = 4.5;
		v4->TargetTreeOffsetY = 22.0;
		v4->PosOffsetY = 5.0;
	}
	v4->PosItpRatio = 0.13f;
	v6 = AllocateUnknownData2(v1);
	v7 = v1->EntityData2;
	v6->field_B8 = 5.0;
	PointerToNormalCamera = v1;
	v6->field_AC = 1.0;
	v6->field_BC = -3.0;
	v7->field_48 = 0.0;
	v4->type = 0;
	return v1;
}

extern "C"
{
	__declspec(dllexport) void Init(const char* path, const HelperFunctions& helperFunctions)
	{
		//WriteJump((void*)AL_NormalCameraExecutor_Main, AL_NormalCameraExecutor_MainReplace);
		WriteJump((void*)0x0057D2E0, AL_NormalCameraExecutor_LoadReplace);
	}
	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}

