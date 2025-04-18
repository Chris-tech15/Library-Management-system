// File: users.c
// Description: This file contains functions to manage user accounts in a library system, including creating, modifying, deleting users, and logging in.

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <ctype.h> // For isdigit  
#include "library.h"  

#define USERNAME_MAX_LEN 50  
#define PASSWORD_MAX_LEN 256  
#define ROLE_MAX_LEN 20  
#define FACULTY_MAX_LEN 50  
#define NAME_MAX_LEN 50  

// Structure to hold user information  
typedef struct {  
    int id;  
    char first_name[NAME_MAX_LEN];  
    char last_name[NAME_MAX_LEN];  
    int age;  
    char role[ROLE_MAX_LEN];  
    char faculty[FACULTY_MAX_LEN];  
    char username[USERNAME_MAX_LEN];  
    char password[PASSWORD_MAX_LEN];  
} USER; 

// Function to create a new user  
void createuser() {  
    USER new_user;  
    FILE *users = fopen("users.csv", "a");  

    if (users == NULL) {  
        printf("Error opening file.\n");  
        return;  
    }  

    // User ID  
    printf("Enter the user's ID: ");  
    scanf("%d", &new_user.id);  

    // Check if ID is valid (integers only)  
    if (new_user.id <= 0) {  
        printf("Invalid ID. Must be a positive integer.\n");  
        fclose(users);  
        return;  
    }  
    getchar(); // clear newline  

    // First name  
    printf("Enter the user's first name: ");  
    fgets(new_user.first_name, sizeof(new_user.first_name), stdin);  
    new_user.first_name[strcspn(new_user.first_name, "\n")] = 0;  

    // Last name  
    printf("Enter the user's last name: ");  
    fgets(new_user.last_name, sizeof(new_user.last_name), stdin);  
    new_user.last_name[strcspn(new_user.last_name, "\n")] = 0;  

    // User age  
    printf("Enter the user's age: ");  
    scanf("%d", &new_user.age);  
    if (new_user.age <= 0) {  
        printf("Invalid age. Must be a positive integer.\n");  
        fclose(users);  
        return;  
    }  
    getchar(); // clear newline  

    // User role  
    printf("Enter the user's role: ");  
    fgets(new_user.role, sizeof(new_user.role), stdin);  
    new_user.role[strcspn(new_user.role, "\n")] = 0;  

    // User faculty  
    printf("Enter the user's faculty: ");  
    fgets(new_user.faculty, sizeof(new_user.faculty), stdin);  
    new_user.faculty[strcspn(new_user.faculty, "\n")] = 0;  

    // User username  
    printf("Enter the user's username: ");  
    fgets(new_user.username, sizeof(new_user.username), stdin);  
    new_user.username[strcspn(new_user.username, "\n")] = 0;  

    // User password  
    printf("Enter the user's password: ");  
    fgets(new_user.password, sizeof(new_user.password), stdin);  
    new_user.password[strcspn(new_user.password, "\n")] = 0;  

    // Writing data to users.csv  
    fprintf(users, "\n%d,%s,%s,%d,%s,%s,%s,%s\n",   
            new_user.id, new_user.first_name, new_user.last_name, new_user.age,  
            new_user.role, new_user.faculty, new_user.username,  
            new_user.password);  

    fclose(users);  

    // Success Message  
    printf("User '%s' created successfully.\n", new_user.username);  
}  


// Function to log in a user  
int login(const char *username, const char *password) {  
    FILE *users = fopen("users.csv", "r");  
    if (users == NULL) {  
        printf("No user database found.\n");  
        return 0; // Login failed  
    }  

    printf("Successfully opened users.csv\n");

    USER user;  
    int logged_in = 0;  

    rewind(users);
    char header_line[256];  
    fgets(header_line, sizeof(header_line), users); // Read and discard the header line  

    // Read user data from the file  
    while (fscanf(users, "%d,%49[^,],%49[^,],%d,%19[^,],%49[^,],%49[^,],%255[^\n]\n",  
                  &user.id, user.first_name, user.last_name, &user.age,  
                  user.role, user.faculty, user.username, user.password) == 8) { 
                    
        // Trim trailing newline characters from username and password
        user.username[strcspn(user.username, "\n")] = 0;
        user.password[strcspn(user.password, "\n")] = 0;

        // Check for username  
        //printf("Input username: [%s], Input password: [%s]\n", username, password);
        //printf("User username: [%s], User password: [%s]\n", user.username, user.password);


        if (strcmp(user.username, username) == 0) {  
            // Check for password  
            if (strcmp(password, user.password) == 0) {  
                logged_in = 1;  
                break; // Exit loop if login is successful  
            }  
        }  
    }  
    fclose(users);  

    if (logged_in == 1) {  
        printf("Login successful!\n");  
        return user.id;  
    } else {  
        printf("Login failed. Invalid username or password.\n");  
        return logged_in;  
    }  
} 


