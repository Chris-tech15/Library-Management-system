// Description: Main file for the Library Management System
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

#define ID_MAX_LEN 10 
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

// Enums for user menu options
typedef enum {
    MAIN_EXIT = 0,
    MAIN_LOGIN = 1,
    MAIN_CREATE_USER = 2
} MainMenuOption;

typedef enum {
    ADMIN_LOGOUT = 0,
    ADMIN_ADD_BOOK = 1,
    ADMIN_LIST_BOOKS,
    ADMIN_MODIFY_BOOK,
    ADMIN_DELETE_BOOK,
    ADMIN_LIST_USERS,
    ADMIN_MODIFY_USER,
    ADMIN_DELETE_USER,
    ADMIN_BORROW_BOOK,
    ADMIN_RETURN_BOOK,
    ADMIN_CHANGE_PASSWORD,
    ADMIN_LIST_BORROWERS  // New option
} AdminMenuOption;

typedef enum {
    STUDENT_LOGOUT = 0,
    STUDENT_LIST_BOOKS = 1,
    STUDENT_LIST_USERS,
    STUDENT_CHANGE_PASSWORD,
    STUDENT_BORROW_BOOK,
    STUDENT_RETURN_BOOK
} StudentMenuOption;

// Prompt user for menu choice with validation
int get_menu_choice(int min, int max) {
    char input[10];
    int choice;

    while (1) {
        printf("Enter your choice: ");
        fgets(input, sizeof(input), stdin);
        if (input[0] == '\n') {
            printf("Empty input is not valid. Try again.\n");
            continue;
        }
        choice = atoi(input);
        if (choice >= min && choice <= max) {
            return choice;
        } else {
            printf("Invalid choice. Please enter a number between %d and %d.\n", min, max);
        }
    }
}

