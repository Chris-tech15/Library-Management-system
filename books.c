#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"
#include <time.h>
#include <ctype.h> // For isdigit() function  
#include <limits.h> // For INT_MAX 

#define ID_MAX_LEN 10  
#define TITLE_MAX_LEN 100  
#define AUTHOR_MAX_LEN 100  
#define COLOUR_MAX_LEN 20  
#define ROLE_MAX_LEN 20  
#define FACULTY_MAX_LEN 50  
#define FIRST_NAME_MAX_LEN 50  
#define LAST_NAME_MAX_LEN 50  

typedef struct {  
    char id[ID_MAX_LEN];                   // Book ID  
    char title[TITLE_MAX_LEN];             // Title of the book  
    char author[AUTHOR_MAX_LEN];           // Author of the book  
    int edition;                            // Edition number  
    int pages;                              // Number of pages  
    char colour[COLOUR_MAX_LEN];           // Colour of the book cover  
    int publication;                        // Year of publication  
    int available_num;                      // Number of available copies  
} BOOK;  

typedef struct {  
    int id;                                 // User ID  
    char first_name[FIRST_NAME_MAX_LEN];   // First name of the user  
    char last_name[LAST_NAME_MAX_LEN];     // Last name of the user  
    int age;                                // User's age  
    char role[ROLE_MAX_LEN];                // Role of the user  
    char faculty[FACULTY_MAX_LEN];         // Faculty associated with the user  
    char username[50];                      // Username  
    char password[256];                     // Store hashed password  
} USER;  

typedef struct {  
    int user_id;                            // ID of the borrowing user  
    char book_id[ID_MAX_LEN];               // ID of the borrowed book  
    char book_title[TITLE_MAX_LEN];         // Title of the borrowed book  
    char book_author[AUTHOR_MAX_LEN];       // Author of the borrowed book  
    char borrower_first_name[FIRST_NAME_MAX_LEN]; // Borrower's first name  
    char borrower_last_name[LAST_NAME_MAX_LEN];   // Borrower's last name  
    char borrower_role[ROLE_MAX_LEN];       // Borrower's role  
    time_t borrow_date;                     // Timestamp for when the book was borrowed  
    time_t return_date;                     // Timestamp for when the book was returned (0 if not returned)  
} Borrower;  

