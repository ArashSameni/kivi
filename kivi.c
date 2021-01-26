/*
*
*   Kivi ==> (Ki)ll (Vi)rus
*   By Arash Sameni
*
*/

#include <mysql/mysql.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "Headers/asciis.h"
#include "Headers/tools.h"
#include "Headers/conio.h"

#define PasswordSize 30
#define EmailSize 70
#define UsernameSize 50

void showLogo();
void showMainMenu();
void createAccount();
void loginToAccount();
void resetPasswordMenu();
char *getInputWithSpace(int size, char *name);
char *getNatCode();
char *getUsername();
char *getPassword();
char *getEmail();
char *getBloodGroup();
void createFiles();
char *editNormalProfile(int *lineNum, char *dataOfUser);
char *editBusProfile(int *lineNum, char *dataOfUser);
int getPositiveInt(char *name, bool optional);
void loggedIn(int lineNum, char *dataOfUser);
int coronaTest();
int ask(char *question);
bool natCodeExists(char *natCode);
bool usernameExists(char *username);
int getcCodeByNat(char *natCode); // find Corona code by national code
void changecCodeByNat(char *natCode, int cCode);
int getcCodeByAreaId(int areaId);
void changecCodeByAreaId(int areaId, int cCode);
void sql_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

typedef struct normalInfo{
    char bloodGroup[4];
    int age;
    int height;
    int weight;
} normalInfo;

typedef struct busInfo{
    char placeName[101];
    char cityName[101];
    int areaId;
} busInfo;

typedef struct user{
    int id;
    int type;
    char name[101];
    char lastName[101];
    char nationalCode[11];
    char username[51];
    char password[51];
    char email[101];
    int coronaCode;
    union{
        normalInfo normalinfo;
        busInfo businfo;
    };
} user;

int main()
{
    createFiles();
    printf(CLEAR);
    showLogo();
    showMainMenu();
    return 0;
}

void showLogo()
{
    float sleepTime = 0.03;
    for (int i = 0; i < 19; i++)
    {
        if (i == 0)
            printf("%c", '\n');
        else if (i == 1)
            puts(FGREEN "\t            kkkkkkkk              iiii                              iiii");
        else if (i == 2)
            puts("\t            k::::::k             i::::i                            i::::i");
        else if (i == 3)
            puts("\t            k::::::k              iiii                              iiii");
        else if (i == 4)
            puts("\t            k::::::k");
        else if (i == 5)
            puts("\t             k:::::k    kkkkkkk iiiiiii vvvvvvv           vvvvvvv iiiiiii");
        else if (i == 6)
            puts("\t             k:::::k   k:::::k  i:::::i  v:::::v         v:::::v  i:::::i");
        else if (i == 7)
            puts("\t             k:::::k  k:::::k    i::::i   v:::::v       v:::::v    i::::i");
        else if (i == 8)
            puts("\t             k:::::k k:::::k     i::::i    v:::::v     v:::::v     i::::i");
        else if (i == 9)
            puts("\t             k::::::k:::::k      i::::i     v:::::v   v:::::v      i::::i");
        else if (i == 10)
            puts("\t             k:::::::::::k       i::::i      v:::::v v:::::v       i::::i");
        else if (i == 11)
            puts("\t             k:::::::::::k       i::::i       v:::::v:::::v        i::::i");
        else if (i == 12)
            puts("\t             k::::::k:::::k      i::::i        v:::::::::v         i::::i");
        else if (i == 13)
            puts("\t            k::::::k k:::::k    i::::::i        v:::::::v         i::::::i");
        else if (i == 14)
            puts("\t            k::::::k  k:::::k   i::::::i         v:::::v          i::::::i");
        else if (i == 15)
            puts("\t            k::::::k   k:::::k  i::::::i          v:::v           i::::::i");
        else if (i == 16)
            puts("\t            kkkkkkkk    kkkkkkk iiiiiiii           vvv            iiiiiiii" RESET);
        else if (i == 17)
            puts(FCYAN BOLD "By Arash Sameni" RESET);
        else if (i == 18)
            puts(FCYAN "==> any where you can use -1 to go back\n" RESET);
        sleepSec(sleepTime);
        fflush(stdout);
    }
}

void showMainMenu()
{
    float sleepTime = 0.07;
    for (int i = 0; i < 4; i++)
    {
        if (i == 0)
            puts(FGREEN "1.Login to account");
        else if (i == 1)
            puts("2.Create a new account");
        else if (i == 2)
            puts("3.Reset password\n" RESET);
        else if (i == 3)
            printf(FWHITE "Input: ");

        sleepSec(sleepTime);
        fflush(stdout);
    }
    int Choosed = 0;
    while (!Choosed)
    {
        char input[25];
        fgets(input, 25, stdin); // To Get Spaces between characters too
        if (!strcmp(input, "1\n") || !strcmp(strlwr(input), "login to account\n"))
        {
            Choosed = 1;
            loginToAccount();
        }
        else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "create a new account\n"))
        {
            Choosed = 2;
            createAccount();
        }
        else if (!strcmp(input, "3\n") || !strcmp(strlwr(input), "reset password\n"))
        {
            Choosed = 3;
            resetPasswordMenu();
        }
        else if (!strcmp(input, "-1\n"))
            exit(0);
        else
        {
            Choosed = 0;
            printf("%s", RESET FRED "Wrong input!\n" FWHITE);
        }
        printf("%c", '\n');
        if (!Choosed)
            printf(FWHITE "Input: ");
    }
}

