//Linked list phone book application
//Written by Nichoas Tate

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
using namespace std;
#define YES 1
#define NO 0

struct address
{
    char name[40];
    char street[40];
    char town[30];
    char county[30];
    char postcode[10];
    address * next;             //Pointer to next entry.
    address * previous;         //Pointer to previous entry.
};

address *start, *last;
fstream list_file;

address * find(char *);

void enter(), search(), save(), load(), list();

void delete_entry(address **, address **);
void store(address *, address **, address **);
void display(address *);
int select_menu();

int main()
{
    int another_go = YES;
    start = last = NULL;

    while(another_go == YES)
    {
        switch(select_menu())
        {
            case 1:
                enter();
                break;
            case 2:
                delete_entry(&start, &last);
                break;
            case 3:
                list();
                break;
            case 4:
                search();
                break;
            case 5:
                save();
                break;
            case 6:
                load();
                break;
            case 7:
                another_go = NO;
                break;
            default:
                cout << "\n\nInternal Error\n\n";
        }
    }

    return 0;
}

int select_menu()
{
    int item;
    system("clear");
    cout << endl << "\t 1\tMake Entry\n" << "\t 2\tDelete Entry\n"
        << "\t 3\tList Entries\n" << "\t 4\tSearch\n"
        << "\t 5\tSave to File\n" << "\t 6\tLoad from File\n"
        << "\t 7\tExit Program\n\n";

    do
    {
        cout << "\t\t\tSelect:   ";
        cin >> item;
        if(cin.fail())     //An  error will occur if alphabetic characters are input.
        {
            cin.clear();
            cin.ignore();
        }
    }while(item < 0 || item > 7);
    return item;
}

void enter()
{
    address * info;
    char again = 'Y';

    do
    {
        if(!(info = new address))
        {
            //Check if memory allocated - if not terminate program.
            cout << "\n\nMEMORY ALLOCATION ERROR";
            exit(-1);
        }

        system("clear");                //***** fgets(buffer, sizeof buffer, stdin);
        cout << "\n\tEnter Name:   ";
        //fgets(info->name, sizeof info, stdin);
        cin.ignore();
        cin.getline(info->name,40);
        //cin >> info->name;
        cout << "\n\tEnter Street:   ";
        //fgets(info->street, sizeof info, stdin);
        cin.getline(info->street,40);
        //cin >> info->street;
        cout << "\n\tEnter Town:   ";
        //fgets(info->town, sizeof info, stdin);
        cin.getline(info->town,30);
        //cin >> info->town;
        cout << "\n\tEnter County:   ";
        //fgets(info->county, sizeof info, stdin);
        cin.getline(info->county,30);
        cout << "\n\tEnter Postcode:   ";
        //fgets(info->postcode, sizeof info, stdin);
        cin.getline(info->postcode,10);

        store(info, &start, &last);     //Put date into list.

        cout << "\n\n\nEnter another?  (Y/N)     ";
        cin >> again;
    }while(again ==  'Y' || again == 'y');
    cin.ignore();
}

//Create linked list.
void store(address * i, address ** start, address ** last)
{
    if(*start == NULL)      //First record has to be treated specially.
    {
        i->next = NULL;     //If first record - no next record.
        i->previous = NULL;        //If first record - no previous record.

        *last = i;          //Store details in last.
        *start = i;         //Store details in start.
    }
    else                    //not first record.
    {
        (*last)->next = i;
        i->next = NULL;
        i->previous = *last;
        *last = i;
    }
}

//Remove entry from list.
void delete_entry(address ** start, address ** last)
{
    address * info;
    char name[40];

    system("clear");
    cout << "\n\n\t\tEnter Name:   ";
    cin.ignore();
    cin.getline(name,40);
    info = find(name);
    if(info)
    {
        if(*start == info)
        {
            *start = info->next;
            if(*start)
                (*start)->previous = NULL;
            else
                *last = NULL;
        }
        else
        {
            info->previous->next = info->next;
            if(info != *last)
                info->next->previous = info->previous;
            else
                *last = info->previous;
        }
        delete info;
        cout << "\n\n\t\tEntry deleted\n";
    }
    else
        cout << "\n\n\n\tPress a key to continue\n\n\n";

    getch();
}
    //Find address.
address * find(char * name)
{
    address * info;
    info = start;

    while(info)
    {
        if(!strcmp(name, info->name))
            return(info);

        info = info->next;
    }
    cout << "\n\n\tName not found\n";
    return(NULL);
}

//Display list.
void list()
{
    address * info;
    int count = 0;

    system("clear");
    info = start;
    cin.ignore();

    while(info)
    {
        count++;
        display(info);
        info = info->next;

        if((count % 4 ) == 0)       //To prevent records scrolling off screen.
        {
            cout << "\n\n\n\tPress a key to continue\n\n\n";
            getch();
        }
    }
    cout << endl << endl;
    cout << "\n\n\n\tPress a key to continue\n\n\n";
    getch();
}

//Print list to screen.
void display(address *info)
{
    cout << endl << "\t" << info->name << endl
        << "\t" << info->street << endl
        << "\t" << info->town << endl
        << "\t" << info->county << endl
        << "\t" << info->postcode << endl;
}

//Look for name in list.
void search()
{
    char name[40];
    address * info;

    system("clear");
    cout << "\n\nEnter name to find:   ";
    cin.ignore();
    cin.getline(name,40);

    info = find(name);

    if(info)
    {
        display(info);
        cout << "\n\n\n\tPress a key to continue\n\n\n";
        getch();
    }
    else
    {
        cout << "\n\n\n\tPress a key to continue\n\n\n";
        getch();
    }
}

//Save list to disk.
void save()
{
    address * info;

    rename("list.dat", "list.old");         //Create backup file.
    list_file.open("list.dat", ios::out);
    if(!list_file)
    {
        cout << "\n\nCould not open file.";
        exit (-1);
    }
    cout << "\n\n\t\tSAVING FILE\n";
    sleep(1);

    info = start;
    while(info)
    {
        list_file << info->name << endl << info->street << endl
                << info->town << endl << info->county << endl
                << info->postcode << endl;
        info = info->next;
    }
    list_file.close();
}

//Load list from disk.
void load()
{
    address*info;

    list_file.open("list.dat",ios::in);
    if(!list_file)
    {
        cout << "\n\nCould not open file.";
        exit (-1);
    }

    while(start)        //Delete any lists already in memory.
    {
        info = start->next;
        delete info;
        start = info;
    }
    start = last = NULL;

    cout << "\n\n\t\tLOADING FILE\n";
    sleep(1);
    while(!list_file.eof())
    {
        info = new address;
        if(!info)
        {
            cout << "\n\n\t\tMEMORY ALLOCATION ERROR\n";
            exit(-1);
        }
        list_file.getline(info->name, 40);
        list_file.getline(info->street, 40);
        list_file.getline(info->town, 30);
        list_file.getline(info->county, 30);
        list_file.getline(info->postcode, 10);

        if(!(list_file.eof()))
        {
            store(info, &start, &last);
        }
    }
    list_file.close();
}

