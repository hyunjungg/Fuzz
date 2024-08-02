#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <sys/types.h>
#include "json/json.h"
#include <Windows.h>
#include <winternl.h>
#include "..\inc\Config.h"
#include "..\inc\SyscallWrapper.h"
#include "..\inc\Alloc.h"
#include "..\inc\Output.h"
#include "..\inc\SetupArg.h"
#include "..\inc\RunSeed.h"

extern "C"
{
    int _NtWrapperArg(ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR);
    void _SetSysid(int);
    DWORD _SysId;

};
// Store current syscall name for debugging purpose.
char curName[SYSNAME_MAXLEN];


void runSyscall(Json::Value* syscallJson) {
  int sysNum, argNum, retID, i;
  char argName[16];
  uint64_t args[SYSARG_MAXNUM];
  Json::Value argJson;
  SyscallWrapper fptr;
  uint64_t status;

  strncpy(curName, syscallJson->get("name", NULL).asCString(), SYSNAME_MAXLEN);
  sysNum = syscallJson->get("sysnum", NULL).asInt();
  argNum = syscallJson->get("argnum", NULL).asInt();
  //retID = syscallJson->get("retID", NULL).asInt();
  printf("name: %s, sysnum = %d, argnum = %d\n", curName, sysNum, argNum);

  // Setup arguments.
  for (i = 0; i < argNum; i++) {
    snprintf(argName, sizeof(argName), "arg%d", i + 1);
    argJson = syscallJson->get(argName, NULL);
    args[i] = prepareArg(&argJson);
    printf("arg%d: 0x%x\n", i + 1, args[i]);
  }

  for (i; i < SYSARG_MAXNUM; i++) {
      args[i] = 0;
  }

  //// Fetch syscall wrapper to invoke.
  //fptr = fetchWrapper(sysNum);
  //if (fptr == NULL) {
  //  printf("Failed to fetch wrapper for sysNum %d\n", sysNum);
  //  exit(1);
  //}

  // Prepare return buffer.
  //retBuf = (uint64_t*)alloc(sizeof(uint64_t));
  //registerOutputPtr(retBuf, retID);

  // Invoke syscall and save the return value.
  printf("Invoke syscall...\n");
  //_SetSysid(sysNum);

  _SysId = sysNum;
  status = _NtWrapperArg(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10]);
  printf("Return value: 0x%x\n\n", status);
}

void runSeed(Json::Value *seedJson) {
  size_t index;
  Json::Value syscallJson;
  for (index = 0; index < seedJson->size(); ++index) {
    syscallJson = seedJson->get(index, NULL);
    runSyscall(&syscallJson);
  }
}
