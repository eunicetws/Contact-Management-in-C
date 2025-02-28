#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME 100 
#define PHONE 15 
#define EMAIL 320 
#define MAX_CONTACTS 1000

// Define the contact structure
typedef struct {
    char first_name[NAME];
    char last_name[NAME];
    char phone_num[PHONE];
    char email[EMAIL];
    char fav;
    char archive;
} Contact;

// Define the suggestion structure
typedef struct {
    char value[320];  
    char type[20];    
} Suggestion;

// basic functions
void add();
void edit();
void del();
void search();
void display();
void fav();
void delete_file();
void archive();
void back_up();
void restore();

void encrypt();
void decrypt();
void authentication();
void get_suggestions(const char* partial_input, int search_choice, Suggestion suggestions[], int* suggestion_count);

bool yesNo(const char* phrase); //Confirm user's choice
void to_lowercase(char *str); // change variable string to lowercase
void r_whitespaces(); //remove leftover whitespcaes
void r_empty_str(const char* phrase, int CONSTANT, const char* prompt, char* input); // ensures non-empty input
bool is_duplicate_contact(const char* first_name, const char* last_name); // ensures no duplicates
bool is_valid_email(const char *email); // validates email format
bool is_valid_phone(const char *phone_num); // validates phone format
void sort_contacts(Contact contacts[], int count); //sorting names alphabetically

char fcontact[] = "contacts.txt";

int main() {
    char fcontact[] = "contacts.txt"; // Define the filename here or pass it as a parameter
    FILE *fp = fopen(fcontact, "a"); //used to create new file when none is created
    fclose(fp);
    decrypt();
    while (1) {
        char choice;

        puts("*********************************************************************");
        puts("                        Contact Numbers"); //6 tabs
        puts("*********************************************************************");
        puts("                        1. Add");
        puts("                        2. Edit");
        puts("                        3. Delete");
        puts("                        4. Search");
        puts("                        5. Display");
        puts("                        6. Delete File");
        puts("                        7. Archive");
        puts("                        8. Backup & Restore");
        puts("                        0. Exit");
        puts("*********************************************************************");

        printf("Please enter your choice: ");
        scanf(" %c", &choice); // Corrected to use the address of choice
        r_whitespaces();

        switch (choice) {
            case '1': 
                add();
                break;
            case '2': 
                edit();
                break;
            case '3': 
                del();
                break;
            case '4':
                search();
                break;
             case '5':
                bool loop = true;
                while (loop) {
                    char choiceD;
                    puts("*********************************************************************");
                    puts("                        Display");
                    puts("*********************************************************************");
                    puts("                        1. All Contacts");
                    puts("                        2. Favourite Contacts");
                    puts("                        3. Exit");
                    puts("*********************************************************************");

                    printf("Please enter your choice: ");
                    scanf(" %c", &choiceD);
                    r_whitespaces();

                    switch (choiceD) {
                        case '1':
                            display();
                            break;
                        case '2':
                            fav();
                            break;
                        case '3':
                            puts("Returning to Main Menu...");
                            loop = false;
                            break;
                        default:
                            puts("Please enter a valid option (1-3).");
                            break;
                    }
                }
                break;
            case '6':
                delete_file();
                break;
            case '7':
                archive();
                break;
            case '8':
                char choiceB_R;
                puts("*********************************************************************");
                puts("                        Backup & Restore");
                puts("*********************************************************************");
                puts("                        1. Backup");
                puts("                        2. Restore");
                puts("                        3. Exit");
                puts("*********************************************************************");
                printf("Please enter your choice: ");
                scanf(" %c", &choiceB_R);
                r_whitespaces();

                while(1){
                    if (choiceB_R == '1'){
                        back_up();
                        break;
                    }
                    else if (choiceB_R == '2'){
                        restore();
                        break;
                    }
                    else if (choiceB_R == '3'){
                        break;
                    }
                    else {
                        printf("Please enter a valid num (1-backup/2-restore): ");
                        scanf(" %c", &choiceB_R);
                        r_whitespaces();
                    }
                }
                break;
            case '0':
                puts("Exiting...");
                encrypt();
                return 0;
            default:
                puts("Please enter a number from 0 to 8.");
                break;
        }
    }
    return 0;
}

