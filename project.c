#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <time.h>
#include <conio.h>
/*NB: our code uses a delaying function, so there might be some delays .. that's not an error, just dilation to let users read messages slowly*/

void delay(int s) ///function to delay the execution of the next command to allow user read messages slowly
{
    int ms = 1000 * s;
    clock_t start_time = clock();
    while (clock() < start_time + ms);
}
typedef struct
{
    int day, month, year;
} DATE;
typedef struct
{
    char last[100];
    char first[100];
    DATE dob;
    char address[100];
    char phone[20];
    char email [100];
} CONTACT;

CONTACT phonebook[100000];
char filename[100];
int count = 0, loaded = 0, save =1; ///count -> counts no of records ,, loaded -> indicator if file is loaded ,, save -> indicator if file is saved

void menu();
void printrecord(int j);
void printrecordhead();
int checkloaded();
void enddisplay(char *string);
int digitscheck(char *s);
void phonenumberEntry(int i);
void phonenumberCheck(char * string);
int emailCheck(char * string);
int startCheck(char string[]);
int atCheck (char string[]);
int dotCheck (char string[]);
int beforeAtCount(char string[]);
int afterAtCount (char string[]);
int afterDotCount (char string[]);
void dobScanConvert(int a);
void dobCheck(int a);
void dobCheckAct(int a);
void deleteFromPhonebook(int c, int element[]);
int deleteconf();
void modificationProcess(int x, int orderarr[]);
void sortlastname();
void sortdob();
void choiceconv(int *p);
void LOAD ();
void QUERY();
void ADD();
void DELETE();
void MODIFY();
void PRINTSORTED();
void SAVE();
void QUIT(int *p);

int main()
{
    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("\a((WELCOME TO OUR PHONEBOOK SYSTEM! ADD, DELETE, MODIFY, SORT OR SEARCH FOR AS MANY CONTACTS AS YOU WANT!))\n");
    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("\a((FOR BETTER DISPLAY, RUN THE PROGRAM IN FULL SCREEN!))\n");
    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("PRESS ANY BUTTON TO START YOUR PROGRAM!\n");
    printf("------------------------------------------------------------------------------------------------------------\n");
    getch();
    system("cls");
    menu();
    return 0;
}

