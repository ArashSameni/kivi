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

#define PasswordSize 50
#define EmailSize 100
#define UsernameSize 50

typedef struct visits
{
    int count;
    int *ids;
    int *timestamps;
} visits;

typedef struct normalInfo
{
    char bloodGroup[4];
    int age;
    int height;
    int weight;
} normalInfo;

typedef struct busInfo
{
    char placeName[101];
    char cityName[101];
    int areaId;
} busInfo;

typedef struct user
{
    int id;
    int type;
    char name[101];
    char lastName[101];
    char nationalCode[11];
    char username[51];
    char password[51];
    char email[101];
    int coronaCode;
    union
    {
        normalInfo normalinfo;
        busInfo businfo;
    };
} user;

void showLogo();
void showMainMenu();
void createAccount();
void loginMenu();
void resetPasswordMenu();
void panelMenu(user loggedUser);
void updateUser(user theUser, bool shouldUpdateInfo);
char *getInputWithSpace(int size, char *name);
char *getNatCode();
char *getUsername();
char *getPassword();
char *getEmail();
char *getBloodGroup();
int getPositiveInt(char *name, bool optional);
int coronaTest();
int ask(char *question);
int getIdByNatCode(char *natCode);
int alreadyVisitedu(int userId1, int userId2); //user // 0 : not found, 1 : UserId1 = loggedinuser, 2 : UserId2 = loggedinuser
bool natCodeExists(char *natCode);
bool usernameExists(char *username);
bool alreadyVisiteda(int userId, int areaId); //area
bool resetPassUserExists(char *username, char *email, char *natCode);
user login(char *username, char *password);
user getUserById(int userId);
user editNormalProfile(user loggedUser);
user editBusProfile(user loggedUser);
user *getUsersByAreaId(int areaId, int *count);
user *getAllUsers(int *count);
visits getVisitedAreas(int userId);
visits getVisitedUsers(int userId);

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

MYSQL *con;
int main()
{
    con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(con, "localhost", "root", "root", "kivi_db", 0, NULL, 0) == NULL)
        finish_with_error(con);

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
    bool wrong = false;
    while (true)
    {
        wrong = false;
        if (!wrong)
        {
            printf(CLEAR);
            printf("%c", '\n');
            puts(FGREEN "\t            kkkkkkkk              iiii                              iiii");
            puts("\t            k::::::k             i::::i                            i::::i");
            puts("\t            k::::::k              iiii                              iiii");
            puts("\t            k::::::k");
            puts("\t             k:::::k    kkkkkkk iiiiiii vvvvvvv           vvvvvvv iiiiiii");
            puts("\t             k:::::k   k:::::k  i:::::i  v:::::v         v:::::v  i:::::i");
            puts("\t             k:::::k  k:::::k    i::::i   v:::::v       v:::::v    i::::i");
            puts("\t             k:::::k k:::::k     i::::i    v:::::v     v:::::v     i::::i");
            puts("\t             k::::::k:::::k      i::::i     v:::::v   v:::::v      i::::i");
            puts("\t             k:::::::::::k       i::::i      v:::::v v:::::v       i::::i");
            puts("\t             k:::::::::::k       i::::i       v:::::v:::::v        i::::i");
            puts("\t             k::::::k:::::k      i::::i        v:::::::::v         i::::i");
            puts("\t            k::::::k k:::::k    i::::::i        v:::::::v         i::::::i");
            puts("\t            k::::::k  k:::::k   i::::::i         v:::::v          i::::::i");
            puts("\t            k::::::k   k:::::k  i::::::i          v:::v           i::::::i");
            puts("\t            kkkkkkkk    kkkkkkk iiiiiiii           vvv            iiiiiiii" RESET);
            puts(FCYAN BOLD "By Arash Sameni" RESET);
            puts(FCYAN "==> any where you can use -1 to go back\n" RESET);
            puts(FGREEN "1.Login to account");
            puts("2.Create a new account");
            puts("3.Reset password\n" RESET);
            printf(FWHITE "Input: ");
        }
        char input[25];
        fgets(input, 25, stdin); // To Get Spaces between characters too
        if (!strcmp(input, "1\n") || !strcmp(strlwr(input), "login to account\n"))
            loginMenu();
        else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "create a new account\n"))
            createAccount();
        else if (!strcmp(input, "3\n") || !strcmp(strlwr(input), "reset password\n"))
            resetPasswordMenu();
        else if (!strcmp(input, "-1\n"))
            exit(0);
        else
        {
            printf("%s", RESET FRED "Wrong input!\n" FWHITE);
            wrong = true;
        }
        printf("%c", '\n');
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
            break;
        else
        {
            Choosed = 0;
            printf("%s", RESET FRED "Wrong input!\n" FWHITE);
        }
    }
    bool goBack = false;

    if (Choosed)
    {
        while (!goBack)
        {
            if (Choosed == 1)
            {
                char *name, *family, *natCode, *username, *password, *email, *bloodGroup;
                int age, height, weight;
                age = height = weight = -2;

                name = getInputWithSpace(100, "Name");
                if (strcmp(name, "-1") == 0)
                {
                    goBack = true;
                    break;
                }

                family = getInputWithSpace(100, "Family");
                if (strcmp(family, "-1") == 0)
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

                char repeatOfPass[51];
                while (strcmp(repeatOfPass, password))
                {
                    int i;
                    char ch1;
                    printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                    for (i = 0; i < 50; i++)
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
                    goBack = true;
                    break;
                }

                height = getPositiveInt("Height", 1);
                if (height == -1)
                {
                    goBack = true;
                    break;
                }

                weight = getPositiveInt("Weight", 1);
                if (weight == -1)
                {
                    goBack = true;
                    break;
                }

                char sql[5000];
                sprintf(sql, "INSERT INTO Users (Type, FirstName, LastName, NationalCode, Username, Password, Email, CoronaCode) VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', %d)",
                        1, name, family, natCode, username, encrypt(password), email, 1);
                if (mysql_query(con, sql))
                    finish_with_error(con);
                int userId = mysql_insert_id(con);

                sprintf(sql, "INSERT INTO NormalInfo (UserId, BloodGroup, Age, Height, Weight) VALUES (%d, '%s', %d, %d, %d)",
                        userId, bloodGroup, age, height, weight);
                if (mysql_query(con, sql))
                    finish_with_error(con);
            }
            else if (Choosed == 2)
            {
                char *name, *family, *natCode, *username, *password, *email, *placeName, *cityName;
                int mantaghe = -2;

                name = getInputWithSpace(100, "Name");
                if (strcmp(name, "-1") == 0)
                {
                    goBack = true;
                    break;
                }

                family = getInputWithSpace(100, "Family");
                if (strcmp(family, "-1") == 0)
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

                char repeatOfPass[51];
                while (strcmp(repeatOfPass, password))
                {
                    int i;
                    char ch1;
                    printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                    for (i = 0; i < 50; i++)
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

                placeName = getInputWithSpace(100, "Place name");
                if (strcmp(placeName, "-1") == 0)
                {
                    goBack = true;
                    break;
                }

                cityName = getInputWithSpace(100, "City name");
                if (strcmp(cityName, "-1") == 0)
                {
                    goBack = true;
                    break;
                }

                mantaghe = getPositiveInt("AreaId", 0);
                if (mantaghe == -1)
                {
                    goBack = true;
                    break;
                }

                char sql[5000];
                sprintf(sql, "INSERT INTO Users (Type, FirstName, LastName, NationalCode, Username, Password, Email, CoronaCode) VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', %d)",
                        2, name, family, natCode, username, encrypt(password), email, 1);
                if (mysql_query(con, sql))
                    finish_with_error(con);
                int userId = mysql_insert_id(con);

                sprintf(sql, "INSERT INTO BusInfo (UserId, PlaceName, CityName, AreaId) VALUES (%d, '%s', '%s', %d)",
                        userId, placeName, cityName, mantaghe);
                if (mysql_query(con, sql))
                    finish_with_error(con);
            }
        }
        if (goBack)
        {
            printf(CLEAR);
        }
        else
        {
            puts(RESET FGREEN BOLD "Your account has been created successfully!" RESET FWHITE);
            printf(RESET FWHITE "press any key to go back");
            getchar();
            printf(CLEAR);
            showLogo();
            showMainMenu();
        }
    }
}

