#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

void stringPermutation(int j, int k);
void binarySearch(char *word);

char (*dictionary)[19];
char (*matches)[19];
char inputTemp[19];
char inputString[19];
int dictionaryIndex, matchCount = 0;
long long timeval_diff(struct timeval *difference, struct timeval *end_time, struct timeval *start_time){
    struct timeval temp_diff;

    if(difference == NULL) {
        difference=&temp_diff;
    }

    difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
    difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

    /* Using while instead of if below makes the code slightly more robust. */

    while(difference->tv_usec<0) {
        difference->tv_usec+=1000000;
        difference->tv_sec -=1;
    }

    return 1000000LL*difference->tv_sec + difference->tv_usec;
}

int main() {
	FILE *ifp;
	int i = 0;
    char choice[1] = {'Y'};

    struct timeval earlier;
    struct timeval later;
    struct timeval interval;

	ifp = fopen("dictionary.txt", "r");
	fscanf(ifp, "%d", &dictionaryIndex);

	dictionary = (char*)malloc(dictionaryIndex * sizeof(*dictionary));
    matches = (char*)malloc(dictionaryIndex * sizeof(*matches));

	for(i = 0; i < dictionaryIndex; i++) {
		fscanf(ifp, "%s", dictionary[i]);
	}
	for(i = 0; i < dictionaryIndex; i++) {
		printf("%s\n", dictionary[i]);
	}

	fclose(ifp);
    printf("Welcome to the Jumble Puzzle Solver!\n\nWould you like to enter a jumbled word? (y/n)\n");
	scanf("%s", choice);

    while((int)toupper(choice[0]) != 78) {
    	printf("What word would you like scored?\n");
    	scanf("%s", inputString);

    	strcpy(inputTemp, inputString);

    	if(gettimeofday(&earlier,NULL)) {
    		perror("first gettimeofday()");
    		exit(1);
    	}

    	for(i = 0; i <= strlen(inputTemp); i++) {
    		stringPermutation(0, i);
    	}

    	if(gettimeofday(&later,NULL)) {
    		perror("second gettimeofday()");
    		exit(1);
    	}

    	timeval_diff(&interval,&later,&earlier);
    	printf("Process completed in %lld microseconds (%ld seconds, %ld microseconds)\n",
    			timeval_diff(&interval,&later,&earlier),interval.tv_sec, interval.tv_usec);

    	if (matchCount == 0) {
    		printf("Sorry, no permutations of %s form a word.\n", inputString);
    	}

    	printf("Would you like to enter a jumbled word?\n");
    	scanf("%s", toupper(choice));
    	matchCount = 0;
    }

	return 0;
}

void stringPermutation(int j, int k) {
	int i = 0;
	char c, searchTemp[19];

	if(j == k) {
        for(i = 0; i < 19; i++) {
            searchTemp[i] = '\0';
        }
		for(i = 0; i < k; i++) {
			searchTemp[i] = inputTemp[i];
			binarySearch(searchTemp);
		}

	}
	else {
		for(i = j; i < strlen(inputTemp); i++) {
			c = inputTemp[i];
			inputTemp[i] = inputTemp[j];
			inputTemp[j] = c;
			stringPermutation(j + 1, k);
			inputTemp[j] = inputTemp[i];
			inputTemp[i] = c;
		}
	}
}

void binarySearch(char *word) {
	int low = 0, high = dictionaryIndex -1;
    int i;
	while(low <= high) {
		int mid = (low+high)/2;

		if(strcmp(word,dictionary[mid]) < 0) {
			high = mid - 1;
		}
		else if(strcmp(word,dictionary[mid]) > 0) {
			low = mid + 1;
		}
		else {
		    if(strcmp(word,dictionary[mid]) == 0 && matchCount == 0) {
		        strcpy(matches[matchCount],word);
		        matchCount++;
                printf("A permutation of %s that is a valid word is %s\n", inputString, word);
                return;
		    }
		    else if (strcmp(word,dictionary[mid]) == 0 && matchCount != 0) {
		        for(i = 0; i < matchCount; i++) {
                    if(strcmp(word, matches[i]) == 0) {
                        return;
                    }
		        }
		        strcpy(matches[matchCount],word);
		        matchCount++;
                printf("A permutation of %s that is a valid word is %s\n", inputString, word);
                return;
		    }
			return;
		}
	}
	return;
}