void createAccount()
{
    printf(CLEAR);
    int Choosed = 0;
    while (!Choosed)
    {
        printf("%s", RESET FWHITE "Which type of account do you want? (1.Normal account / 2.Business account): ");
        char input[20];
        fgets(input, 25, stdin);
        if (!strcmp(input, "1\n") || !strcmp(strlwr(input), "normal account\n"))
            Choosed = 1;
        else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "business account\n"))
            Choosed = 2;
        else if (!strcmp(input, "-1\n"))
        {
            printf(CLEAR);
            showLogo();
            showMainMenu();
        }
        else
        {
            Choosed = 0;
            printf("%s", RESET FRED "Wrong input!\n" FWHITE);
        }
    }
    bool goBack = false;

    while (!goBack)
    {
        if (Choosed == 1)
        {
            char *name, *family, *natCode, *username, *password, *email, *bloodGroup;
            int age, height, weight;
            age = height = weight = -2;

            name = getInputWithSpace(40, "Name");
            if (strcmp(name, "-1\n") == 0)
            {
                goBack = true;
                break;
            }

            family = getInputWithSpace(40, "Family");
            if (strcmp(family, "-1\n") == 0)
            {
                goBack = true;
                break;
            }

            natCode = getNatCode();
            if (strcmp(natCode, "-1") == 0)
            {
                goBack = true;
                break;
            }
            while (natCodeExists(natCode))
            {
                puts(RESET FRED "National Code Exists!" RESET FGREEN);
                natCode = getNatCode();
                if (strcmp(natCode, "-1") == 0)
                {
                    goBack = true;
                    break;
                }
            }

            username = getUsername();
            if (strcmp(username, "-1") == 0)
            {
                goBack = true;
                break;
            }
            while (usernameExists(username))
            {
                puts(RESET FRED "Username Exists!" RESET FGREEN);
                username = getUsername();
                if (strcmp(username, "-1") == 0)
                {
                    goBack = true;
                    break;
                }
            }

            password = getPassword();
            if (strcmp(password, "-1") == 0)
            {
                goBack = true;
                break;
            }

            char repeatOfPass[30];
            while (strcmp(repeatOfPass, password))
            {
                int i;
                char ch1;
                printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                for (i = 0; i < 30; i++)
                {
                    ch1 = getch();
                    if (ch1 != 10)
                    {
                        if (ch1 != 127)
                        {
                            printf("*");
                            repeatOfPass[i] = ch1;
                        }
                        else if (i > 0)
                        {
                            printf("\b \b");
                            i -= 2;
                        }
                        else
                            i = -1; // To not remove other strings!
                    }
                    else
                        break;
                }
                repeatOfPass[i] = '\0';
                if (strcmp(repeatOfPass, password))
                    puts(RESET FRED "\nThe password and repeat of password dont match!" RESET FGREEN);
            }
            printf("%c", '\n');

            email = getEmail();
            if (strcmp(email, "-1") == 0)
            {
                goBack = true;
                break;
            }

            bloodGroup = getBloodGroup();
            if (strcmp(bloodGroup, "-1") == 0)
            {
                goBack = true;
                break;
            }

            age = getPositiveInt("Age", 1);
            if (age == -1)
            {
                getchar();
                goBack = true;
                break;
            }

            height = getPositiveInt("Height", 1);
            if (height == -1)
            {
                getchar();
                goBack = true;
                break;
            }

            weight = getPositiveInt("Weight", 1);
            if (weight == -1)
            {
                getchar();
                goBack = true;
                break;
            }

            bool fileExist = false;
            FILE *fptr = fopen("users.csv", "a");
            //type, name,family, natCode, username, password,email, Corona Code, bloodGroup, age, height, weight
            fprintf(fptr, "\n%d,%s,%s,%s,%s,%s,%s,%d,%s,%d,%d,%d", 1, removeNL(name), removeNL(family), natCode, strlwr(username), encrypt(password), strlwr(email), 1, bloodGroup, age, height, weight);
            fclose(fptr);
        }
        else if (Choosed == 2)
        {
            char *name, *family, *natCode, *username, *password, *email, *placeName, *cityName;
            int mantaghe = -2;

            name = getInputWithSpace(40, "Name");
            if (strcmp(name, "-1\n") == 0)
            {
                goBack = true;
                break;
            }

            family = getInputWithSpace(40, "Family");
            if (strcmp(family, "-1\n") == 0)
            {
                goBack = true;
                break;
            }

            natCode = getNatCode();
            if (strcmp(natCode, "-1") == 0)
            {
                goBack = true;
                break;
            }

            username = getUsername();
            if (strcmp(username, "-1") == 0)
            {
                goBack = true;
                break;
            }

            password = getPassword();
            if (strcmp(password, "-1") == 0)
            {
                goBack = true;
                break;
            }

            char repeatOfPass[30];
            while (strcmp(repeatOfPass, password))
            {
                int i;
                char ch1;
                printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                for (i = 0; i < 30; i++)
                {
                    ch1 = getch();
                    if (ch1 != 10)
                    {
                        if (ch1 != 127)
                        {
                            printf("*");
                            repeatOfPass[i] = ch1;
                        }
                        else if (i > 0)
                        {
                            printf("\b \b");
                            i -= 2;
                        }
                        else
                            i = -1; // To not remove other strings!
                    }
                    else
                        break;
                }
                repeatOfPass[i] = '\0';
                if (strcmp(repeatOfPass, password))
                    puts(RESET FRED "\nThe password and repeat of password dont match!" RESET FGREEN);
            }
            printf("%c", '\n');

            email = getEmail();
            if (strcmp(email, "-1") == 0)
            {
                goBack = true;
                break;
            }

            placeName = getInputWithSpace(80, "Place name");
            if (strcmp(placeName, "-1\n") == 0)
            {
                goBack = true;
                break;
            }

            cityName = getInputWithSpace(50, "City name");
            if (strcmp(cityName, "-1\n") == 0)
            {
                goBack = true;
                break;
            }

            mantaghe = getPositiveInt("Area", 0);
            if (mantaghe == -1)
            {
                goBack = true;
                break;
            }

            bool fileExist = false;
            FILE *fptr = fopen("users.csv", "a");
            fileExist = true;
            //1 for white. 2 for red
            // type, name, family, natCode, username, password, email, CoronaCode, placeName, cityName, mantaghe
            fprintf(fptr, "\n%d,%s,%s,%s,%s,%s,%s,%d,%s,%s,%d", 2, removeNL(name), removeNL(family), natCode, strlwr(username), encrypt(password), strlwr(email), 1, removeNL(placeName), removeNL(cityName), mantaghe);
            fclose(fptr);
        }
        break;
    }
    if (goBack)
    {
        printf(CLEAR);
        createAccount();
    }
    else
    {
        getchar();
        puts(RESET FGREEN BOLD "Your account has been created successfully!" RESET FWHITE);
        printf(RESET FWHITE "press any key to go back");
        getchar();
        printf(CLEAR);
        showLogo();
        showMainMenu();
    }
}

void loginToAccount()
{
    bool goBack = false;

    while (!goBack)
    {
        char *password;
        char *username;
        printf(CLEAR);

        username = getUsername();
        if (strcmp(username, "-1") == 0)
        {
            goBack = true;
            break;
        }

        password = getPassword();
        if (strcmp(password, "-1") == 0)
        {
            goBack = true;
            break;
        }
        printf("%c", '\n');
        if (access("users.csv", F_OK) == 0)
        { // If file exists
            char *line = NULL;
            size_t len = 0;
            ssize_t read;
            bool userExists = false;
            bool passCorrect = false;
            encrypt(password);
            FILE *fp = fopen("users.csv", "r");
            int lineNumber = 0;
            if (strcmp(username, "admin") == 0)
            {
                userExists = true;
                while ((read = getline(&line, &len, fp)) != -1)
                { // read = tool string
                    lineNumber++;
                    char *type = separateBy(line, ',', 1);
                    if (strcmp("0", type) == 0)
                    {
                        char *passSep = separateBy(line, ',', 3);
                        if (strcmp(password, removeNL(passSep)) == 0)
                        {
                            fclose(fp);
                            loggedIn(lineNumber, line);
                            passCorrect = true;
                        }
                        free(passSep);
                        free(type);
                        break;
                    }
                    free(type);
                }
            }
            else
            {
                while ((read = getline(&line, &len, fp)) != -1)
                { // read = tool string
                    lineNumber++;
                    char *userSep = separateBy(line, ',', 5);
                    if (strcmp(strlwr(username), userSep) == 0)
                    {
                        userExists = true;
                        char *passSep = separateBy(line, ',', 6);
                        if (strcmp(password, passSep) == 0)
                        {
                            fclose(fp);
                            loggedIn(lineNumber, line);
                            passCorrect = true;
                        }
                        free(passSep);
                        free(userSep);
                        break;
                    }
                    free(userSep);
                }
            }

            if (userExists && !passCorrect)
                puts(RESET FRED "Password is incorrect!" RESET FGREEN);
            else if (!userExists)
                puts(RESET FRED "Username not found!" RESET FGREEN);

            fclose(fp);
            if (line)
                free(line);
        }
        else
        { // If file does'nt exists
            puts(RESET FRED "\nNo users exists!" RESET FWHITE);
            printf("press any key to go back");
            getchar();
            printf(CLEAR);
            showLogo();
            showMainMenu();
            break;
        }
        getchar();
    }
    if (!goBack)
    {
        printf("press any key to go back");
        getchar();
    }
    printf(CLEAR);
    showLogo();
    showMainMenu();
}