void choiceconv(int *p) ///takes integer input as strings to validate it then convert it into integer
{
    char com[1000];
    gets(com);
    if (digitscheck(com)) *p = atoi(com);
    else *p = 0;
}
void menu() ///Prints command menu, scans the number of command and calls its function, repeatedly, until user quits
{
    int command = 0;
    while(command !=8)
    {
        printf("-------------------------------------------\n");
        printf("ENTER THE NUMBER OF THE COMMAND YOU WANT:\n");
        printf("-------------------------------------------\n");
        printf("1 LOAD CONTACTS FROM A FILE\n2 QUERY - SEARCH FOR A CONTACT\n3 ADD A CONTACT\n4 DELETE CONTACTS\n5 MODIFY A CONTACT\n");
        printf("6 PRINT CONTACTS SORTED\n7 SAVE YOUR CHANGES\n8 QUIT FROM PROGRAM\n");
        printf("-------------------------------------------\n---COMMAND NUMBER: ");
        choiceconv(&command);
        switch(command)
        {
        case 1:
            LOAD(); break;
        case 2:
            QUERY(); break;
        case 3:
            ADD(); break;
        case 4:
            DELETE(); break;
        case 5:
            MODIFY(); break;
        case 6:
            PRINTSORTED(); break;
        case 7:
            SAVE(); break;
        case 8:
            QUIT(&command); break;
        default:
            printf("\a\n**ERROR: WRONG ENTRY! PLEASE, ENTER A VALID COMMAND NUMBER FROM THE LIST!\n\n");
        }
        delay(3); ///give user some time to read messages
        if (command!=8) system("cls");
    }
}
void printrecord(int j) ///prints a single record/contact
{
    printf("%10s %13s",phonebook[j].last,phonebook[j].first);
    printf("%12d-%d-%d ",phonebook[j].dob.day,phonebook[j].dob.month,phonebook[j].dob.year);
    printf("%30s %19s %26s\n",phonebook[j].address,phonebook[j].phone,phonebook[j].email);
}
void printrecordhead() ///prints a header for the list of contacts
{
    printf("\n------------------------------------------------------------------------------------------------------------------------\n");
    printf("   Last name      First Name       Date of Birth                    Address           Phone Number            Email");
    printf("\n------------------------------------------------------------------------------------------------------------------------\n");
}
void enddisplay(char * string) ///take permission from user to end the display of contacts, to clear the screen
{
    printf("\nPress any button when you're done viewing the %s!\n",string);
    getch();
    printf("\nTAKING YOU BACK TO THE MENU!\n");
}
int checkloaded() ///Checks if the user loaded a file or not, using global variable (loaded)
{
    if (!loaded)
    {
        printf("\a\n**ERROR! YOU SHOULD LOAD A FILE FIRST!\n\n"); return 0;
    }
    else return 1;
}
int digitscheck(char *s) ///Checks if string is only formed of numbers or not - used to validate phone numbers
{
    while (*s)
    {
        if (isdigit(*s++) == 0) return 0;
    }
    return 1;
}
void phonenumberEntry(int i) ///gives user 2 choices for phone numbers type to validate it correctly later on
{
    int choice = 0;
    printf("\n 1. International/Hotline phone number\n 2. Local phone number\n");
    printf("----------------------------------------\n");
    printf("--CHOOSE THE TYPE OF PHONE NUMBER YOU'D LIKE TO ADD: ");
    choiceconv(&choice);
    while (choice !=1 && choice !=2)
    {
        printf("INVALID ENTRY! TRY AGAIN: ");
        choiceconv(&choice);
    }
    printf("\nEnter the phone number:\n");
    if (choice == 2)
    {
        gets(phonebook[i].phone);
        phonenumberCheck(phonebook[i].phone);
    }
    else if (choice == 1)
    {
        while (1)
        {
            gets(phonebook[i].phone);
            if (digitscheck(phonebook[i].phone))break;
            else printf("\nINVALID PHONE NUMBER!! THE NUMBER YOU ENTER MUST CONSIST OF NUMBERS ONLY!\nRE-ENTER PHONE NUMBER:\n");
        }
    }
}
void phonenumberCheck(char * string) ///validates phone number and rescans it if wrong
{
    ///A) 11 digits for mobile, 9 or 10 for land line B)must begin with 0 C)must consist of only nums D) if mobile, 2nd digit is '1' & 3rd is (0/1/2/5)
    while((strlen(string)!=11 && strlen(string)!= 9 && strlen(string)!=10)|| string[0]!='0' || !digitscheck(string) || (strlen(string) == 11 && (string[1] !='1' || (string[2] !='0' && string[2] !='1' && string[2] !='2' && string[2] !='5'))))
    {
        printf("\a\n**INVALID PHONENUMBER!!\nPLEASE, ENTER A VALID PHONE NUMBER:\n(starts with 0 +11 digits if mobile,9 or 10 digits if landline + contains only numbers + mobile start with(010/011/012/015)):\n\n");
        scanf("%s",string);
        getchar(); //absorb new line in case of repetitive entry of phone numbers, so that it won't affect the next input
    }
}
void dobScanConvert(int a) ///scan date of birth as string then convert it to characters, to prevent errors
{
    char d[10],m[10],y[10];
    scanf("%[^-]-%[^-]-%[^\n]",d,m,y);
    getchar();
    if (digitscheck(d) && digitscheck(m) && digitscheck(y))
    {
        phonebook[a].dob.day = atoi(d), phonebook[a].dob.month = atoi(m), phonebook[a].dob.year = atoi(y);
    }
    else
    {
        phonebook[a].dob.day = 0, phonebook[a].dob.month = 0, phonebook[a].dob.year = 0;
    }
}
void dobCheck(int a) ///validates date of birth and rescans it if wrong
{
    while(1)
    {
        ///Generally, year must be in range 1806-2021, month 1-12, day 1-31
        if (phonebook[a].dob.year < 1806 || phonebook[a].dob.year > 2021 || phonebook[a].dob.month <1 || phonebook[a].dob.month >12 || phonebook[a].dob.day<1 || phonebook[a].dob.day >31)
        {
            dobCheckAct(a); continue;
        }
        ///special case 1: in leap years, in Feb = 29 days
        if ((phonebook[a].dob.year%4)==0 && phonebook[a].dob.month ==2 && phonebook[a].dob.day >29)
        {
            dobCheckAct(a); continue;
        }
        ///special case 2: other than leap years, in Feb = 28 days
        if ((phonebook[a].dob.year%4)!=0 && phonebook[a].dob.month ==2 && phonebook[a].dob.day >28)
        {
            dobCheckAct(a); continue;
        }
        ///special case 3: in months: 4,6,9,11 --> 30 days
        if ((phonebook[a].dob.month == 4 || phonebook[a].dob.month == 6 || phonebook[a].dob.month == 9 || phonebook[a].dob.month == 11) && phonebook[a].dob.day> 30)
        {
            dobCheckAct(a); continue;
        }
        break;
    }
}
void dobCheckAct(int a) ///action taken in case of invalid date of birth
{
    printf("\a\n**INVALID DATE OF BIRTH!\nPLEASE, ENTER A VALID DATE OF BIRTH IN THE FOLLOWING FORMAT (DD-MM-YYYY):\n(Year in range (1806-2021), month (1-12), days (1-31) & be careful of leap years & months with only 30 days!)\n");
    dobScanConvert(a);
}
int emailCheck(char* a) ///validates Email
{
    char email[321];  ///maximum of 320 characters + null character
    strcpy(email,a);
    int emailLength = strlen(email);
    int i, valid = 0, posAt = 0;

    if (!strcasecmp(email,"no email")) return 1;

    for (i = 0; i < emailLength; i++) ///Checking presence of '@' to prevent execution of strtok in case of absence of '@' & producing errors
    {
        if(email[i]== '@')
        {
            i++;
            for (; i<emailLength; i++)
            {
                if(email[i]== '.')
                {
                    i++;
                    if(email[i] != '\0') valid =1;
                }
            }
        }
    }
    if (!valid)
    {
        printf("\a\nINVALID EMAIL! TRY AGAIN!\n"); return 0;
    }
    for (i = 0; i < emailLength; i++) ///Checking that no '@' is followed by '.' to prevent execution of strtok in that case & producing errors
    {
        if(email[i] == '@' && email[i+1]== '.') valid = 0;
    }
    if (!valid)
    {
        printf("\a\nINVALID EMAIL! THERE SHOULD BE AT LEAST 1 CHARACTER IN LOWER DOMAIN (SA: GMAIL)\n"); return 0;
    }
    for(i=0 ; i<emailLength ; i++) ///Check for spaces in the whole email address
    {
        if (email[i] == 32) // Ascii code for space
        {
            printf("\a\nINVALID EMAIL: EMAIL CAN NOT CONTAIN SPACES.\n"); return 0;
        }
    }
    int countpre=0;
    for(i=0 ; i<emailLength ; i++) ///Check for spaces in the whole email address
    {
        if (email[i] == '@') break;
        else countpre++;
    }
    if (countpre >64)
    {
        printf("\a\nINVALID EMAIL: PREFIX LENGTH SHOULD BE LESS THAN 64 CHARACTERS.\n"); return 0;
    }
    for ( i=1 ; i<emailLength ; i++)  ///Check for consecutive Special Characters
    {
        if ((email[i] >= 32 && email[i] <= 47) || (email[i] >= 58 && email[i] <= 64) || (email[i] >= 91 && email[i] <= 96) || (email[i] >= 123 && email[i] <= 126))
        {
            if ((email[i+1] >= 32 && email[i+1] <= 47) || (email[i+1] >= 58 && email[i+1] <= 64) || (email[i+1] >= 91 && email[i+1] <= 96) || (email[i+1] >= 123 && email[i+1] <= 126))
            {
                printf("\a\nINVALID EMAIL: TWO OR MORE CONSECUTIVE SPECIAL CHARACTERS.\n"); return 0;
            }
        }
    }
    char *tokenAt = malloc(65); ///maximum of 64 characters + null character
    tokenAt = strtok(email, "@");
    posAt = strlen(tokenAt); ///length of prefix
    if (email[0] == 64) ///ASCII code for @ sign,, checks that length of prefix is >0
    {
        printf("\n\aEMAIL PREFIX MUST CONTAIN ATLEAST ONE CHARACTER.\n"); return 0;
    }

    ///Check if first character is a Special Character
    if ((email[0] >= 32 && email[0] <= 47) || (email[0] >= 58 && email[0] <= 64) || (email[0] >= 91 && email[0] <= 96) || (email[0] >= 123 && email[0] <= 126))
    {
        printf("\a\nINVALID EMAIL: FIRST CHARACTER CAN NOT BE A SPECIAL CHARACTER\n"); return 0;
    }

    ///Check if last character is a Special Character
    if ((email[posAt - 1] >= 32 && email[posAt - 1] <= 47) || (email[posAt - 1] >= 58 && email[posAt - 1] <= 64) || (email[posAt - 1] >= 91 && email[posAt - 1] <= 96) || (email[posAt - 1] >= 123 && email[posAt - 1] <= 126))
    {
        printf("\a\nINVALID EMAIL: LAST CHARACTER IN THE PREFIX CAN NOT BE A SPECIAL CHARACTER\n"); return 0;
    }

    char *tokenPeriod = malloc(256); //maximum of 255 characters plus the null character
    int tokPeriodLen=0;
    tokenPeriod = strtok(NULL,".");
    tokPeriodLen = strlen(tokenPeriod);

    if (!(isalpha(tokenPeriod[0])) && !(isdigit(tokenPeriod[0])))
    {
        printf("\n\aINVALID EMAIL: EMAIL DOMAIN MUST START AND CONTAIN AT LEAST ONE LETTER OR ONE NUMBER.\n");return 0;
    }
    if (tokenPeriod[tokPeriodLen-1] == 45)
    {
        printf("\n\aINVALID EMAIL: EMAIL DOMAIN MUST NOT END WITH A - SIGN.\n"); return 0;
    }

    for (i=0 ; i<tokPeriodLen ; i++)
    {
        if (!(isalpha(tokenPeriod[i])) && !(isdigit(tokenPeriod[i])) && !(tokenPeriod[i] == 45))
        {
        printf("\n\aINVALID EMAIL! DOMAIN MUST CONTAIN ONLY LETTERS , NUMBERS OR '-' SIGN.\n"); return 0;
        }
    }
    tokenPeriod = strtok(NULL,"\n");
    tokPeriodLen = strlen(tokenPeriod);

    for (i=0 ; i<tokPeriodLen ; i++)
    {
        if (!(isalpha(tokenPeriod[i])) && !(isdigit(tokenPeriod[i])) && !(tokenPeriod[i] == 45) && tokenPeriod[i]!='.')
        {
            printf("\a\nINVALID EMAIL1 DOMAIN MUST CONTAIN ONLY LETTERS , NUMBERS OR '-' SIGN.\n"); return 0;
        }
    }
    ///a loop to measure length of every domain separated by periods
    int count = 0;
    for (i=0 ; i<tokPeriodLen+1 ; i++)
    {
        if (tokenPeriod[i] == '.' || tokenPeriod[i] =='\0')
        {
            if (count < 2)
            {
                printf("\a\nINVALID EMAIL! PERIODS SHOULD SEPARATE MORE THAN 1 CHARACTER\n"); return 0;
            }
            else count = 0;

        }
        else count++;
    }
    return 1;
    free(tokenAt);
    free(tokenPeriod);
}