// Function to change a user's password
void changepassword(int user_id, const char *new_password) {
    FILE *users = fopen("users.csv", "r"); // Open the user file for reading
    FILE *temp = fopen("temp.csv", "w");   // Open a temporary file for writing
    if (users == NULL || temp == NULL) {
        printf("Error opening files.\n"); // Handle file opening errors
        return;
    }

    USER user;
    int found = 0;
    char line[512];
    char old_password[PASSWORD_MAX_LEN]; // Buffer for the old password

    // Copy the header from the original to the temp file
    fgets(line, sizeof(line), users);
    fprintf(temp, "%s", line);

    // Read user data, change password if ID matches
    while (fscanf(users, "%d,%49[^,],%49[^,],%d,%19[^,],%49[^,],%49[^,],%255[^\n]\n",
                  &user.id, user.first_name, user.last_name, &user.age,
                  user.role, user.faculty, user.username, user.password) == 8) {
        if (user.id == user_id) {
            found = 1; // Mark that the user ID was found

            // Get old password and verify
            printf("Enter your old password: ");
            fgets(old_password, sizeof(old_password), stdin);
            old_password[strcspn(old_password, "\n")] = 0;

            // Verify the old password
            if (strcmp(old_password, user.password) != 0) {
                printf("Incorrect old password.\n");
                fclose(users);
                fclose(temp);
                remove("temp.csv");
                return;
            }

            // Store the new password directly
            strcpy(user.password, new_password);
            fprintf(temp, "%d,%s,%s,%d,%s,%s,%s,%s\n",
                    user.id, user.first_name, user.last_name, user.age,
                    user.role, user.faculty, user.username, user.password); // Write the modified user data to the temp file with the plain text password
        } else {
            // Keep the original record
            fprintf(temp, "%d,%s,%s,%d,%s,%s,%s,%s\n",
                    user.id, user.first_name, user.last_name, user.age,
                    user.role, user.faculty, user.username, user.password); // Write the original user data to the temp file
        }
    }
    fclose(users);   // Close the original user file
    fclose(temp);    // Close the temporary file

    // Replace original file with the temporary file
    remove("users.csv");
    rename("temp.csv", "users.csv");

    if (found) {
        printf("Password changed successfully.\n"); // Success message
    } else {
        printf("User ID not found.\n"); // Error message
    }
}

// Function to list all users  
void listuser() {  
    FILE *users = fopen("users.csv", "r"); // Open the user file for reading  
    if (users == NULL) {  
        printf("No user database found.\n"); // Error message if file cannot be opened  
        return;  
    }  

    USER user;  // Assuming USER structure does not now include username and password  
    char line[512];  
    
    fgets(line, sizeof(line), users);  // Skip the header line  
    
    printf("\nUsers:\n");  
    printf("%-5s %-20s %-20s %-5s %-10s %-30s\n", "ID", "First Name", "Last Name", "Age", "Role", "Faculty");  
    printf("------------------------------------------------------------------------------------------\n");  

    // Read each line and parse user information  
    while (fgets(line, sizeof(line), users) != NULL) {  
        // Parse user data, ignoring username and password  
        if (sscanf(line, "%d,%49[^,],%49[^,],%d,%19[^,],%49[^,]",  
                   &user.id, user.first_name, user.last_name,   
                   &user.age, user.role, user.faculty) == 6) { // Expecting 6 fields  
            printf("%-5d %-20s %-20s %-5d %-10s %-30s\n",  
                   user.id, user.first_name, user.last_name, user.age, user.role, user.faculty);  
        } else {  
            printf("Error reading or parsing line: %s\n", line); // More specific error message  
        }  
    }  
    fclose(users); // Close the user file  
}  
    

