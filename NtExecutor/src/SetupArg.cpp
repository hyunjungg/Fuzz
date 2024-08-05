#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <sys/types.h>
#include "json/json.h"
#include <Windows.h>
#include "..\inc\Config.h"
#include "..\inc\Alloc.h"
#include "..\inc\Output.h"
#include "..\inc\SetupArg.h"



void nop(void) {

}

//
//void storeArg(Json::Value* argJson, uint64_t* curPtr, size_t ptrSize) {
//  char buf[64];
//  int id;
//  size_t size, width, index, offset;
//  uint64_t * ptr;
//  Json::Value contentJson, arrayJson, elemJson, fieldJson;
//
//  strncpy(buf, argJson->get("kind", NULL).asCString(), sizeof(buf));
//
//  if (strcmp(buf, "funcptr") == 0) {
//    *curPtr = (uint64_t)nop;
//  }
//  else if (strcmp(buf, "string") == 0) {
//    strncpy((char*)curPtr, argJson->get("val", NULL).asCString(), ptrSize);
//  }
//  else if (strcmp(buf, "byte") == 0) {
//    *(uint8_t*)curPtr = (uint8_t)argJson->get("val", NULL).asUInt();
//  }
//  else if (strcmp(buf, "word") == 0) {
//    *(uint16_t*)curPtr = (uint16_t)argJson->get("val", NULL).asUInt();
//  }
//  else if (strcmp(buf, "dword") == 0) {
//    *curPtr = argJson->get("val", NULL).asUInt();
//  }
//  else if (strcmp(buf, "output") == 0) {
//    id = argJson->get("id", NULL).asInt();
//    *curPtr = fetchOutput(id);
//  }
//  else if (strcmp(buf, "outptr") == 0) {
//    size = argJson->get("size", NULL).asInt();
//    id = argJson->get("id", NULL).asInt();
//    ptr = (uint64_t*)alloc(size);
//    registerOutputPtr(ptr, id);
//    *(uint64_t**)curPtr = ptr;
//  }
//  else if (strcmp(buf, "inptr") == 0) {
//    size = argJson->get("size", NULL).asInt();
//    ptr = (uint64_t*)alloc(size);
//    contentJson = argJson->get("content", NULL);
//    storeArg(&contentJson, ptr, size);
//    *(uint64_t**)curPtr = ptr;
//  }
//  else if (strcmp(buf, "array") == 0) {
//    width = argJson->get("width", NULL).asInt();
//    arrayJson = argJson->get("elems", NULL);
//    for (index = 0; index < arrayJson.size(); ++index) {
//      elemJson = arrayJson.get(index, NULL);
//      storeArg(&elemJson, curPtr + width * index, ptrSize - width * index);
//    }
//  }
//  else if (strcmp(buf, "struct") == 0) {
//    arrayJson = argJson->get("fields", NULL);
//    for (index = 0; index < arrayJson.size(); ++index) {
//      elemJson = arrayJson.get(index, NULL);
//      offset = elemJson.get("offset", NULL).asInt();
//      fieldJson = elemJson.get("content", NULL);
//      storeArg(&fieldJson, curPtr + offset, ptrSize - offset);
//    }
//  }
//  else {
//    printf("Unexpected argument kind to store: %s\n", buf);
//    exit(1);
//  }
//}

uint64_t prepareArg(Json::Value* argJson) {
  char buf[64];
  size_t size;
  int id;
  uint64_t* ptr;
  Json::Value contentJson;

  strncpy(buf, argJson->get("kind", NULL).asCString(), sizeof(buf));

  if (strcmp(buf, "funcptr") == 0) {
    return (uint64_t)nop;
  }
  else if (strcmp(buf, "byte") == 0 || strcmp(buf, "word") == 0  || strcmp(buf, "dword") == 0 || strcmp(buf, "qword") == 0) {
      return argJson->get("val", NULL).asUInt64();
  }
  else if (strcmp(buf, "retval") == 0) {
    id = argJson->get("id", NULL).asInt();
    return fetchOutput(id);
  }
  else if (strcmp(buf, "outptr") == 0) {
    size = argJson->get("size", NULL).asInt();
    id = argJson->get("id", NULL).asInt(); 
    ptr = (uint64_t*)alloc(size);
    registerOutputPtr(ptr, id);
    return (uint64_t)ptr;
  }
  else if (strcmp(buf, "inptr") == 0) {
    size = argJson->get("size", NULL).asInt();
    ptr = (uint64_t*)alloc(size);
    contentJson = argJson->get("val", NULL);
    if (contentJson.isArray()) {  // byte array 인 경우

        for (size_t index = 0; index < contentJson.size(); index++) {
            byte value = contentJson.get(index, NULL).asUInt();
            *((byte*)ptr + index) = value;
        }
    }
    else { // primitive 인 경우 

        *ptr = prepareArg(&contentJson);
    }

    return (uint64_t)ptr;
  }
  else if (strcmp(buf, "struct") == 0) { // struct 필드인 경우 offset 이 추가됨 
    size = argJson->get("size", NULL).asInt();
    ptr = (uint64_t*)alloc(size);
    contentJson = argJson->get("val", NULL);

    for (size_t index = 0; index < contentJson.size(); index++) {
        Json::Value value = contentJson.get(index, NULL);
        unsigned int offset = value.get("offset", NULL).asUInt();

        uint64_t * offset_ptr = (uint64_t *)((byte*)ptr + offset);
        *offset_ptr = prepareArg(&value);
    }

    return (uint64_t)ptr;
  }
  else {
    printf("Unexpected argument kind to prepare: %s\n", buf);
    exit(1);
  }

  return 0;
}
