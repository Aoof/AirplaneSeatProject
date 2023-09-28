#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SAVE_FILE "./SAVE.DAT"

// Seat structure declaration
typedef struct 
{
    char id[3]; 
    char firstname[21];
    char lastname[21];
    char status[10]; // "Empty" or "Assigned"
}Seat;

// Menu
void ShowMenu(Seat[]);

// Declaring all the functions the way way they were asked
void DisplayAll(Seat[]); 
void DisplayEmpty(Seat[]);
void DisplayAssigned(Seat[]);
void AssignSeat(Seat[]);
void DeleteOne(Seat[]);
void DeleteAll(Seat[]); 

// Declaring the functions for saving and loading the seat array 
void SetupSeats(Seat[]);
void ImportSeats(Seat[]);
void ExportSeats(Seat[]);

int main(void)
{
    // Allocate memory for the seat array and importing it if it exists, otherwise create it
    Seat *seats;
    seats = (Seat *)calloc(12, sizeof(Seat));
    ImportSeats(seats);

    ShowMenu(seats);
    return 0;
}

void ShowMenu(Seat seats[])
{
    char choice;
    do
    {
        printf( "\nA. Display ALL seat assignments (including Empty).\n"
                "B. Show ONLY a list of empty seats and show total of empty seats.\n"
                "C. Show ONLY the assigned seats with name of person and total assigned.\n"
                "D. Assign the customer to an empty seat.\n"
                "E. Delete ONE seat assignment.\n"
                "F. Delete ALL seat assignments.\n"
                "Q. Quit program.\n> ");

        // Weird stuff were happening with scanf so I just flushed the input cache before every scanf in the program
        fflush(stdin);
        scanf("%c", &choice); 
        printf("\n");

        // used toupper() to remove case sensitive input :> this is a smily face btw
        switch (toupper(choice))
        {
        // For every case there is a function and I simply export the data after a function that tampers with it
        case 'A': 
            DisplayAll(seats);
            break;
        case 'B': 
            DisplayEmpty(seats);
            break;
        case 'C':
            DisplayAssigned(seats);
            break;
        case 'D':
            // So assigning a seat would perform changes on the seat array so I must save these changes by exporting it 
            AssignSeat(seats);
            ExportSeats(seats);
            break;
        case 'E':
            // Deleting a seat would also change it so must save it and so on...
            DeleteOne(seats);
            ExportSeats(seats);
            break;
        case 'F':
            DeleteAll(seats);
            ExportSeats(seats);
            break;
        }
        // I needed to add this so the user would be able to read the data in a cleaner way, not very speedy but
        // I thought it was necessary, because if I struggled with reading it the user probably will.
        // So Please press enter to continue... :>
        if (toupper(choice) == 'Q')
            exit(1);
        printf("Please press enter to continue...\n");
        fflush(stdin);
        getchar();
    } while (toupper(choice) != 'Q');
}

// I/O FILE MANAGEMENT
void ImportSeats(Seat seats[]) 
{
    // Read binary data if exists otherwise create a clean setup and save it
    FILE *fp;

    if ((fp = fopen(SAVE_FILE, "rb")) == NULL) {
        printf("Couldn't find a save.data file (created one in local directory)\n");
        SetupSeats(seats);
        ExportSeats(seats);
        return;
    }
    fread(seats, sizeof(Seat), 12, fp);
    fclose(fp);
}

void ExportSeats(Seat seats[])
{
    // Write binary data regardless if the file exists or not, it's rewriting it anyway so no need to update the data file
    FILE *fp;

    if ((fp = fopen(SAVE_FILE, "wb+")) == NULL) {
        printf("Couldn't open file to write\n");
        exit(1);
    }
    fwrite(seats, sizeof(Seat), 12, fp);
    fclose(fp);
}

void SetupSeats(Seat seats[])
{
    // Setting up a clean seat array with proper seat id values
    char id[3]; // A0
    for (int i = 0; i < 12; i++)
    {
        char alpha[7] = "ABCDEF";
        id[0] = alpha[i/2];
        id[1] = (i % 2) ? '2' : '1';

        strcpy(seats[i].id, id);
        strcpy(seats[i].status, "Empty");
        strcpy(seats[i].firstname, "");
        strcpy(seats[i].lastname, "");
    }
}

void DisplayAll(Seat seats[])
{
    // Displaying all seats whether they're empty or assigned

    char displayedText[43];
    printf("Current seat assignments:\n");
    
    // aligning the labels 1 and 2 are for the seats in each row since every row has 2 seats
    printf("\t     %-27c%-27c\n", '1', '2');
    for (int i = 0; i < 12;)
    {
        // Compare if the seat was empty then set the displayedText to be either the fullname or just Empty
        if (strcmp(seats[i].status, "Empty"))
        {
            strcpy(displayedText, seats[i].firstname);
            strcat(strcat(displayedText, " "), seats[i].lastname); // Firstname (space) Lastname
        }
        else
            strcpy(displayedText, seats[i].status); // Status (Empty most definitely)

        printf("\t%-4c %-27s", seats[i].id[0], displayedText); // Print out the displayText with proper alignment and also add the row id before it
        
        // Second seat in the row
        if (strcmp(seats[i+1].status, "Empty"))
        {
            strcpy(displayedText, seats[i+1].firstname);
            strcat(strcat(displayedText, " "), seats[i+1].lastname); // Firstname (space) Lastname
        }
        else
            strcpy(displayedText, seats[i+1].status); // Status (Empty most definitely)
        
        printf("%-27s\n", displayedText);

        // go to the next row
        i += 2;
    }
}

