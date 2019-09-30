#include <crypt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define SHDW_LINE_LEN 256
#define WORD_LEN 80

typedef struct SHADOWDATA
{
	char * id;
	char * salt;
	char * hash;
}ShadowData;


int main(){

	FILE *shadow;
	FILE *dict;
	ShadowData fullShadow[52];
	char *hash;
	char hashHolder[256] = "";

	shadow = fopen("shadow", "r");
	if(shadow == NULL){
		fprintf(stderr, "Cannot open shadow file \n");
		exit(1);
	}

	dict   = fopen("words.txt", "r");
	if(dict == NULL){
		fprintf(stderr, "Cannot open dict file\n");
		exit(1);
	}

	char shdw_line[SHDW_LINE_LEN];
	int num_accounts = 0;
	while(fgets(shdw_line, SHDW_LINE_LEN, shadow)!=NULL){
		char *token = strtok(shdw_line, ":");
		printf("ID: %s\n", token);
		fullShadow[num_accounts].id = (char *) malloc( strlen(token));
		strcpy(fullShadow[num_accounts].id, token);
		
		char *shdw_hash = strtok(NULL, ":");
		if(strcmp(shdw_hash, "*")!=0 && strcmp(shdw_hash, "!")!=0){
			token = strtok(shdw_hash, "$");
			token = strtok(NULL, "$");
			printf("  salt: %s\n", token);
			fullShadow[num_accounts].salt = (char *) malloc( strlen(token));
			strcpy(fullShadow[num_accounts].salt, token);
			
			token = strtok(NULL, "$");
			printf("  hash: %s\n", token);
			fullShadow[num_accounts].hash = (char *) malloc( strlen(token));
			strcpy(fullShadow[num_accounts].hash, token);
			
			num_accounts++;
			//////////////////////
			// Part A: 
			//  These values need to 
			//  be stored in an array
			//////////////////////
		}
	}

	//Test to check proper data storage
	/*printf("ID\tsalt\thash\n");
	for(int i = 0; i < num_accounts; i ++)
		printf("%s\t%s\t%s\n",fullShadow[i].id, fullShadow[i].salt, fullShadow[i].hash);*/
	

	char word[WORD_LEN];
	printf("Entering Password Check: \n");
	while(fgets(word, WORD_LEN, dict)!=NULL){
		//printf("Testing Word : %s\n",word);
		//printf("Fixing Word\n");
		word[strlen(word) - 1] = 0; //remove \n from word, it messes with the hash
		
		for(int i=0; i<num_accounts; i++){
			//////////////////////
			// Part B: 
			//  For each account, compute
			//  the hash for that dictionary
			//  word and the users known salt
			//  as shown below:
			//     hash = crypt("password", "$6$_____");
			//  then check if the password is 
			//  the same as that users entry 
			//  from  /etc/shadow, if so
			//  you've successfully cracked it,
			//  print the password and userid
			//////////////////////
			
			//prepare Salt for crypt function
			char saltHolder[48] = "$6$";
			//printf("cating salt\n");
			strcat(saltHolder, fullShadow[i].salt);
			
			hash = crypt(word, saltHolder);
			
			//printf("cating hash\n");
			//put together full hash
			strcat(hashHolder, saltHolder);
			strcat(hashHolder, "$");
			strcat(hashHolder, fullShadow[i].hash);

			if (strcmp(hash,hashHolder) == 0)
			{
				printf("Password Found!\n%s\t:\t%s\n",fullShadow[i].id, word);
			}
			/*else
			{
				printf("\nhash from fnc:\t\t%s\n",hash);
				printf("Expected Hash:\t\t%s\n",fullShadow[i].hash);
				getchar();
			}*/
			memset(hashHolder, 0, strlen(hashHolder));
			
		}
	}

	for(int i = 0; i < num_accounts; i++)
	{
		free(fullShadow[i].id);
		free(fullShadow[i].salt);
		free(fullShadow[i].hash);
	}
}
