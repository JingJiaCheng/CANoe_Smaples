//****************************************************************************************
//   File name: KeyGenAlgoEx.h
// Description: Abstract base class defines the interface for the key generation algorithm
//        Date: 2005-04-01
//   Copyright: Vector Informatik GmbH - 2003
//****************************************************************************************
// This comment is needed for the delivery script: INTERPRETATION_API
#ifndef KEY_GEN_ALGO_INTERFACE_H
#define KEY_GEN_ALGO_INTERFACE_H
#if _MSC_VER > 1000
#pragma once
#endif

#define KEYGENALGO_EXPORTS

#ifdef KEYGENALGO_EXPORTS
#define KEYGENALGO_API extern "C" __declspec(dllexport)
#else
#define KEYGENALGO_API __declspec(dllimport)
#endif

enum VKeyGenResultEx
{
  KGRE_Ok = 0,
  KGRE_BufferToSmall = 1,
  KGRE_SecurityLevelInvalid = 2,
  KGRE_VariantInvalid = 3,
  KGRE_UnspecifiedError = 4
};

// The client has to provide a keyArray buffer and has to transfer this buffer - 
// including its size - to the GenerateKey method. The method checks, if the size is
// sufficient. The client can discover the required size by examining the service used
// transfer the key to the ECU.
// Returns false if the key could not be generated:
//  -> keyArraySize to small
//  -> generation for specified security level not possible
//  -> variant unknown
KEYGENALGO_API VKeyGenResultEx GenerateKeyEx(
   const unsigned char* ipSeedArray, unsigned int iSeedArraySize,
   const unsigned int iSecurityLevel, const char* ipVariant,
   unsigned char* iopKeyArray, unsigned int iMaxKeyArraySize, 
   unsigned int& oActualKeyArraySize);

extern unsigned int key_generate_level_1(unsigned int seed);
extern unsigned int key_generate_level_2(unsigned int seed);

#endif // KEY_GEN_ALGO_INTERFACE_H
