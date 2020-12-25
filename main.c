#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <pthread.h>
#define MAX_LEN 256
int lineCount = 0;
int url(char *z){
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	char *previousURL;
	previousURL = z;
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, z);
		curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
		res = curl_easy_perform(curl);
		if(res == CURLE_OK || res != CURLE_OK){
			long response_code;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			if(response_code >= 400) printf("failed, response code: %ld, url:%s\n", response_code, previousURL);
			else if(response_code >= 300) printf("Redirecting, response code: %ld, url:%s\n", response_code, previousURL);
			else if(response_code >= 200) printf("Successful, response code: %ld,url:%s\n", response_code, previousURL);
			curl_easy_cleanup(curl);
	}
}
}

void readLines(char *p){
    FILE *fileptr = fopen(p, "r");
    char bufr[100];


    for (int i = 0; fgets(bufr, 100, fileptr) != NULL; i++) {
        lineCount++;
    
    }
    fclose(fileptr);
}

char **wordlist(char *filename, int lineCount) {  
    FILE* fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
      perror("Failed: ");
      exit(EXIT_FAILURE);
    }

    char** result = calloc(lineCount, sizeof(char*));

    char buffer[MAX_LEN]; 
    for (int i = 0; i < lineCount && fgets(buffer, sizeof(buffer), fp) != 0; ++i){
        buffer[strcspn(buffer, "\n")] = 0;
        result[i] = strdup(buffer); 
    }

    fclose(fp);
    return result;
}
void *threadLoop(char *z, int j, char **p, char *_url){
	int numLines = lineCount / atoi(z);
	int factor = j-1;
	char temp[MAX_LEN];
	printf("this is on thread:%d\n", j);
	for(int i=factor*numLines; i<j*numLines; i++){
		strcpy(temp, _url);
		strcat(temp, p[i]);
		temp[strcspn(temp, "\r\n")] = 0; 
		url(temp);
		 
	}
}
struct Arguments { char* z; int j; char** p; char* url; };
void* genericThreadStarter(void* data){
    struct Arguments* d = data;
    threadLoop(d->z, d->j, d->p, d->url);
    return NULL;
}
int main(int argc, char *argv[]) {
if(argc == 1) printf("URL FUZZER \nUse -url to enter your url then follow it with a wordlist\nUse -w to enter a wordlist, following it with the path for the wordlist\nExample: urlfuzz -url www.google.com/ -w rockyou.txt -threads numberofthreads\n PS:DONT FORGET THE / AT THE END OF THE URL");
	else if(argc<5) printf("Missing arguments, make sure you enter the URL and the Wordlist to use.\n");
	else { 

    readLines(argv[4]);
    wordlist(argv[4], lineCount);
    char **result = wordlist(argv[4], lineCount);
    pthread_t thread_ids[atoi(argv[6])];
    struct Arguments args[lineCount];
    for(int j=1; j <= atoi(argv[6]); j++){
	        args[j].z = argv[6];
          	args[j].j = j;
          	args[j].p = result;
         	args[j].url = argv[2]; 
	     	pthread_create(&thread_ids[j],NULL,genericThreadStarter, &args[j]); 
		}
	pthread_exit(NULL);

	 
    	//pthread_exit(NULL);
    for(int i = 0; i < lineCount; ++i) {
        free(result[i]);
    }

    free(result);
	}
}

