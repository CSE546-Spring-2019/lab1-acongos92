#include <stdio.h>
#include <string.h>

int findSearchStringCount(char string[], char search[], int stringSize, int searchSize);

int main(int count, char** args){
    //arg one is test string arg 2 is search string
    printf("COUNT: %d\n", findSearchStringCount(args[1], args[2], strlen(args[1]), 0));
}


int findSearchStringCount(char string[], char searchString[], int stringSize, int searchIndex){
    int count = 0; int endSearch = 0; int searchStringIndex = 0; int finishScan = 0;
    int localIndex = searchIndex;
    char firstChar = searchString[0]; 
    while((localIndex < stringSize) && !endSearch){
        //case where the string at search index matches the first character of the search string
        if(string[localIndex] == firstChar){
            //case where we havent found the first character of the search string yet
            if(searchStringIndex){
                count += findSearchStringCount(string, searchString, stringSize, localIndex);
                //set flag that we just want to scan for remainder of searchString
                finishScan = 1; 
                searchStringIndex++;
            }
            else if (strlen(searchString) == 1){
                count++;
                searchStringIndex = 0;
            }else{
                searchStringIndex++;
            }
            
        }
        //case where we have found the first character, but it isnt repeated
        else if (searchStringIndex){
            if(searchStringIndex < strlen(searchString) && string[localIndex] == searchString[searchStringIndex]){
                searchStringIndex++;
                if (searchStringIndex == strlen(searchString)){
                    count++;
                    searchStringIndex = 0;
                    endSearch = finishScan;
                }
            }else{
                searchStringIndex = 0;
                endSearch = finishScan; 
            }
        }

        localIndex++;
    }
    return count;
}