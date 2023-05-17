//to-do: line break
//Pre-requisite: Build and run package_fwrite.c to create the files which are read,and used in this file
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
//File contains the function to print the service persons list
#include"servicePerson_header.h"

//Structure to store services available along with the charges in each service field
struct service
{
    char issue[35];
    float charge;
};

//Structure to store information(name, pin,and phone number) about the users
struct userInfo
{
    char name[50];
    int pin;
    char phnNo[11];
}new_user, exist_users;

//Function to generate captcha
int captcha_generation(struct userInfo user)
{
    char code[5] = {'P', 'S', 'G', 'C', 'T'}, captchaCode[5], enteredCaptcha[5];
    int i, j=0, randNo[4];

    printf("\n!!SECRET CODE!!\t\n");
    printf("_________________________\n");
    for(i=0; i<5; i++)
    {
        printf("| %c |", code[i]);
    }
    printf("\n");
    for(i=0; i<5; i++)
    {
        printf("| %d |", i);
    }
    printf("\n_________________________\n");
    printf("*Strictly Confidential may not be circulated or reproduced*\n");

    //Random 4 digit number is generated by hashing method from phone number
    for(i=6; i<10; i++)
    {
        randNo[j] = (user.phnNo[i]-'0')%5;
        j++;
    }

    //After the user enters, the captcha is checked
    printf("\nEnter Captcha for ");
    for(i=0; i<4; i++)
    {
        printf("%d", randNo[i]);
    }
    printf(": ");
    for(i=0; i<4; i++)
    {
        captchaCode[i] = code[randNo[i]];
    }
    captchaCode[i] = '\0';
    do
    {
        scanf("%s", enteredCaptcha);
        printf("\n");
        if(strcmp(enteredCaptcha, captchaCode)==0)
        {
            printf("\n\t***CORRECT CAPTCHA***\t\n");
            read_serviceInfo();
            break;
        }
        else
        {
            printf("\n\t***WRONG CAPTCHA***\t\n\nType the Correct Captcha: ");
        }
    }while(strcmp(enteredCaptcha, captchaCode)!=0);
}

//Function to sign in
int user_signin()
{
    FILE * fp;
    char userName[50];
    int pinNo, choice2, flag=0;

    do
    {
        printf("Enter User Name: ");
        scanf("%s", userName);
        printf("Enter 4-Digit Pin Number: ");
        do
        {
            do
            {
                scanf("%d", &pinNo);
                if((pinNo>9999) || (pinNo<1000))
                {
                    printf("\n\t***INVALID PIN NUMBER***\t\n\n>>>Re-Enter a valid 4-Digit Pin Number: ");
                }
            }while((pinNo>9999) || (pinNo<1000));
            fp = fopen("user_Info.txt", "r");
            while(!feof(fp))
            {
                fread(&exist_users, sizeof(struct userInfo), 1, fp);
                if(strcmp(userName, exist_users.name)==0)
                {
                    if(pinNo==exist_users.pin)
                    {
                        flag = 1;
                        break;
                    }
                    else
                    {
                        flag = 2;
                        break;
                    }
                }
                else
                {
                    flag = 0;
                }
            }
            fclose(fp);
            if(flag==1)
            {
                printf("\n\t***LOGIN SUCCESSFUL***\t\n");
                captcha_generation(exist_users);
                break;
            }
            else if(flag==2)
            {
                printf("\n\t***INVALID PIN NUMBER***\t\n\n>>>Re-Enter pin number: ");
            }
            else if(flag==0)
            {
                printf("\n\t***INVALID USER NAME***\t\n\nChoose 1 to Re-Enter OR 2 to Create Account\n>>>Choice: ");
                do
                {
                    scanf("%d", &choice2);
                    if(choice2==2)
                    {
                        user_signup();
                        break;
                    }
                    else if(choice2!=1 && choice2!=2)
                    {
                        printf("\t***INVALID CHOICE***\t\n>>>Re-Enter:");
                    }
                }while(choice2!=1 && choice2!=2);
            }
        }while(flag==2);
        if(flag!=0)
            break;
    }while(choice2==1);
}

//Function to get the file name
char * get_filename(int field)
{
    char * file_name = malloc(40);
    switch (field)
    {
        case 1: strcpy(file_name, "electricianServices.txt");
                break;
        case 2: strcpy(file_name, "plumberServices.txt");
                break;
        case 3: strcpy(file_name, "repairmanServices.txt");
                break;
        case 4: strcpy(file_name, "carpenterServices.txt");
                break;
    }
    return(file_name);
}

