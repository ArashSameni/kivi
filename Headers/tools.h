#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

//Sleep for seconds
void sleepSec(float seconds)
{
    usleep(seconds * 1000000);
}

//String to lowercase
char *strlwr(char *str)
{
    unsigned char *p = (unsigned char *)str;

    while (*p)
    {
        *p = tolower((unsigned char)*p);
        p++;
    }

    return str;
}

//String length
int len(char *inp)
{
    int count = 0;
    while (inp[count])
    {
        count++;
    }
    return count;
}

bool checkEmail(char *email)
{
    char *p = email;
    int count = 0;
    int tedad = 0; //:|
    while (*p)
    {
        if (*p == '@' && count)
            tedad++;
        if (*p == '.' && count && tedad)
            return true;
        count++;
        p++;
    }
    return false;
}

char *encrypt(char *password)
{
    char *p = password;
    while (*p)
    {
        *p = *p + 5;
        if (*p == ',')
            *p = '.';
        p++;
    }
    return password;
}

bool isOnlyNumbers(char *text)
{
    char *p = text;
    while (*p)
    {
        if (!(*p >= 48 && *p <= 57))
            return false;
        p++;
    }
    return true;
}

int countOfLines(char *text)
{
    int count = 1;
    while (*text)
    {
        if(*text == '\n')
            count++;
        text++;
    }
    return count;
}