void DisplayEmpty(Seat seats[]) 
{
    int mt = 0;
    printf("\tSeat #\tStatus\n");
    for (int i = 0; i < 12; i++)
        if (strcmp(seats[i].status, "Assigned")) 
        {
            // If the seat was empty display it and increment the counter
            mt++;
            printf("\t%s\t%s\n", seats[i].id, seats[i].status);
        }
    printf("\n");
    printf("The total amount of empty seats is %d\n", mt);
}

void DisplayAssigned(Seat seats[]) {
    int occu = 0;
    char newName[43];
    printf("\tSeat #\tHolder\t\t\t\tStatus\n");
    for (int i = 0; i < 12; i++)
        if (!strcmp(seats[i].status, "Assigned"))
        {
            // If the seat was assigned to someone display it and increment the counter
            occu++;
            strcpy(newName, seats[i].firstname);
            strcat(strcat(newName, " "), seats[i].lastname); // Firstname (space) Lastname
            printf("\t%s\t%-27s\t%s\n", seats[i].id, newName, seats[i].status); // Proper alignment
        }

    printf("\n");
    printf("The total amount of assigned seats is %d\n", occu);
}

void AssignSeat(Seat seats[]) {
    char seatCode[3];
    char realCode[3];
    char firstName[21];
    char lastName[21];

    // Display empty seats since we're gonna pick from them
    DisplayEmpty(seats);

    printf("Please enter the desired seat id: (Q to cancel) "); 
    fflush(stdin);
    gets(seatCode);

    // (Q to cancel) check & removing caps sensitivity 
    seatCode[0] = toupper(seatCode[0]);
    if (seatCode[0] == 'Q' && seatCode[1] == '\0')
        return;

    // Loop through all seats to find and perform on the correct seat
    for (int i=0; i<12; i++)
    {
        // check the first character and second characters of the entered code and make sure that the user is trying on an empty seat
        if (seats[i].id[0] != seatCode[0] || seats[i].id[1] != seatCode[1] || strcmp(seats[i].status, "Empty"))
            continue; // Simply skip the next few lines if any of the conditions were met
            
        // Get the first and last names from the user (Q to cancel functionality applies here too)
        printf("Please enter the first name: (Q to cancel) ");
        fflush(stdin);
        gets(firstName);
        if (toupper(firstName[0]) == 'Q' && firstName[1] == '\0')
            break;

        printf("Please enter the last name: (Q to cancel) ");
        fflush(stdin);
        gets(lastName);
        if (toupper(lastName[0]) == 'Q' && lastName[1] == '\0')
            break;

        // Assign the seat by filling in the details entered and changing the status
        strcpy(seats[i].firstname, firstName);
        strcpy(seats[i].lastname, lastName);
        strcpy(seats[i].status, "Assigned");
        break; // Break since we already found the correct seat and performed everything we wanted on it
    }
}

void DeleteOne(Seat seats[]) {
    char seatCode[3];
    char confirm;
    // Display the assigned seats since you won't delete an empty seat
    // Also, Same format as the assign seat since deleting is just assigning a seat to no one with few tweaks on the conditions and added confirmation 
    
    DisplayAssigned(seats);

    printf("Please enter the desired seat id: (Q to cancel) ");
    fflush(stdin);
    gets(seatCode);

    seatCode[0] = toupper(seatCode[0]);
    if (seatCode[0] == 'Q')
        return;

    for (int i = 0; i < 12; i++)
    {
        if (seats[i].id[0] != seatCode[0] || seats[i].id[1] != seatCode[1] || strcmp(seats[i].status, "Assigned"))
            continue;

        // ✨ Confirmation ✨
        printf("\nAre you sure you want to delete this seat assignment? (Y/n) ");
        fflush(stdin);
        scanf("%c", &confirm);

        if (toupper(confirm) != 'Y')
            break;

        // Default values
        strcpy(seats[i].firstname, "\0");
        strcpy(seats[i].lastname, "\0");
        strcpy(seats[i].status, "Empty");
        break;
    }
}

void DeleteAll(Seat seats[]) {
    char confirm;

    // Same as delete but without having to get the seat id since we wanna go berserk on everyone
    DisplayAssigned(seats);

    // Sweet little warning :>
    printf("Would you like to delete all of the assigned seats (BULK ACTION WARNING!) ? (Y/n) ");
    fflush(stdin);
    scanf("%c", &confirm);

    if (toupper(confirm) != 'Y')
        return;

    // SetupSeats(seats) // could've used a clean setup but idk which is more efficient
    for (int i = 0; i < 12; i++)
    {
        strcpy(seats[i].firstname, "");
        strcpy(seats[i].lastname, "");
        strcpy(seats[i].status, "Empty");
    }
}