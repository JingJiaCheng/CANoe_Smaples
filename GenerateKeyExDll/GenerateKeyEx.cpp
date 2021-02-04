//////////////////////////////////////////////////////////////////////////
///Example for a SeedKey.Dll used in CANoe.DiVa
///See also CANoe.DiVa help==>Proceedings/Entry masks/"General" entry mask
//////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "GenerateKeyEx.h"

// KeyGeneration.cpp : Defines the entry point for the DLL application.
BOOL APIENTRY DllMain(HANDLE hModule,
					  DWORD ul_reason_for_call,
					  LPVOID lpReserved)
{
	return TRUE;
}

KEYGENALGO_API VKeyGenResultEx GenerateKeyEx(
	const unsigned char *ipSeedArray,  /* Array for the seed [in] */
	unsigned int iSeedArraySize,	   /* Length of the array for the seed [in] */
	const unsigned int iSecurityLevel, /* Security level [in] */
	const char *ipVariant,			   /* Name of the active variant [in] */
	unsigned char *iopKeyArray,		   /* Array for the key [in, out] */
	unsigned int iMaxKeyArraySize,	   /* Maximum length of the array for the key [in] */
	unsigned int &oActualKeyArraySize) /* Length of the key [out] */
{
	unsigned int seed = 0;
	unsigned int key = 0;

	//Copy seed from parameter to a integer
	//Note: The byte order in the seed array is equal to the byte order in the bus message
	seed = ipSeedArray[3];
	seed = seed | (ipSeedArray[2] << 8);
	seed = seed | (ipSeedArray[1] << 16);
	seed = seed | (ipSeedArray[0] << 24);

	//begin calculate key from seed------------------------------------------------------------

	//for security access with Services 0x27 01 ->0x27 02, for security leve1: app diagnose.
	if (iSecurityLevel == 0x01)
	{
		key = key_generate_level_1(seed);
	}
	//for security access with Services 0x27 09 ->0x27 0A, for security level2: bootloader.
	else if (iSecurityLevel == 0x02)
	{
		key = key_generate_level_2(seed);
	}
	else
	{
	}
	//end calculate key from seed------------------------------------------------------------

	//Copy key to the output buffer
	//Note: The first byte of the key array will be the first key byte of the bus message
	iopKeyArray[3] = key & 0xff;
	iopKeyArray[2] = (key >> 8) & 0xff;
	iopKeyArray[1] = (key >> 16) & 0xff;
	iopKeyArray[0] = (key >> 24) & 0xff;

	//setting length of key
	oActualKeyArraySize = 4;

	return KGRE_Ok;
}


unsigned int key_generate_level_1(unsigned int seed)
{
	unsigned char u8Loop = 0;
	unsigned int seedKey = seed;
	const unsigned int KEY_MASK = 0x8EC9AA7E;

	for (u8Loop = 0; u8Loop < 35; u8Loop++)
	{
		if (seedKey & 0x80000000)
		{
			seedKey = seedKey << 1;
			seedKey = seedKey ^ KEY_MASK;
		}
		else
		{
			seedKey = seedKey << 1;
		}
	}

	return seedKey;
}


unsigned int key_generate_level_2(unsigned int seed)
{
	unsigned char u8Loop = 0;
	unsigned int seedKey = seed;
	const unsigned int KEY_MASK = 0x8EC8BB8E;

	for (u8Loop = 0; u8Loop < 35; u8Loop++)
	{
		if (seedKey & 0x80000000)
		{
			seedKey = seedKey << 1;
			seedKey = seedKey ^ KEY_MASK;
		}
		else
		{
			seedKey = seedKey << 1;
		}
	}

	return seedKey;
}
