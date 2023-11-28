/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
|-----------------------------------------------------------------------------------------------|
|          D I A R Y   M A N A G E M E N T   S Y S T E M   D O C U M E N T A T I O N S          |
|-----------------------------------------------------------------------------------------------|


Release         :           v1.0.0

Contributors    :           Sorbopriyo Roy, Rima Raj, Puja Kumari, Gulshan Kumar, Aman Kumar Patwa

Release Notes   :           This is a Diary System/Journalling System to help different users to maintain their personal diaries.
                            These diaries can only be accessed by its authors. All the data is stored in FILE SYSTEM in binary
                            format.This prevents cheaters from accessing unauthorised contents.
                            All the features are listed below -
                                1. New user registration
                                2. User authorisation
                                3. Writing new diary
                                4. Reading old diaries
                                5. Querying all old diaries in a single list
                            This program has been designed while keeping lot of edge cases in mind. Still there maybe some bugs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

// Libraries
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

// Global Declarations
#define USERDB "users.bin"
#define DIARYDB "diary.bin"
char currentSessionUserID = '\0';

typedef struct diaryUser
{
    char username[20];
    char password[25];
    char name[50];
    char email[100];
} user;

typedef struct DiaryEntry
{
    int diaryID;
    char username[20];
    char writeDate[50];
    char diaryName[50];
    char content[1000];
} diary;

// Function Prototypes
int generateNewID();
void mainMenu();
void login();
void signup();
void addEntry();
void displayEntries();
void searchEntry();
bool isEmptyString(char arr[]);
bool isThereSpaceInString(char arr[]);
bool linearSearchUserFile(char username[]);

// The Authentication UI
int main()
{
    int choice;
    do
    {
        // If active session found, then calling main menu
        if (currentSessionUserID != '\0')
            mainMenu();

        // Authentication Menu
        system("cls");
        printf("|----------D I A R Y   M A N A G E M E N T   S Y S T E M----------|\n");
        printf("|                         Authentication                          |\n");
        printf("|-----------------------------------------------------------------|\n");
        printf("1. Sign in\n");
        printf("2. Sign up\n");
        printf("0. Enter anything to exit\n");
        printf("Enter your choice: ");

        // Input Handling
        if (scanf("%d", &choice) != 1)
        {
            goto shutdown;
        }
        fflush(stdin);

        // Menu Action Logic
        switch (choice)
        {
        case 1:
            login();
            break;
        case 2:
            signup();
            break;
        default:
        shutdown:
            printf("-> Closing the program...\n");
            getch();
            return 0;
        }
    } while (true);
    return 0;
}

// The Main Menu UI
void mainMenu()
{
    int choice;
    do
    {
        // Diary Menu
        system("cls");
        printf("|----------D I A R Y   M A N A G E M E N T   S Y S T E M----------|\n");
        printf("|                            Main Menu                            |\n");
        printf("|-----------------------------------------------------------------|\n");
        printf("1. Write something\n");
        printf("2. Read your diaries\n");
        printf("3. Show all diaries\n");
        printf("0. Enter anything to logout\n");

        // Input Handling
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            fflush(stdin);
            goto logout;
        }
        else
            fflush(stdin);

        // Diary menu logic
        switch (choice)
        {
        case 1:
            addEntry();
            break;
        case 2:
            displayEntries();
            break;
        case 3:
            searchEntry();
            break;
        default:
        logout:
            printf("-> Logging out\n");
            currentSessionUserID = '\0';
            getch();
            break;
        }
    } while (currentSessionUserID != '\0');
}

