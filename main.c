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

int main() {
    int logged_in = 0;
    int user_id;
    char username[50];
    char password[256];
    char role[20];
    FILE *users_file;
    char input[10];
    int choice;

    printf("\n\t\t\t\t\tWELCOME TO LIBRARY MANAGEMENT SYSTEM\n\n\n");

    while (1) {
        printf("1. Login\n");
        printf("2. Create User\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);

        switch (choice) {
            case 1:
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

            case 2:
                createuser();
                break;

            case 0:
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }

        if (logged_in) {
            if (strcmp(role, "student") != 0) {
                while (1) {
                    printf("\nMenu:\n");
                    printf("1. Add Book\n2. List Books\n3. Modify Book\n4. Delete Book\n");
                    printf("5. List Users\n6. Modify User\n7. Delete User\n8. Borrow Book\n9. Return Book\n");
                    printf("10. Change Password\n0. Logout\n");
                    printf("Enter your choice: ");
                    fgets(input, sizeof(input), stdin);
                    choice = atoi(input);

                    switch (choice) {
                        case 1: addbook(); break;
                        case 2: listbook(); break;
                        case 3: modifybook(); break;
                        case 4: deletebook(); break;
                        case 5: listuser(); break;
                        case 6: modifyuser(); break;
                        case 7: deleteuser(); break;
                        case 8: {
                            char book_id[ID_MAX_LEN];
                            printf("Enter user ID: ");
                            fgets(input, sizeof(input), stdin);
                            user_id = atoi(input);
                            printf("Enter book ID: ");
                            fgets(book_id, sizeof(book_id), stdin);
                            book_id[strcspn(book_id, "\n")] = '\0';
                            trim_spaces(book_id);

                            borrowbook(user_id, book_id);
                            break;
                        }
                        case 9: {
                            char book_id[ID_MAX_LEN];
                            printf("Enter user ID: ");
                            fgets(input, sizeof(input), stdin);
                            user_id = atoi(input);
                            printf("Enter book ID: ");
                            fgets(book_id, sizeof(book_id), stdin);
                            book_id[strcspn(book_id, "\n")] = '\0';
                            trim_spaces(book_id);
                        
                            returnbook(user_id, book_id);
                            break;
                        }
                        case 10: {
                            char new_password[256];
                            printf("Enter your new password: ");
                            fgets(new_password, sizeof(new_password), stdin);
                            new_password[strcspn(new_password, "\n")] = 0;

                            changepassword(user_id, new_password);
                            break;
                        }
                        case 0:
                            printf("Logging out.\n");
                            logged_in = 0;
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                    if (choice == 0) break;
                }
            } else { // Non-student role (admin, librarian, etc.)
                while (1) {
                    printf("\nMenu:\n1. List Books\n2. List Users\n3. Change Password\n4. Borrow Book\n5. Return Book\n0. Logout\n");
                    printf("Enter your choice: ");
                    fgets(input, sizeof(input), stdin);
                    choice = atoi(input);

                    switch (choice) {
                        case 1: listbook(); break;
                        case 2: listuser(); break;
                        case 3: {
                            char new_password[256];
                            printf("Enter your new password: ");
                            fgets(new_password, sizeof(new_password), stdin);
                            new_password[strcspn(new_password, "\n")] = 0;

                            changepassword(user_id, new_password);
                            break;
                        }
                        case 4: {
                            char book_id[ID_MAX_LEN];
                            printf("Enter user ID: ");
                            fgets(input, sizeof(input), stdin);
                            user_id = atoi(input);
                            printf("Enter book ID: ");
                            fgets(book_id, sizeof(book_id), stdin);
                            book_id[strcspn(book_id, "\n")] = '\0';

                            borrowbook(user_id, book_id);
                            break;
                        }
                        case 5: {
                            char book_id[ID_MAX_LEN];
                            printf("Enter user ID: ");
                            fgets(input, sizeof(input), stdin);
                            user_id = atoi(input);
                            printf("Enter book ID: ");
                            fgets(book_id, sizeof(book_id), stdin);
                            book_id[strcspn(book_id, "\n")] = '\0';

                            returnbook(user_id, book_id);
                            break;
                        }
                        case 0:
                            printf("Logging out.\n");
                            logged_in = 0;
                            break;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                    if (choice == 0) break;
                }
            }
        }
    }
    return 0;
}