//Function to list out all the services available,and calculates the total charge
int read_serviceInfo()
{
    char file_name[40];
    int field, issueNo, choice3, fieldNo[4] = {0, 0, 0, 0}, j, count = 0;
    float tot = 0;
    struct service issue[4];

    //Display,and selection of the service fields
    printf("\n***SERVICES AVAILABLE***\n");
    do
    {
        printf("___________________________________________________________________________________\n");
        printf("1.Electrical Services\n2.Plumbing Services\n3.Repair work\n4.Carpentry\n");
        printf("___________________________________________________________________________________\n");
        printf("Service Required: ");
        do
        {
            scanf("%d", &field);
            if(field!=1 && field!=2 && field!=3 && field!=4)
                printf("\n\t***INVALID CHOICE***\t\n>>>Re-Enter: ");
        }while(field!=1 && field!=2 && field!=3 && field!=4);
        fieldNo[field-1] = 1;

        //Opening the service files
        strcpy(file_name, get_filename(field));
        FILE * fp;
        fp = fopen(file_name, "r");
        if (fp==NULL)
        {
            fprintf(stderr, "\nError to open the file\n");
            exit (1);
        }
        for(j=0; j<4; j++)
        {
            fread(&issue[j], sizeof(struct service), 1, fp);
            printf("%d.%s || Rs.%.2f\n", j+1, issue[j].issue, issue[j].charge);
        }

        //Selection of issue
        printf("\n___________________________________________________________________________________\n>>>Choose: ");
        do
        {
            scanf("%d", &issueNo);
            if(issueNo<1 || issueNo>4)
                printf("\n\t***INVALID CHOICE***\t\n>>>Re-Enter: ");
        }while(issueNo<1 || issueNo>4);

        //Option to add further services required or to proceed
        printf("1.To Add Further Services\t\t2.To Proceed\n>>>Choice: ");
        do
        {
            scanf("%d", &choice3);
            if(choice3==1 || choice3==2)
            {
                tot = tot+issue[issueNo-1].charge;
                sleep(2);

                //Display of the bill
                if(choice3==2)
                {
                    printf("___________________________________________________________________________________\nPAYMENT SUMMARY:\n___________________________________________________________________________________\n");
                    printf("Total Amount For Services.................................Rs.%6.2f", tot);
                    printf("\nMinimum Order Fees........................................Rs.%6.2f", 25.00);
                    printf("\nConvenience Fees..........................................Rs.%6.2f", 30.00);
                    printf("\n___________________________________________________________________________________\n");
                    printf("TOTAL.....................................................Rs.%6.2f", tot+25.00+30.00);
                    printf("\n___________________________________________________________________________________\n");
                    printf("*ONLY CASH ON DELIVERY*\n");
                    sleep(2);
                }
            }
            else
            {
                printf("\n\t***INVALID CHOICE***\t\n>>>Re-Enter: ");
            }
        }while(choice3!=1 && choice3!=2);
    }while(choice3==1);
    read_serviceManInfo(fieldNo);
}

//Function to sign up
int user_signup()
{
    FILE * fp;
    int flag, choice4, len;

    //Enter the details of the user
    do
    {
        printf("Enter User Name: ");
        scanf("%s", new_user.name);
        flag = 0;
        fp = fopen("user_Info.txt", "r");
        while(!feof(fp))
        {
            fread(&exist_users, sizeof(struct userInfo), 1, fp);

            //Comparison of the entered user name,and already existing user names to find whether the account already exists
            if(strcmp(exist_users.name, new_user.name)==0)
            {
                printf("\n\t***ACCOUNT ALREADY EXISTS WITH THIS USERNAME***\t\n");
                printf("\nDo You Want to ??\n\t   1.Sign in instead                       2.Create account\n");
                printf(">>>Choice: ");
                do
                {
                    scanf("%d", &choice4);
                    if(choice4==1)
                    {
                        return user_signin();
                    }
                    else if(choice4==2)
                    {
                        flag = 1;
                    }
                    else
                    {
                        printf("\t***INVALID CHOICE***\t\n>>>Re-Enter:");
                    }
                }while(choice4!=1 && choice4!=2);
            }
            if(flag==1)
                break;
        }
        if(flag==0)
            break;
        fclose(fp);
    }while(flag==1);

    //After the user enters, validity of the pin is checked
    printf("Enter 4-Digit Pin Number: ");
    do
    {
        scanf("%d", &new_user.pin);
        if((new_user.pin>9999) || (new_user.pin<1000))
        {
            printf("\n\t***INVALID PIN NUMBER***\t\n\n>>>Re-Enter a valid 4-Digit Pin Number: ");
        }
    }while((new_user.pin>9999) || (new_user.pin<1000));

    //After the user enters, validity of the phone number is checked
    printf("Enter your Phone Number: ");
    do
    {
        scanf("%s", new_user.phnNo);
        len = strlen(new_user.phnNo);
        if(len!=10)
        {
            printf("\n\t***INVALID PHONE NUMBER***\t\n\n>>>Re-Enter your phone number: ");
        }
    }while(len!=10);

    //Creation of the account,and storing the details of the new user
    fp = fopen("user_Info.txt", "a");
    fwrite(&new_user, sizeof(new_user), 1, fp);
    fclose(fp);
    printf("\n\t***ACCOUNT CREATED SUCCESSFULLY***");

    //Sign in process
    printf("\n\nLogging in......\n");
    sleep(3);
    user_signin();
}

//The main function
int main()
{
    int choice1;

    //Display of the options to sign in,and sign up
    printf("---------------------------------------------------------------------------");
    printf("\n\t\tWELCOME TO \"ONE CLICK SERVICES\"!\t\t\n\nExperienced,hand picked Professionals to serve you at your Doorstep!\n");
    printf("---------------------------------------------------------------------------");
    printf("\n\t   1.Sign in                       2.Sign up\n");
    printf("---------------------------------------------------------------------------\n>>>Choose: ");

    //User can either select sign in or sign up
    do
    {
        scanf("%d", &choice1);
        if(choice1==1)
        {
            user_signin();
            break;
        }
        else if(choice1==2)
        {
            user_signup();
            break;
        }
        else
        {
            printf("\t***INVALID CHOICE***\t\n>>>Re-Enter: ");
        }
    }while(choice1!=1 && choice1!=2);
}