void add() {
    Contact contacts;
    
    while (1) {
        bool confirm = false;
        bool resume = true;
        
        FILE *temp = fopen("temp.txt", "w");
        if (temp == NULL) {
            printf("Error creating temporary file.\n");
            return;
        }

        puts("Enter Name: ");
        r_empty_str("\tFirst Name: ", NAME, "first name", contacts.first_name);
        r_empty_str("\tLast Name: ", NAME, "last name", contacts.last_name);
        
        do {
            r_empty_str("\tPhone Number: ", PHONE, "phone number", contacts.phone_num); // Add the error prompt here
            if (!is_valid_phone(contacts.phone_num)) {
                puts("Invalid phone number format. Please enter only digits, +, - or spaces.");
            }
        } while (!is_valid_phone(contacts.phone_num));
        
        do {
            r_empty_str("\tEmail: ", EMAIL, "email", contacts.email); // Add the error prompt here
            if (!is_valid_email(contacts.email)) {
                puts("Invalid email format. Please enter a valid email address.");
            }
        } while (!is_valid_email(contacts.email));
        // Ask if the contact should be added to favorites
        bool in = yesNo("Put it in Favourite?");
        contacts.fav = (in) ? 'y' : 'n';

        contacts.archive = 'n';

        // Convert strings to lowercase
        to_lowercase(contacts.first_name);
        to_lowercase(contacts.last_name);
        to_lowercase(contacts.email);

        // Confirm before saving the record
        confirm = yesNo("Are you sure?");

        // Write the record to the temporary file
        if (confirm) {
            fprintf(temp, "%s\n%s\n%s\n%s\n%c\n%c\n", 
                    contacts.first_name, contacts.last_name, 
                    contacts.phone_num, contacts.email, 
                    contacts.fav, contacts.archive);
            fclose(temp);

            // Append the temporary file content to the main contacts file
            FILE *fp = fopen(fcontact, "a");
            temp = fopen("temp.txt", "r");

            if (fp == NULL || temp == NULL) {
                printf("Error opening file.\n");
                fclose(fp);
                fclose(temp);
                remove("temp.txt");
                return;
            }

            // Copy the contents from temp to fp
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), temp)) {
                fputs(buffer, fp);
            }

            fclose(fp);
            fclose(temp);
            remove("temp.txt");

            puts("Contact successfully added.\n");
        } else {
            fclose(temp);
            remove("temp.txt");
            puts("Contact was not added.\n");
        }

        // Ask if the user wants to continue adding records
        resume = yesNo("Do you want to continue?");
        puts("");
        if (!resume) {
            puts("\nReturning to Main Menu");
            break;
        }
    }
}