// Logging in a user
void login()
{
    system("cls");
    // Declarations
    FILE *userFilePTR;
    user iterator;
    char userID[20], password[25];

    // Opening the file
    userFilePTR = fopen(USERDB, "rb");
    if (userFilePTR == NULL)
    {
        printf("-> This is the first time this program is being used. You must create an user account first\nPress any key...\n");
        getch();
        return;
    }

    // Login UI
    printf("|----------D I A R Y   M A N A G E M E N T   S Y S T E M----------|\n");
    printf("|                              LOGIN                              |\n");
    printf("|-----------------------------------------------------------------|\n");

    // Input Handling
    printf("Username: ");
    scanf("%[^\n]s", &userID);
    fflush(stdin);
    printf("Password: ");
    scanf("%[^\n]s", &password);
    fflush(stdin);

    // Authentication Logic
    while (fread(&iterator, 1, sizeof(user), userFilePTR))
    {
        if (strcmp(userID, iterator.username) == 0 && strcmp(password, iterator.password) == 0)
        {
            printf("Login successful. Welcome %s\nPress any key to continue...", iterator.name);
            strcpy(&currentSessionUserID, iterator.username);
            fclose(userFilePTR);
            getch();
            return;
        }
    }
    printf("Wrong username or password. Press any key...");
    fclose(userFilePTR);
    getch();
}

// Registering New User
void signup()
{
    system("cls");
    // Declaration
    FILE *userFilePTR;
    char pwd1[25], name[50], fname[50], lname[50];
    user new;

    // r, w, ab - > append in binary mode
    // Opening the file
    userFilePTR = fopen(USERDB, "ab");
    if (userFilePTR == NULL)
    {
        printf("Can't open files. Press any key...\n");
        getch();
        return;
    }

    // Sign up UI
    printf("|----------D I A R Y   M A N A G E M E N T   S Y S T E M----------|\n");
    printf("|                             SIGN UP                             |\n");
    printf("|-----------------------------------------------------------------|\n");

    // Username Input
    do
    {
        printf("Username: ");
        scanf("%[^\n]s", &new.username);
        fflush(stdin);
        if (linearSearchUserFile(new.username))
            printf("Username has already been taken, take another!\n\n");
        if (isThereSpaceInString(new.username))
            printf("Username cannot have spaces!\n\n");
        if (isEmptyString(new.username))
            printf("Username cannot be empty\n\n");
    } while (linearSearchUserFile(new.username) || isThereSpaceInString(new.username) || isEmptyString(new.username));

    // Password Input
    do
    {
        printf("Password: ");
        scanf("%[^\n]s", &new.password);
        fflush(stdin);
        if (isThereSpaceInString(new.password))
        {
            printf("Password cannot have spaces!\n\n");
            goto loop;
        }
        if (isEmptyString(new.password))
        {
            printf("Password cannot be empty\n\n");
            goto loop;
        }
        printf("Type Password again: ");
        scanf("%[^\n]s", &pwd1);
        fflush(stdin);
        if (strcmp(new.password, pwd1) != 0)
            printf("Passwords didn't match, try again...\n\n");
    loop:
    } while (strcmp(new.password, pwd1) || isThereSpaceInString(new.password) || isEmptyString(new.password));

    // Name Input
    printf("First Name: ");
    scanf("%s", &fname);
    fflush(stdin);
    printf("Last Name: ");
    scanf("%s", &lname);
    fflush(stdin);
    strcat(name, fname);
    strcat(name, " ");
    strcat(name, lname);
    strcpy(new.name, name);

    // Email Input
    printf("Email: ");
    scanf("%s", &new.email);
    fflush(stdin);

    // Registering new user
    if (fwrite(&new, 1, sizeof(user), userFilePTR))
        printf("You are now registered.\nUsername is: %s\nPassword is: %s\nPress any key...", new.username, new.password);
    else
        printf("Registering unsuccessful :(\nPress any key...");
    fclose(userFilePTR);
    getch();
}

// Adding new diary
void addEntry()
{
    system("cls");
    // Declarations
    diary new;
    FILE *diaryFilePTR;
    time_t currentTime;

    // Opening the file
    diaryFilePTR = fopen(DIARYDB, "ab");
    if (diaryFilePTR == NULL)
    {
        printf("Can't open files. Press any key...\n");
        getch();
        return;
    }

    // Inputs
    new.diaryID = generateNewID();
    time(&currentTime);
    strcpy(new.writeDate, ctime(&currentTime));
    strcpy(new.username, &currentSessionUserID);
    printf("Diary name: ");
    scanf("%[^\n]s", new.diaryName);
    fflush(stdin);
    printf("Write your thoughts: ");
    scanf("%[^\n]s", new.content);
    fflush(stdin);

    // Saving the data
    if (fwrite(&new, 1, sizeof(diary), diaryFilePTR))
        printf("New Diary has been added to the library. Press any key...\n");
    else
        printf("Some error occured. Press any key...\n");
    fclose(diaryFilePTR);
    getch();
}

