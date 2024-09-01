#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h> // For stat()
#include <fcntl.h>

#define HASHSIZE 127 //Creates a hashtable for all 127 ASCII characters
#define HASHING(word) ((unsigned int)(word)[0] % HASHSIZE) //Creates the ID for the hashtable

//Struct to handle the linked list for words that will be linked to the Hashtable
typedef struct wordNode {
    char* word;
    struct wordNode* next;
} wordNode;

//Creates the Hashtable linked list
wordNode* hashtable[HASHSIZE];

//Function to inserts words found in the Dictionary to the Hashtable in order
void insertToHash(char* wordArr) {
    wordNode* newWord = (wordNode*)malloc(sizeof(wordNode));
    newWord->word = strdup(wordArr);
    newWord->next = NULL; //Above creates a WordNode to insert the new word into Hashtable linkedList

    unsigned int id = HASHING(wordArr); //Gets the id to put in HashTable
    if (hashtable[id] == NULL) {//Checks if that id already has a spot
        newWord->next = hashtable[id];
        hashtable[id] = newWord;
    } 
    else if (strcmp(wordArr, hashtable[id]->word) < 0){//Checks if newWord should be at the front of the list
        newWord->next = hashtable[id];
        hashtable[id] = newWord;
    }
    else {//Iterates through linkedlist with ID to find correct spot to put newWord
        wordNode* currentWord = hashtable[id];
        while (currentWord->next != NULL && strcmp(wordArr, currentWord->next->word) > 0) {
            currentWord = currentWord->next;
        }
        newWord->next = currentWord->next;
        currentWord->next = newWord;
    }
}

//Function to load Dicitionary into Hashtable
void createDictHashTable(char* dictPath) {
    int fd = open(dictPath, O_RDONLY);
    if (fd == -1) {
        printf("Error opening file: %s\n", dictPath);
    }
    else {
        char bufferArr[2048], wordArr[46]; //bufferArr is for read() buffer, wordArr is for holding words, 46 because longest word is 45
        int bytesToRead, indexOfWord = 0;

        while ((bytesToRead = read(fd, bufferArr, sizeof(bufferArr) - 1)) > 0) {
            for (int i = 0; i < bytesToRead; i++) { //Loop through buffer to find individual words
                if (isspace(bufferArr[i]) || bufferArr[i] == '\n') { //Checks if its whitespace or end of line
                    if (!(indexOfWord < 1)) { //Checks if its at the end of an word
                        wordArr[indexOfWord] = '\0'; //Puts Null terminator for end of string
                        indexOfWord = 0;
                        insertToHash(wordArr); //Function to insert new found Word into HashTable
                    }
                } 
                else {
                    wordArr[indexOfWord] = bufferArr[i];
                    indexOfWord++;
                }
            }
        }
        close(fd);
    }
}
void checkFiles(char* filePath);
//Function to iterate through directories
void checkDirectories(char* basePath) {
    struct dirent *rd;
    DIR *direct = opendir(basePath);

    if (direct) {
        while ((rd = readdir(direct)) != NULL) {
            if (rd->d_name[0] == '.') { //Ignores any file that starts with .
                continue;
            }
            //This chunk fixes the new directory name
            struct stat path_stat;
            char path[500];
            int len = strlen(basePath);
            strncpy(path, basePath, len);
            path[len] = '/';
            int oldLen = len+1;
            len = strlen(rd->d_name) + 1;
            strncpy(&path[oldLen], rd->d_name, len);

            //Checks if theres more directory or only .txt files
            stat(path, &path_stat);
            if (S_ISREG(path_stat.st_mode)){
                if (strstr(rd->d_name, ".txt") != NULL) {
                    checkFiles(path);
                }
            }
            else {
                checkDirectories(path);
            }
        }
        closedir(direct);
    }
}