void edit() {
    while (1) {
        FILE *fp, *temp;
        Contact contacts;
        char first_name[NAME], last_name[NAME];
        int found = 0;
        bool confirm = false;
        bool resume = true;

        fp = fopen("contacts.txt", "r");
        if (fp == NULL) {
            printf("Error opening file.\n");
            return;
        }

        temp = fopen("temp.txt", "w");
        if (temp == NULL) {
            printf("Error creating temporary file.\n");
            fclose(fp);
            return;
        }

        // Get input for the contact to edit
        r_empty_str("\tFirst Name: ", NAME, "first name", first_name);
        r_empty_str("\tLast Name: ", NAME, "last name", last_name);
        
        to_lowercase(first_name);
        to_lowercase(last_name);

        while ((fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n %c\n %c\n", 
                            contacts.first_name, contacts.last_name, contacts.phone_num, contacts.email, 
                            &contacts.fav, &contacts.archive) == 6)) {
            to_lowercase(contacts.first_name);
            to_lowercase(contacts.last_name);

            if (strcmp(contacts.first_name, first_name) == 0 && strcmp(contacts.last_name, last_name) == 0) {
                // Prompt user to enter new details
                puts("Enter the New Name");
                r_empty_str("\tFirst Name: ", NAME, "first name", contacts.first_name);
                r_empty_str("\tLast Name: ", NAME, "last name", contacts.last_name);

                // Validate phone number
                do {
                    r_empty_str("\tPhone Number: ", PHONE, "phone number", contacts.phone_num);
                    if (!is_valid_phone(contacts.phone_num)) {
                        puts("Invalid phone number format. Please enter only digits, +, - or spaces.");
                    }
                } while (!is_valid_phone(contacts.phone_num));
                
                // Validate email
                do {
                    r_empty_str("\tEmail: ", EMAIL, "email", contacts.email);
                    if (!is_valid_email(contacts.email)) {
                        puts("Invalid email format. Please enter a valid email address.");
                    }
                } while (!is_valid_email(contacts.email));

                // Set favorite status
                bool in = yesNo("Put it in Favourite?");
                contacts.fav = in ? 'y' : 'n';
                

                // Set archive status
                contacts.archive = 'n';

                // Convert fields to lowercase for consistency
                to_lowercase(contacts.first_name);
                to_lowercase(contacts.last_name);
                to_lowercase(contacts.email);

                found = 1;
                confirm = yesNo("Are you sure?");
            }

            fprintf(temp, "%s\n%s\n%s\n%s\n%c\n%c\n", 
                            contacts.first_name, contacts.last_name, contacts.phone_num, contacts.email, 
                            contacts.fav, contacts.archive);
        }

        fclose(fp);
        fclose(temp);

        if (found == 0) {
            puts("Sorry, No Record Found\n");
        } else if (confirm) {
            // Replace original file content with updated record
            fp = fopen("contacts.txt", "w");
            temp = fopen("temp.txt", "r");

            if (fp == NULL || temp == NULL) {
                printf("Error updating the file.\n");
                if (fp) fclose(fp);
                if (temp) fclose(temp);
                return;
            }

            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), temp)) {
                fputs(buffer, fp);
            }

            fclose(fp);
            fclose(temp);
            remove("temp.txt");

            puts("Record successfully modified.\n");
        } else {
            puts("Record modification was cancelled.\n");
        }

        resume = yesNo("Do you want to continue?\n");
        if (!resume) {
            puts("\nReturning to Main Menu");
            break;
        }
    }
}

void del(){
    while(1){
        FILE *fp,*temp;
        char first_name[NAME], last_name[NAME];
        Contact contacts;
        int found = 0; //Flag to check if the record is found
        bool confirm = false;//Confirm deletion
        bool resume = true;

        fp = fopen(fcontact, "r");
        if (fp == NULL){
            printf("Error opening file.\n");
            return;
        }

        temp = fopen("temp.txt", "w");
        if (temp ==NULL){
            printf("Error creating temporary file.\n");
            fclose(fp);
            return;
        }

        r_empty_str("First Name: ",NAME, "first name" ,first_name);
        r_empty_str("Last Name: ",NAME, "last name" ,last_name);

        to_lowercase(first_name);
        to_lowercase(last_name);

        while ((fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n %c\n %c\n", 
                            contacts.first_name, contacts.last_name, contacts.phone_num, contacts.email, 
                            &contacts.fav, &contacts.archive) == 6)){ 
           if (strcmp(contacts.first_name, first_name) == 0 && strcmp(contacts.last_name, last_name) == 0){
                found = 1; //Record found
                confirm = yesNo("Are you sure you want to delete this contact?");
                if (confirm){
                    puts("Contact deleted.\n");
                    continue; //Skip writing this contact to temp file
                }else{
                    fprintf(temp, "%s\n%s\n%s\n%s\n%c\n%c\n", 
                            contacts.first_name, contacts.last_name, contacts.phone_num, contacts.email, 
                            contacts.fav, contacts.archive);
                }
           }else{
                //Write the contact to the temporary file if it is not the one to delete
                fprintf(temp, "%s\n%s\n%s\n%s\n%c\n%c\n", 
                            contacts.first_name, contacts.last_name, contacts.phone_num, contacts.email, 
                            contacts.fav, contacts.archive);
           }
        }
        fclose(fp);
        fclose(temp);

        if(!found){
            puts("Sorry, No Record Found\n");
        }else{
            //Replace the content with its updated records
            fp = fopen(fcontact,"w");
            temp = fopen("temp.txt", "r");

            char buffer[1024];
            while(fgets(buffer, sizeof(buffer), temp)){
                fputs(buffer, fp);
            }

            fclose(fp);
            fclose(temp);
            remove("temp.txt"); //Remove the temporary file
        }

        resume = yesNo("Do you want to continue deleting?");
        puts("");
        
        if(!resume){
            puts("\nReturning to Main Menu...");
            break;
        }
    }
}