// Function to modify user information  
void modifyuser() {
    int target_id;
    FILE *users = fopen("users.csv", "r");  // Open user file for reading
    FILE *temp = fopen("temp.csv", "w");    // Open temp file for writing
    if (users == NULL || temp == NULL) {
        printf("Error opening files.\n"); // Error message
        return;
    }

    printf("Enter the ID of the user to modify: ");
    scanf("%d", &target_id);            // Get the ID of the user to modify
    getchar();                          // Consume the newline character

    USER user;                         // Assuming USER structure is already defined
    int found = 0;
    char line[512];

    fgets(line, sizeof(line), users);  // Read the header line
    fprintf(temp, "%s", line);          // Write the header line to the temp file

    // Read and modify user information based on input
    while (fscanf(users, "%d,%49[^,],%49[^,],%d,%19[^,],%49[^,],%49[^,],%255[^\n]\n",
                  &user.id, user.first_name, user.last_name, &user.age, user.role, user.faculty,
                  user.username, user.password) == 8) {  // Adjusted for 8 fields

        if (user.id == target_id) {  
            found = 1;  // Mark that the user was found
            printf("Modifying user %d. Leave fields blank to keep current values.\n", target_id);

            char input[200];

            // First Name
            printf("Enter new first name [%s]: ", user.first_name);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n') {
                input[strcspn(input, "\n")] = 0;
                strcpy(user.first_name, input);  // Update first name
            }

            // Last Name
            printf("Enter new last name [%s]: ", user.last_name);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n') {
                input[strcspn(input, "\n")] = 0;
                strcpy(user.last_name, input);  // Update last name
            }

            // Age
            printf("Enter new age [%d]: ", user.age);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n') {
                user.age = atoi(input);  // Update age
            }

            // Role
            printf("Enter new role [%s]: ", user.role);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n') {
                input[strcspn(input, "\n")] = 0;
                strcpy(user.role, input);  // Update role
            }

            // Faculty
            printf("Enter new faculty [%s]: ", user.faculty);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n') {
                input[strcspn(input, "\n")] = 0;
                strcpy(user.faculty, input);  // Update faculty
            }

            // Username (keep the current username if left blank)
            printf("Enter new username [%s]: ", user.username);
            fgets(input, sizeof(input), stdin);
            if (input[0] != '\n') {
                input[strcspn(input, "\n")] = 0;
                strcpy(user.username, input);  // Update username
            }
        }

        // Write the user data to the temp file, including the username and password
        fprintf(temp, "%d,%s,%s,%d,%s,%s,%s,%s\n", user.id, user.first_name, user.last_name, user.age,
                user.role, user.faculty, user.username, user.password);
    }

    fclose(users);  // Close user file
    fclose(temp);   // Close temp file
    remove("users.csv");        // Delete original user file
    rename("temp.csv", "users.csv"); // Rename temp file to user file

    if (found) {
        printf("User %d modified successfully.\n", target_id); // Success message
    } else {
        printf("User %d not found.\n", target_id); // Error message
    }
}



void deleteuser() {
    int target_id;
    FILE *users_file = fopen("users.csv", "r");
    FILE *temp_file = fopen("temp.csv", "w");
    if (users_file == NULL || temp_file == NULL) {
        printf("Error opening files.\n");
        return;
    }

    printf("Enter the ID of the user to delete: ");
    scanf("%d", &target_id);
    getchar(); // Consume the newline character

    USER user;
    int found = 0;
    char line[512]; // Increased buffer size to handle potentially long lines

    // Copy the header line from the original file to the temporary file
    fgets(line, sizeof(line), users_file);
    fprintf(temp_file, "%s", line);

    // Iterate through the users in the original file
    while (fgets(line, sizeof(line), users_file) != NULL) {
        // Parse the line using sscanf.  Added check for number of arguments.
        if (sscanf(line, "%d,%49[^,],%49[^,],%d,%19[^,],%49[^,],%49[^,],%255[^\n]",
                   &user.id, user.first_name, user.last_name, &user.age, user.role,
                   user.faculty, user.username, user.password) == 8) {
            if (user.id == target_id) {
                found = 1;
                printf("User \"%s\" deleted.\n", user.username);
                // Don't write the user to the temp file
            } else {
                // Write the user to the temp file
                fprintf(temp_file, "%d,%s,%s,%d,%s,%s,%s,%s\n",
                        user.id, user.first_name, user.last_name, user.age, user.role,
                        user.faculty, user.username, user.password);
            }
        } else {
            // Handle the case where a line in the CSV file is malformed.
            printf("Skipping malformed line: %s", line);
        }
    }

    fclose(users_file);
    fclose(temp_file);

    // Replace the original file with the temporary file
    remove("users.csv");
    rename("temp.csv", "users.csv");

    if (!found) {
        printf("User ID not found.\n");
    }
}