void resetPasswordMenu()
{
    bool goBack = false;

    while (!goBack)
    {
        char *username, *email, *natCode, *password;
        printf(CLEAR);

        username = getUsername();
        if (strcmp(username, "-1") == 0)
        {
            goBack = true;
            break;
        }

        email = getEmail();
        if (strcmp(email, "-1") == 0)
        {
            goBack = true;
            break;
        }

        natCode = getNatCode();
        if (strcmp(natCode, "-1") == 0)
        {
            goBack = true;
            break;
        }

        if (access("users.csv", F_OK) == 0)
        { // If file exists
            FILE *fp;
            char *line = NULL;
            size_t leng = 0;
            ssize_t read;
            bool userExists = false;
            bool isEmailCorrect = false;
            bool isNatCorrect = false;
            bool fileClosed = false;

            fp = fopen("users.csv", "r");
            if (fp == NULL)
                exit(EXIT_FAILURE);

            int lineNumber = 0;
            while ((read = getline(&line, &leng, fp)) != -1)
            { // read = tool string
                lineNumber++;
                char *userSep = separateBy(line, ',', 5);
                if (strcmp(strlwr(username), userSep) == 0)
                {
                    userExists = true;
                    char *emailSep = separateBy(line, ',', 7);
                    char *natSep = separateBy(line, ',', 4);
                    if (strcmp(strlwr(email), emailSep) == 0)
                        isEmailCorrect = true;
                    if (strcmp(natCode, natSep) == 0)
                        isNatCorrect = true;
                    free(emailSep);
                    free(natSep);
                    free(userSep);
                    break;
                }
                free(userSep);
            }

            if (userExists && isEmailCorrect && isNatCorrect)
            {
                printf(RESET FGREEN "New "); //To print new password
                password = getPassword();
                if (strcmp(password, "-1") == 0)
                {
                    goBack = true;
                    break;
                }

                char repeatOfPass[30];
                while (strcmp(repeatOfPass, password))
                {
                    int i;
                    char ch1;
                    printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                    for (i = 0; i < 30; i++)
                    {
                        ch1 = getch();
                        if (ch1 != 10)
                        {
                            if (ch1 != 127)
                            {
                                printf("*");
                                repeatOfPass[i] = ch1;
                            }
                            else if (i > 0)
                            {
                                printf("\b \b");
                                i -= 2;
                            }
                            else
                                i = -1; // To not remove other strings!
                        }
                        else
                            break;
                    }
                    repeatOfPass[i] = '\0';
                    if (strcmp(repeatOfPass, password))
                        puts(RESET FRED "\nThe password and repeat of password dont match!" RESET FGREEN);
                }
                printf("%c", '\n');

                removeLineFromFile(lineNumber, "users.csv", fp);
                fileClosed = true;
                fp = fopen("users.csv", "a");
                if (fp == NULL)
                    exit(EXIT_FAILURE);
                fprintf(fp, "\n");
                char *repCommas = replaceBetweenCommas(removeNL(line), encrypt(password), 6);
                fprintf(fp, repCommas);
                free(repCommas);
                fclose(fp);
                puts(RESET FGREEN BOLD "Password has been changed successfully!" RESET FWHITE);
            }
            else if (userExists)
            {
                if (!isEmailCorrect)
                    puts(RESET FRED "Email is incorrect!" RESET FGREEN);
                if (!isNatCorrect)
                    puts(RESET FRED "National Code is incorrect!" RESET FGREEN);
            }
            else
                puts(RESET FRED "Username is incorrect!" RESET FGREEN);

            if (!fileClosed)
                fclose(fp);
            if (line)
                free(line);
        }
        else
        { // If file does'nt exists
            puts(RESET FRED "\nNo users exists!" RESET FWHITE);
            printf("press any key to go back");
            getchar();
            printf(CLEAR);
            showLogo();
            showMainMenu();
        }
        break;
    }
    if (!goBack)
    {
        printf("press any key to go back");
        getchar();
    }
    printf(CLEAR);
    showLogo();
    showMainMenu();
}

char *getInputWithSpace(int size, char *name)
{
    char *input = malloc(sizeof(char) * (size + 1));
    *input = '\0';
    while ((len(input) < 4 || len(input) > size) && strcmp(input, "-1\n") != 0)
    { // 4 because of \n at the last of string
        printf(RESET FGREEN);
        printf("%s", name);
        printf(": " RESET FWHITE);
        fgets(input, size, stdin);
        if (len(input) == 1)
        {
            printf(RESET FRED "Please enter your ");
            printf("%s", name);
            printf("!\n");
        }
        else if (len(input) < 4 && strcmp(input, "-1\n") != 0)
        {
            printf(RESET FRED "Your ");
            printf("%s", name);
            printf(" should be more than 3 letters!\n");
        }
        else if (len(input) > size)
        {
            printf(RESET FRED "Your ");
            printf("%s", name);
            printf(" should be less than %d letters!\n", size);
        }
    }
    return input;
}

char *getNatCode()
{
    char *natCode = malloc(sizeof(char) * 12);
    *natCode = '\0';
    bool isNumber = false;
    while (((len(natCode) != 10) || !isNumber) && strcmp(natCode, "-1") != 0)
    {
        printf(RESET FGREEN "National code: " RESET FWHITE);
        fgets(natCode, 12, stdin);
        if (strcmp(natCode, "\n") == 0)
            puts(RESET FRED "Please enter your national code!" RESET FGREEN);
        else
        {
            natCode[strlen(natCode) - 1] = '\0';
            isNumber = isOnlyNumbers(natCode);
            if ((len(natCode) != 10 || !isNumber) && strcmp(natCode, "-1") != 0)
                puts(RESET FRED "Your national code is incorrect!" RESET FGREEN);
        }
    }
    return natCode;
}

char *getUsername()
{
    char *username = malloc(sizeof(char) * (UsernameSize + 2));
    *username = '\0';
    while ((len(username) < 3 || len(username) > UsernameSize) && strcmp(username, "-1") != 0)
    {
        printf(RESET FGREEN "Username: " RESET FWHITE);
        fgets(username, UsernameSize + 2, stdin);
        if (strcmp(username, "\n") == 0)
            puts(RESET FRED "Please enter your username!" RESET FGREEN);
        else
        {
            username[strlen(username) - 1] = '\0';
            if (len(username) < 3 && strcmp(username, "-1") != 0)
                puts(RESET FRED "The username should be more than 3 letters!" RESET FGREEN);
            else if (len(username) > UsernameSize)
                puts(RESET FRED "The username should be less than 50 letters!" RESET FGREEN);
        }
    }
    return username;
}

char *getPassword()
{
    char *password = malloc(sizeof(char) * (PasswordSize + 1));
    *password = '\0';
    while ((len(password) < 6 || len(password) > PasswordSize) && strcmp(password, "-1") != 0)
    {
        int i;
        char ch1;
        printf(RESET FGREEN "Password: " RESET FWHITE);
        for (i = 0; i < PasswordSize; i++)
        {
            ch1 = getch();
            if (ch1 != 10)
                if (ch1 != 127)
                {
                    printf("*");
                    password[i] = ch1;
                }
                else if (i > 0)
                {
                    printf("\b \b");
                    i -= 2;
                }
                else
                    i = -1;
            else
                break;
        }
        password[i] = '\0';
        if (len(password) < 6 && strcmp(password, "-1") != 0)
            puts(RESET FRED "\nThe password should be more than 6 letters!" RESET FGREEN);
    }
    return password;
}

char *getEmail()
{
    char *email = malloc(sizeof(char) * (EmailSize + 2));
    *email = '\0';
    while ((len(email) < 3 || len(email) > EmailSize || !checkEmail(email)) && strcmp(email, "-1") != 0)
    {
        printf(RESET FGREEN "Email: " RESET FWHITE);
        fgets(email, EmailSize + 2, stdin);
        if (strcmp(email, "\n") == 0)
            puts(RESET FRED "Please enter your email!" RESET FGREEN);
        else
        {
            email[strlen(email) - 1] = '\0';
            if (len(email) < 3 && strcmp(email, "-1") != 0)
                puts(RESET FRED "Your email should be more than 3 letters!" RESET FGREEN);
            else if (len(email) > EmailSize)
                puts(RESET FRED "Your email should be less than 70 letters!" RESET FGREEN);
            else if (!checkEmail(email))
                puts(RESET FRED "Your email is incorrect!" RESET FGREEN);
        }
    }
    return email;
}

char *getBloodGroup()
{
    char *bloodGroup = malloc(sizeof(char) * 5);
    *bloodGroup = '\0';
    do
    {

        printf(RESET FGREEN "Blood Group" RESET FCYAN "(Enter to skip)" RESET FGREEN ": " RESET FWHITE);
        fgets(bloodGroup, 5, stdin);
        bloodGroup[strlen(bloodGroup) - 1] = '\0';
        if (len(bloodGroup) > 3)
            puts(RESET FRED "Your blood group should be less than 3 letters!" RESET FGREEN);

    } while (len(bloodGroup) > 3);

    return bloodGroup;
}

