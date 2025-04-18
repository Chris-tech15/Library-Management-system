// Description: Header file for library management system

#ifndef LIBRARY_H
#define LIBRARY_H

void addbook();
void listbook();
void modifybook();
void deletebook();
void createuser();
int login(const char *username, const char *password);
void changepassword(int user_id, const char *new_password);
void listuser();
void modifyuser();
void deleteuser();
void borrowbook(int user_id, const char *book_id);
void returnbook(int user_id, const char *book_id);
void trim_spaces(char *str);
void listborrowers();

#endif