// Display a particular diary
void displayEntries()
{
    system("cls");
    // Declarations
    int id;
    FILE *diaryFilePTR;
    diary iterator;

    // Opening the file
    diaryFilePTR = fopen(DIARYDB, "rb");
    if (diaryFilePTR == NULL)
    {
        printf("This is the first time this program is being used. You must create an diary to see. Press any key...\n");
        getch();
        return;
    }

    // Search Query
    printf("Enter the Diary ID that you want to see: ");
    if (scanf("%d", &id) != 1)
        goto invalid;

    while (fread(&iterator, 1, sizeof(diary), diaryFilePTR))
    {
        if (iterator.diaryID == id && strcmp(iterator.username, &currentSessionUserID) == 0)
        {
            printf("Diary ID: %d\n", iterator.diaryID);
            printf("Diary Name: %s\n", iterator.diaryName);
            printf("Date & Time: %s", iterator.writeDate);
            printf("Content: %s\n\n", iterator.content);
            printf("Press any key to return...");
            fflush(stdin);
            fclose(diaryFilePTR);
            getch();
            return;
        }
    }
invalid:
    printf("You have entered invalid diary ID. Press any key...");
    fflush(stdin);
    fclose(diaryFilePTR);
    getch();
}

// Search diaries by name
void searchEntry()
{
    system("cls");
    // Declarations
    int count = 0;
    FILE *diaryFilePTR;
    diary iterator;

    // Opening the file
    diaryFilePTR = fopen(DIARYDB, "rb");
    if (diaryFilePTR == NULL)
    {
        printf("This is the first time this program is being used. You must create an diary to see. Press any key...\n");
        getch();
        return;
    }

    printf("|----------D I A R Y   M A N A G E M E N T   S Y S T E M----------|\n");
    printf("|                         Search Results                          |\n");
    printf("|-----------------------------------------------------------------|\n");

    while (fread(&iterator, 1, sizeof(diary), diaryFilePTR))
    {
        if (strcmp(iterator.username, &currentSessionUserID) == 0)
        {
            count++;
            printf("%d\t\t%s\t\t%s", iterator.diaryID, iterator.diaryName, iterator.writeDate);
        }
    }
    printf("\n%d search results\n", count);
    fclose(diaryFilePTR);
    printf("Press any key...");
    getch();
}

// Generates a new diary ID
int generateNewID()
{
    int count = 0;
    FILE *diaryFilePTR;
    diary iterator;
    diaryFilePTR = fopen(DIARYDB, "rb");
    while (fread(&iterator, 1, sizeof(diary), diaryFilePTR))
    {
        count++;
    }
    fclose(diaryFilePTR);
    return count + 1;
}

// Checks if there is any space in the string
bool isThereSpaceInString(char arr[])
{
    for (int i = 0; arr[i] != '\0'; i++)
    {
        if (arr[i] == ' ')
            return true;
    }
    return false;
}

// Checks if the string is empty or not
bool isEmptyString(char arr[])
{
    if (arr[0] == '\0')
        return true;
    return false;
}

// Searches if a user is present with username or not
bool linearSearchUserFile(char username[])
{
    FILE *userFilePTR;
    user iterator;

    userFilePTR = fopen(USERDB, "rb");
    if (userFilePTR == NULL)
    {
        return false;
    }

    while (fread(&iterator, 1, sizeof(user), userFilePTR))
    {
        if (strcmp(username, iterator.username) == 0)
        {
            fclose(userFilePTR);
            return true;
        }
    }
    fclose(userFilePTR);
    return false;
}