int getPositiveInt(char *name, bool optional)
{
    int inp = -2;
    while (inp < 0 && inp != -1)
    {
        printf(RESET FGREEN);
        printf("%s", name);
        if (optional)
            printf(RESET FCYAN "(0 to skip)" RESET FGREEN);
        printf(": " RESET FWHITE);
        scanf("%d", &inp);
        if (inp < 0 && inp != -1)
        {
            if (inp == -2)
            { //Yani horoof zade
                printf(RESET FRED "Your ");
                printf("%s", name);
                printf(" only have numbers!\n" RESET FGREEN);
                int c;
                while ((c = getchar()) != '\n')
                {
                }
            }
            else
            {
                printf(RESET FRED "Your ");
                printf("%s", name);
                printf(" should be grater than 0!\n" RESET FGREEN);
            }
        }
    }
    return inp;
}

void loggedIn(int lineNum, char *dataOfUser)
{
    printf(CLEAR);
    removeNL(dataOfUser);
    if (*dataOfUser == '1')
    {
        char *name = separateBy(dataOfUser, ',', 2);         //free kardam
        char *nationalCode = separateBy(dataOfUser, ',', 4); //free kardam
        char *cCodeSep = separateBy(dataOfUser, ',', 8);
        int coronaCode = atoi(cCodeSep);
        free(cCodeSep);

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        int Choosed = 0;
        while (!Choosed || Choosed == -1)
        {
            if (Choosed == 0)
            {
                printf(CLEAR);
                printf(RESET FCYAN BOLD "\t\t\t   Welcome %s\n", name);

                //Show time
                tm = *localtime(&t);
                printf(RESET FWHITE "\t\t\t%d-%02d-%02d %02d:%02d:%02d\n\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

                if (coronaCode == 1)
                    puts(RESET FWHITE BOLD "Corona Code: 1.\tYou are healthy!\n");
                else if (coronaCode == 2)
                    puts(RESET FYELLOW BOLD "Corona Code: 2.\tYou are suspicious!\n");
                else if (coronaCode == 3)
                    puts(RESET FORANGE BOLD "Corona Code: 3.\tYou are dangerous!\n");
                else if (coronaCode == 4)
                    puts(RESET FRED BOLD "Corona Code: 4.\tYou are ill!\n");
                puts(RESET FCYAN "1.Corona test");
                puts("2.Visit person");
                puts("3.Visit area");
                puts("4.Tests history");
                puts("5.Visits history");
                puts("6.Edit profile");
                puts("7.Lock account");
                puts("8.Exit app\n" RESET);
                printf(FWHITE "Input: ");
            }

            char input[25];
            fgets(input, 25, stdin); // To Get Spaces between characters too
            if (!strcmp(input, "1\n") || !strcmp(strlwr(input), "corona test\n"))
            {
                int coronaPercent = coronaTest();
                if (coronaPercent != -1)
                {
                    printf(CLEAR);
                    char cCode[2] = "1";
                    switch (coronaPercent)
                    {
                    case 0 ... 15:
                        coronaCode = 1;
                        cCode[0] = '1';
                        break;
                    case 16 ... 50:
                        coronaCode = 2;
                        cCode[0] = '2';
                        break;
                    case 51 ... 75:
                        coronaCode = 3;
                        cCode[0] = '3';
                        break;
                    case 76 ... 100:
                        coronaCode = 4;
                        cCode[0] = '4';
                        break;
                    }

                    FILE *fp = fopen("users.csv", "r");
                    if (fp == NULL)
                        exit(EXIT_FAILURE);
                    lineNum = removeLineFromFile(lineNum, "users.csv", fp);
                    fp = fopen("users.csv", "a");
                    if (fp == NULL)
                        exit(EXIT_FAILURE);
                    fprintf(fp, "\n");
                    dataOfUser = replaceBetweenCommas(dataOfUser, cCode, 8);
                    fprintf(fp, dataOfUser);
                    fclose(fp);

                    if (coronaCode == 3 || coronaCode == 4)
                    {
                        FILE *fp = fopen("pvisits.csv", "r");
                        char *line = NULL;
                        size_t len = 0;
                        ssize_t read;
                        while ((read = getline(&line, &len, fp)) != -1)
                        {
                            char *natCode1 = separateBy(line, ',', 1);
                            char *natCode2 = separateBy(line, ',', 2);
                            if (strcmp(nationalCode, natCode1) == 0)
                            {
                                char *date = removeNL(separateBy(line, ',', 3));
                                struct tm tm;
                                char temp[30] = {};
                                strcat(temp, date);
                                strcat(temp, " 00:00:00");
                                strptime(temp, "%Y-%m-%d %H:%M:%S", &tm);
                                time_t t = mktime(&tm);
                                int daysPassed = (time(NULL) - t) / 86400;
                                if (daysPassed <= 7)
                                {
                                    int cCode = getcCodeByNat(natCode2);
                                    if (coronaCode == 4 && cCode < 3)
                                    {
                                        changecCodeByNat(natCode2, 3);
                                        lineNum--;
                                    }
                                    else if (coronaCode == 3 && cCode < 2)
                                    {
                                        changecCodeByNat(natCode2, 2);
                                        lineNum--;
                                    }
                                }
                            }
                            else if (strcmp(nationalCode, natCode2) == 0)
                            {
                                char *date = removeNL(separateBy(line, ',', 3));
                                struct tm tm;
                                char temp[30] = {};
                                strcat(temp, date);
                                strcat(temp, " 00:00:00");
                                strptime(temp, "%Y-%m-%d %H:%M:%S", &tm);
                                time_t t = mktime(&tm);
                                int daysPassed = (time(NULL) - t) / 86400;
                                if (daysPassed <= 7)
                                {
                                    int cCode = getcCodeByNat(natCode1);
                                    if (coronaCode == 4 && cCode < 3)
                                    {
                                        changecCodeByNat(natCode1, 3);
                                        lineNum--;
                                    }
                                    else if (coronaCode == 3 && cCode < 2)
                                    {
                                        changecCodeByNat(natCode1, 2);
                                        lineNum--;
                                    }
                                }
                            }
                        }
                        fclose(fp);
                        if (line)
                            free(line);

                        if (coronaCode == 4)
                        {
                            fp = fopen("avisits.csv", "r");
                            line = NULL;
                            len = 0;
                            while ((read = getline(&line, &len, fp)) != -1)
                            {
                                char *personId = separateBy(line, ',', 1);
                                if (strcmp(nationalCode, personId) == 0)
                                {
                                    char *date = removeNL(separateBy(line, ',', 3));
                                    struct tm tm;
                                    char temp[30] = {};
                                    strcat(temp, date);
                                    strcat(temp, " 00:00:00");
                                    strptime(temp, "%Y-%m-%d %H:%M:%S", &tm);
                                    time_t t = mktime(&tm);
                                    int daysPassed = (time(NULL) - t) / 86400;
                                    if (daysPassed <= 7)
                                    {
                                        int areaId = 0;
                                        sscanf(separateBy(line, ',', 2), "%d", &areaId);
                                        changecCodeByAreaId(areaId, 2);
                                    }
                                }
                            }
                            fclose(fp);
                            if (line)
                                free(line);
                        }
                    }

                    //Add To History
                    fp = fopen("history.csv", "a");
                    if (fp == NULL)
                        exit(EXIT_FAILURE);
                    fprintf(fp, "\n");
                    fprintf(fp, "%s,%d,%d-%02d-%02d", nationalCode, coronaCode, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
                    fclose(fp);
                }
                int Choosed = 0;
            }
            else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "visit person\n"))
            {
                printf(CLEAR);
                printf(RESET FGREEN "Person ");
                char *natCode = getNatCode();
                if (strcmp(natCode, "-1") != 0)
                {
                    while (!natCodeExists(natCode) || !strcmp(nationalCode, natCode))
                    {
                        puts(RESET FRED "National code doesn't exist!" RESET FGREEN);
                        natCode = getNatCode();
                        if (strcmp(natCode, "-1") == 0)
                            break;
                    }
                    if (strcmp(natCode, "-1") != 0)
                    {
                        FILE *fp = fopen("pvisits.csv", "a");
                        if (fp == NULL)
                            exit(EXIT_FAILURE);
                        fprintf(fp, "\n");
                        fprintf(fp, "%s,%s,%d-%02d-%02d", nationalCode, natCode, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
                        fclose(fp);
                        int cCode = getcCodeByNat(natCode);
                        if (cCode > coronaCode + 1)
                        {
                            coronaCode = cCode - 1;
                            char sCode[2];
                            sCode[0] = '0' + coronaCode;
                            sCode[1] = '\0';
                            dataOfUser = replaceBetweenCommas(dataOfUser, sCode, 8);
                            FILE *fp = fopen("users.csv", "r");
                            if (fp == NULL)
                                exit(EXIT_FAILURE);
                            lineNum = removeLineFromFile(lineNum, "users.csv", fp);
                            fp = fopen("users.csv", "a");
                            if (fp == NULL)
                                exit(EXIT_FAILURE);
                            fprintf(fp, "\n");
                            fprintf(fp, dataOfUser);
                            fclose(fp);
                        }
                        else if (coronaCode > cCode + 1)
                        {
                            changecCodeByNat(natCode, coronaCode - 1);
                        }
                        printf(RESET FGREEN BOLD "Successfully added!\n");
                        printf(RESET FWHITE "press any key to go back");
                        getchar();
                    }
                }
            }
            else if (!strcmp(input, "3\n") || !strcmp(strlwr(input), "visit area\n"))
            {
                printf(CLEAR);
                int mantaghe = getPositiveInt("Area ID", 0);
                getchar();
                if (mantaghe != -1)
                {
                    FILE *fp = fopen("avisits.csv", "a");
                    if (fp == NULL)
                        exit(EXIT_FAILURE);
                    fprintf(fp, "\n");
                    fprintf(fp, "%s,%d,%d-%02d-%02d", nationalCode, mantaghe, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
                    fclose(fp);

                    if (coronaCode == 1)
                    {
                        int cCode = getcCodeByAreaId(mantaghe);
                        if (cCode == 2)
                        {
                            coronaCode = 2;
                            char sCode[2];
                            sCode[0] = '0' + coronaCode;
                            sCode[1] = '\0';
                            dataOfUser = replaceBetweenCommas(dataOfUser, sCode, 8);
                            FILE *fp = fopen("users.csv", "r");
                            if (fp == NULL)
                                exit(EXIT_FAILURE);
                            lineNum = removeLineFromFile(lineNum, "users.csv", fp);
                            fp = fopen("users.csv", "a");
                            if (fp == NULL)
                                exit(EXIT_FAILURE);
                            fprintf(fp, "\n");
                            fprintf(fp, dataOfUser);
                            fclose(fp);
                        }
                    }
                    else if (coronaCode == 4)
                        changecCodeByAreaId(mantaghe, 2);

                    printf(RESET FGREEN BOLD "Successfully added!\n");
                    printf(RESET FWHITE "press any key to go back");
                    getchar();
                }
            }
            else if (!strcmp(input, "4\n") || !strcmp(strlwr(input), "tests history\n"))
            {
                printf(CLEAR);
                FILE *fp = fopen("history.csv", "r");
                char *line = NULL;
                size_t len = 0;
                ssize_t read;
                printf(RESET FWHITE "  %-15s%-15s\n", "Corona Code", "Date");
                int count = 1;
                while ((read = getline(&line, &len, fp)) != -1)
                {
                    if (strcmp(nationalCode, separateBy(line, ',', 1)) == 0)
                    {
                        char *code = separateBy(line, ',', 2);
                        char *date = removeNL(separateBy(line, ',', 3));
                        if (strcmp("1", code) == 0)
                            printf(RESET FWHITE "%d.%-15s%-15s\n", count, "Healthy", date);
                        else if (strcmp("2", code) == 0)
                            printf(RESET FYELLOW "%d.%-15s%-15s\n", count, "Suspicious", date);
                        else if (strcmp("3", code) == 0)
                            printf(RESET FORANGE "%d.%-15s%-15s\n", count, "Dangerous", date);
                        else if (strcmp("4", code) == 0)
                            printf(RESET FRED "%d.%-15s%-15s\n", count, "Ill", date);
                        count++;
                    }
                }
                fclose(fp);
                if (line)
                    free(line);
                printf(RESET FWHITE "press any key to go back");
                getchar();
            }
            else if (!strcmp(input, "5\n") || !strcmp(strlwr(input), "visits history\n"))
            {
                printf(CLEAR);
                FILE *fp = fopen("pvisits.csv", "r");
                char *line = NULL;
                size_t len = 0;
                ssize_t read;
                printf(RESET FGREEN "  %-15s%-15s\n", "National Code", "Date");
                int count = 1;
                while ((read = getline(&line, &len, fp)) != -1)
                {
                    char *natCode1 = separateBy(line, ',', 1);
                    char *natCode2 = separateBy(line, ',', 2);
                    if (strcmp(nationalCode, natCode1) == 0)
                    {
                        char *date = removeNL(separateBy(line, ',', 3));
                        printf(RESET FCYAN "%d.%-15s%-15s\n", count, natCode2, date);
                        count++;
                    }
                    else if (strcmp(nationalCode, natCode2) == 0)
                    {
                        char *date = removeNL(separateBy(line, ',', 3));
                        printf(RESET FCYAN "%d.%-15s%-15s\n", count, natCode1, date);
                        count++;
                    }
                }
                fclose(fp);
                if (line)
                    free(line);
                printf("\n\n");

                fp = fopen("avisits.csv", "r");
                line = NULL;
                len = 0;
                printf(RESET FGREEN "  %-15s%-15s\n", "AreaId", "Date");
                count = 1;
                while ((read = getline(&line, &len, fp)) != -1)
                {
                    char *personId = separateBy(line, ',', 1);
                    if (strcmp(nationalCode, personId) == 0)
                    {
                        char *date = removeNL(separateBy(line, ',', 3));
                        printf(RESET FCYAN "%d.%-15s%-15s\n", count, separateBy(line, ',', 2), date);
                        count++;
                    }
                }
                fclose(fp);
                if (line)
                    free(line);

                printf(RESET FWHITE "press any key to go back");
                getchar();
            }
            else if (!strcmp(input, "6\n") || !strcmp(strlwr(input), "edit profile\n"))
            {
                dataOfUser = editNormalProfile(&lineNum, dataOfUser);
                free(name);
                free(nationalCode);
                name = separateBy(dataOfUser, ',', 2);
                nationalCode = separateBy(dataOfUser, ',', 4);
            }
            else if (!strcmp(input, "7\n") || !strcmp(strlwr(input), "lock account\n"))
            {
                Choosed = 3;
                free(name);
                free(nationalCode);
                loginToAccount();
            }
            else if (!strcmp(input, "8\n") || !strcmp(strlwr(input), "exit app\n"))
                exit(0);
            else if (!strcmp(input, "-1\n"))
            {
                Choosed = 3;
                free(name);
                free(nationalCode);
                loginToAccount();
            }
            else
            {
                printf("%s", RESET FRED "Wrong input!\n\n" FWHITE);
                printf(FWHITE "Input: ");
                Choosed = -1;
            }
        }
    }
    else if (*dataOfUser == '2')
    {
        char *cCodeSep = separateBy(dataOfUser, ',', 8);
        char *areaIdSep = separateBy(dataOfUser, ',', 11);
        int coronaCode = atoi(cCodeSep); // 1: white 2: red
        int areaId = atoi(areaIdSep);
        free(cCodeSep);
        free(areaIdSep);

        //Show time
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        int Choosed = 0;
        char *name = NULL;
        while (!Choosed || Choosed == -1)
        {
            if (Choosed == 0)
            {
                printf(CLEAR);
                tm = *localtime(&t);
                if (name)
                    free(name);
                name = separateBy(dataOfUser, ',', 2);
                printf(RESET BOLD FPANEL2 "\t\t\t   Welcome %s\n", name);
                printf(RESET FWHITE "\t\t\t%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

                areaIdSep = separateBy(dataOfUser, ',', 11);
                areaId = atoi(areaIdSep);
                free(areaIdSep);

                if (coronaCode == 1)
                    puts(RESET FWHITE "Area Corona Code: White");
                else if (coronaCode == 2)
                    puts(RESET FRED "Area Corona Code: Red");
                printf(RESET FWHITE "Your AreaId: %d\n\n", areaId);

                puts(RESET FPANEL2 "1.Clear the place");
                puts("2.Edit profile");
                puts("3.Lock account");
                puts("4.Exit app\n" RESET);
                printf(FWHITE "Input: ");
            }

            char input[25];
            fgets(input, 25, stdin); // To Get Spaces between characters too
            if (!strcmp(input, "1\n") || !strcmp(strlwr(input), "clear the place\n"))
            {
                if (coronaCode != 1)
                {
                    char cCode[2] = "1";
                    FILE *fp = fopen("users.csv", "r");
                    if (fp == NULL)
                        exit(EXIT_FAILURE);
                    lineNum = removeLineFromFile(lineNum, "users.csv", fp);
                    fp = fopen("users.csv", "a");
                    if (fp == NULL)
                        exit(EXIT_FAILURE);
                    fprintf(fp, "\n");
                    free(dataOfUser);
                    char *temp = replaceBetweenCommas(dataOfUser, cCode, 8);
                    dataOfUser = removeNL(temp);
                    free(temp);
                    fprintf(fp, dataOfUser);
                    fclose(fp);
                    coronaCode = 1;
                }
                Choosed = 0;
            }
            else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "edit profile\n"))
            {
                dataOfUser = editBusProfile(&lineNum, dataOfUser);
                Choosed = 0;
            }
            else if (!strcmp(input, "3\n") || !strcmp(strlwr(input), "lock account\n"))
            {
                Choosed = 3;
                free(name);
                loginToAccount();
            }
            else if (!strcmp(input, "4\n") || !strcmp(strlwr(input), "exit app\n"))
                exit(0);
            else if (!strcmp(input, "-1\n"))
            {
                Choosed = 3;
                free(name);
                loginToAccount();
            }
            else
            {
                Choosed = -1;
                printf("%s", RESET FRED "Wrong input!\n" FWHITE);
                printf(FWHITE "Input: ");
            }
            printf("%c", '\n');
        }
    }
    else if (*dataOfUser == '0')
    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        int Choosed = 0;
        while (!Choosed || Choosed == -1)
        {
            if (Choosed == 0)
            {
                printf(CLEAR);
                printf(RESET FMAGNETA BOLD "\t\t\t   Welcome " RESET BOLD FADMIN "admin\n");
                tm = *localtime(&t);
                printf(RESET FWHITE "\t\t\t%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                puts(RESET FADMIN "1.Show users");
                puts("2.Change user status");
                puts("3.Change password");
                puts("4.Lock account");
                puts("5.Exit app\n" RESET);
                printf(FWHITE "Input: ");
            }
            char input[25];
            fgets(input, 25, stdin); // To Get Spaces between characters too
            if (!strcmp(input, "1\n") || !strcmp(strlwr(input), "show users\n"))
            {
                printf(CLEAR);
                FILE *fp = fopen("users.csv", "r");
                char *line = NULL;
                size_t len = 0;
                ssize_t read;
                printf(RESET FGREEN "  %-15s%-15s%-15s\n", "National Code", "Name", "Corona Code");
                int count = 1;
                while ((read = getline(&line, &len, fp)) != -1)
                {
                    char *typeSep = separateBy(line, ',', 1);
                    if (strcmp("1", typeSep) == 0)
                    {
                        char *natCode = separateBy(line, ',', 4);
                        char *user_name = separateBy(line, ',', 2);
                        char *cCode = removeNL(separateBy(line, ',', 8));
                        printf(RESET FADMIN "%d.%-15s%-15s%-15s\n", count, natCode, user_name, cCode);
                        free(natCode);
                        free(user_name);
                        free(cCode);
                        count++;
                    }
                    free(typeSep);
                }
                fclose(fp);
                if (line)
                    free(line);
                printf(RESET FWHITE "press any key to go back");
                getchar();
            }
            else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "change user status\n"))
            {
                printf(CLEAR);
                char *natCode = getNatCode();
                if (strcmp(natCode, "-1") != 0)
                {
                    int cCode = getcCodeByNat(natCode);
                    if (cCode == 4)
                    {
                        int inp = -2;
                        while (inp != -1 && inp != 1 && inp != 2 && inp != 3 && inp != 4)
                        {
                            printf(RESET FGREEN);
                            printf("New Corona Code(-1 For dead): ");
                            scanf("%d", &inp);
                            getchar();
                            if (inp != -1 && inp != 1 && inp != 2 && inp != 3 && inp != 4)
                                printf(RESET FRED "Corona code can only be -1/1/2/3/4\n");
                        }
                        changecCodeByNat(natCode, inp);
                        printf(RESET FGREEN BOLD "Successfully changed!\n");
                        printf(RESET FWHITE "press any key to go back");
                        getchar();
                    }
                    else
                    {
                        printf(RESET FRED "The corona code is %d, You cant edit it!\n", cCode);
                        printf(RESET FWHITE "press any key to go back");
                        getchar();
                    }
                }
            }
            else if (!strcmp(input, "3\n") || !strcmp(strlwr(input), "change password\n"))
            {
                printf(CLEAR);
                char *password;
                printf(RESET FGREEN "New "); //To print new password
                password = getPassword();
                if (strcmp(password, "-1") != 0)
                {
                    char repeatOfPass[30];
                    while (strcmp(repeatOfPass, password))
                    {
                        int i;
                        char ch1;
                        printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                        for (i = 0; i < 30; i++)
                        {
                            ch1 = getch();
                            if (ch1 != 10)
                            {
                                if (ch1 != 127)
                                {
                                    printf("*");
                                    repeatOfPass[i] = ch1;
                                }
                                else if (i > 0)
                                {
                                    printf("\b \b");
                                    i -= 2;
                                }
                                else
                                    i = -1; // To not remove other strings!
                            }
                            else
                                break;
                        }
                        repeatOfPass[i] = '\0';
                        if (strcmp(repeatOfPass, password))
                            puts(RESET FRED "\nThe password and repeat of password dont match!" RESET FGREEN);
                    }
                    printf("%c", '\n');
                    FILE *fp = fopen("users.csv", "r");
                    lineNum = removeLineFromFile(lineNum, "users.csv", fp);
                    fp = fopen("users.csv", "a");
                    if (fp == NULL)
                        exit(EXIT_FAILURE);
                    fprintf(fp, "\n");
                    dataOfUser = replaceBetweenCommas(dataOfUser, encrypt(password), 3);
                    fprintf(fp, dataOfUser);
                    fclose(fp);
                    puts(RESET FGREEN BOLD "Password has been changed successfully!" RESET FWHITE);
                    getchar();
                }
            }
            else if (!strcmp(input, "4\n") || !strcmp(strlwr(input), "lock account\n"))
            {
                Choosed = 2;
                loginToAccount();
            }
            else if (!strcmp(input, "5\n") || !strcmp(strlwr(input), "exit app\n"))
                exit(0);
            else if (!strcmp(input, "-1\n"))
                loginToAccount();
            else
            {
                Choosed = -1;
                printf("%s", RESET FRED "Wrong input!\n" FWHITE);
                printf(FWHITE "Input: ");
            }
            printf("%c", '\n');
        }
    }
}

