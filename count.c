#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 80

//Returns true if we have valid arguments
int checkArgs(int argc, char** args);

//returns true if both in and outfile are opened successfully 
int openFiles(FILE** inFile, FILE** outFile, char* inName, char* outName);

//counts the size of a file* in bytes returns the file pointer to the start of the file when done
unsigned countFileSize(FILE* file);

//writes the file size output line to file*, returns -1 on error writing 
int writeFileSizeLine(FILE* outfile, unsigned byteCount);

//writes the word count output line to file*, returns -1 on error writing
int writeSearchStringCountLine(FILE* outfile, int count);

//recursively searches a character array for a search string, returning the count 
int findSearchStringInByteArray(char string[], char searchString[], int stringSize, int searchIndex);

//returns count of searchString occurrence in inFile
int getSearchStringCount(FILE* inFile, char searchString[]);

void printArray(char arr[], int arrSize, int start, int end);
int main(int argc, char** args){
    FILE* inFile;
    FILE* outFile;
    if(checkArgs(argc, args) && openFiles(&inFile, &outFile, args[1], args[3])){
        unsigned fileSize = countFileSize(inFile);
        if(writeFileSizeLine(outFile, fileSize) < 0){
            printf ("ERROR: writing to output file failed\n");
            exit(1);
        }
        int occurrence = getSearchStringCount(inFile, args[2]);
        if (writeSearchStringCountLine(outFile, occurrence) < 0){
            printf("ERROR: writing to output file failed\n");
            exit(1);
        }
        fclose(inFile);
        fclose(outFile);
    }else{
        printf("Input should be 'count <filename> <search string> <output name>\n");
    }
    return 0;
}

int openFiles(FILE** inFile, FILE** outFile, char* inName, char* outName){
    *inFile = fopen(inName, "r");
    *outFile = fopen(outName, "w+");
    if(*inFile == NULL || *outFile == NULL){
        printf("Error Opening input or output file\n");
        fflush(stdout);
    }
    return *inFile != NULL && *outFile != NULL;
}
int checkArgs(int argc, char** args){
    return argc == 4;
}

unsigned countFileSize(FILE* file){
    char buff[1]; 
    unsigned size = 0;
    while(fread(buff, 1, 1, file)){
        size++;
    }
    rewind(file);
    return size;
}

int writeFileSizeLine(FILE* outfile, unsigned byteCount){
    printf("Size of file is %u\n", byteCount);
    int writeCode = fprintf(outfile, "Size of file is %u\n", byteCount);
    fflush(outfile);
    return writeCode;
}

int writeSearchStringCountLine(FILE* outfile, int count){
    printf("Number of matches = %d\n", count);
    int writeCode = fprintf(outfile, "Number of matches = %d\n", count);
    fflush(outfile);
    return writeCode;
}

int findSearchStringInByteArray(char string[], char searchString[], int stringSize, int searchIndex){
    int count = 0; int endSearch = 0; int searchStringIndex = 0; int finishScan = 0;
    int localIndex = searchIndex;
    char firstChar = searchString[0]; 
    while((localIndex < stringSize) && !endSearch){
        //case where the string at search index matches the first character of the search string
        if(string[localIndex] == firstChar){
            //case where we havent found the first character of the search string yet
            if(searchStringIndex){
                count += findSearchStringInByteArray(string, searchString, stringSize, localIndex);
                //set flag that we just want to scan for remainder of searchString
                finishScan = 1;
                if(string[localIndex] == searchString[searchStringIndex]){
                    searchStringIndex++;
                }else{
                    endSearch = 1;
                } 

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

int getSearchStringCount(FILE* inFile, char searchString[]){
    int count = 0;
    long bytesRead = 0;
    unsigned itsRewindTime = strlen(searchString) - 1;
    char buffer[BUFFER_SIZE];
    size_t readChars = fread(buffer, 1, BUFFER_SIZE, inFile);
    bytesRead += readChars;
    while(readChars){
        count += findSearchStringInByteArray(buffer, searchString, readChars, 0);
        if(readChars == BUFFER_SIZE){
            bytesRead -= itsRewindTime;
            fseek(inFile, bytesRead, 0);
        }
        readChars = fread(buffer, 1, BUFFER_SIZE, inFile);
        bytesRead += readChars;

    }
    return count;
    
}
