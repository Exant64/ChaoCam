// ChaoCam.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SA2ModLoader.h"


struct AL_NormalCameraExecutor_Data1
{
	EntityData1 EntityData1;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	NJS_VECTOR field_40;
	char gap4C[12];
	NJS_VECTOR lookAtPosMaybe;
	char gap64[20];
	float float78;
	float float7C;
	float float80;
	float float84;
	int dword88;
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
const int NormalizeVectorMaybePtr = 0x0077F3A0;
void NormalizeVectorMaybe(NJS_VECTOR *a1)
{
	__asm
	{
		mov esi, a1
		call NormalizeVectorMaybePtr
		fstp st
	}
}

const int ParamFukidasiExecutor_LoadPtr = 0x005659E0;
void  ParamFukidasiExecutor_Load(ObjectMaster *a1)
{
	__asm
	{
		_asm
		{
			mov eax, a1
			call ParamFukidasiExecutor_LoadPtr
		}
	}
}

const int GetChaoObjectPtr = 0x530410;
ObjectMaster * GetChaoObject(int a1, int a2)
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

void  njCalcVector(NJS_VECTOR *a1, NJS_VECTOR *a2, NJS_MATRIX_PTR a3)
{
	Float v3; // ST00_4
	Float v4; // ST04_4

	v3 = a3[M10] * a1->x + a3[M11] * a1->y + a3[M12] * a1->z;
	v4 = a3[M20] * a1->x + a3[M21] * a1->y + a3[M22] * a1->z;
	a2->x = a3[M01] * a1->y + *a3 * a1->x + a3[M02] * a1->z;
	a2->y = v3;
	a2->z = v4;
}

const int njRotateYPtr = 0x004275B0;
void  njRotateY(NJS_MATRIX_PTR matrixPtr, signed int a2)
{
	__asm
	{
		push a2
		mov ebx, matrixPtr
		call njRotateYPtr
		add esp, 4
	}

}

const int njTranslatePtr = 0x00427470;
void  njTranslate(float *a1, float a2, float a3, float a4) 
{
	__asm
	{
		push a4
		push a3
		push a2
		mov eax, a1
		call njTranslatePtr
		add esp, 12
	}
}

const int njTranslateVPtr = 0x00428A30;
void  njTranslateV(NJS_VECTOR *a1 )
{
	__asm
	{
		mov eax, a1
		call njTranslateVPtr
	}
}

const int sub_487060Ptr = 0x487060;
void  SetFogShaderMaybe(int a1 )
{
	__asm
	{
		mov eax, a1
		call sub_487060Ptr
	}
}
DataPointer(char, ControlEnabled, 0x0174AFFE);
FastcallFunctionPointer(double, __fastcall njCos, (signed int a1), 0x042AC30);
FastcallFunctionPointer(signed int, __fastcall njPushMatrix, (NJS_MATRIX_PTR a1), 0x0077FE10);
FastcallFunctionPointer(double, __fastcall njSin, (signed int a1), 0x0042AAB0);
FunctionPointer(signed int, njPushMatrixEx, (), 0x00429710);
FunctionPointer(signed int, njPopMatrixEx, (), 0x00429000);
DataPointer(float*, MatrixBase, 0x01A557FC);
DataPointer(ObjectMaster*, OdekakeMachinePtr, 0x01AED318);
DataPointer(int, SomeOdekakeFlag, 0x01AED31C);
DataPointer(int, dword_1AED310, 0x01AED310);
DataPointer(int, dword_267053C, 0x267053C);
DataPointer(int, dword_2670588, 0x2670588);
DataPointer(NJS_MATRIX, flt_25F02A0, 0x25F02A0);
DataArray(int, ChaoObjectCount, 0x1DC0F80, 10);

FunctionPointer(void, sub_46DED0, (), 0x46DED0);
FunctionPointer(void, sub_46DF30, (), 0x46DF30);
FunctionPointer(void, sub_42D340, (), 0x42D340);
ObjectMaster* handObjectPtr;
bool backedUpPlayerSub = false;
DataArray(char, byte_1DE4664, 0x1DE4664, 10);
DataPointer(char, byte_1DE4400, 0x1DE4400);
DataPointer(Rotation, CameraRotation, 0x01DCFF18);

void Hand_Display(ObjectMaster *a1)
{
	SetFogShaderMaybe(byte_1DE4664[0]);
	njPushMatrixEx();

	njTranslate(MatrixBase, a1->Data1.Entity->Position.x, a1->Data1.Entity->Position.y, a1->Data1.Entity->Position.z);
	Has_texlist_batadvPlayerChara_in_it[8] = 0x1366AFC; //njSetTexture but weird because SA2 is a fuck
	sub_42D340(); //god knows what, SA2 requires this for some reason
	ProcessChunkModelsWithCallback((NJS_OBJECT*)0x125BF74, ProcessChunkModel); //njCnkDrawObject

	njPopMatrixEx();
	SetFogShaderMaybe(byte_1DE4400);
}

void Hand_Main(ObjectMaster *a1)
{
	float v4 = njSin(CameraRotation.y);
	float v5 = CameraRotation.y;
	NJS_VECTOR a2;
	a2.y = 0.64999998;
	a2.x = v4 * 0.079999998;
	a2.z = njCos(v5) * 0.079999998;
	if (Controllers[0].on & Buttons_Up)
		a1->Data1.Entity->Position.z -= a2.x * 3;
	if (Controllers[0].on & Buttons_Down)
		a1->Data1.Entity->Position.z += a2.x * 3;
	if (Controllers[0].on & Buttons_Left)
		a1->Data1.Entity->Position.x -= a2.z * 3;
	if(Controllers[0].on & Buttons_Right)
		a1->Data1.Entity->Position.x += a2.z * 3;

}
void Hand_Delete(ObjectMaster *a1)
{
	handObjectPtr = 0;
	DeleteObject_(a1);
}
void Hand_Load()
{
	ObjectMaster* loaded = LoadObject(5, "AL_TCGHand", Hand_Main, LoadObj_Data1);
	loaded->DisplaySub = Hand_Display;
	handObjectPtr = loaded;
}

FunctionPointer(void, sub_57D4C0, (), 0x57D4C0);

DataPointer(ObjectMaster* , PointerToChaoCamera, 0x01AED2E8);
void __cdecl AL_NormalCameraExecutor_MainReplace(ObjectMaster *a1)
{
	AL_NormalCameraExecutor_Data1 *camData1; // edi
	NJS_VECTOR *playerPos1; // ebx
	signed int v3; // ecx
	signed int v4; // ecx
	NJS_MATRIX_PTR v5; // esi
	float *v6; // ebx
	float *v7; // eax
	float *v8; // ebx
	int v9; // edx
	ObjectMaster *v10; // ecx
	NJS_VECTOR *v11; // eax
	double v12; // st7
	double v13; // st7
	double v14; // st6
	ObjectMaster *heldChao; // eax
	NJS_VECTOR *lookAtPosMaybe; // eax
	EntityData1* v17; // esi
	signed int v18; // ebx
	Data1Ptr v19; // ebx
	signed int v20; // esi
	double v21; // st7
	ObjectMaster *v22; // esi
	double v23; // st7
	UnknownData2 *v24; // ebx
	float v25; // ST20_4
	double v26; // st7
	double v27; // st7
	float v28; // ST1C_4
	double v29; // st7
	NJS_VECTOR *v30; // [esp+10h] [ebp-28h]
	__int16 v31; // [esp+14h] [ebp-24h]
	NJS_VECTOR resultPosition; // [esp+18h] [ebp-20h]
	NJS_VECTOR v33; // [esp+2Ch] [ebp-Ch]
	camData1 = (AL_NormalCameraExecutor_Data1 *)a1->Data1.Undefined;

	if (!GetChaoObject(0, a1->Data1.Entity->Index))
	{
		ControlEnabled = 1;
		playerPos1 = &MainCharObj1[0]->Position;
		v30 = &MainCharObj1[0]->Position;
	}
	else
	{
		ControlEnabled = 0;
		playerPos1 = &GetChaoObject(0, a1->Data1.Entity->Index)->Data1.Entity->Position;
		v30 = &GetChaoObject(0, a1->Data1.Entity->Index)->Data1.Entity->Position;
		if (MenuButtons_Pressed[0] & Buttons_A)
			ParamFukidasiExecutor_Load(GetChaoObject(0, a1->Data1.Entity->Index));
	}
	/*
	if(handObjectPtr)
	{
		playerPos1 = &handObjectPtr->Data1.Entity->Position;
		v30 = &handObjectPtr->Data1.Entity->Position;
	}
	*/
	v31 = MainCharObj1[0]->Action;
	if (!camData1->EntityData1.Action)        // InitCam
	{
		v3 = 0x4000 - MainCharObj1[0]->Rotation.y;
		resultPosition.x = playerPos1->x;
		camData1->EntityData1.Position.x = resultPosition.x - njSin(v3) * 10.0;
		camData1->EntityData1.Position.y = camData1->float80 + playerPos1->y;
		v4 = 0x4000 - MainCharObj1[0]->Rotation.y;
		resultPosition.x = playerPos1->z;
		camData1->EntityData1.Position.z = resultPosition.x - njCos(v4) * 10.0;
		camData1->field_40.x = camData1->EntityData1.Position.x;
		camData1->field_40.y = camData1->EntityData1.Position.y;
		camData1->field_40.z = camData1->EntityData1.Position.z;
		++camData1->EntityData1.Action;
	}
	resultPosition.x = camData1->field_40.x - playerPos1->x;
	resultPosition.y = 0.0;
	resultPosition.z = camData1->field_40.z - playerPos1->z;
	NormalizeVectorMaybe(&resultPosition);
	v5 = MatrixBase;
	resultPosition.x = resultPosition.x * 25.0;
	resultPosition.z = 25.0 * resultPosition.z;
	
	if (MenuButtons_Pressed[0] & Buttons_X)
	{

		a1->Data1.Entity->Index++;
		if (a1->Data1.Entity->Index > ChaoObjectCount[0] + 2)
			a1->Data1.Entity->Index = ChaoObjectCount[0] + 2;
		/*
		if (ChaoObjectCount[0] > 0) 
		{
			if (a1->Data1.Entity->Index >= ChaoObjectCount[0] + 2)
				if (backedUpPlayerSub) {
					backedUpPlayerSub = false;
					sub_46DF30();
				}
			if (a1->Data1.Entity->Index < ChaoObjectCount[0] + 2)
			{
				if (!backedUpPlayerSub)
				{
					backedUpPlayerSub = true;
					sub_46DED0();
				}
			}
		}
		*/

	}
	if (MenuButtons_Pressed[0] & Buttons_Y)
	{

		a1->Data1.Entity->Index--;
		if (a1->Data1.Entity->Index < 0)
			a1->Data1.Entity->Index = 0;
		/*
		if (ChaoObjectCount[0] > 0)
		{
			if (a1->Data1.Entity->Index >= ChaoObjectCount[0] + 2)
				sub_46DF30();
			if (a1->Data1.Entity->Index < ChaoObjectCount[0] + 2)
				sub_46DED0();
		}
		*/
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
		v13 = 0.03999999910593033;
		camData1->lookAtPosMaybe.x = (lookAtPosMaybe->x - camData1->lookAtPosMaybe.x) * 0.03999999910593033
			+ camData1->lookAtPosMaybe.x;
		camData1->lookAtPosMaybe.y = (lookAtPosMaybe->y + camData1->float7C - camData1->lookAtPosMaybe.y)
			* 0.03999999910593033
			+ camData1->lookAtPosMaybe.y;
		v14 = lookAtPosMaybe->z;
		goto LABEL_25;
	}
	if (!SomeOdekakeFlag || !OdekakeMachinePtr)
	{
		v13 = 0.4000000059604645;
		camData1->lookAtPosMaybe.x = (playerPos1->x - camData1->lookAtPosMaybe.x) * 0.4000000059604645
			+ camData1->lookAtPosMaybe.x;
		camData1->lookAtPosMaybe.y = (camData1->float78 + playerPos1->y - camData1->lookAtPosMaybe.y)
			* 0.2000000029802322
			+ camData1->lookAtPosMaybe.y;
		v14 = playerPos1->z;
		goto LABEL_25;
	}
	v11 = &OdekakeMachinePtr->Data1.Entity->Position;
	v33.x = v11->x;
	v33.y = v11->y + 10.0;
	v33.z = v11->z;
	if (!dword_1AED310)
	{
		v13 = 0.03999999910593033;
		camData1->lookAtPosMaybe.x = (v33.x - camData1->lookAtPosMaybe.x) * 0.03999999910593033
			+ camData1->lookAtPosMaybe.x;
		camData1->lookAtPosMaybe.y = (v33.y - camData1->lookAtPosMaybe.y) * 0.03999999910593033
			+ camData1->lookAtPosMaybe.y;
		v14 = v33.z;
	LABEL_25:
		camData1->lookAtPosMaybe.z = v13 * (v14 - camData1->lookAtPosMaybe.z) + camData1->lookAtPosMaybe.z;
		goto LABEL_26;
	}
	camData1->lookAtPosMaybe = v33;
LABEL_26:
	if (SomeOdekakeFlag && v10)
	{
		if (v9)
		{
			v17 = (EntityData1 *)v10->Data1.Entity; //oof
			v18 = v17->Rotation.y + 22528;
			camData1->field_40.x = njSin(v18) * 45.0 + v17->Position.x;
			camData1->field_40.y = v17->Position.y + 16.0;
			camData1->field_40.z = njCos(v18) * 45.0 + v17->Position.z;
			camData1->EntityData1.Position.x = camData1->field_40.x;
			camData1->EntityData1.Position.y = camData1->field_40.y;
			camData1->EntityData1.Position.z = camData1->field_40.z;
			goto LABEL_33;
		}
		v19.Undefined = (EntityData1 *)v10->Data1.Entity;
		v20 = v19.Entity->Rotation.y + 22528;
		camData1->field_40.x = njSin(v20) * 37.0 + v19.Entity->Position.x;
		camData1->field_40.y = v19.Entity->Position.y + 12.0;
		v21 = njCos(v20) * 37.0 + v19.Entity->Position.z;
	}
	else
	{
		camData1->field_40.x = playerPos1->x + resultPosition.x;
		camData1->field_40.y = camData1->float80 + playerPos1->y;
		v21 = playerPos1->z + resultPosition.z;
	}
	camData1->field_40.z = v21;
LABEL_33:
	v22 = a1;
	v23 = a1->EntityData2->field_DC + 2.5;
	if (camData1->field_40.y < v23)
	{
		camData1->field_40.y = v23;
	}
	v24 = a1->EntityData2;
	if (v24->field_40 & 0x800)
	{
		camData1->float84 = 0.02;
	}
	else if (SomeOdekakeFlag)
	{
		camData1->float84 = 0.05;
	}
	else
	{
		v25 = camData1->float84 + 0.009999999776482582;
		camData1->float84 = v25;
		if (0.13 < (double)v25)
		{
			camData1->float84 = 0.13;
		}
	}
	resultPosition.x = (camData1->field_40.x - camData1->EntityData1.Position.x) * camData1->float84;
	if (v24->field_40 & 0x400 && (v26 = camData1->field_40.y - camData1->EntityData1.Position.y, v26 < 30.0))
	{
		v27 = v26 * 0.1000000014901161 * 0.1000000014901161;
	}
	else
	{
		v27 = (camData1->field_40.y - camData1->EntityData1.Position.y) * 0.1000000014901161;
	}
	resultPosition.y = v27;
	resultPosition.z = (camData1->field_40.z - camData1->EntityData1.Position.z) * camData1->float84;
	v28 = njScalor(&resultPosition);
	v29 = v28;
	if (v28 > 0.0 && v29 > 3.0)
	{
		NormalizeVectorMaybe(&resultPosition);
		v22 = a1;
		resultPosition.x = resultPosition.x * 3.0;
		resultPosition.y = resultPosition.y * 3.0;
		resultPosition.z = 3.0 * resultPosition.z;
		v29 = (float)3.0;
	}
	camData1->EntityData1.Position.x = camData1->EntityData1.Position.x + resultPosition.x;
	camData1->EntityData1.Position.y = resultPosition.y + camData1->EntityData1.Position.y;
	camData1->EntityData1.Position.z = camData1->EntityData1.Position.z + resultPosition.z;
	if (camData1->dword88)
	{
		if (v29 > 0.0099999998 || v24->field_40 & 0x800)
		{
			camData1->dword88 = 0;
			AddToCollisionList(v22);
			return;
		}
	}
	else
	{
		if (!(v24->field_40 & 0x800) && v29 < 0.0049999999f)
		{
			camData1->dword88 = 1;
			sub_53E020((int)v22);
			AddToCollisionList(a1);
			return;
		}
		sub_53E020((int)v22);
		v22 = a1;
	}
	AddToCollisionList(a1);
}

void LoadHandPlusCam() 
{
	AL_NormalCameraExecutor_Load();
	//Hand_Load();
}
ObjectMaster *AL_NormalCameraExecutor_LoadReplace()
{
	ObjectMaster *v1; // esi
	MemManFuncs *v2; // eax
	int v3; // eax
	AL_NormalCameraExecutor_Data1 *v4; // edi
	double v5; // st7
	UnknownData2 *v6; // eax
	UnknownData2 *v7; // edx

	if (PointerToChaoCamera)
	{
		return 0;
	}
	v1 = AllocateObjectMaster(AL_NormalCameraExecutor_MainReplace, 2, "AL_NormalCameraExecutor");
	v2 = MemoryManager;
	v1->DeleteSub = (ObjectFuncPtr)0x0057D2D0;
	v3 = (int)v2->Allocate(144, (char*)"..\\..\\src\\Chao\\al_camera.c", 410);
	v4 = (AL_NormalCameraExecutor_Data1 *)(v3 + 4);
	*(int *)v3 = 305419896;
	memset((void *)(v3 + 4), 0, 0x8Cu);
	v1->Data1.Undefined = &v4->EntityData1;
	InitCollision(v1, (CollisionData*)0x8A57B0, 2, 5u);
	if (MainCharObj2[0] && MainCharObj2[0]->CharID == Characters_Eggman)
	{
		v4->float78 = 10.0;
		v4->float7C = 24.0;
		v5 = 11.5;
	}
	else
	{
		v4->float78 = 4.5;
		v4->float7C = 22.0;
		v5 = 5.0;
	}
	v4->float80 = v5;
	v4->float84 = 0.13f;
	v6 = AllocateUnknownData2(v1);
	v7 = v1->EntityData2;
	v6->field_B8 = 5.0;
	PointerToChaoCamera = v1;
	v6->field_AC = 1.0;
	v6->field_BC = -3.0;
	v7->field_48 = 0.0;
	v4->field_30 = 0;
	return v1;
}
FunctionPointer(void, sub_5987B0, (), 0x5987B0);
FunctionPointer(void, sub_539BE0, (), 0x539BE0);
void OverwriteRaceCam(int a1)
{
	sub_5987B0();
	sub_539BE0();
	sub_57D4C0();
	AL_NormalCameraExecutor_LoadReplace();
}

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		WriteJump((void*)AL_NormalCameraExecutor_Main, AL_NormalCameraExecutor_MainReplace);
	}
	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}