void search(){
    while(1){
        
        FILE *fp = fopen(fcontact, "r");

        if (fp == NULL) {
            perror("Error opening file.\n");
            return;
        }
        fclose(fp);

        Contact contacts;

        bool resume = true;
        int search_choice = 0;
        char partial_input[320] = "";
        Suggestion suggestions[MAX_CONTACTS];
        int suggestion_count = 0;

        printf("Search by (1-First name/2-Last name/3-Phone num/4-Email): ");
        scanf("%d", &search_choice);
        r_whitespaces();

        if (search_choice >= 1 && search_choice <= 4){
            r_empty_str("Enter the keyword to search (partial input allowed): ", 1000, "search keyword", partial_input);
            partial_input[strcspn(partial_input, "\n")] = '\0'; 
        

            get_suggestions(partial_input, search_choice, suggestions, &suggestion_count);

            
            // Read through contacts & search for the keyword
            if (suggestion_count > 0) {
                    printf("\nFound %d suggestions:\n", suggestion_count);
                    for (int i = 0; i < suggestion_count; i++) {
                        printf("%d. %s (%s)\n", i + 1, suggestions[i].value, suggestions[i].type);
                    } 
                    
            } else {
                        printf("\nNo matches found for '%s'\n", partial_input);
            }
        } else {
                    printf("\nPlease enter a valid number (1-4)\n");
        }
                    
        // Continue or return to main menu
        resume = yesNo("\nDo you want to continue?");  
        if (!resume) {
            puts("\nReturning to Main Menu");
            break;        
        }
    }
}

void display() {
    FILE *fp = fopen(fcontact, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Contact contacts[MAX_CONTACTS];
    int count = 0;

    while (count < MAX_CONTACTS && 
           fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n %c\n %c\n",
                  contacts[count].first_name, contacts[count].last_name,
                  contacts[count].phone_num, contacts[count].email,
                  &contacts[count].fav, &contacts[count].archive) == 6) {
        count++;
    }
    fclose(fp);

    // Sort contacts alphabetically by name
    sort_contacts(contacts, count);

    puts("*********************************************************************");
    puts("                          Contact Info                               ");
    puts("*********************************************************************");
    for (int i = 0; i < count; i++) {
        if (contacts[i].archive == 'n') {
            contacts[i].first_name[0] = toupper(contacts[i].first_name[0]);
            contacts[i].last_name[0] = toupper(contacts[i].last_name[0]);

            printf("\n\tFirst Name:\t%s\n\tLast Name:\t%s\n\tContact num:\t%s\n\tEmail:\t\t%s\n",
                   contacts[i].first_name, contacts[i].last_name, 
                   contacts[i].phone_num, contacts[i].email);
        }
    }
    puts("*********************************************************************\n");
}

