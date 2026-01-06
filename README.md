#  C-Based Banking Management System

A **menu-driven banking management system written in C** that supports user account creation, secure deposits and withdrawals, persistent storage, and transaction history logging using binary files.

This project demonstrates **structured programming, file I/O, data persistence, and basic security concepts** using standard C.

---

##  Features

-  Create and store multiple bank users (up to 100)
-  Password-protected deposits and withdrawals
- Persistent data storage using binary files
- Transaction logging and history per account
- Account lookup by account number
- Display all registered users in tabular format
- Input validation for:
  - Account number range
  - Minimum deposit / withdrawal limits
  - Balance constraints

---

## Data Structures

### User
```c
struct User {
    char name[50];
    int accountno;
    float balance;
    char password[7];
};

struct Transaction {
    int accountno;
    char details[100];
};

##  FILE STRUCTURE
.
├── main.c                # Core application logic
├── users.dat             # Binary file storing user records
├── transactions.dat      # Binary file storing transaction logs
└── README.md

NOTE:
users.dat and transactions.dat are created automatically at runtime.
────────────────────────────────────

##  MENU OPTIONS
────────────────────────────────────
1: Add Users
2: Deposit
3: Withdraw
4: Display All Users
5: Search Account
6: Show Transactions
7: Exit
────────────────────────────────────

##  SECURITY RULES IMPLEMENTED
────────────────────────────────────
• Password authentication required for:
  - Deposits
  - Withdrawals
• Minimum transaction amount: 500
• Withdrawal blocked if:
  - Insufficient balance
  - Incorrect password
────────────────────────────────────

##   COMPILATION & EXECUTION
──────────────────────────────────── 
Compile:
gcc main.c -o banking_system

Run:
./banking_system

────────────────────────────────────


## CONCEPTS DEMONSTRATED
────────────────────────────────────
• Structs and arrays
• Binary file handling (fread, fwrite)
• Input validation
• String handling (fgets, strcmp, strncpy)
• Modular function design
• Menu-driven CLI applications
────────────────────────────────────

## POSSIBLE ENHANCEMENTS
• Hash passwords instead of plaintext
• Admin vs User roles
• Export transactions to text / CSV
• Timestamped transaction logs
• Sort users by balance or name
• Port to a client-server or database-backed system
────────────────────────────────────
