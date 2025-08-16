#ifndef STDIO_H
#define STDIO_H 0721
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H 0721
#include <stdlib.h>
#endif

#ifndef CURL_H
#define CURL_H 0721
#include <curl/curl.h>
#endif

#ifndef CJSON_H
#define CJSON_H 0721
#include <cjson/cJSON.h>
#endif

#ifndef FUKOAI_CONFIG
#define FUKOAI_CONFIG 0720
#include "fukoAi.config"
#endif

typedef struct fukoAi{
	cJSON* root;
	cJSON* messages;
	int messagesSize;
	cJSON* response;
}fukoAi;

typedef struct fukoAi_config{
	char* model;
	char* system_content;
}fukoAi_config;

char* fukoAi_SteamToString(FILE* steam);
char* fukoAi_curl_OpenAiGET(char* json_data);
cJSON* fukoAi_GetMessages(cJSON* response);
cJSON* fukoAi_CreateContent(char* role, char* str_content);

fukoAi* fukoAi_CreateAi(fukoAi_config* cfig);

cJSON* fukoAi_SetChat_UserChat(fukoAi* Ai, char* content);
cJSON* fukoAi_SetChat_EgoChat(fukoAi* Ai, char* content);
cJSON* fukoAi_SetChat_SysChat(fukoAi* Ai, char* content);
cJSON* fukoAi_Perform(fukoAi* Ai);

char* fukoAi_easy_MemoryChat(fukoAi* Ai, char* content);
char* fukoAi_easy_NoMemoryChat(fukoAi* Ai, char* content);