void loginMenu()
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

        bool loggedIn = false;
        if (usernameExists(username))
        {
            password = getPassword();
            printf("\n");
            if (strcmp(password, "-1") != 0)
            {
                encrypt(password);

                user loggedUser = login(username, password);

                if (loggedUser.id == 0)
                    puts(RESET FRED "Password is incorrect!" RESET FGREEN);
                else
                {
                    if (loggedUser.coronaCode == -1)
                        printf(RESET FWHITE "Sorry, but you are %s :))!", RESET BBLACK FRED "DEAD" RESET FWHITE);
                    else
                    {
                        panelMenu(loggedUser);
                        loggedIn = true;
                    }
                }
            }
        }
        else
            puts(RESET FRED "Username not found!" RESET FGREEN);
        if (!loggedIn)
            getchar();
    }
    if (!goBack)
    {
        printf("press any key to go back");
        getchar();
    }
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
        if (usernameExists(username))
        {
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

            if (resetPassUserExists(username, email, natCode))
            {
                printf(RESET FGREEN "New "); //To print new password
                password = getPassword();
                if (strcmp(password, "-1") == 0)
                {
                    goBack = true;
                    break;
                }

                char repeatOfPass[51];
                while (strcmp(repeatOfPass, password))
                {
                    int i;
                    char ch1;
                    printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                    for (i = 0; i < 50; i++)
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

                char sql[5000];
                sprintf(sql, "UPDATE Users SET Password='%s' WHERE Username='%s'",
                        encrypt(password), username);
                if (mysql_query(con, sql))
                    finish_with_error(con);
                puts(RESET FGREEN BOLD "Password has been changed successfully!" RESET FWHITE);
                break;
            }
            else
                puts(RESET FRED "Email or Nationalcode is incorrect!" RESET FGREEN);
        }
        else
            puts(RESET FRED "Username not found!" RESET FGREEN);
        getchar();
    }
    if (!goBack)
    {
        printf("press any key to go back");
        getchar();
    }
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
    return removeNL(input);
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
    return strlwr(username);
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
    return strlwr(email);
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
        getchar();
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

