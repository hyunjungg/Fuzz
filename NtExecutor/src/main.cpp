
#include <stdlib.h>
#include "json/json.h"
#include "Config.h"
#include "SyscallWrapper.h"
#include "RunSeed.h"
#include <stdio.h>
#include "json/json.h"

char jsonBuf[JSON_MAXLEN];

int main(int argc, char ** argv)
{
  FILE* fp;
  JSONCPP_STRING err;
  Json::CharReaderBuilder builder;
  Json::Value seedJson;
  const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
  int jsonLen = 0;

  //if (argc != 2) {
  //  printf("Usage: %s <seed json file>\n", argv[0]);
  //  exit(1);
  //}

  //fp = fopen(argv[1], "rt");
  fp = fopen("C:\\Temp\\Untitled-1.json", "rt");
  if (fp == NULL) {
    printf("Failed to read file %s\n", argv[1]);
    exit(1);
  }

  fseek(fp, 0L, SEEK_END);
  jsonLen = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  if (jsonLen < 0 || jsonLen >= JSON_MAXLEN) {
    printf("Invalid file size of %s\n", argv[1]);
    fclose(fp);
    exit(1);
  }

  fread(jsonBuf, jsonLen, sizeof(char), fp);
  fclose(fp);

  if (!reader->parse(jsonBuf, jsonBuf + jsonLen, &seedJson, &err)) {
    printf("[FATAL] Failed to parse JSON content\n");
    exit(1);
  }


  runSeed(&seedJson);


  return 0;
}