int coronaTest()
{
    printf(CLEAR);
    int coronaPercent = 0;

    int cough = ask("Excessive dry coughs(y/n): ");
    if (cough == -1)
        return -1;
    if (cough)
        coronaPercent += 30;

    int fever = ask("Beyond 37 degrees fever(y/n): ");
    if (fever == -1)
        return -1;
    if (fever)
        coronaPercent += 20;

    int breath = ask("Shortness of breath(y/n): ");
    if (breath == -1)
        return -1;
    if (breath)
        coronaPercent += 20;

    int pain = ask("Pain and bruising(y/n): ");
    if (pain == -1)
        return -1;
    if (pain)
        coronaPercent += 15;

    int sore = ask("Sore throat(y/n): ");
    if (sore == -1)
        return -1;
    if (sore)
        coronaPercent += 15;

    return coronaPercent;
}

int ask(char *question)
{
    char *answer = malloc(sizeof(char) * 3);
    *answer = '\0';

    while (strcmp(strlwr(answer), "y") != 0 && strcmp(strlwr(answer), "n") != 0 && strcmp(answer, "-1") != 0)
    {
        printf(RESET FCYAN);
        printf("%s", question);
        printf(RESET FWHITE);
        scanf("%s", answer);
        if (strcmp(strlwr(answer), "y") != 0 && strcmp(strlwr(answer), "n") != 0 && strcmp(answer, "-1"))
            puts(RESET FRED "Wrong Answer!" RESET FGREEN);
    }
    getchar();
    if (strcmp(strlwr(answer), "y") == 0)
        return 1;
    else if (strcmp(strlwr(answer), "n") == 0)
        return 0;
    else
        return -1;
}

