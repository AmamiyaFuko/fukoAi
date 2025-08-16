#include <cjson/cJSON.h>
#include <stdlib.h>
#include <stdio.h>
#include "fukoAi.h"

int main(){
	fukoAi_config zhu_config = {.model = DS_R1, .system_content = "{name:zhu}"};
	fukoAi* zhu = fukoAi_CreateAi(&zhu_config);
	while(1){
		printf(">>-STARFISH->");
		char content[2048];
		scanf("%2000s", content);
		printf("%s\n", content);
		char* response = fukoAi_easy_MemoryChat(zhu, content);
		printf("%s\n",response);
		//free(response);
	}
	return 0;
}