void fav() {

    FILE *fp = fopen(fcontact, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Contact contacts[MAX_CONTACTS];
    int count = 0;

    while (count < MAX_CONTACTS && 
           fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n %c\n %c\n",
                  contacts[count].first_name, contacts[count].last_name,
                  contacts[count].phone_num, contacts[count].email,
                  &contacts[count].fav, &contacts[count].archive) == 6) {
        count++;
    }

    sort_contacts(contacts, count);

    puts("*********************************************************************");
    puts("                             Favourite                               ");
    puts("*********************************************************************");

    for (int i = 0; i < count; i++) {
        if (contacts[i].archive == 'n' && contacts[i].fav == 'y') {
            contacts[i].first_name[0] = toupper(contacts[i].first_name[0]);
            contacts[i].last_name[0] = toupper(contacts[i].last_name[0]);

            printf("\n\tFirst Name:\t%s\n\tLast Name:\t%s\n\tContact num:\t%s\n\tEmail:\t\t%s\n",
                   contacts[i].first_name, contacts[i].last_name, 
                   contacts[i].phone_num, contacts[i].email);
        }
    }
    puts("*********************************************************************");

    fclose(fp);

    FILE *fprw = fopen(fcontact, "r+"); //file pointer read write
    if (fprw == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Contact contact;
    int found = 0;
    char choice;

    puts("                        1. Add to Favourite");
    puts("                        2. Remove from Favourite");
    puts("                        3. Exit");
    puts("*********************************************************************\n");

    printf("Enter your choice: ");
    scanf(" %c", &choice);
    r_whitespaces();

    if (choice != '1' && choice != '2' && choice != '3') {
        puts("Please enter from 1 to 3");
        fclose(fprw);
        return;
    }

    if (choice == '3'){
        return;
    }
    

    char first_name[NAME], last_name[NAME];
    printf("Enter First Name: ");
    scanf("%s", first_name);
    to_lowercase(first_name);

    printf("Enter Last Name: ");
    scanf("%s", last_name);
    to_lowercase(last_name);

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error opening temporary file.\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n %c\n %c\n",
                  contact.first_name, contact.last_name,
                  contact.phone_num, contact.email,
                  &contact.fav, &contact.archive) == 6) {
        
        if (strcmp(contact.first_name, first_name) == 0 && strcmp(contact.last_name, last_name) == 0) {
            found = 1;

            if (choice == '1' && contact.fav != 'y') {
                contact.fav = 'y';
                puts("Contact added to favourites.");
            } else if (choice == '2' && contact.fav != 'n') {
                contact.fav = 'n';
                puts("Contact removed from favourites.");
            } else {
                puts("Contact is already in the selected state.");
            }
        }

        fprintf(temp, "%s\n%s\n%s\n%s\n%c\n%c\n",
                contact.first_name, contact.last_name, 
                contact.phone_num, contact.email, 
                contact.fav, contact.archive);
    }

    fclose(fprw);
    fclose(temp);

    if (!found) {
        puts("No matching contact found.");
    } else {
        remove(fcontact);
        rename("temp.txt", fcontact);
    }
}

void delete_file() {
    // Attempt to delete the file
        if (yesNo("Are you sure you want to delete the entire contacts file?")){
            if(remove(fcontact) == 0){
                printf("File \"%s\" successfully deleted.\n", fcontact); //puts doesn't support formated strings
                puts("\nReturning to Main Menu...");
            }else{
                perror("Error deleting this file.");
            }
        }else{
            puts("File deletion canceled.");
            puts("Returning to Main Menu...");
        }
}

