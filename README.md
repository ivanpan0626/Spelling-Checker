Spelling Checker(), CS214 SPR24

Project by: Ivan Pan(ip278), Aysenur Kochan(ak1965)

Implementation of spchk(): Design
**spchk.c**; 
    -2 Macros defined, HASHSIZE and HASHING
        -HASHSIZE is to create the hastable size
        -HASHING is used to find the id in the table for searches later
    -wordNode is a struct created to handle linked list for the dictionary words that will be put into the hash

    -createDictHashTable(), opens the dictionary file path and uses read() with an buffer and a word array to hold each specific word. Repeatedly reads through the file till EOF and calls insertToHash() function everytime a new word is found. This is indicated by the \n found in the file. Also ignores whitespaces.
    -insertToHash(), creates the wordNodes then uses the hashing macro defined above to insert in the correct position in the Hashtable.

    -checkDirectories(), opens the dir, and then checks all contents within the directory, it looks for other directories in there and recursively calls itself to check inside those directories. Then it checks if the files in the directories starts with ., if it does, it is ignored. If not, and it ends with .txt, the function checkFiles() will be called
    -checkFiles(), Opens the file that is inserted in the parameter. Then it loops through the file using read() and buffer with a wordArr to hold words. Also keeps tracks of which line and col the word belongs on using numLine and numCol. Each character from the file is checked to see if its a valid letter and then a string is formed once null terminator is found. The wordArr will be sent to function alignWord() to fix the spacing in the wordArr.

    -alignWord(), this function checks to find the first and last letter in the wordArr. Finds the difference in position to get the length and then moves it to the beginning of the array to fix any alignment issues. Then it is sent off to compareWithDict() function to check if this is a valid word in the provided dictionary
    -compareWithDict(), this function takes the wordArr and then creates 3 different tests. One will check the word in base form (the way it was typed in the file), since its lexicographic order, A and a are not the same in the Hashtable. Second one will capitalize the first letter and third will lowercase the first letter. This is to make sure lowercase and uppercase versions are both checked. Then in each test there will be 4 checks. 1) checks if base form is already a valid word. 2) checks if the first letter in the word is capitalized. 3) checks if all the letters are caps. 4) checks if all the letters are lowercase. This is to make sure that the 3 correct variations of the word will pass the check no mater how its typed. 

**testCases**
Provided testing: ./spchk /usr/share/dict/words ./testCases tester.txt
   -Confirms that the spelling checker can go through all files under a direcotry and checks for .txt files and ignores files starting with ., also confirms it takes more than one file/directory argument.

   ./testCases/.test1.txt and ./testCases/.test2.txt
   -Should ignore both these files, provided incorrect spelling to see if program runs on those files. No errors are reported indicating those files are correctly ignored

   ./testCases/caps.txt
   -Should check all 3 variations of spelling a word, all lowercase, all caps, and first letter is capitalized. Obviously, their base spelling too. Only errors should be reported are the ones with weird capitalization. HoUSE, Mcdonald, McDOnald, and dASh should be the only errors.

   ./testCases/PositionAnnotated.txt
   -Should check for correct position annotation on words. Starts from line 1 and col 1. Correctly iterates the line and col annotation on words. 

   ./testCases/trailingPunc.txt
   -Should check for all trailing punc scenarios named in the p2.pdf, no puncs in the beginning word would affect with the spelling and hyphens are ignored as long as both parts are spelled correctly. Should print no errors out. 

__HOW TO COMPILE__
$ make
executables: ./spchk <insert dictionary> <insert files/directory>...

Provided testing: ./spchk /usr/share/dict/words ./testCases tester.txt
