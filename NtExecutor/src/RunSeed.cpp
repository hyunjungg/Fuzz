#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <sys/types.h>
#include "json/json.h"
#include <windows.h>
#include <winternl.h>
#include "Config.h"
#include "SyscallWrapper.h"
#include "Alloc.h"
#include "Output.h"
#include "SetupArg.h"
#include "RunSeed.h"
#include <stdarg.h>
#include "nyx_api.h"


extern "C"
{
    //int _NtWrapperArg(ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR, ULONG_PTR);
    DWORD SysId;

  static inline int _NtWrapperArg(
  uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
  uint64_t arg5, uint64_t arg6, uint64_t arg7, uint64_t arg8,
  uint64_t arg9, uint64_t arg10, uint64_t arg11)
  {
      uint64_t result;
asm volatile (
    "sub $88, %%rsp\n\t"           // 88바이트(11 * 8)만큼 스택 공간 확보 (11개의 8바이트 인자)
    "mov %[arg1], %%rcx\n\t"       // arg1을 RCX에 저장
    "mov %[arg2], %%rdx\n\t"       // arg2를 RDX에 저장
    "mov %[arg3], %%r8\n\t"        // arg3을 R8에 저장
    "mov %[arg4], %%r9\n\t"        // arg4를 R9에 저장
    "mov %[arg5], %%rax\n\t"       // arg5를 임시로 RAX에 저장
    "mov %%rax, 40(%%rsp)\n\t"     // RAX를 RSP+40에 저장
    "mov %[arg6], %%rax\n\t"       // arg6를 임시로 RAX에 저장
    "mov %%rax, 48(%%rsp)\n\t"     // RAX를 RSP+48에 저장
    "mov %[arg7], %%rax\n\t"       // arg7를 임시로 RAX에 저장
    "mov %%rax, 56(%%rsp)\n\t"     // RAX를 RSP+56에 저장
    "mov %[arg8], %%rax\n\t"       // arg8를 임시로 RAX에 저장
    "mov %%rax, 64(%%rsp)\n\t"     // RAX를 RSP+64에 저장
    "mov %[arg9], %%rax\n\t"       // arg9를 임시로 RAX에 저장
    "mov %%rax, 72(%%rsp)\n\t"     // RAX를 RSP+72에 저장
    "mov %[arg10], %%rax\n\t"      // arg10을 임시로 RAX에 저장
    "mov %%rax, 80(%%rsp)\n\t"     // RAX를 RSP+80에 저장
    "mov %[arg11], %%rax\n\t"      // arg11을 임시로 RAX에 저장
    "mov %%rax, 88(%%rsp)\n\t"     // RAX를 RSP+88에 저장
    "mov %[sysid], %%rax\n\t"         // SysId 값을 RAX로 이동 (전역 변수)
    "mov %%rcx, %%r10\n\t"         // RCX 값을 R10으로 이동
    "syscall\n\t"                  // 시스템 호출
    "add $88, %%rsp\n\t"           // 스택 복원 (할당한 스택 프레임 제거)
    : "=a" (result)                // 결과는 RAX에 저장됨
    : [arg1] "g" (arg1), [arg2] "g" (arg2), [arg3] "g" (arg3), [arg4] "g" (arg4),
      [arg5] "g" (arg5), [arg6] "g" (arg6), [arg7] "g" (arg7), [arg8] "g" (arg8),
      [arg9] "g" (arg9), [arg10] "g" (arg10), [arg11] "g" (arg11), [sysid] "m" (SysId)
    : "rsp", "r10", "r11", "rcx", "rdx", "r8", "r9", "memory"
);




      return result;
  }

};



// Store current syscall name for debugging purpose.

char curName[SYSNAME_MAXLEN];
#include <stdint.h>

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
  hprintf("name: %s, sysnum = %d, argnum = %d\n", curName, sysNum, argNum);

  // Setup arguments.
  for (i = 0; i < argNum; i++) {
    snprintf(argName, sizeof(argName), "arg%d", i + 1);
    argJson = syscallJson->get(argName, NULL);
    args[i] = prepareArg(&argJson);
    hprintf("arg%d: 0x%x\n", i + 1, args[i]);
  }

  for (i; i < SYSARG_MAXNUM; i++) {
      args[i] = 0;
  }

  // Invoke syscall and save the return value.
  hprintf("Invoke syscall...\n");

  SysId = sysNum;
  status = _NtWrapperArg(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10]);
  hprintf("Return value: 0x%x\n\n", status);


}

void runSeed(Json::Value *seedJson) {
  size_t index;
  Json::Value syscallJson;
  for (index = 0; index < seedJson->size(); ++index) {
    syscallJson = seedJson->get(index, NULL);
    runSyscall(&syscallJson);
  }

}