void createFiles()
{
    FILE *fptr;
    if (access("users.csv", F_OK) != 0)
    {
        fptr = fopen("users.csv", "w");
        fprintf(fptr, "UserType,Name,Family,National Code,Username,Password,Email,CoronaCode\n");
        fprintf(fptr, "0,admin,firns678");
        fclose(fptr);
    }
    if (access("history.csv", F_OK) != 0)
    {
        fptr = fopen("history.csv", "w");
        fprintf(fptr, "Id,Corona Code,Date");
        fclose(fptr);
    }
    if (access("pvisits.csv", F_OK) != 0)
    {
        fptr = fopen("pvisits.csv", "w");
        fprintf(fptr, "Id1,Id2,Date");
        fclose(fptr);
    }
    if (access("avisits.csv", F_OK) != 0)
    {
        fptr = fopen("avisits.csv", "w");
        fprintf(fptr, "Person Id,Area Id,Date");
        fclose(fptr);
    }
}

char *editNormalProfile(int *lineNum, char *dataOfUser)
{
    int Choosed = 0;
    while (!Choosed || Choosed == -1)
    {
        if (Choosed == 0)
        {
            printf(CLEAR);
            puts(RESET FCYAN "1.Name");
            puts("2.Family");
            puts("3.National code");
            puts("4.Password");
            puts("5.Email");
            puts("6.Blood Group");
            puts("7.Age");
            puts("8.Height");
            puts("9.Weight\n");
            printf(RESET FWHITE "What do you want to edit: ");
        }

        char input[25];
        fgets(input, 25, stdin); // To Get Spaces between characters too
        if (!strcmp(input, "1\n") || !strcmp(strlwr(input), "name\n"))
        {
            char *name;
            printf(CLEAR);
            name = getInputWithSpace(40, "Name");
            if (strcmp(name, "-1\n") != 0)
            {
                dataOfUser = replaceBetweenCommas(dataOfUser, removeNL(name), 2);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "family\n"))
        {
            char *family;
            printf(CLEAR);
            family = getInputWithSpace(40, "Family");
            if (strcmp(family, "-1\n") != 0)
            {
                dataOfUser = replaceBetweenCommas(dataOfUser, removeNL(family), 3);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "3\n") || !strcmp(strlwr(input), "national code\n"))
        {
            printf(CLEAR);
            char *natCode = getNatCode();
            if (strcmp(natCode, "-1") != 0)
            {
                dataOfUser = replaceBetweenCommas(dataOfUser, natCode, 4);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "4\n") || !strcmp(strlwr(input), "password\n"))
        {
            printf(CLEAR);
            char *password = getPassword();
            if (strcmp(password, "-1") != 0)
            {
                char repeatOfPass[30];
                while (strcmp(repeatOfPass, password))
                {
                    int i;
                    char ch1;
                    printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                    for (i = 0; i < 30; i++)
                    {
                        ch1 = getch();
                        if (ch1 != 10)
                        {
                            if (ch1 != 127)
                            {
                                printf("*");
                                repeatOfPass[i] = ch1;
                            }
                            else if (i > 0)
                            {
                                printf("\b \b");
                                i -= 2;
                            }
                            else
                                i = -1; // To not remove other strings!
                        }
                        else
                            break;
                    }
                    repeatOfPass[i] = '\0';
                    if (strcmp(repeatOfPass, password))
                        puts(RESET FRED "\nThe password and repeat of password dont match!" RESET FGREEN);
                }
                printf("%c", '\n');
                dataOfUser = replaceBetweenCommas(dataOfUser, encrypt(password), 6);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "5\n") || !strcmp(strlwr(input), "email\n"))
        {
            printf(CLEAR);
            char *email = getEmail();
            if (strcmp(email, "-1") != 0)
            {
                dataOfUser = replaceBetweenCommas(dataOfUser, strlwr(email), 7);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "6\n") || !strcmp(strlwr(input), "blood group\n"))
        {
            printf(CLEAR);
            char *bloodGroup = getBloodGroup();
            if (bloodGroup)
            {
                dataOfUser = replaceBetweenCommas(dataOfUser, bloodGroup, 9);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "7\n") || !strcmp(strlwr(input), "age\n"))
        {
            printf(CLEAR);
            int age = getPositiveInt("Age", 1);
            if (age != -1)
            {
                char Age[5];
                sprintf(Age, "%d", age);
                dataOfUser = replaceBetweenCommas(dataOfUser, Age, 10);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "8\n") || !strcmp(strlwr(input), "height\n"))
        {
            printf(CLEAR);
            int height = getPositiveInt("Height", 1);
            if (height != -1)
            {
                char Height[5];
                sprintf(Height, "%d", height);
                dataOfUser = replaceBetweenCommas(dataOfUser, Height, 11);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "9\n") || !strcmp(strlwr(input), "weight\n"))
        {
            printf(CLEAR);
            int weight = getPositiveInt("Weight", 1);
            if (weight != -1)
            {
                char Weight[5];
                sprintf(Weight, "%d", weight);
                dataOfUser = replaceBetweenCommas(dataOfUser, Weight, 12);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "-1\n"))
        {
            break;
        }
        else
        {
            printf("%s", RESET FRED "Wrong input!\n\n" FWHITE);
            printf(FWHITE "What do you want to edit: ");
            Choosed = -1;
        }

        if (Choosed)
        {
            FILE *fp = fopen("users.csv", "r");
            if (fp == NULL)
                exit(EXIT_FAILURE);
            *lineNum = removeLineFromFile(*lineNum, "users.csv", fp);
            fp = fopen("users.csv", "a");
            if (fp == NULL)
                exit(EXIT_FAILURE);
            fprintf(fp, "\n");
            fprintf(fp, dataOfUser);
            fclose(fp);
            Choosed = 0;
        }
    }
    return dataOfUser;
}