//Compares the words found in .txt files with HashTable contained all words from dicitionary
void compareWithDict(char* wordArr, char* filePath, int numLine, int numCol) {
    char tempArr[46]; 
    strcpy(tempArr, wordArr);
    for (char* p = tempArr; *p; p++) {//lowercase all to find correct hashid
            *p = tolower(*p);
    }

    int index = HASHING(tempArr); //Uses the hashing macro to find ID
    for (wordNode* current = hashtable[index]; current != NULL; current = current->next) {//Repeats till end of List
        strcpy(tempArr, current->word);
        tempArr[0] = toupper(tempArr[0]);
        //temp char array to hold original word but in 3 different spelling formats that are deemed correct
        if (strcmp(current->word, wordArr) == 0) { //Checks base case
            return;
        }
        if (strcmp(wordArr, tempArr) == 0) { //Checks when first letter is uppercase
            return;
        }
        for (char* p = tempArr; *p; p++) {
            *p = toupper(*p);
        }
        if (strcmp(tempArr, wordArr) == 0) { //Checks when all letters are uppercase
            return;
        }
        for (char* p = tempArr; *p; p++) {
            *p = tolower(*p);
        }
        if (strcmp(tempArr, wordArr) == 0) { //Checks when all letters are lowercase
            return;
        }
    }

    strcpy(tempArr, wordArr);
    for (char* p = tempArr; *p; p++) {//uppercase all to find correct hashid
            *p = toupper(*p);
    }
     //Repeats process if word isn't found in lowercase section
    index = HASHING(tempArr); //Uses the hashing macro to find ID
    for (wordNode* current = hashtable[index]; current != NULL; current = current->next) {//Repeats till end of List
        strcpy(tempArr, current->word);
        tempArr[0] = toupper(tempArr[0]);
        //temp char array to hold original word but in 3 different spelling formats that are deemed correct
        if (strcmp(current->word, wordArr) == 0) { //Checks base case
            return;
        }
        if (strcmp(wordArr, tempArr) == 0) { //Checks when first letter is uppercase
            return;
        }
        for (char* p = tempArr; *p; p++) {
            *p = toupper(*p);
        }
        if (strcmp(tempArr, wordArr) == 0) { //Checks when all letters are uppercase
            return;
        }
        for (char* p = tempArr; *p; p++) {
            *p = tolower(*p);
        }
        if (strcmp(tempArr, wordArr) == 0) { //Checks when all letters are lowercase
            return;
        }
    }

    //Repeats process if word isn't found in uppercase and base section
    index = HASHING(wordArr); //Uses the hashing macro to find ID
    for (wordNode* current = hashtable[index]; current != NULL; current = current->next) {//Repeats till end of List
        strcpy(tempArr, current->word);
        tempArr[0] = toupper(tempArr[0]);
        //temp char array to hold original word but in 3 different spelling formats that are deemed correct
        if (strcmp(current->word, wordArr) == 0) { //Checks base case
            return;
        }
        if (strcmp(tempArr, wordArr) == 0) { //Checks when first letter is uppercase
            return;
        }
        for (char* p = tempArr; *p; p++) {
            *p = toupper(*p);
        }
        if (strcmp(tempArr, wordArr) == 0) { //Checks when all letters are uppercase
            return;
        }
        for (char* p = tempArr; *p; p++) {
            *p = tolower(*p);
        }
        if (strcmp(tempArr, wordArr) == 0) { //Checks when all letters are lowercase
            return;
        }
    }

    printf("%s (%d,%d): %s\n", filePath, numLine, numCol, wordArr);
}

//Function to move align words to start of array
void alignWord(char* filePath, char* wordArr, int NumLine, int NumCol) {
    while (*wordArr && !((*wordArr >= 'A' && *wordArr <= 'Z') || (*wordArr >= 'a' && *wordArr <= 'z'))) {
        wordArr++; 
    } //Finds the first letter

    char* endofWordArr = wordArr + strlen(wordArr) - 1;
    while (endofWordArr > wordArr && !((* endofWordArr >= 'A' && *endofWordArr <= 'Z') || (*endofWordArr >= 'a' && *endofWordArr <= 'z'))){
        endofWordArr--;
    } //Finds last letter

    int length = strlen(endofWordArr) + strlen(wordArr) - 1; //Finds the length to displace by
    memmove(wordArr, wordArr, length);
    wordArr[length] = '\0';
    compareWithDict(wordArr, filePath, NumLine, NumCol);
}

//Function to process files and check content with dictionary
void checkFiles(char* filePath) {
    int fd = open(filePath, O_RDONLY);
    if (fd == -1) {
        printf("Error opening file: %s\n", filePath);
    }
    else {
        char bufferArr[2048], wordArr[46]; //bufferArr is for read() buffer, wordArr is for holding words, 46 because longest word is 45
        int bytesToRead, indexOfWord = 0, numLine = 1, numCol = 1;
        char character;

        while ((bytesToRead = read(fd, bufferArr, sizeof(bufferArr) - 1)) > 0) {
            for (int i = 0; i < bytesToRead; i++) { //Loop through buffer to find individual words
                character = bufferArr[i];
                if (!((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z') || character == '\'')) {
                    if (indexOfWord > 0) { //End of word and sends to alignWord function to check with dict
                        wordArr[indexOfWord] = '\0';
                        alignWord(filePath, wordArr, numLine, numCol - indexOfWord);
                        indexOfWord = 0;
                    }
                    if (character == '\n') {
                        numCol = 0;
                        numLine++;
                    }
                } 
                else {
                    if (indexOfWord < (sizeof(wordArr) - 1)) {//Adds char to wordArr to form word
                        wordArr[indexOfWord++] = character;
                    }
                }
                numCol++;
            }   
        }
        if (indexOfWord > 0) { //For EOF last word
            wordArr[indexOfWord] = '\0';
            alignWord(filePath, wordArr, numLine, numCol - indexOfWord);
        }
        close(fd);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("How to use: %s <dictionary directory/file> <txt file/directory>...\n", argv[0]);
        return 1;
    }
    struct stat path_stat;
    int setUp = 0;
    
    for (int i = 2; i < argc; i++) {
        stat(argv[i], &path_stat);
        if (S_ISREG(path_stat.st_mode)){//Check if its file, then uses CheckFile function if not then checkDirectory function
            if (setUp == 0){//Sets up dictionary if file is found
                memset(hashtable, 0, sizeof(hashtable));
                createDictHashTable(argv[1]);
                setUp = 1;
            }
            checkFiles(argv[i]);
        } else if (S_ISDIR(path_stat.st_mode)) {
            if (setUp == 0){
                memset(hashtable, 0, sizeof(hashtable));
                createDictHashTable(argv[1]);
                setUp = 1;
            }
            checkDirectories(argv[i]);
        }
        else{
            printf("Error opening file: %s\n", argv[i]);
        }
    }
    return 0;
}