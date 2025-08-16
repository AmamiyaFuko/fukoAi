#include "fukoAi.h"

char* fukoAi_SteamToString(FILE* steam){
	fseek(steam,0,SEEK_END);
	size_t lenght = ftell(steam);
	char* reString = (char*)malloc(lenght+1);
	reString[lenght] = '\0';
	fseek(steam,0,SEEK_SET);
	fread(reString, 1, lenght, steam);
	fclose(steam);
	return reString;
}

char* fukoAi_curl_OpenAiGET(char* json_data){
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, BASE_URL);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	struct curl_slist* header = NULL;
	header = curl_slist_append(header, "Authorization: Bearer " API_KEY); //APK_KEY and BASE_URL in api_config.h
	header = curl_slist_append(header, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

	FILE* tfp = tmpfile();
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, tfp);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	char* reString = fukoAi_SteamToString(tfp);
	free(json_data);
	return reString;
}

cJSON* fukoAi_CreateContent(char* role, char* str_content){
	cJSON* content = cJSON_CreateObject();
	cJSON_AddStringToObject(content, "role", role);
	cJSON_AddStringToObject(content, "content", str_content);
	return content;
}


fukoAi* fukoAi_CreateAi(fukoAi_config* cfig){
	cJSON* root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "model", cfig->model);
	
	cJSON* messages = cJSON_AddArrayToObject(root, "messages");
	
	fukoAi* reAi = (fukoAi*)malloc(sizeof(fukoAi));
	reAi->root = root;
	reAi->messages = messages;
	reAi->messagesSize = 0;
	reAi->response = cJSON_CreateObject();

	cJSON* sysContent = fukoAi_CreateContent("system", cfig->system_content);
	cJSON_AddItemToArray(messages, sysContent);

	return reAi;
}

cJSON* fukoAi_SetChat_UserChat(fukoAi* Ai, char* content){
	cJSON* userContent = fukoAi_CreateContent("user", content);
	cJSON_AddItemToArray(Ai->messages, userContent);
	Ai->messagesSize ++;
	return userContent;
}

cJSON* fukoAi_SetChat_EgoChat(fukoAi* Ai, char* content){
	cJSON* egoContent = fukoAi_CreateContent("assistant", content);
	cJSON_AddItemToArray(Ai->messages, egoContent);
	Ai->messagesSize ++; 
	return egoContent;
}

cJSON* fukoAi_SetChat_SysChat(fukoAi* Ai, char* content){
	cJSON* sysContent = fukoAi_CreateContent("system", content);
	cJSON_AddItemToArray(Ai->messages, sysContent);
	Ai->messagesSize ++;
	return sysContent;
}

cJSON* fukoAi_PerformChat(fukoAi* Ai){
	cJSON_Delete(Ai->response);
	char* json_data = cJSON_Print(Ai->root);
	char* response = fukoAi_curl_OpenAiGET(json_data);
	cJSON* json_response = cJSON_Parse(response);
	free(response);
	return json_response;
}

cJSON* fukoAi_GetMessages(cJSON* response){
	cJSON* choices_array = cJSON_GetObjectItem(response, "choices");
	cJSON* no_finish_response = cJSON_GetArrayItem(choices_array, 0);
	cJSON* messages = cJSON_GetObjectItem(no_finish_response, "message");
	return messages;
}

char* fukoAi_easy_MemoryChat(fukoAi* Ai, char* content){
	fukoAi_SetChat_UserChat(Ai, content);	
	cJSON* response = fukoAi_PerformChat(Ai);
	cJSON* messages = fukoAi_GetMessages(response);
	cJSON* reContent = cJSON_GetObjectItem(messages, "content");
	fukoAi_SetChat_SysChat(Ai, reContent->valuestring);
	Ai->response = response;
	return reContent->valuestring;
}

char* fukoAi_easy_NoMemoryChat(fukoAi* Ai, char* content){
	cJSON* user_content = fukoAi_SetChat_UserChat(Ai, content);	
	cJSON* response = fukoAi_PerformChat(Ai);
	cJSON* messages = fukoAi_GetMessages(response);
	char* reString = cJSON_GetObjectItem(messages, "content")->valuestring;
	Ai->response = response;
	cJSON_DeleteItemFromArray(Ai->messages, Ai->messagesSize -1);
	cJSON_Delete(user_content);
	return reString;
}