char *editBusProfile(int *lineNum, char *dataOfUser)
{
    int Choosed = 0;
    while (!Choosed || Choosed == -1)
    {
        if (Choosed == 0)
        {
            printf(CLEAR);
            puts(RESET FCYAN "1.Name");
            puts("2.Family");
            puts("3.National code");
            puts("4.Password");
            puts("5.Email");
            puts("6.Place name");
            puts("7.City name");
            puts("8.Area\n");
            printf(RESET FWHITE "What do you want to edit: ");
        }

        char input[25];
        fgets(input, 25, stdin); // To Get Spaces between characters too
        if (!strcmp(input, "1\n") || !strcmp(strlwr(input), "name\n"))
        {
            char *name;
            printf(CLEAR);
            name = getInputWithSpace(40, "Name");
            if (strcmp(name, "-1\n") != 0)
            {
                dataOfUser = replaceBetweenCommas(dataOfUser, removeNL(name), 2);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "family\n"))
        {
            char *family;
            printf(CLEAR);
            family = getInputWithSpace(40, "Family");
            if (strcmp(family, "-1\n") != 0)
            {
                dataOfUser = replaceBetweenCommas(dataOfUser, removeNL(family), 3);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "3\n") || !strcmp(strlwr(input), "national code\n"))
        {
            printf(CLEAR);
            char *natCode = getNatCode();
            if (strcmp(natCode, "-1") != 0)
            {
                dataOfUser = replaceBetweenCommas(dataOfUser, natCode, 4);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "4\n") || !strcmp(strlwr(input), "password\n"))
        {
            printf(CLEAR);
            char *password = getPassword();
            if (strcmp(password, "-1") != 0)
            {
                char repeatOfPass[30];
                while (strcmp(repeatOfPass, password))
                {
                    int i;
                    char ch1;
                    printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                    for (i = 0; i < 30; i++)
                    {
                        ch1 = getch();
                        if (ch1 != 10)
                        {
                            if (ch1 != 127)
                            {
                                printf("*");
                                repeatOfPass[i] = ch1;
                            }
                            else if (i > 0)
                            {
                                printf("\b \b");
                                i -= 2;
                            }
                            else
                                i = -1; // To not remove other strings!
                        }
                        else
                            break;
                    }
                    repeatOfPass[i] = '\0';
                    if (strcmp(repeatOfPass, password))
                        puts(RESET FRED "\nThe password and repeat of password dont match!" RESET FGREEN);
                }
                printf("%c", '\n');
                dataOfUser = replaceBetweenCommas(dataOfUser, encrypt(password), 6);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "5\n") || !strcmp(strlwr(input), "email\n"))
        {
            printf(CLEAR);
            char *email = getEmail();
            if (strcmp(email, "-1") != 0)
            {
                dataOfUser = replaceBetweenCommas(dataOfUser, strlwr(email), 7);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "6\n") || !strcmp(strlwr(input), "place name\n"))
        {
            printf(CLEAR);
            char *placeName = getInputWithSpace(80, "Place name");
            if (strcmp(placeName, "-1\n") != 0)
            {
                dataOfUser = replaceBetweenCommas(dataOfUser, removeNL(placeName), 9);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "7\n") || !strcmp(strlwr(input), "city name\n"))
        {
            printf(CLEAR);
            char *cityName = getInputWithSpace(50, "City name");
            if (strcmp(cityName, "-1\n") != 0)
            {
                dataOfUser = replaceBetweenCommas(dataOfUser, removeNL(cityName), 10);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "8\n") || !strcmp(strlwr(input), "area\n"))
        {
            printf(CLEAR);
            int mantaghe = getPositiveInt("Area", 0);
            if (mantaghe != -1)
            {
                char Mantaghe[5];
                sprintf(Mantaghe, "%d", mantaghe);
                dataOfUser = replaceBetweenCommas(dataOfUser, Mantaghe, 11);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "-1\n"))
        {
            break;
        }
        else
        {
            printf("%s", RESET FRED "Wrong input!\n\n" FWHITE);
            printf(FWHITE "What do you want to edit: ");
            Choosed = -1;
        }

        if (Choosed)
        {
            FILE *fp = fopen("users.csv", "r");
            if (fp == NULL)
                exit(EXIT_FAILURE);
            *lineNum = removeLineFromFile(*lineNum, "users.csv", fp);
            fp = fopen("users.csv", "a");
            if (fp == NULL)
                exit(EXIT_FAILURE);
            fprintf(fp, "\n");
            fprintf(fp, dataOfUser);
            fclose(fp);
            Choosed = 0;
        }
    }
    return dataOfUser;
}

