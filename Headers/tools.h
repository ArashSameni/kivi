#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

//Sleep for seconds
void sleepSec(float seconds){
    usleep(seconds * 1000000);
}

//String to lowercase
char *strlwr(char *str)
{
    unsigned char *p = (unsigned char *)str;

    while (*p) {
        *p = tolower((unsigned char)*p);
        p++;
    }

    return str;
}

//String length
int len(char* inp){
    int count = 0;
    while(inp[count]){
        count++;
    }
    return count;
}

char *removeNL(char *str){ //To remove \n after of strings
    unsigned char *p = (unsigned char *)str;

    while (*p) {
        if(*p != '\n')
            *p = ((unsigned char)*p);
        else
            *p = '\0';
        p++;
    }

    return str;
}

bool checkEmail(char* email){
    char* p = email;
    int count = 0;
    int tedad = 0;//:|
    while(*p){
        if(*p == '@' && count)
            tedad++;
        if(*p == '.' && count && tedad)
            return true;
        count++;
        p++;
    }
    return false;
}

char * separateBy(char* text, char splitChar, int pos){
    int count = 0;
    int start = 0;
    int end = 0;
    char* p = text;
    while(*p){
        if(*p == splitChar)
        {
            count++;
            if(count == pos)
                break;
            else
                start = end+1;
        }
        end++;
        p++;
    }
    if(count < pos-1)
    {
        char *error =  (char *)malloc(sizeof(char) * 3);
        strcpy(error, "-1");
        return error;
    }
    char *answer = malloc (sizeof (char) * len(text));                        // Ta bad inke method tamom shod az bein nare
    int counter = 0;
    for (int i = start; i < end; i++)
    {
        answer[counter] = text[i];
        counter++;
    }
    answer[counter] = '\0';
    return answer;
}

char* encrypt(char* password){
    char* p = password;
    while(*p){
        *p = *p + 5;
        if(*p == ',')
            *p = '.';
        p++;
    }
    return password;
}

int removeLineFromFile(int line,char* filename, FILE* fptr){ // removes a line from file
    rewind(fptr);
    FILE *fileptr2;
    char ch = ' ';
    int temp = 1;

    fileptr2 = fopen("temp_file.csv", "w");
    while (ch != EOF)
    {
        ch = getc(fptr);
        if (ch == '\n')
            temp++;
        if (temp != line && ch != EOF)
            putc(ch, fileptr2);
    }
    fclose(fptr);
    fclose(fileptr2);
    remove(filename);
    rename("temp_file.csv", filename);
    return temp;
}

char* replaceBetweenCommas(char * text, char* newText, int pos){
    int count = 0;
    int start = 0;
    int end = 0;
    char* p = text;
    char *answer = malloc (sizeof (char) * (len(text)+len(newText)));
    while(*p){
        if(*p == ',')
        {
            count++;
            if(count == pos)
                break;
            else
                start = end+1;
        }
        end++;
        p++;
    }
    for (int i = 0; i < start; i++)
        answer[i] = text[i];
    for (int i = start; i < len(newText)+start; i++)
        answer[i] = newText[i-start];

    int last = len(text) - end + start + len(newText);
    if(len(text) - end > 0){
        for (int i = end; i < len(text); i++)
        {
            last = i-end+len(newText)+start;
            answer[last] = text[i];
        }
        answer[++last] = '\0';
    }else
        answer[last] = '\0';
    return answer;
}

bool isOnlyNumbers(char* text){
    char* p = text;
    while(*p){
        if(!(*p >= 48 && *p <= 57))
            return false;
        p++;
    }
    return true;
}