void addbook() {
    BOOK book;
    FILE *books = fopen("books.csv", "a");

    if (books == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Prompt for the book's ID
    printf("Enter the book's ID: ");
    fgets(book.id, sizeof(book.id), stdin);
    book.id[strcspn(book.id, "\n")] = 0;

    if (strlen(book.id) >= ID_MAX_LEN) {
        printf("Book ID must be shorter than %d characters.\n", ID_MAX_LEN);
        fclose(books);
        return;
    }

    // Flush the input buffer just in case
    while (getchar() != '\n');  // <-- ADD THIS LINE if needed

    // Prompt for the book's Title
    printf("Enter the book's Title: ");
    fgets(book.title, sizeof(book.title), stdin); // <-- Was getting skipped before
    book.title[strcspn(book.title, "\n")] = 0;

    // Prompt for the book's Author
    printf("Enter the book's Author: ");
    fgets(book.author, sizeof(book.author), stdin);
    book.author[strcspn(book.author, "\n")] = 0;

    printf("Enter the book's Edition (an integer): ");
    while (scanf("%d", &book.edition) != 1) {
        printf("Invalid input. Please enter an integer for the Edition: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n'); // <-- FLUSH BUFFER

    printf("Enter the book's number of pages (an integer): ");
    while (scanf("%d", &book.pages) != 1 || book.pages < 0) {
        printf("Invalid input. Please enter a non-negative integer for the number of pages: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n'); // <-- FLUSH BUFFER

    printf("Enter the book's Colour: ");
    fgets(book.colour, sizeof(book.colour), stdin);
    book.colour[strcspn(book.colour, "\n")] = 0;

    printf("Enter the book's Publication year (a positive integer): ");
    while (scanf("%d", &book.publication) != 1 || book.publication <= 0) {
        printf("Invalid input. Please enter a positive integer for the Publication year: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n'); // <-- FLUSH BUFFER

    printf("Enter the book's number of available copies (an integer): ");
    while (scanf("%d", &book.available_num) != 1 || book.available_num < 0) {
        printf("Invalid input. Please enter a non-negative integer for the number of available copies: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n'); // <-- FLUSH BUFFER

    fprintf(books, "%s,%s,%s,%d,%d,%s,%d,%d\n",
            book.id, book.title, book.author, book.edition, book.pages,
            book.colour, book.publication, book.available_num);

    fclose(books);
    printf("Book '%s' added successfully.\n", book.title);
}

// Function to list the books  
void listbook() {  
    BOOK book;  
    // Open the books CSV file in read mode to display records  
    FILE *books = fopen("books.csv", "r");  

    // Check if the file was opened successfully  
    if (books == NULL) {  
        printf("No book database found.\n");  
        return;  
    }  

    // Skip header line (read and discard)  
    char line[512];  
    fgets(line, sizeof(line), books); // Read and discard the header line  

    // Print header for the books list  
    printf("\nBooks Available:\n");  
    printf("%-4s %-30s %-30s %-7s\n", "ID", "Title", "Author", "Avail");  
    printf("---------------------------------------------------------------\n");  

    // Read each book record from the file  
    while (fscanf(books, "%4s,%99[^,],%99[^,],%d,%d,%19[^,],%d,%d\n",  
                  book.id, book.title, book.author,  
                  &book.edition, &book.pages, book.colour,  
                  &book.publication, &book.available_num) == 8) {  

        // Debugging print to check if we are reading the values
        //printf("Debugging: Book ID: %s, Title: %s, Author: %s, Available: %d\n", 
               // book.id, book.title, book.author, book.available_num);

        // Print each book's details in a formatted manner  
        printf("%-4s %-30s %-30s %-7d\n",  
               book.id, book.title, book.author, book.available_num);  
    }  

    // Check for read errors or end of file  
    if (ferror(books)) {  
        printf("Error reading from file.\n");  
    } else if (feof(books)) {  
        // Normal end of file reached  
        printf("\nEnd of book list.\n");  
    }  

    // Close the file after reading  
    fclose(books);  
}

//function to modify a Book record in the database by ID
void modifybook() {
    char target_id[ID_MAX_LEN]; // Buffer to store the target book ID
    int found = 0;               // Flag to track if the book is found

    // Ask for the ID of the book to modify
    printf("Enter the ID of the book to modify: ");
    fgets(target_id, sizeof(target_id), stdin);
    target_id[strcspn(target_id, "\n")] = 0; // Remove newline character

    // Open the books CSV file in read mode and create a temporary file in write mode
    FILE *books = fopen("books.csv", "r");
    FILE *temp = fopen("temp.csv", "w"); // Temporary file to store updated records

    // Check if both files were opened successfully
    if (!books || !temp) {
        printf("Error opening files.\n");
        if (books) fclose(books); // Close books file if opened
        if (temp) fclose(temp);   // Close temp file if opened
        return; // Exit if any file opening fails
    }

    char line[512];
    fgets(line, sizeof(line), books); // Read the CSV header line
    fprintf(temp, "%s", line);        // Copy header to temporary file

    BOOK book;

    // Read each line (book entry) from the CSV file one by one
    while (fscanf(books, "%4s,%99[^,],%99[^,],%d,%d,%19[^,],%d,%d\n", // Update format specifier to %4s
                  book.id, book.title, book.author,
                  &book.edition, &book.pages, book.colour,
                  &book.publication, &book.available_num) == 8) {

        // Check if the current book matches the target ID
        if (strcmp(book.id, target_id) == 0) {
            found = 1; // Mark that we've found the book to modify
            printf("Modifying book '%s'. Leave fields blank to keep current values.\n", book.title);

            char input[100]; // Buffer for user input to modify book details

            // Prompt for new title or keep old
            printf("Enter new Title [%s]: ", book.title);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n') { // If input is not blank
                input[strcspn(input, "\n")] = 0; // Remove newline character
                strcpy(book.title, input); // Update title
            }

            // Prompt for new author or keep old
            printf("Enter new Author [%s]: ", book.author);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n') { // If input is not blank
                input[strcspn(input, "\n")] = 0; // Remove newline character
                strcpy(book.author, input); // Update author
            }

            // Prompt for new edition or keep old
            printf("Enter new Edition [%d]: ", book.edition);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n') // If input is not blank, read and update
                book.edition = atoi(input);

            // Prompt for new number of pages or keep old
            printf("Enter new Pages [%d]: ", book.pages);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n')
                book.pages = atoi(input); // If input is not blank, read and update

            // Prompt for new colour or keep old
            printf("Enter new Colour [%s]: ", book.colour);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n') { // If input is not blank
                input[strcspn(input, "\n")] = 0; // Remove newline character
                strcpy(book.colour, input); // Update colour
            }

            // Prompt for new publication year or keep old
            printf("Enter new Publication year [%d]: ", book.publication);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n')
                book.publication = atoi(input); // If input is not blank, read and update

            // Prompt for new available number or keep old
            printf("Enter new Available number [%d]: ", book.available_num);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n')
                book.available_num = atoi(input); // If input is not blank, read and update
        }

        // Write the current (or modified) book entry to the temporary file
        fprintf(temp, "%s,%s,%s,%d,%d,%s,%d,%d\n",
                book.id, book.title, book.author, book.edition,
                book.pages, book.colour, book.publication, book.available_num);
    }

    // Close both files
    fclose(books);
    fclose(temp);

    // Replace original file with modified temp file
    remove("books.csv"); // Delete the old file
    rename("temp.csv", "books.csv"); // Rename temp file to original file name

    // Final message to indicate whether the modification was successful
    if (found)
        printf("Book ID %s modified successfully.\n", target_id);
    else
        printf("Book ID %s not found.\n", target_id);
}


// Function to delete a book from the database by ID
void deletebook() {
    char target_id[ID_MAX_LEN]; // Buffer to store the ID of the book to delete  
    int found = 0;              // Flag to track if the book was found  

    // Ask for the ID of the book to delete  
    printf("Enter the ID of the book to delete: ");  
    fgets(target_id, sizeof(target_id), stdin);  
    target_id[strcspn(target_id, "\n")] = 0; // Remove newline character  

    // Open the books CSV file in read mode and create a temporary file in write mode  
    FILE *books = fopen("books.csv", "r");  
    FILE *temp = fopen("temp.csv", "w"); // Temporary file for updated records  

    // Check if both files were opened successfully  
    if (!books || !temp) {  
        printf("Error opening files.\n");  
        if (books) fclose(books); // Close the books file if it was opened  
        if (temp) fclose(temp);   // Close the temp file if it was opened  
        return; // Exit if there was a problem opening the files  
    }  

    char line[512];  
    fgets(line, sizeof(line), books); // Read and discard the header line  
    fprintf(temp, "%s", line);        // Write the header to the temporary file  

    BOOK book;  

    // Loop through all book entries  
    while (fscanf(books, "%4s,%99[^,],%99[^,],%d,%d,%19[^,],%d,%d\n",  
                  book.id, book.title, book.author,  
                  &book.edition, &book.pages, book.colour,  
                  &book.publication, &book.available_num) == 8) {  

        // Check if the current book matches the target ID  
        if (strcmp(book.id, target_id) == 0) {  
            found = 1; // Book found, set flag  
            continue;  // Skip writing this entry to the temp file  
        }  

        // Copy other entries to the temp file if they are not deleted  
        fprintf(temp, "%s,%s,%s,%d,%d,%s,%d,%d\n",  
                book.id, book.title, book.author, book.edition,  
                book.pages, book.colour, book.publication, book.available_num);  
    }  

    // Close both files  
    fclose(books);  
    fclose(temp);  

    // Replace the original file with the updated temporary file  
    remove("books.csv"); // Delete the original file  
    rename("temp.csv", "books.csv"); // Rename the temp file to original name  

    // Final status message to inform the user whether the deletion was successful  
    if (found)  
        printf("Book ID %s deleted successfully.\n", target_id);  
    else  
        printf("Book ID %s not found.\n", target_id);  
} 

void borrowbook(int user_id, const char *book_id) {
    FILE *books_file = fopen("books.csv", "r+");
    FILE *borrowers_file = fopen("borrowers.csv", "r+");
    if (books_file == NULL || borrowers_file == NULL) {
        printf("Error opening files.\n");
        return;
    }

    BOOK book;
    int found = 0;
    char line[512];

    // Check if the book_id is valid
    printf("Searching for book ID: %s\n", book_id);

    // Skip header in books.csv
    fgets(line, sizeof(line), books_file);

    while (fgets(line, sizeof(line), books_file) != NULL) {
        line[strcspn(line, "\n")] = 0;

        if (sscanf(line, "%9[^,],%99[^,],%99[^,],%d,%d,%19[^,],%d,%d",
                   book.id, book.title, book.author,
                   &book.edition, &book.pages, book.colour,
                   &book.publication, &book.available_num) == 8) {

            //printf("Read Book ID: %s\n", book.id);

            if (strcmp(book.id, book_id) == 0) {
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        printf("Book ID not found.\n");
        fclose(books_file);
        fclose(borrowers_file);
        return;
    }

    // Check if the user_id is valid
    USER user;
    found = 0;
    FILE *users_file = fopen("users.csv", "r");
    if (users_file == NULL) {
        printf("No user database found.\n");
        fclose(books_file);
        fclose(borrowers_file);
        return;
    }

    // Skip header in users.csv
    char header[512];
    fgets(header, sizeof(header), users_file);

    while (fscanf(users_file, "%d,%49[^,],%49[^,],%d,%19[^,],%49[^,],%49[^,],%255[^\n]\n",
                  &user.id, user.first_name, user.last_name, &user.age, user.role,
                  user.faculty, user.username, user.password) == 8) {
        //printf("Read User ID: %d\n", user.id); // Optional debug
        if (user.id == user_id) {
            found = 1;
            break;
        }
    }
    fclose(users_file);

    if (!found) {
        printf("User ID not found.\n");
        fclose(books_file);
        fclose(borrowers_file);
        return;
    }

    if (book.available_num <= 0) {
        printf("Book is not available.\n");
        fclose(books_file);
        fclose(borrowers_file);
        return;
    }

    rewind(borrowers_file);
    Borrower borrower;
    while (fscanf(borrowers_file, "%d,%9[^,],%99[^,],%99[^,],%49[^,],%49[^,],%19[^,],%lld,%lld\n",
                  &borrower.user_id, borrower.book_id, borrower.book_title, borrower.book_author,
                  borrower.borrower_first_name, borrower.borrower_last_name, borrower.borrower_role,
                  &borrower.borrow_date, &borrower.return_date) == 9) {
        if (borrower.user_id == user_id && strcmp(borrower.book_id, book_id) == 0) {
            printf("User has already borrowed this book.\n");
            fclose(books_file);
            fclose(borrowers_file);
            return;
        }
    }

    // Update the book's available quantity
    rewind(books_file);
    FILE *temp_file = fopen("temp_books.csv", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file.\n");
        fclose(books_file);
        fclose(borrowers_file);
        return;
    }

    fgets(line, sizeof(line), books_file); // Header
    fprintf(temp_file, "%s", line);

    while (fgets(line, sizeof(line), books_file) != NULL) {
        BOOK temp_book;
        if (sscanf(line, "%9[^,],%99[^,],%99[^,],%d,%d,%19[^,],%d,%d",
                   temp_book.id, temp_book.title, temp_book.author,
                   &temp_book.edition, &temp_book.pages, temp_book.colour,
                   &temp_book.publication, &temp_book.available_num) == 8) {
            if (strcmp(temp_book.id, book_id) == 0) {
                temp_book.available_num--;
            }
            fprintf(temp_file, "%s,%s,%s,%d,%d,%s,%d,%d\n",
                    temp_book.id, temp_book.title, temp_book.author,
                    temp_book.edition, temp_book.pages, temp_book.colour,
                    temp_book.publication, temp_book.available_num);
        }
    }

    fclose(books_file);
    fclose(temp_file);
    remove("books.csv");
    rename("temp_books.csv", "books.csv");

    // Record the borrowing info
    borrower.user_id = user_id;
    strcpy(borrower.book_id, book.id);
    strcpy(borrower.book_title, book.title);
    strcpy(borrower.book_author, book.author);
    strcpy(borrower.borrower_first_name, user.first_name);
    strcpy(borrower.borrower_last_name, user.last_name);
    strcpy(borrower.borrower_role, user.role);
    borrower.borrow_date = time(NULL);
    borrower.return_date = 0;

    FILE *borrowers_file_append = fopen("borrowers.csv", "a");
    if (borrowers_file_append == NULL) {
        printf("Error opening borrowers file.\n");
        fclose(borrowers_file);
        return;
    }

    fprintf(borrowers_file_append, "\n%d,%s,%s,%s,%s,%s,%s,%lld,%lld\n",
            borrower.user_id, borrower.book_id, borrower.book_title, borrower.book_author,
            borrower.borrower_first_name, borrower.borrower_last_name, borrower.borrower_role,
            borrower.borrow_date, borrower.return_date);

    fclose(borrowers_file_append);
    fclose(borrowers_file);

    printf("Book borrowed successfully.\n");
}



// Function to record that a user has returned a book
void trim_spaces(char *str) {
    size_t start = 0;
    size_t end = strlen(str);
    if (end == 0) return;
    end--;

    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') start++;
    while (end > start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) end--;

    size_t new_len = end - start + 1;
    memmove(str, str + start, new_len);
    str[new_len] = '\0';
}

void returnbook(int user_id, const char *book_id) {
    FILE *borrowers_file = fopen("borrowers.csv", "r");
    FILE *temp_borrowers = fopen("temp_borrowers.csv", "w");
    if (!borrowers_file || !temp_borrowers) {
        printf("Error opening borrower files.\n");
        if (borrowers_file) fclose(borrowers_file);
        if (temp_borrowers) fclose(temp_borrowers);
        return;
    }

    Borrower borrower;
    int found = 0;

    // Write the header first
    fprintf(temp_borrowers, "user_id,book_id,book_title,book_author,borrower_first_name,borrower_last_name,borrower_role,borrow_date,return_date\n");

    char line[1024];
    fgets(line, sizeof(line), borrowers_file); // Skip header

    while (fscanf(borrowers_file, "%d,%9[^,],%99[^,],%99[^,],%49[^,],%49[^,],%19[^,],%lld,%lld\n",
                  &borrower.user_id, borrower.book_id, borrower.book_title, borrower.book_author,
                  borrower.borrower_first_name, borrower.borrower_last_name, borrower.borrower_role,
                  (long long *)&borrower.borrow_date, (long long *)&borrower.return_date) == 9) {

            /*printf("DEBUG: user_id=%d, book_id=%s, return_date=%lld\n",
                    borrower.user_id, borrower.book_id, (long long)borrower.return_date);
            printf("     > user_id match: %s\n", borrower.user_id == user_id ? "yes" : "no");
            printf("     > book_id match: %s\n", strcmp(borrower.book_id, book_id) == 0 ? "yes" : "no");
            printf("     > return_date == 0: %s\n", borrower.return_date == 0 ? "yes" : "no");
            
            printf("Entered book_id = '%s'\n", book_id);
            printf("Borrowed book_id = '%s'\n", borrower.book_id);
            printf("strcmp result: %d\n", strcmp(borrower.book_id, book_id));*/

        
        if (borrower.user_id == user_id && strcmp(borrower.book_id, book_id) == 0 && borrower.return_date == 0) {
            printf("Borrow record found. Returning book...\n");
            borrower.return_date = time(NULL);
            found = 1;
        }

        fprintf(temp_borrowers, "%d,%s,%s,%s,%s,%s,%s,%lld,%lld\n",
                borrower.user_id, borrower.book_id, borrower.book_title, borrower.book_author,
                borrower.borrower_first_name, borrower.borrower_last_name, borrower.borrower_role,
                (long long)borrower.borrow_date, (long long)borrower.return_date);
    }

    fclose(borrowers_file);
    fclose(temp_borrowers);

    if (!found) {
        printf("Borrow record not found.\n");
        remove("temp_borrowers.csv");
        return;
    }

    remove("borrowers.csv");
    rename("temp_borrowers.csv", "borrowers.csv");

    // --- Update books.csv to increase available count ---
    FILE *books_file = fopen("books.csv", "r");
    FILE *temp_books = fopen("temp_books.csv", "w");
    if (!books_file || !temp_books) {
        printf("Error opening book files.\n");
        if (books_file) fclose(books_file);
        if (temp_books) fclose(temp_books);
        return;
    }

    BOOK temp_book;
    fgets(line, sizeof(line), books_file); // Read header
    fprintf(temp_books, "%s", line);       // Write header

    while (fgets(line, sizeof(line), books_file)) {
        if (sscanf(line, "%9[^,],%99[^,],%99[^,],%d,%d,%19[^,],%d,%d",
                   temp_book.id, temp_book.title, temp_book.author,
                   &temp_book.edition, &temp_book.pages, temp_book.colour,
                   &temp_book.publication, &temp_book.available_num) == 8) {
            if (strcmp(temp_book.id, book_id) == 0) {
                temp_book.available_num++;
            }
            fprintf(temp_books, "%s,%s,%s,%d,%d,%s,%d,%d\n",
                    temp_book.id, temp_book.title, temp_book.author,
                    temp_book.edition, temp_book.pages, temp_book.colour,
                    temp_book.publication, temp_book.available_num);
        }
    }

    fclose(books_file);
    fclose(temp_books);
    remove("books.csv");
    rename("temp_books.csv", "books.csv");

    printf("Book returned successfully.\n");
}

void listborrowers() {
    FILE *file = fopen("borrowers.csv", "r");
    if (!file) {
        printf("Error: Unable to open borrowers.csv\n");
        return;
    }

    char line[512];
    printf("\n%-10s %-10s %-25s %-20s %-15s %-15s %-10s %-12s\n", 
        "User ID", "Book ID", "Title", "Author", 
        "First Name", "Last Name", "Role", "Status");
    printf("-----------------------------------------------------------------------------------------------\n");

    // Skip header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char user_id[20], book_id[20], title[100], author[100];
        char first_name[50], last_name[50], role[20];
        char borrow_date[20], return_date[20], status[20];

        sscanf(line, "%19[^,],%19[^,],%99[^,],%99[^,],%49[^,],%49[^,],%19[^,],%19[^,],%19[^\n]",
            user_id, book_id, title, author, 
            first_name, last_name, role, 
            borrow_date, return_date);

        if (strcmp(return_date, "0") == 0) {
            strcpy(status, "Not Returned");
        } else {
            strcpy(status, "Returned");
        }

        printf("%-10s %-10s %-25s %-20s %-15s %-15s %-10s %-12s\n",
            user_id, book_id, title, author, 
            first_name, last_name, role, status);
    }

    fclose(file);
}