void archive() {
    FILE *fp, *fpw;
    Contact contacts[MAX_CONTACTS];
    int count = 0;

    fp = fopen("contacts.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n %c\n %c\n",
                  contacts[count].first_name, contacts[count].last_name,
                  contacts[count].phone_num, contacts[count].email,
                  &contacts[count].fav, &contacts[count].archive) == 6) {
        count++;
    }
    fclose(fp);

    while (1) {
        char choice;
        bool confirm = false;

        puts("*********************************************************************");
        puts("                        Archive Menu");
        puts("*********************************************************************");
        puts("                        1. Archive");
        puts("                        2. Unarchive");
        puts("                        3. View");
        puts("                        4. Exit");
        puts("*********************************************************************\n");

        printf("Choose an option: ");
        scanf(" %c", &choice);
        r_whitespaces();

        if (choice != '1' && choice != '2' && choice != '3' && choice != '4') {
            puts("Please enter a number from 1 to 4.");
            continue;
        }

        if (choice == '4') {
            puts("Returning to Main Menu...");
            break;
        }

        if (choice == '3') { // Display archived contacts
            sort_contacts(contacts, count);
            puts("*********************************************************************");
            puts("                          Archive                                    ");
            puts("*********************************************************************");

            for (int i = 0; i < count; i++) {
                if (contacts[i].archive == 'y') {
                    contacts[i].first_name[0] = toupper(contacts[i].first_name[0]);
                    contacts[i].last_name[0] = toupper(contacts[i].last_name[0]);

                    printf("\n\tFirst Name:\t%s\n\tLast Name:\t%s\n\tContact num:\t%s\n\tEmail:\t\t%s\n",
                           contacts[i].first_name, contacts[i].last_name, contacts[i].phone_num, contacts[i].email);
                }
            }
            puts("*********************************************************************\n");

            fp = fopen("contacts.txt", "r");
            if (fp == NULL) {
                printf("Error opening file.\n");
                return;
            }

            while (fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n %c\n %c\n",
                        contacts[count].first_name, contacts[count].last_name,
                        contacts[count].phone_num, contacts[count].email,
                        &contacts[count].fav, &contacts[count].archive) == 6) {
                count++;
            }
            fclose(fp);     //sort messes with contacts struct, this returns it back to normal

            continue;
        }

        // Archive/Unarchive contacts
        while (1) {char first_name[NAME], last_name[NAME];
            int found = 0;

            puts("Enter Name:");
            r_empty_str("\tFirst Name: ", NAME, "first name", first_name);
            r_empty_str("\tLast Name: ", NAME, "last name", last_name);

            to_lowercase(first_name);
            to_lowercase(last_name);

            fpw = fopen("contacts_temp.txt", "w");
            if (fpw == NULL) {
                printf("Error creating temporary file.\n");
                return;
            }

            for (int i = 0; i < count; i++) {
                if (strcmp(contacts[i].first_name, first_name) == 0 &&
                    strcmp(contacts[i].last_name, last_name) == 0) {
                    found = 1;
                    if (choice == '1' && contacts[i].archive == 'n') {
                        confirm = yesNo("Are you sure you want to archive this contact?");
                        if (confirm) {
                            contacts[i].archive = 'y';
                        }
                    } else if (choice == '2' && contacts[i].archive == 'y') {
                        confirm = yesNo("Are you sure you want to unarchive this contact?");
                        if (confirm) {
                            contacts[i].archive = 'n';
                        }
                    } else {
                        puts(choice == '1' ? "Contact is already archived." : "Contact is not archived.");
                    }
                }
                to_lowercase(contacts[i].first_name);
                to_lowercase(contacts[i].last_name);
                fprintf(fpw, "%s\n%s\n%s\n%s\n%c\n%c\n", contacts[i].first_name, contacts[i].last_name,
                        contacts[i].phone_num, contacts[i].email, contacts[i].fav, contacts[i].archive);
            }

            fclose(fpw);
            if (found == 0) {
                puts("Sorry, No Record Found\n");
            }

            // Replace original file with the modified temporary file
            remove("contacts.txt");
            rename("contacts_temp.txt", "contacts.txt");

            if (!yesNo("Do you want to continue?")) {
                puts("\nReturning to Archive Menu");
                break;
        }
        }
    }
}