int main() {
    int logged_in = 0;
    int user_id;
    char username[50];
    char password[256];
    char role[20];
    FILE *users_file;
    int choice;

    printf("\n\t\t\t\t\tWELCOME TO LIBRARY MANAGEMENT SYSTEM\n\n\n");

    while (1) {
        // Main menu options
        printf("1. Login\n");
        printf("2. Create User\n");
        printf("0. Exit\n");

        choice = get_menu_choice(MAIN_EXIT, MAIN_CREATE_USER);

        switch ((MainMenuOption)choice) {
            case MAIN_LOGIN:
                // Login process
                printf("Enter username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0';

                printf("Enter password: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = '\0';

                user_id = login(username, password);
                if (user_id != 0) {
                    logged_in = 1;
                    users_file = fopen("users.csv", "r");
                    if (users_file != NULL) {
                        USER user;
                        char header[512];
                        fgets(header, sizeof(header), users_file);

                        // Search user details
                        while (fscanf(users_file, "%d,%49[^,],%49[^,],%d,%19[^,],%49[^,],%49[^,],%255[^\n]\n",
                            &user.id, user.first_name, user.last_name, &user.age, user.role,
                            user.faculty, user.username, user.password) == 8) {

                            if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
                                strcpy(role, user.role);
                                user_id = user.id;
                                break;
                            }
                        }
                        fclose(users_file);
                    } else {
                        printf("Error opening users.csv\n");
                        return 1;
                    }

                    printf("Login successful. Welcome, User ID: %d, Role: %s\n", user_id, role);
                } else {
                    printf("Login failed. Invalid username or password.\n");
                }
                break;

            case MAIN_CREATE_USER:
                // Create new user
                createuser();
                break;

            case MAIN_EXIT:
                // Exit application
                printf("Exiting program.\n");
                return 0;
        }

        // Menu after login
        if (logged_in) {
            // Admin or librarian menu
            if (strcmp(role, "student") != 0) {
                while (1) {
                    printf("\nMenu:\n");
                    printf("1. Add Book\n2. List Books\n3. Modify Book\n4. Delete Book\n");
                    printf("5. List Users\n6. Modify User\n7. Delete User\n8. Borrow Book\n9. Return Book\n");
                    printf("10. Change Password\n11. List Borrowers\n0. Logout\n");

                    choice = get_menu_choice(ADMIN_LOGOUT, ADMIN_LIST_BORROWERS);

                    switch ((AdminMenuOption)choice) {
                        case ADMIN_ADD_BOOK: addbook(); break;
                        case ADMIN_LIST_BOOKS: listbook(); break;
                        case ADMIN_MODIFY_BOOK: modifybook(); break;
                        case ADMIN_DELETE_BOOK: deletebook(); break;
                        case ADMIN_LIST_USERS: listuser(); break;
                        case ADMIN_MODIFY_USER: modifyuser(); break;
                        case ADMIN_DELETE_USER: deleteuser(); break;
                        case ADMIN_BORROW_BOOK: {
                            char book_id[ID_MAX_LEN];
                            char input[10];
                            int target_user_id;
                            printf("Enter user ID: ");
                            fgets(input, sizeof(input), stdin);
                            target_user_id = atoi(input);
                            printf("Enter book ID: ");
                            fgets(book_id, sizeof(book_id), stdin);
                            book_id[strcspn(book_id, "\n")] = '\0';
                            trim_spaces(book_id);
                            borrowbook(target_user_id, book_id);
                            break;
                        }
                        case ADMIN_RETURN_BOOK: {
                            char book_id[ID_MAX_LEN];
                            char input[10];
                            int target_user_id;
                            printf("Enter user ID: ");
                            fgets(input, sizeof(input), stdin);
                            target_user_id = atoi(input);
                            printf("Enter book ID: ");
                            fgets(book_id, sizeof(book_id), stdin);
                            book_id[strcspn(book_id, "\n")] = '\0';
                            trim_spaces(book_id);
                            returnbook(target_user_id, book_id);
                            break;
                        }
                        case ADMIN_CHANGE_PASSWORD: {
                            char new_password[256];
                            printf("Enter your new password: ");
                            fgets(new_password, sizeof(new_password), stdin);
                            new_password[strcspn(new_password, "\n")] = 0;
                            changepassword(user_id, new_password);
                            break;
                        }
                        case ADMIN_LIST_BORROWERS:
                            listborrowers();
                            break;
                        case ADMIN_LOGOUT:
                            printf("Logging out.\n");
                            logged_in = 0;
                            break;
                    }
                    if (choice == 0) break;
                }
            } else {
                // Student menu
                while (1) {
                    printf("\nMenu:\n1. List Books\n2. List Users\n3. Change Password\n4. Borrow Book\n5. Return Book\n0. Logout\n");
                    choice = get_menu_choice(STUDENT_LOGOUT, STUDENT_RETURN_BOOK);

                    switch ((StudentMenuOption)choice) {
                        case STUDENT_LIST_BOOKS: listbook(); break;
                        case STUDENT_LIST_USERS: listuser(); break;
                        case STUDENT_CHANGE_PASSWORD: {
                            char new_password[256];
                            printf("Enter your new password: ");
                            fgets(new_password, sizeof(new_password), stdin);
                            new_password[strcspn(new_password, "\n")] = 0;
                            changepassword(user_id, new_password);
                            break;
                        }
                        case STUDENT_BORROW_BOOK: {
                            char book_id[ID_MAX_LEN];
                            printf("Enter book ID: ");
                            fgets(book_id, sizeof(book_id), stdin);
                            book_id[strcspn(book_id, "\n")] = '\0';
                            borrowbook(user_id, book_id);
                            break;
                        }
                        case STUDENT_RETURN_BOOK: {
                            char book_id[ID_MAX_LEN];
                            printf("Enter book ID: ");
                            fgets(book_id, sizeof(book_id), stdin);
                            book_id[strcspn(book_id, "\n")] = '\0';
                            returnbook(user_id, book_id);
                            break;
                        }
                        case STUDENT_LOGOUT:
                            printf("Logging out.\n");
                            logged_in = 0;
                            break;
                    }
                    if (choice == 0) break;
                }
            }
        }
    }
    return 0;
}