void LOAD () ///takes file name from user and loads it
{
    char fname[100];
    printf("\n---ENTER THE FILE'S NAME: (FOR EX: filename.txt) --> ");
    scanf("%s",fname);
    getchar();
    FILE * f = fopen(fname,"r");
    if (f!=NULL) ///if file is found
    {
        strcpy(filename,fname);
        count = 0; ///counts number of records, global variable
        while (!feof(f))
        {
            fscanf(f,"%[^,],",phonebook[count].last);
            fscanf(f,"%[^,],",phonebook[count].first);
            fscanf(f,"%d-%d-%d,",&phonebook[count].dob.day,&phonebook[count].dob.month,&phonebook[count].dob.year);
            fscanf(f,"%[^,],",phonebook[count].address);
            fscanf(f,"%[^,],",phonebook[count].phone);
            fscanf(f,"%[^\n]\n",phonebook[count].email);
            count++;
        }
        loaded=1;
        printf("\nFILE IS SUCCESSFULY LOADED!\n\n");
    }
    else
        printf("\a\n**ERROR! NO FILE WITH SUCH NAME EXISTS!!\n\n"); ///if file isn't found
    printf("\nTAKING YOU BACK TO THE MENU!\n");
    fclose(f);
}
void QUERY() ///searches for contact
{
    if(checkloaded()==0) ///file loading validation
        return;
    char lname[100];
    printf("\nPLEASE, ENTER THE LAST NAME OF THE CONTACT YOU'RE SEARCHING FOR:  ");
    gets(lname);
    int j, found = 0, element[100000], elcount = 0;
    for(j = 0; j<count; j++)
    {
        if (!strcasecmp(phonebook[j].last, lname))
        {
            found = 1;
            element[elcount] = j; ///store the order of that contact in the array 'element'
            elcount++;
        }
    }
    if (found)
    {
        printrecordhead();
        for(j = 0; j<elcount; j++)
            printrecord(element[j]);
        enddisplay("search results");
    }
    else
        printf("\a\n**SORRY, NO CONTACT WITH THAT LAST NAME EXISTS!\n\n");
}
void ADD() ///adds a contact
{
    if(checkloaded()==0) ///file loading validation
        return;
    printf("\nPLEASE, ENTER THE DATA IN THE FOLLOWING ORDER:\nLast name:\n");
    gets(phonebook[count].last);
    printf("\nFirst Name:\n");
    gets(phonebook[count].first);
    printf("\nDate of Birth in the following format (DD-MM-YYYY):\n");
    dobScanConvert(count);
    dobCheck(count); //Date of birth validation
    printf("\nAddress:\n");
    gets(phonebook[count].address);
    printf("\nPhone Number:\n");
    phonenumberEntry(count);
    printf("\nE-mail (in the following format: example@domain.com) -- if contact has no email, type (no email):\n");
    gets(phonebook[count].email);
    while(!emailCheck(phonebook[count].email)) //Email validation
    {
        printf("\nEnter a valid Email in the following format (example@domain.com) -- if contact has no email, type (no email):\n");
        gets(phonebook[count].email);
    }
    printf("\n---((NEW CONTACT)):\n ");
    printrecordhead();
    printrecord(count);
    enddisplay("new contact");
    count++; ///increase number of records of phone book
    save = 0; ///a change has happened without saving
}
void DELETE() ///deletes all contacts with those last and first names
{
    if(checkloaded()==0) ///file loading validation
        return;
    char *deleteKeyl = malloc(100);
    char *deleteKeyf = malloc(100);
    printf("\nENTER THE LAST NAME OF THE CONTACT YOU WANT TO DELETE:\n");
    gets(deleteKeyl);
    printf("\nENTER THEIR FIRST NAME:\n");
    gets(deleteKeyf);
    int j, found = 0, element[100000], elcount = 0; ///elcount to count how many records with those names
    for(j = 0; j<count; j++)
    {
        if (!strcasecmp(phonebook[j].last,deleteKeyl) && !strcasecmp(phonebook[j].first,deleteKeyf))
        {
            found = 1;
            element[elcount] = j; ///store the order of that contact in the array 'element'
            elcount++;
        }
    }
    if(!found)
        printf("\a\n**SORRY, NO CONTACT WITH THOSE LAST & FIRST NAMES EXISTS!\n\n");
    else
    {
        printrecordhead();
        for(j = 0; j<elcount; j++)
            printrecord(element[j]);
        if(deleteconf())
        {
            printf("\aDELETION IS CONFIRMED!\n");
            printf("\nTAKING YOU BACK TO THE MENU!\n");
            deleteFromPhonebook(elcount, element);
            save = 0; ///a change happened without saving
        }
        else
            printf("\aDELETION IS CANCELED!\n\nTAKING YOU BACK TO THE MENU!\n");
    }
    free(deleteKeyf);
    free(deleteKeyl);
}
void deleteFromPhonebook(int c, int element[]) ///deletes contacts from phone book - c-> no of contacts with those names, element[]-> array of orders of those contacts
{
    int n, m, a;
    for (n = 0 ; n < c ; n++)
    {
        for(m = element[n]; m < count; m++) ///loop to delete nth record, by shifting all contacts a step backward
            phonebook[m] = phonebook[m+1];
        count--; ///as a result for deletion, number of contacts would decrease
        for (a = 0; n+1+a < c ; a++) ///loop to dec order of each remaining stored contact in array 'element' by 1, to suit the shift in upper loop
            element[n+1+a]--;
    }
}
int deleteconf() ///confirm deleting
{
    printf("\a\nNB: ALL CONTACTS WITH THOSE FIRST & LAST NAMES WILL BE DELETED\nTO CONFIRM DELETION, ENTER '1'. ELSE, CONTACTS WON'T BE DELETED!\n");
    int confirm =0;
    choiceconv(&confirm);
    if(confirm == 1) return 1;
    else return 0;
}
void MODIFY() ///displays all contacts with a certain last name & allows user to modify one of them
{
    if(checkloaded()==0) ///file loading validation
        return;
    char lname[100];
    printf("\nENTER THE LAST NAME OF THE CONTACT YOU WANT TO MODIFY: ");
    gets(lname);
    int j, x, i = 0, found = 0, order[count];
    for(j = 0; j < count ; j++)
    {
        if (!strcasecmp(phonebook[j].last, lname))
        {
            order[i] = j;
            i++; found = 1;
        }
    }
    if(!found)
    {
        printf("\a\n**SORRY, NO CONTACT WITH THAT LAST NAME EXISTS!\n\n"); return;
    }
    if (i>1)
    {
        printrecordhead();
        for (x = 0 ; x<i ; x++)
        {
            printf("%d", x+1); printrecord(order[x]);
        }
        printf("\n---CHOOSE WHICH CONTACT YOU WOULD LIKE TO MODIFY:\n(If you don't want to modify any of those, choose any contact then choose \"done modifying\" without editing any record)\n");
        choiceconv(&x);
        while ( x<1 || x>i)
        {
            printf("\a\n**ERROR: WRONG INPUT! TRY AGAIN: ");
            choiceconv(&x);
        }
    }
    else x = 1;
    modificationProcess(x, order);
    printf("\nTAKING YOU BACK TO THE MENU!\n");
    save = 0; ///a change has happened without saving
}
void modificationProcess(int x, int order[]) ///modifies the contact - x->the order of the contact in the search results list, order[] -> array of their order in original array
{
    int choice = 0;
    while(choice !=7)
    {
        printf("\n1 LAST NAME: %s\n", phonebook[order[x-1]].last);
        printf("2 FIRST NAME: %s\n", phonebook[order[x-1]].first);
        printf("3 DATE OF BIRTH: %d-%d-%d\n", phonebook[order[x-1]].dob.day,phonebook[order[x-1]].dob.month, phonebook[order[x-1]].dob.year);
        printf("4 ADDRESS: %s\n", phonebook[order[x-1]].address);
        printf("5 PHONE NUMBER: %s\n", phonebook[order[x-1]].phone);
        printf("6 EMAIL: %s\n",phonebook[order[x-1]].email);
        printf("7 DONE MODIFYING\n");
        printf("--------------------------------------------------\n");
        printf("---CHOOSE WHICH FIELD YOU WOULD LIKE TO MODIFY:  ");
        choiceconv(&choice);
        switch (choice)
        {
        case 1:
            printf("\nEnter modified last name: ");
            gets(phonebook[order[x-1]].last);
            break;
        case 2:
            printf("\nEnter modified first name: ");
            gets(phonebook[order[x-1]].first);
            break;
        case 3:
            printf("\nEnter modified Date of Birth: ");
            dobScanConvert(order[x-1]);
            dobCheck(order[x-1]); //date of birth validation
            break;
        case 4:
            printf("\nEnter modified address: ");
            gets(phonebook[order[x-1]].address);
            break;
        case 5:
            printf("\nPhone number modification:\n");
            phonenumberEntry(order[x-1]);
            break;
        case 6:
            printf("\nEnter modified Email in the following format (example@domain.com) - if contact doesn't have email, enter (no email): \n");
            gets(phonebook[order[x-1]].email);
            while(!emailCheck(phonebook[order[x-1]].email)) //Email validation
            {
                printf("\nEnter a valid Email in the following format (example@domain.com) - if contact doesn't have email, enter (no email): \n");
                gets(phonebook[order[x-1]].email);
            }
            break;
        case 7:
            printf("\a\nMODIFICATION PROCESS HAS ENDED!\n");
            break;
        default:
            printf("\a\n**WRONG ENTRY! PLEASE, ENTER A VALID NUMBER FROM THE LIST!\n");
        }
    }
}
void PRINTSORTED() ///sorts array and prints it
{
    if(checkloaded()==0) ///file loading validation
        return;
    int s=0;
    printf("\nSORT THE CONTACTS ACCORDING TO:\n1.LAST NAME\n2.DATE OF BIRTH\n");
    printf("----------------------------------\n----CHOICE NUMBER: ");
    while (s!=1 && s!=2)
    {
        choiceconv(&s);
        switch(s)
        {
        case 1:
            sortlastname(); break;
        case 2:
            sortdob(); break;
        default:
            printf("\a\nWRONG ENTRY! PLEASE, ENTER A VALID NUMBER!\nSORT THE CONTACTS ACCORDING TO:\n1.LAST NAME\n2.DATE OF BIRTH\n");
            printf("----------------------------------\n----CHOICE NUMBER: ");
        }
    }
    enddisplay("sorted list of contacts");
}
void sortlastname() ///sorts contacts according to their last names, (first name if they have same last name)
{
    CONTACT temp;
    int i,j;
    for(i=0; i<count-1; i++)
    {
        for(j=i+1; j<count; j++)
        {
            if(strcasecmp(phonebook[i].last,phonebook[j].last)>0)
            {
                temp=phonebook[i];  phonebook[i] = phonebook[j];  phonebook[j] = temp; //swap process
            }
            else if(strcasecmp(phonebook[i].last,phonebook[j].last)==0 && strcasecmp(phonebook[i].first,phonebook[j].first)>0)
            {
                    temp=phonebook[i];  phonebook[i]=phonebook[j];  phonebook[j] = temp; //swap process
            }
        }
    }
    save = 0;
    printf("\n---((CONTACTS AFTER SORTING)):\n");
    printrecordhead();
    for(i=0; i<count; i++)
       printrecord(i);
}
void sortdob() ///sorts contacts according to their date of births
{
    CONTACT temp;
    int i,j;
    for(i=0; i<count-1; i++)
    {
        for(j=i+1; j<count; j++)
        {
            if(phonebook[i].dob.year>phonebook[j].dob.year)
            {
                temp=phonebook[i];  phonebook[i] = phonebook[j];  phonebook[j] = temp; //swap process
            }
            else if(phonebook[i].dob.year == phonebook[j].dob.year && phonebook[i].dob.month>phonebook[j].dob.month)
            {
                temp=phonebook[i];  phonebook[i] = phonebook[j];  phonebook[j] = temp; //swap process
            }
            else if(phonebook[i].dob.year == phonebook[j].dob.year && phonebook[i].dob.month == phonebook[j].dob.month && phonebook[i].dob.day>phonebook[j].dob.day)
            {
                temp=phonebook[i];  phonebook[i] = phonebook[j];  phonebook[j] = temp; //swap process
            }
        }
    }
    save = 0;
    printf("\n---((CONTACTS AFTER SORTING))::\n");
    printrecordhead();
    for(i=0; i<count; i++)
       printrecord(i);
}
void SAVE() ///save changes
{
    if(checkloaded()==0) ///file loading validation
        return;
    FILE * f = fopen(filename,"w");
    int i;
    for(i=0; i<count; i++)
    {
        fprintf(f,"%s,",phonebook[i].last);
        fprintf(f,"%s,",phonebook[i].first);
        fprintf(f,"%d-%d-%d,",phonebook[i].dob.day,phonebook[i].dob.month,phonebook[i].dob.year);
        fprintf(f,"%s,",phonebook[i].address);
        fprintf(f,"%s,",phonebook[i].phone);
        fprintf(f,"%s\n",phonebook[i].email);
    }
    save = 1;
    printf("\a\nCHANGES ARE SUCCESSFULLY SAVED!\n\n");
    fclose(f);
}
void QUIT(int *p) ///quits from program (dealing with the case of changing records without saving)
{
    if(!loaded)
    {
        printf("\n\aGOODBYE!\n"); return;
    }
    if(!save)
    {
        int c = 0;
        printf("\nWARNING : YOUR FILE HAS NOT BEEN SAVED YET\n");
        while(c !=1 && c !=2 && c!=3)
        {
            printf("CHOOSE ONE OF THE FOLLOWING OPTIONS:\n");
            printf("-------------------------------------------\n");
            printf("1 SAVE AND QUIT\n2 QUIT WITHOUT SAVING\n3 CANCEL (BACK TO MENU)\n");
            printf("-------------------------------------------\n---CHOICE NUMBER: ");
            choiceconv(&c);
            switch(c)
            {
            case 1:
                SAVE();
                printf("\n\aGOODBYE!\n"); break;
            case 2:
                printf("\n\aGOODBYE!\n"); break;
            case 3:
                *p = 0;
                printf("\nTAKING YOU BACK TO THE MENU!\n"); break;
            default:
                printf("\a\n**ERROR: WRONG ENTRY!\n\n");
                delay(1);
            }
        }
    }
    else
    {
        printf("\nGOODBYE!\n");return;
    }
}