void back_up(){
    FILE *fp = fopen(fcontact, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    FILE *backUp = fopen("backUp.txt", "w");
    if (back_up == NULL) {
        printf("Error creating backup file.\n");
        fclose(fp);
        return;
    }

    int c;
    // read contents from contacts file
    while ((c = fgetc(fp)) != EOF)
    {
        fputc(c, backUp);       // write content to backUp.txt
    }
    puts("All contacts are backed up succesfully!");

    // close opened files
    fclose(fp);
    fclose(backUp);
}

void restore(){
    FILE *fp = fopen(fcontact, "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    FILE *backUp = fopen("backUp.txt", "r");
    if (back_up == NULL) {
        printf("Error opening backup file. Back-up file does not exist.\n");
        fclose(fp);
        return;
    }

    int c;
    // read contents from back-up file
    while ((c = fgetc(backUp)) != EOF)
    {
        fputc(c, fp);       // write contents to contacts.txt
    }
    puts("All contacts are restored succesfully!");

    // close opened files
    fclose(fp);
    fclose(backUp);
}

bool yesNo(const char* phrase){
    char choice;
    bool value;

    while (1) {
        printf("%s (y/n): ", phrase);
        scanf(" %c", &choice); // Use choice, not sure
        r_whitespaces();
        puts("");

        if (choice == 'y' || choice == 'Y' ) {
            value = true;
            break;
        } else if (choice == 'n' || choice == 'N' ) {
            value = false;
            break;
        } else {
            puts("Please enter 'y' for yes, and 'n' for no");
        }
    }
    return value;
}

void to_lowercase(char *str) { //declares a pointer named str that can hold the address of a character
    for(int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void r_whitespaces(){
    int ch;
        while ((ch = getchar()) != '\n')
            continue; //discard any leftover whitespaces
            //getchar() reads a character from the input buffer, discards and repeat until the character is not a newline ('\n') or end-of-file (EOF)
}

// bubble sorting the contacts by first name alphabetical order
void sort_contacts(Contact contacts[], int count) {
    Contact temp;
    
    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            // Compare first names first
            int first_name_compare = strcmp(contacts[j].first_name, contacts[j+1].first_name);
            
            // If first names are the same, compare last names
            if (first_name_compare > 0 || 
                (first_name_compare == 0 && 
                 strcmp(contacts[j].last_name, contacts[j+1].last_name) > 0)) {
                // Swap the contacts
                temp = contacts[j];
                contacts[j] = contacts[j+1];
                contacts[j+1] = temp;
            }
        }
    }
}

// Validation functions

// Input presence check
void r_empty_str(const char* phrase, int CONSTANT, const char* error_prompt, char* input) {
    bool is_empty;
    
    do {
        printf("%s", phrase);
        fgets(input, CONSTANT, stdin);
        input[strcspn(input, "\n")] = 0;  // Remove trailing newline

        // Check if the string is empty or contains only whitespace
        is_empty = true; // Declare is_empty here
        for (int i = 0; input[i] != '\0'; i++) {
            if (!isspace((unsigned char)input[i])) { 
                is_empty = false;
                break;
            }
        }

        if (is_empty) { 
            printf("Error: %s cannot be empty. Please try again.\n", error_prompt);
        }

    } while (input[0] == '\0' || is_empty);
}

// Duplication check
bool is_duplicate_contact(const char* first_name, const char* last_name) {
    FILE *fp = fopen(fcontact, "r");
    
    if (fp == NULL) { 
        return false;
    }
    
    char contacts_first_name[NAME], contacts_last_name[NAME];
    while (fscanf(fp, "%s\n%s\n", contacts_first_name, contacts_last_name) == 2) {
        if (strcmp(contacts_first_name, first_name) == 0 && strcmp(contacts_last_name, last_name) == 0) {
            fclose(fp);
            return true; // Duplicate found
        }
    }
    fclose(fp);
    return false; // No duplicates
}

// Email format check
bool is_valid_email(const char *email) {
    // Check for the presence of '@' and '.'
    const char *at_pos = strchr(email, '@');
    const char *dot_pos = strrchr(email, '.');

    // Email must contain one '@', one '.', and '@' must come before '.'
    if (at_pos == NULL || dot_pos == NULL || at_pos >= dot_pos) {
        return false;
    }

    // Check for valid characters before and after the '@'
    for (const char *ptr = email; *ptr; ptr++) {
        if (!isalnum(*ptr) && *ptr != '@' && *ptr != '.' && *ptr != '_' && *ptr != '-') {
            return false; 
        }
    }

    // Ensure there is at least one character before the '@' and after the '.'
    if (at_pos == email || *(dot_pos + 1) == '\0') {
        return false;
    }

    return true;
}

// Phone number format (digits only) check
bool is_valid_phone(const char *phone) {
    // Check if the phone number is empty
    if (strlen(phone) == 0) {
        return false;
    }

    // Check for valid characters and ensure there is at least one digit
    bool has_digit = false;
    for (const char *ptr = phone; *ptr; ptr++) {
        if (!isdigit(*ptr) && *ptr != '+' && *ptr != '-' && *ptr != ' ') {
            return false; 
        }
        
        if (isdigit(*ptr)) {
            has_digit = true; // Track if at least one digit is present
        }
    }

    // Ensure that there are at least 7 characters and at least one digit
    if (strlen(phone) < 7 || !has_digit) {
        return false;
    }

    return true;
}

//Encrypt contacts.txt File
void encrypt(){
    FILE *fp, *temp;
    char ch;

    fp = fopen("contacts.txt", "r");
    if (fp == NULL){
        puts("File read error");
        exit(1);
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL){
        puts("File write error");
        fclose(fp);
        exit(1);
    }

    while ((ch = fgetc(fp))!= EOF){
        fputc(ch+3, temp);
    }

    fclose(fp);
    fclose(temp);
    

    if (remove("contacts.txt")!=0){
        puts("Error deleting the original file");
        exit(1);
    }
    if(rename("temp.txt", "contacts.txt")!=0){
        puts("Error renaming the temporary file");
        exit(1);
    }
}

//Decrypt encrypted.txt File
void decrypt() {
    FILE *fp, *temp;
    char ch;

    // Open the encrypted file for reading
    authentication();
    fp = fopen("contacts.txt", "r");
    if (fp == NULL) {
        puts("File read error");
        exit(1);
    }

    // Open a temporary file for writing the decrypted data
    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        puts("File write error");
        fclose(fp);
        exit(1);
    }

    // Decrypt and write to the temporary file
    while ((ch = fgetc(fp)) != EOF) {
        fputc(ch - 3, temp);
    }

    fclose(fp);
    fclose(temp);

    // Replace the original file with the decrypted file
    if (remove("contacts.txt") != 0) {
        puts("Error deleting the original file");
        exit(1);
    }

    if (rename("temp.txt", "contacts.txt") != 0) {
        puts("Error renaming the temporary file");
        exit(1);
    }
}

//Authentication
void authentication(){
    int authenticated=0; //Initialize variables for password
    const char password[] = "dRSiMoN";
    char inputPassword[50];
    while(!authenticated){
        printf("\nEnter the password:");
        scanf("%49s", inputPassword); //read password input
        if(strcmp(inputPassword, password)==0){
            printf("Correct password\n");
            authenticated = 1;
        } else{
            printf("Wrong password. Please try again.\n");
        }
    }
}

void get_suggestions(const char* partial_input, int search_choice, Suggestion suggestions[], int* suggestion_count) {
    
    FILE *fp = fopen("contacts.txt", "r");
    if (fp == NULL) {
        perror("Error opening file.\n");
        return;
    }

    
    Contact contacts;
    *suggestion_count = 0;
    char lower_partial[320];
    strcpy(lower_partial, partial_input);
    to_lowercase(lower_partial);

    if (strlen(lower_partial) == 0) {
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n %c\n %c\n",
                  contacts.first_name, contacts.last_name,
                  contacts.phone_num, contacts.email,
                  &contacts.fav, &contacts.archive) == 6) {
        
        // Convert contact fields to lowercase for case-insensitive comparison
        to_lowercase(contacts.first_name);
        to_lowercase(contacts.last_name);
        to_lowercase(contacts.email);

        char* field_to_check = NULL;
        const char* field_type = NULL;

        // Determine which field to check based on search_choice
        switch(search_choice) {
            case 1:
                field_to_check = contacts.first_name;
                field_type = "First Name";
                break;
            case 2:
                field_to_check = contacts.last_name;
                field_type = "Last Name";
                break;
            case 3:
                field_to_check = contacts.phone_num;
                field_type = "Phone";
                break;
            case 4:
                field_to_check = contacts.email;
                field_type = "Email";
                break;
        }


        if (field_to_check && strncmp(field_to_check, lower_partial, strlen(lower_partial)) == 0) {
            
            contacts.first_name[0] = toupper(contacts.first_name[0]);
            contacts.last_name[0] = toupper(contacts.last_name[0]);

            if (search_choice == 1 || search_choice == 2) {
                sprintf(suggestions[*suggestion_count].value, "%s %s", contacts.first_name, contacts.last_name);
            } else if (search_choice == 3) {
                sprintf(suggestions[*suggestion_count].value, "%s %s (%s)", contacts.first_name, contacts.last_name, contacts.phone_num);
            } else if (search_choice == 4) {
                sprintf(suggestions[*suggestion_count].value, "%s %s (%s)", contacts.first_name, contacts.last_name, contacts.email);
            }
            strcpy(suggestions[*suggestion_count].type, field_type);
            (*suggestion_count)++;
        }

    }

    fclose(fp);
}