bool natCodeExists(char *natCode)
{
    FILE *fp = fopen("users.csv", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    bool ex = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *natcodeSep = separateBy(line, ',', 4);
        if (strcmp(natCode, natcodeSep) == 0)
        {
            ex = 1;
            free(natcodeSep);
            break;
        }
        free(natcodeSep);
    }
    fclose(fp);
    if (line)
        free(line);
    return ex;
}

bool usernameExists(char *username)
{
    FILE *fp = fopen("users.csv", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    bool ex = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        char userSep = separateBy(line, ',', 5);
        if (strcmp(username, userSep) == 0)
        {
            ex = 1;
            free(userSep);
            break;
        }
        free(userSep);
    }
    fclose(fp);
    if (line)
        free(line);
    return ex;
}

int getcCodeByNat(char *natCode)
{
    FILE *fp = fopen("users.csv", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int cCode = 1;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *natCodeSep = separateBy(line, ',', 4);
        if (strcmp(natCode, natCodeSep) == 0)
        {
            char *cCodeSep = separateBy(line, ',', 8);
            sscanf(cCodeSep, "%d", &cCode);
            free(cCodeSep);
            free(natCodeSep);
            break;
        }
        free(natCodeSep);
    }
    fclose(fp);
    if (line)
        free(line);
    return cCode;
}

void changecCodeByNat(char *natCode, int cCode)
{
    FILE *fp = fopen("users.csv", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int lineNum = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        lineNum++;
        char *natCodeSep = separateBy(line, ',', 4);
        if (strcmp(natCode, natCodeSep) == 0)
        {
            free(natCodeSep);
            break;
        }
        free(natCodeSep);
    }
    fclose(fp);
    line = removeNL(line);
    char sCode[3];
    sprintf(sCode, "%d", cCode);

    line = removeNL(line);
    line = replaceBetweenCommas(line, sCode, 8);
    fp = fopen("users.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    removeLineFromFile(lineNum, "users.csv", fp);
    fp = fopen("users.csv", "a");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    fprintf(fp, "\n");
    fprintf(fp, line);
    fclose(fp);
    if (line)
        free(line);
}

int getcCodeByAreaId(int areaId)
{
    FILE *fp = fopen("users.csv", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int cCode = 1;
    char str[2];
    sprintf(str, "%d", areaId);
    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *typeSep = separateBy(line, ',', 1);
        if (!strcmp("2", typeSep))
        {
            char *areaIdSep = separateBy(line, ',', 11);
            if (!strcmp(str, areaIdSep))
            {
                char *cCodeSep = separateBy(line, ',', 8);
                sscanf(cCodeSep, "%d", &cCode);
                free(cCodeSep);
                free(areaIdSep);
                free(typeSep);
                break;
            }
            free(areaIdSep);
        }
        free(typeSep);
    }
    fclose(fp);
    if (line)
        free(line);
    return cCode;
}

void changecCodeByAreaId(int areaId, int cCode)
{
    FILE *fp = fopen("users.csv", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int lineNum = 0;
    char str[2];
    sprintf(str, "%d", areaId);
    while ((read = getline(&line, &len, fp)) != -1)
    {
        lineNum++;
        char *typeSep = separateBy(line, ',', 1);
        if (!strcmp("2", typeSep))
        {
            char *areaIdSep = separateBy(line, ',', 11);
            if (!strcmp(str, removeNL(areaIdSep)))
            {
                free(typeSep);
                free(areaIdSep);
                break;
            }
            free(areaIdSep);
        }
        free(typeSep);
    }
    fclose(fp);

    char sCode[2];
    sprintf(sCode, "%d", cCode);

    line = removeNL(line);
    line = replaceBetweenCommas(line, sCode, 8);
    fp = fopen("users.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    removeLineFromFile(lineNum, "users.csv", fp);
    fp = fopen("users.csv", "a");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    fprintf(fp, "\n");
    fprintf(fp, line);
    fclose(fp);
    if (line)
        free(line);
}