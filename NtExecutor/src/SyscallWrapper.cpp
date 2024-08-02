#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <tlhelp32.h>
#include <winternl.h>
#include "..\inc\Config.h"
#include "..\inc\SyscallWrapper.h"

//SyscallWrapper wrapperFptrs[SYSNUM_MAXRANGE];
//
//SyscallWrapper fetchWrapper(int sysNum) {
//  if (sysNum < SYSNUM_MAXRANGE) {
//    return wrapperFptrs[sysNum];
//  }
//  else {
//    return NULL;
//  }
//}

//int NtCreateEventWrapper(unsigned int* args){
//  return _NtCreateEventStub(args[0], args[1], args[2], args[3], args[4]);
//}
//
//int NtCloseWrapper(unsigned int* args){
//  return NtCloseStub(args[0]);
//}
//
//void registerWrapper(void) {
//  wrapperFptrs[51] = NtCreateEventWrapper;
//}