void panelMenu(user loggedUser)
{
    printf(CLEAR);
    if (loggedUser.type == 1)
    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        int Choosed = 0;
        while (!Choosed || Choosed == -1)
        {
            if (Choosed == 0)
            {
                printf(CLEAR);
                printf(RESET FCYAN BOLD "\t\t\t   Welcome %s\n", loggedUser.name);

                //Show time
                tm = *localtime(&t);
                printf(RESET FWHITE "\t\t\t%d-%02d-%02d %02d:%02d:%02d\n\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                int coronaCode = loggedUser.coronaCode;
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
                if (loggedUser.coronaCode != 4)
                {
                    int coronaPercent = coronaTest();
                    if (coronaPercent != -1)
                    {
                        printf(CLEAR);
                        switch (coronaPercent)
                        {
                        case 0 ... 15:
                            loggedUser.coronaCode = 1;
                            break;
                        case 16 ... 50:
                            loggedUser.coronaCode = 2;
                            break;
                        case 51 ... 75:
                            loggedUser.coronaCode = 3;
                            break;
                        case 76 ... 100:
                            loggedUser.coronaCode = 4;
                            break;
                        }
                        updateUser(loggedUser, false);
                        int coronaCode = loggedUser.coronaCode;
                        if (coronaCode == 3 || coronaCode == 4)
                        {
                            visits userVisits = getVisitedUsers(loggedUser.id);
                            int nowUnix = time(0);
                            for (int i = 0; i < userVisits.count; i++)
                            {
                                int visitUnix = userVisits.timestamps[i];
                                if (((nowUnix - visitUnix) / 86400) <= 7)
                                {
                                    user target = getUserById(userVisits.ids[i]);
                                    if (loggedUser.coronaCode == 3 && target.coronaCode < 2)
                                    {
                                        target.coronaCode = 2;
                                        updateUser(target, false);
                                    }
                                    else if (loggedUser.coronaCode == 4 && target.coronaCode < 3)
                                    {
                                        target.coronaCode = 3;
                                        updateUser(target, false);
                                    }
                                }
                            }

                            if (coronaCode == 4)
                            {
                                visits areaVisits = getVisitedAreas(loggedUser.id);
                                for (int i = 0; i < areaVisits.count; i++)
                                {
                                    int visitUnix = areaVisits.timestamps[i];
                                    if (((nowUnix - visitUnix) / 86400) <= 7)
                                    {
                                        int uCounts = 0;
                                        user *targets = getUsersByAreaId(areaVisits.ids[i], &uCounts);
                                        for (int i = 0; i < uCounts; i++)
                                        {
                                            targets[i].coronaCode = 1;
                                            updateUser(targets[i], false);
                                        }
                                    }
                                }
                            }
                        }

                        char sql[5000];
                        sprintf(sql, "INSERT INTO CoronaTests (UserId, CoronaCode, Stamp) VALUES (%d, %d, %d)",
                                loggedUser.id, loggedUser.coronaCode, time(0));
                        if (mysql_query(con, sql))
                            finish_with_error(con);
                    }
                }
                else
                {
                    printf(CLEAR);
                    puts(RESET FRED "You are ill, you can't test any more!" RESET FGREEN);
                    getchar();
                }
            }
            else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "visit person\n"))
            {
                printf(CLEAR);
                printf(RESET FGREEN "Person ");
                char *natCode = getNatCode();
                if (strcmp(natCode, "-1") != 0)
                {
                    while (!natCodeExists(natCode) || !strcmp(loggedUser.nationalCode, natCode))
                    {
                        puts(RESET FRED "National code doesn't exist!" RESET FGREEN);
                        natCode = getNatCode();
                        if (strcmp(natCode, "-1") == 0)
                            break;
                    }

                    if (strcmp(natCode, "-1") != 0)
                    {
                        char sql[5000];
                        int userId2 = getIdByNatCode(natCode);
                        user visitedUser = getUserById(userId2);

                        int visited = alreadyVisitedu(loggedUser.id, visitedUser.id);

                        if (visited == 1)
                            sprintf(sql, "UPDATE UserVisits SET Stamp=%d WHERE UserId1=%d and UserId2=%d",
                                    time(0), loggedUser.id, visitedUser.id);
                        else if (visited == 2)
                            sprintf(sql, "UPDATE UserVisits SET Stamp=%d WHERE UserId1=%d and UserId2=%d",
                                    time(0), visitedUser.id, loggedUser.id);
                        else
                            sprintf(sql, "INSERT INTO UserVisits (UserId1, UserId2, Stamp) VALUES (%d, %d, %d)",
                                    loggedUser.id, userId2, time(0));

                        if (mysql_query(con, sql))
                            finish_with_error(con);

                        int cCode = visitedUser.coronaCode;
                        if (cCode > loggedUser.coronaCode + 1)
                        {
                            loggedUser.coronaCode = cCode - 1;
                            updateUser(loggedUser, false);
                        }
                        else if (loggedUser.coronaCode > cCode + 1)
                        {
                            visitedUser.coronaCode = loggedUser.coronaCode - 1;
                            updateUser(visitedUser, false);
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
                if (mantaghe != -1)
                {
                    bool visited = alreadyVisiteda(loggedUser.id, mantaghe);
                    char sql[5000];
                    if (visited)
                        sprintf(sql, "UPDATE AreaVisits SET Stamp=%d WHERE UserId=%d and AreaId=%d",
                                time(0), loggedUser.id, mantaghe);
                    else
                        sprintf(sql, "INSERT INTO AreaVisits (UserId, AreaId, Stamp) VALUES (%d, %d, %d)",
                                loggedUser.id, mantaghe, time(0));

                    if (mysql_query(con, sql))
                        finish_with_error(con);

                    if (loggedUser.coronaCode == 1)
                    {
                        int count = 0;
                        user *areaUsers = getUsersByAreaId(mantaghe, &count);
                        if (count > 0)
                        {
                            for (int i = 0; i < count; i++)
                            {
                                int cCode = areaUsers[i].coronaCode;
                                if (cCode == 2)
                                {
                                    loggedUser.coronaCode = 2;
                                    updateUser(loggedUser, false);
                                    break;
                                }
                            }
                        }
                    }
                    else if (loggedUser.coronaCode == 4)
                    {
                        int count = 0;
                        user *areaUsers = getUsersByAreaId(mantaghe, &count);
                        if (count > 0)
                        {
                            for (int i = 0; i < count; i++)
                            {
                                int cCode = areaUsers[i].coronaCode;
                                if (cCode == 1)
                                {
                                    areaUsers[i].coronaCode = 2;
                                    updateUser(areaUsers[i], false);
                                }
                            }
                        }
                    }
                    printf(RESET FGREEN BOLD "Successfully added!\n");
                    printf(RESET FWHITE "press any key to go back");
                    getchar();
                }
            }
            else if (!strcmp(input, "4\n") || !strcmp(strlwr(input), "tests history\n"))
            {
                printf(CLEAR);
                char sql[5000];
                sprintf(sql, "SELECT * FROM CoronaTests Where UserId=%d", loggedUser.id);
                if (mysql_query(con, sql))
                    finish_with_error(con);

                MYSQL_RES *result = mysql_store_result(con);
                if (result == NULL)
                    finish_with_error(con);

                printf(RESET FWHITE "%-3s%-15s%-30s\n", "", "Corona Condition", "Time Passed");
                MYSQL_ROW row;
                int count = 1;
                while ((row = mysql_fetch_row(result)))
                {
                    int code;
                    sscanf(row[1], "%d", &code);
                    int stamp;
                    sscanf(row[2], "%d", &stamp);

                    int nowUnix = time(0);
                    stamp = nowUnix - stamp;
                    int days = stamp / 86400;
                    int hours = (stamp - (days * 86400)) / 3600;
                    char passed[100];
                    sprintf(passed, "%d Days and %d Hours", days, hours);
                    if (code == 1)
                        printf(RESET FWHITE "%-3d%-15s%-30s\n", count, "Healthy", passed);
                    else if (code == 2)
                        printf(RESET FYELLOW "%-3d%-15s%-30s\n", count, "Suspicious", passed);
                    else if (code == 3)
                        printf(RESET FORANGE "%-3d%-15s%-30s\n", count, "Dangerous", passed);
                    else if (code == 4)
                        printf(RESET FRED "%-3d%-15s%-30s\n", count, "Ill", passed);
                    else if (code == -1)
                        printf(RESET FRED "%-3d%-15s%-30s\n", count, RESET FRED BBLACK "DEAD", passed);
                    count++;
                }

                mysql_free_result(result);

                printf(RESET FWHITE "press any key to go back");
                getchar();
            }
            else if (!strcmp(input, "5\n") || !strcmp(strlwr(input), "visits history\n"))
            {
                printf(CLEAR);
                printf(RESET FGREEN "%-3s%-20s%-15s%-30s\n", "", "Name", "National Code", "Date");

                visits userVisits = getVisitedUsers(loggedUser.id);
                for (int i = 0; i < userVisits.count; i++)
                {
                    int visitUnix = userVisits.timestamps[i];
                    user target = getUserById(userVisits.ids[i]);
                    int nowUnix = time(0);
                    visitUnix = nowUnix - visitUnix;
                    int days = visitUnix / 86400;
                    int hours = (visitUnix - (days * 86400)) / 3600;
                    char passed[100];
                    sprintf(passed, "%d Days and %d Hours", days, hours);
                    printf(RESET FCYAN "%-3d%-20s%-15s%-30s\n", i + 1, target.name, target.nationalCode, passed);
                }
                printf("\n\n");

                visits areaVisits = getVisitedAreas(loggedUser.id);
                printf(RESET FGREEN "%-3s%-20s%-30s\n", "", "AreaId", "Date");
                for (int i = 0; i < areaVisits.count; i++)
                {
                    int visitUnix = areaVisits.timestamps[i];
                    int nowUnix = time(0);
                    visitUnix = nowUnix - visitUnix;
                    int days = visitUnix / 86400;
                    int hours = (visitUnix - (days * 86400)) / 3600;
                    char passed[100];
                    sprintf(passed, "%d Days and %d Hours", days, hours);
                    printf(RESET FCYAN "%-3d%-20d%-30s\n", i + 1, areaVisits.ids[i], passed);
                }

                printf(RESET FWHITE "press any key to go back");
                getchar();
            }
            else if (!strcmp(input, "6\n") || !strcmp(strlwr(input), "edit profile\n"))
                loggedUser = editNormalProfile(loggedUser);
            else if (!strcmp(input, "7\n") || !strcmp(strlwr(input), "lock account\n"))
                break;
            else if (!strcmp(input, "8\n") || !strcmp(strlwr(input), "exit app\n"))
                exit(0);
            else if (!strcmp(input, "-1\n"))
                break;
            else
            {
                printf("%s", RESET FRED "Wrong input!\n\n" FWHITE);
                printf(FWHITE "Input: ");
                Choosed = -1;
            }
        }
    }
    else if (loggedUser.type == 2)
    {
        //Show time
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        int Choosed = 0;
        while (!Choosed || Choosed == -1)
        {
            if (Choosed == 0)
            {
                printf(CLEAR);
                tm = *localtime(&t);
                printf(RESET BOLD FPANEL2 "\t\t\t   Welcome %s\n", loggedUser.name);
                printf(RESET FWHITE "\t\t\t%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

                if (loggedUser.coronaCode == 1)
                    puts(RESET FWHITE "Area Corona Code: White");
                else if (loggedUser.coronaCode == 2)
                    puts(RESET FRED "Area Corona Code: Red");
                printf(RESET FWHITE "Your AreaId: %d\n\n", loggedUser.businfo.areaId);

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
                if (loggedUser.coronaCode == 2)
                {
                    loggedUser.coronaCode = 1;
                    int count = 0;
                    user *users = getUsersByAreaId(loggedUser.businfo.areaId, &count);
                    char sql[5000];
                    for (int i = 0; i < count; i++)
                    {
                        sprintf(sql, "UPDATE Users SET CoronaCode=1 WHERE Id=%d",
                                users[i].id);
                        if (mysql_query(con, sql))
                            finish_with_error(con);
                    }
                }
            }
            else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "edit profile\n"))
            {
                loggedUser = editBusProfile(loggedUser);
            }
            else if (!strcmp(input, "3\n") || !strcmp(strlwr(input), "lock account\n"))
            {
                Choosed = 3;
                break;
            }
            else if (!strcmp(input, "4\n") || !strcmp(strlwr(input), "exit app\n"))
                exit(0);
            else if (!strcmp(input, "-1\n"))
            {
                Choosed = 3;
                break;
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
    else if (loggedUser.type == 3)
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
            fgets(input, 25, stdin);
            if (!strcmp(input, "1\n") || !strcmp(strlwr(input), "show users\n"))
            {
                printf(CLEAR);
                printf(RESET FGREEN "%-3s%-10s%-30s%-20s%-20s\n", "", "Type", "Full Name", "National Code", "Corona Condition");
                printf(RESET FADMIN);
                int count = 0;
                user *AllUsers = getAllUsers(&count);
                for (int i = 0; i < count; i++)
                {
                    char FullName[200];
                    sprintf(FullName, "%s %s", AllUsers[i].name, AllUsers[i].lastName);

                    char CoronaCondition[50];
                    if (AllUsers[i].coronaCode == 1)
                        strcpy(CoronaCondition, RESET FWHITE "Healthy" RESET FADMIN);
                    else if (AllUsers[i].coronaCode == 2)
                        strcpy(CoronaCondition, RESET FYELLOW "Suspicious" RESET FADMIN);
                    else if (AllUsers[i].coronaCode == 3)
                        strcpy(CoronaCondition, RESET FORANGE "Dangerous" RESET FADMIN);
                    else if (AllUsers[i].coronaCode == 4)
                        strcpy(CoronaCondition, RESET FRED "Ill" RESET FADMIN);
                    else if (AllUsers[i].coronaCode == -1)
                        strcpy(CoronaCondition, RESET FRED BBLACK "DEAD" RESET FADMIN);
                    printf(RESET FADMIN "%-3d%-10d%-30s%-20s%-20s\n", i + 1, AllUsers[i].type, FullName, AllUsers[i].nationalCode, CoronaCondition);
                }
                printf(RESET FWHITE "press any key to go back");
                getchar();
            }
            else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "change user status\n"))
            {
                printf(CLEAR);
                char *natCode = getNatCode();
                if (strcmp(natCode, "-1") != 0)
                {
                    while (!natCodeExists(natCode))
                    {
                        puts(RESET FRED "National code doesn't exist!" RESET FGREEN);
                        natCode = getNatCode();
                        if (strcmp(natCode, "-1") == 0)
                            break;
                    }

                    if (strcmp(natCode, "-1") != 0)
                    {
                        int uId = getIdByNatCode(natCode);
                        user targetUser = getUserById(uId);
                        if (targetUser.coronaCode == 4 || targetUser.coronaCode == -1)
                        {
                            int inp = -2;
                            while (inp != -1 && inp != 1 && inp != 2 && inp != 3 && inp != 4)
                            {
                                printf(RESET FGREEN);
                                printf("New Corona Code(-1 For dead): " RESET FWHITE);
                                scanf("%d", &inp);
                                getchar();
                                if (inp != -1 && inp != 1 && inp != 2 && inp != 3 && inp != 4)
                                    printf(RESET FRED "Corona code can only be -1/1/2/3/4\n");
                            }
                            targetUser.coronaCode = inp;
                            updateUser(targetUser, false);
                            printf(RESET FGREEN BOLD "Successfully changed!\n");
                            printf(RESET FWHITE "press any key to go back");
                            getchar();
                        }
                        else
                        {
                            printf(RESET FRED "Users corona code is %d, You cant edit it!\n", targetUser.coronaCode);
                            printf(RESET FWHITE "press any key to go back");
                            getchar();
                        }
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
                    char repeatOfPass[51];
                    while (strcmp(repeatOfPass, password))
                    {
                        int i;
                        char ch1;
                        printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                        for (i = 0; i < 50; i++)
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
                    strcpy(loggedUser.password, encrypt(password));
                    updateUser(loggedUser, false);
                    puts(RESET FGREEN BOLD "Password has been changed successfully!" RESET FWHITE);
                    getchar();
                }
            }
            else if (!strcmp(input, "4\n") || !strcmp(strlwr(input), "lock account\n"))
                break;
            else if (!strcmp(input, "5\n") || !strcmp(strlwr(input), "exit app\n"))
                exit(0);
            else if (!strcmp(input, "-1\n"))
                break;
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

user editNormalProfile(user loggedUser)
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

        bool updateInfo = false;
        char input[25];
        fgets(input, 25, stdin); // To Get Spaces between characters too
        if (!strcmp(input, "1\n") || !strcmp(strlwr(input), "name\n"))
        {
            char *name;
            printf(CLEAR);
            printf("Your name: %s\n", loggedUser.name);
            name = getInputWithSpace(100, "Name");
            if (strcmp(name, "-1") != 0)
            {
                strcpy(loggedUser.name, name);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "family\n"))
        {
            char *family;
            printf(CLEAR);
            printf("Your family: %s\n", loggedUser.lastName);
            family = getInputWithSpace(100, "Family");
            if (strcmp(family, "-1") != 0)
            {
                strcpy(loggedUser.lastName, family);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "3\n") || !strcmp(strlwr(input), "national code\n"))
        {
            printf(CLEAR);
            printf("Your national code: %s\n", loggedUser.nationalCode);
            char *natCode = getNatCode();
            if (strcmp(natCode, "-1") != 0)
            {
                while (natCodeExists(natCode))
                {
                    puts(RESET FRED "National code exists!" RESET FGREEN);
                    natCode = getNatCode();
                    if (strcmp(natCode, "-1") == 0)
                        break;
                }

                if (strcmp(natCode, "-1") != 0)
                {
                    strcpy(loggedUser.nationalCode, natCode);
                    Choosed = 1;
                }
            }
        }
        else if (!strcmp(input, "4\n") || !strcmp(strlwr(input), "password\n"))
        {
            printf(CLEAR);
            char *password = getPassword();
            if (strcmp(password, "-1") != 0)
            {
                char repeatOfPass[51];
                while (strcmp(repeatOfPass, password))
                {
                    int i;
                    char ch1;
                    printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                    for (i = 0; i < 50; i++)
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
                strcpy(loggedUser.password, encrypt(password));
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "5\n") || !strcmp(strlwr(input), "email\n"))
        {
            printf(CLEAR);
            printf("Your email: %s\n", loggedUser.email);
            char *email = getEmail();
            if (strcmp(email, "-1") != 0)
            {
                strcpy(loggedUser.email, email);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "6\n") || !strcmp(strlwr(input), "blood group\n"))
        {
            printf(CLEAR);
            printf("Your blood group: %s\n", loggedUser.normalinfo.bloodGroup);
            char *bloodGroup = getBloodGroup();
            if (bloodGroup)
            {
                strcpy(loggedUser.normalinfo.bloodGroup, bloodGroup);
                updateInfo = true;
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "7\n") || !strcmp(strlwr(input), "age\n"))
        {
            printf(CLEAR);
            printf("Your age: %d\n", loggedUser.normalinfo.age);
            int age = getPositiveInt("Age", 1);
            if (age != -1)
            {
                loggedUser.normalinfo.age = age;
                updateInfo = true;
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "8\n") || !strcmp(strlwr(input), "height\n"))
        {
            printf(CLEAR);
            printf("Your height: %d\n", loggedUser.normalinfo.height);
            int height = getPositiveInt("Height", 1);
            if (height != -1)
            {
                loggedUser.normalinfo.height = height;
                updateInfo = true;
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "9\n") || !strcmp(strlwr(input), "weight\n"))
        {
            printf(CLEAR);
            printf("Your weight: %d\n", loggedUser.normalinfo.weight);
            int weight = getPositiveInt("Weight", 1);
            if (weight != -1)
            {
                loggedUser.normalinfo.weight = weight;
                updateInfo = true;
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
            updateUser(loggedUser, updateInfo);
            puts(RESET FGREEN BOLD "Your account has been updated successfully!" RESET FWHITE);
            printf(RESET FWHITE "press any key to go back");
            getchar();
        }
    }
    return loggedUser;
}

user editBusProfile(user loggedUser)
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

        bool updateInfo = false;
        char input[25];
        fgets(input, 25, stdin); // To Get Spaces between characters too
        if (!strcmp(input, "1\n") || !strcmp(strlwr(input), "name\n"))
        {
            char *name;
            printf(CLEAR);
            printf("Your name: %s\n", loggedUser.name);
            name = getInputWithSpace(100, "Name");
            if (strcmp(name, "-1") != 0)
            {
                strcpy(loggedUser.name, name);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "2\n") || !strcmp(strlwr(input), "family\n"))
        {
            char *family;
            printf(CLEAR);
            printf("Your family: %s\n", loggedUser.lastName);
            family = getInputWithSpace(100, "Family");
            if (strcmp(family, "-1") != 0)
            {
                strcpy(loggedUser.lastName, family);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "3\n") || !strcmp(strlwr(input), "national code\n"))
        {
            printf(CLEAR);
            printf("Your national code: %s\n", loggedUser.nationalCode);
            char *natCode = getNatCode();
            if (strcmp(natCode, "-1") != 0)
            {
                while (natCodeExists(natCode))
                {
                    puts(RESET FRED "National code exists!" RESET FGREEN);
                    natCode = getNatCode();
                    if (strcmp(natCode, "-1") == 0)
                        break;
                }

                if (strcmp(natCode, "-1") != 0)
                {
                    strcpy(loggedUser.nationalCode, natCode);
                    Choosed = 1;
                }
            }
        }
        else if (!strcmp(input, "4\n") || !strcmp(strlwr(input), "password\n"))
        {
            printf(CLEAR);
            char *password = getPassword();
            if (strcmp(password, "-1") != 0)
            {
                char repeatOfPass[51];
                while (strcmp(repeatOfPass, password))
                {
                    int i;
                    char ch1;
                    printf(RESET FGREEN "\nRepeat Password: " RESET FWHITE);
                    for (i = 0; i < 50; i++)
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
                strcpy(loggedUser.password, encrypt(password));
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "5\n") || !strcmp(strlwr(input), "email\n"))
        {
            printf(CLEAR);
            printf("Your email: %s\n", loggedUser.email);
            char *email = getEmail();
            if (strcmp(email, "-1") != 0)
            {
                strcpy(loggedUser.email, email);
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "6\n") || !strcmp(strlwr(input), "place name\n"))
        {
            printf(CLEAR);
            printf("Place name: %s\n", loggedUser.businfo.placeName);
            char *placeName = getInputWithSpace(100, "Place name");
            if (strcmp(placeName, "-1") != 0)
            {
                strcpy(loggedUser.businfo.placeName, placeName);
                updateInfo = true;
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "7\n") || !strcmp(strlwr(input), "city name\n"))
        {
            printf(CLEAR);
            printf("City name: %s\n", loggedUser.businfo.cityName);
            char *cityName = getInputWithSpace(100, "City name");
            if (strcmp(cityName, "-1") != 0)
            {
                strcpy(loggedUser.businfo.cityName, cityName);
                updateInfo = true;
                Choosed = 1;
            }
        }
        else if (!strcmp(input, "8\n") || !strcmp(strlwr(input), "area\n"))
        {
            printf(CLEAR);
            printf("AreaId: %d\n", loggedUser.businfo.areaId);
            int mantaghe = getPositiveInt("Area", 0);
            if (mantaghe != -1)
            {
                loggedUser.businfo.areaId = mantaghe;
                updateInfo = true;
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
            updateUser(loggedUser, updateInfo);
            puts(RESET FGREEN BOLD "Your account has been updated successfully!" RESET FWHITE);
            printf(RESET FWHITE "press any key to go back");
            getchar();
        }
    }
    return loggedUser;
}

bool natCodeExists(char *natCode)
{
    char sql[5000];
    bool res = false;
    sprintf(sql, "SELECT EXISTS(SELECT * FROM Users WHERE NationalCode='%s')", natCode);
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
        finish_with_error(con);

    MYSQL_ROW row;

    if ((row = mysql_fetch_row(result)))
        if (strcmp(row[0], "1") == 0)
            res = true;

    mysql_free_result(result);
    return res;
}

bool usernameExists(char *username)
{
    if (strcmp(username, "admin") == 0)
        return true;

    char sql[5000];
    bool res = false;
    sprintf(sql, "SELECT EXISTS(SELECT * FROM Users WHERE Username='%s')", username);
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
        finish_with_error(con);

    MYSQL_ROW row;

    if ((row = mysql_fetch_row(result)))
        if (strcmp(row[0], "1") == 0)
            res = true;

    mysql_free_result(result);
    return res;
}

user login(char *username, char *password)
{
    char sql[5000];
    sprintf(sql, "SELECT * FROM Users Where Username='%s' and Password='%s'", username, password);
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
        finish_with_error(con);

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    MYSQL_FIELD *field;

    user loggedUser;
    if ((row = mysql_fetch_row(result)))
    {
        int uid;
        sscanf(row[0], "%d", &uid);
        loggedUser = getUserById(uid);
    }
    else
        loggedUser.id = 0;
    return loggedUser;
}

void updateUser(user theUser, bool shouldUpdateInfo)
{
    char sql[5000];
    sprintf(sql, "UPDATE Users SET FirstName='%s', LastName= '%s', NationalCode= '%s', Username= '%s', Password= '%s', Email= '%s', CoronaCode= '%d' WHERE Id=%d",
            theUser.name, theUser.lastName, theUser.nationalCode, theUser.username, theUser.password, theUser.email, theUser.coronaCode, theUser.id);

    if (mysql_query(con, sql))
        finish_with_error(con);

    if (shouldUpdateInfo)
    {
        if (theUser.type == 1)
        {
            sprintf(sql, "UPDATE NormalInfo SET BloodGroup='%s', Age= '%d', Height= '%d', Weight= '%d' WHERE UserId=%d",
                    theUser.normalinfo.bloodGroup, theUser.normalinfo.age, theUser.normalinfo.height, theUser.normalinfo.weight, theUser.id);
        }
        else
        {
            sprintf(sql, "UPDATE BusInfo SET PlaceName='%s', CityName= '%s', AreaId= '%d' WHERE UserId=%d",
                    theUser.businfo.placeName, theUser.businfo.cityName, theUser.businfo.areaId, theUser.id);
        }
        if (mysql_query(con, sql))
            finish_with_error(con);
    }
}

visits getVisitedAreas(int userId)
{
    char sql[5000];
    sprintf(sql, "SELECT * FROM AreaVisits Where UserId=%d", userId);
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
        finish_with_error(con);

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    MYSQL_FIELD *field;

    int count = 0;
    int *ids = (int *)malloc(sizeof(int));
    int *stamps = (int *)malloc(sizeof(int));
    while ((row = mysql_fetch_row(result)))
    {
        count++;
        if (count != 1)
        {
            ids = realloc(ids, sizeof(int) * count);
            stamps = realloc(stamps, sizeof(int) * count);
        }
        sscanf(row[1], "%d", &ids[count - 1]);
        sscanf(row[2], "%d", &stamps[count - 1]);
    }

    visits uvisits;
    uvisits.count = count;
    uvisits.ids = ids;
    uvisits.timestamps = stamps;
    mysql_free_result(result);
    return uvisits;
}

user getUserById(int userId)
{
    char sql[5000];
    sprintf(sql, "SELECT * FROM Users Where Id=%d", userId);
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
        finish_with_error(con);

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    MYSQL_FIELD *field;

    user loggedUser;
    row = mysql_fetch_row(result);
    sscanf(row[0], "%d", &loggedUser.id);
    sscanf(row[1], "%d", &loggedUser.type);
    strcpy(loggedUser.name, row[2]);
    strcpy(loggedUser.lastName, row[3]);
    strcpy(loggedUser.nationalCode, row[4]);
    strcpy(loggedUser.username, row[5]);
    strcpy(loggedUser.password, row[6]);
    strcpy(loggedUser.email, row[7]);
    sscanf(row[8], "%d", &loggedUser.coronaCode);
    if (loggedUser.type == 1)
    {
        sprintf(sql, "SELECT * FROM NormalInfo Where UserId=%d", loggedUser.id);
        if (mysql_query(con, sql))
            finish_with_error(con);

        MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL)
            finish_with_error(con);

        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;
        MYSQL_FIELD *field;

        if ((row = mysql_fetch_row(result)))
        {
            strcpy(loggedUser.normalinfo.bloodGroup, row[1]);
            sscanf(row[2], "%d", &loggedUser.normalinfo.age);
            sscanf(row[3], "%d", &loggedUser.normalinfo.height);
            sscanf(row[4], "%d", &loggedUser.normalinfo.weight);
        }
    }
    else if (loggedUser.type == 2)
    {
        sprintf(sql, "SELECT * FROM BusInfo Where UserId=%d", loggedUser.id);
        if (mysql_query(con, sql))
            finish_with_error(con);

        MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL)
            finish_with_error(con);

        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;
        MYSQL_FIELD *field;

        if ((row = mysql_fetch_row(result)))
        {
            strcpy(loggedUser.businfo.placeName, row[1]);
            strcpy(loggedUser.businfo.cityName, row[2]);
            sscanf(row[3], "%d", &loggedUser.businfo.areaId);
        }
    }

    mysql_free_result(result);
    return loggedUser;
}

user *getUsersByAreaId(int areaId, int *count)
{
    char sql[5000];
    sprintf(sql, "SELECT * FROM BusInfo Where AreaId=%d", areaId);
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
        finish_with_error(con);

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    MYSQL_FIELD *field;

    user *users = malloc(sizeof(user));
    *count = 0;
    while ((row = mysql_fetch_row(result)))
    {
        (*count)++;
        if (*count != 1)
            users = realloc(users, *count * sizeof(user));
        int uid;
        sscanf(row[0], "%d", &uid);
        users[*count - 1] = getUserById(uid);
    }
    mysql_free_result(result);
    return users;
}

user *getAllUsers(int *count)
{
    char sql[5000];
    sprintf(sql, "SELECT * FROM Users WHERE NOT Username='admin'");
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
        finish_with_error(con);

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    MYSQL_FIELD *field;

    user *users = malloc(sizeof(user));
    *count = 0;
    while ((row = mysql_fetch_row(result)))
    {
        (*count)++;
        if (*count != 1)
            users = realloc(users, *count * sizeof(user));

        user selectedUser;
        sscanf(row[0], "%d", &selectedUser.id);
        sscanf(row[1], "%d", &selectedUser.type);
        strcpy(selectedUser.name, row[2]);
        strcpy(selectedUser.lastName, row[3]);
        strcpy(selectedUser.nationalCode, row[4]);
        strcpy(selectedUser.username, row[5]);
        strcpy(selectedUser.password, row[6]);
        strcpy(selectedUser.email, row[7]);
        sscanf(row[8], "%d", &selectedUser.coronaCode);
        users[*count - 1] = selectedUser;
    }
    mysql_free_result(result);
    return users;
}

visits getVisitedUsers(int userId)
{
    char sql[5000];
    sprintf(sql, "SELECT * FROM UserVisits Where UserId1=%d OR UserId2=%d", userId, userId);
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
        finish_with_error(con);

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    MYSQL_FIELD *field;

    int count = 0;
    int *ids = (int *)malloc(sizeof(int));
    int *stamps = (int *)malloc(sizeof(int));
    while ((row = mysql_fetch_row(result)))
    {
        count++;
        if (count != 1)
        {
            ids = realloc(ids, sizeof(int) * count);
            stamps = realloc(stamps, sizeof(int) * count);
        }
        int uId;
        sscanf(row[0], "%d", &uId);
        if (uId == userId)
            sscanf(row[1], "%d", &uId);
        ids[count - 1] = uId;
        sscanf(row[2], "%d", &stamps[count - 1]);
    }

    visits uvisits;
    uvisits.count = count;
    uvisits.ids = ids;
    uvisits.timestamps = stamps;
    mysql_free_result(result);
    return uvisits;
}

int getIdByNatCode(char *natCode)
{
    char sql[5000];
    sprintf(sql, "SELECT * FROM Users Where NationalCode='%s'", natCode);
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
        finish_with_error(con);

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    MYSQL_FIELD *field;

    int userId;
    row = mysql_fetch_row(result);
    sscanf(row[0], "%d", &userId);
    mysql_free_result(result);
    return userId;
}

int alreadyVisitedu(int userId1, int userId2)
{
    char sql[5000];
    int res = 0;
    sprintf(sql, "SELECT EXISTS(SELECT * FROM UserVisits WHERE UserId1=%d and UserId2=%d)", userId1, userId2);
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
        finish_with_error(con);

    MYSQL_ROW row;

    if ((row = mysql_fetch_row(result)))
        if (strcmp(row[0], "1") == 0)
            res = 1;
    mysql_free_result(result);
    if (res)
        return res;

    sprintf(sql, "SELECT EXISTS(SELECT * FROM UserVisits WHERE UserId1=%d and UserId2=%d)", userId2, userId1);
    if (mysql_query(con, sql))
        finish_with_error(con);

    result = mysql_store_result(con);
    if (result == NULL)
        finish_with_error(con);

    if ((row = mysql_fetch_row(result)))
        if (strcmp(row[0], "1") == 0)
            res = 2;

    return res;
}

bool alreadyVisiteda(int userId, int areaId)
{
    char sql[5000];
    bool res = false;
    sprintf(sql, "SELECT EXISTS(SELECT * FROM AreaVisits WHERE UserId=%d and AreaId=%d)", userId, areaId);
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
        finish_with_error(con);

    MYSQL_ROW row;

    if ((row = mysql_fetch_row(result)))
        if (strcmp(row[0], "1") == 0)
            res = true;

    mysql_free_result(result);
    return res;
}

bool resetPassUserExists(char *username, char *email, char *natCode)
{
    char sql[5000];
    bool res = false;
    sprintf(sql, "SELECT EXISTS(SELECT * FROM Users WHERE Username='%s' and Email='%s' and NationalCode='%s')", username, email, natCode);
    if (mysql_query(con, sql))
        finish_with_error(con);

    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
        finish_with_error(con);

    MYSQL_ROW row;

    if ((row = mysql_fetch_row(result)))
        if (strcmp(row[0], "1") == 0)
            res = true;

    mysql_free_result(result);
    return res;
}