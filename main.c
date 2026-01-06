#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define FILENAME "users.dat"
#define TRANSACTION_FILE "transactions.dat"

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

void saveUsers(struct User s[], int n);
int loadUsers(struct User s[]);
void deposit(struct User s[], int n);
void withdraw(struct User s[], int n);
void Display(struct User s[], int n);
void acc_lookup(struct User s[], int n);
void UserInput(struct User s[], int *n);
void ShowTransactions(int accountno);
void logTransaction(int accno, const char *details);

int main() {
    struct User s[MAX_USERS];
    int choice;
    int n = loadUsers(s); // load from file
    int acc;

    do {
        printf("\n==== MENU OPTIONS ====\n");
        printf("1: Add Users\n");
        printf("2: Deposit\n");
        printf("3: Withdraw\n");
        printf("4: Display\n");
        printf("5: Search Account\n");
        printf("6: Show Transactions\n");
        printf("7: Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                UserInput(s, &n);
                saveUsers(s, n);
                break;
            case 2:
                deposit(s, n);
                saveUsers(s, n);
                break;
            case 3:
                withdraw(s, n);
                saveUsers(s, n);
                break;
            case 4:
                Display(s, n);
                break;
            case 5:
                acc_lookup(s, n);
                break;
            case 6:
                printf("Enter account number: ");
                scanf("%d", &acc);
                ShowTransactions(acc);
                break;
            case 7:
                saveUsers(s, n);
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid Input\n");
        }
    } while (choice != 7);

    return 0;
}

void UserInput(struct User s[], int *n) {
    int new_n;
    printf("Please enter the number of users: ");
    scanf("%d", &new_n);
    getchar();

    for (int i = 0; i < new_n && *n < MAX_USERS; i++, (*n)++) {
        printf("User %d Details:\n", *n + 1);
        printf("Name: ");
        fgets(s[*n].name, sizeof(s[*n].name), stdin);
        s[*n].name[strcspn(s[*n].name, "\n")] = 0;

        printf("AccountNo: ");
        scanf("%d", &s[*n].accountno);
        while (s[*n].accountno < 4000 || s[*n].accountno > 9800) {
            printf("Account number must be between 4000 and 9800: ");
            scanf("%d", &s[*n].accountno);
        }

        printf("Balance: ");
        scanf("%f", &s[*n].balance);
        while (s[*n].balance < 0) {
            printf("Balance cannot be negative: ");
            scanf("%f", &s[*n].balance);
        }

        getchar();
        printf("Password: ");
        fgets(s[*n].password, sizeof(s[*n].password), stdin);
        s[*n].password[strcspn(s[*n].password, "\n")] = 0;
    }

    if (*n >= MAX_USERS) {
        printf("Maximum user limit reached.\n");
    }
}

void saveUsers(struct User s[], int n) {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error saving users.\n");
        return;
    }
    fwrite(&n, sizeof(int), 1, fp);
    fwrite(s, sizeof(struct User), n, fp);
    fclose(fp);
}

int loadUsers(struct User s[]) {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) return 0;

    int n;
    fread(&n, sizeof(int), 1, fp);
    fread(s, sizeof(struct User), n, fp);
    fclose(fp);
    return n;
}

void deposit(struct User s[], int n) {
    int acc, found = 0;
    float amount;
    char pass[7];

    printf("Enter account to deposit into: ");
    scanf("%d", &acc);
    getchar();

    for (int i = 0; i < n; i++) {
        if (s[i].accountno == acc) {
            printf("Amount to deposit: ");
            scanf("%f", &amount);
            getchar();

            if (amount < 500) {
                printf("Minimum deposit is 500.\n");
                return;
            }

            printf("Enter password: ");
            fgets(pass, sizeof(pass), stdin);
            pass[strcspn(pass, "\n")] = 0;

            if (strcmp(s[i].password, pass) == 0) {
                s[i].balance += amount;
                printf("Deposit successful. New balance: %.2f\n", s[i].balance);

                char log_msg[100];
                sprintf(log_msg, "Deposited: $%.2f || Balance: $%.2f", amount, s[i].balance);
                logTransaction(s[i].accountno, log_msg);

                found = 1;
                break;
            } else {
                printf("Wrong password.\n");
                return;
            }
        }
    }

    if (!found) printf("Account number not found.\n");
}

void withdraw(struct User s[], int n) {
    int acc, found = 0;
    float amount;
    char pass[7];

    printf("Enter account to withdraw from: ");
    scanf("%d", &acc);
    getchar();

    for (int i = 0; i < n; i++) {
        if (s[i].accountno == acc) {
            printf("Amount to withdraw: ");
            scanf("%f", &amount);
            getchar();

            printf("Enter password: ");
            fgets(pass, sizeof(pass), stdin);
            pass[strcspn(pass, "\n")] = 0;

            if (strcmp(s[i].password, pass) == 0) {
                if (amount < 500) {
                    printf("Minimum withdrawal is 500.\n");
                    return;
                } else if (amount > s[i].balance) {
                    printf("Insufficient balance.\n");
                    return;
                } else {
                    s[i].balance -= amount;
                    printf("Withdraw successful. New balance: %.2f\n", s[i].balance);

                    char log_msg[100];
                    sprintf(log_msg, "Withdrew: $%.2f || Balance: $%.2f", amount, s[i].balance);
                    logTransaction(s[i].accountno, log_msg);

                    found = 1;
                    break;
                }
            } else {
                printf("Wrong password.\n");
                return;
            }
        }
    }

    if (!found) printf("Account number not found.\n");
}

void Display(struct User s[], int n) {
    printf("\n--- All Registered Users ---\n");
    printf("+----------------------+------------+-------------+\n");
    printf("| %-20s | %-10s | %-11s |\n", "Name", "Account No", "Balance ($)");
    printf("+----------------------+------------+-------------+\n");
    for (int i = 0; i < n; i++) {
        printf("| %-20s | %-10d | %-11.2f |\n", s[i].name, s[i].accountno, s[i].balance);
    }
    printf("+----------------------+------------+-------------+\n");
}

void acc_lookup(struct User s[], int n) {
    int acc, found = 0;
    printf("Enter account number to look up: ");
    scanf("%d", &acc);

    for (int i = 0; i < n; i++) {
        if (s[i].accountno == acc) {
            printf("\nCustomer Details\n");
            printf("NAME: %s\n", s[i].name);
            printf("AccNo: %d\n", s[i].accountno);
            printf("BAL: %.2f\n", s[i].balance);
            found = 1;
            break;
        }
    }

    if (!found) printf("Account number not found.\n");
}

void logTransaction(int accno, const char *details) {
    FILE *fp = fopen(TRANSACTION_FILE, "ab");
    if (!fp) {
        printf("Failed to open transaction file.\n");
        return;
    }

    struct Transaction t;
    t.accountno = accno;
    strncpy(t.details, details, sizeof(t.details) - 1);
    t.details[sizeof(t.details) - 1] = '\0';

    fwrite(&t, sizeof(t), 1, fp);
    fclose(fp);
}

void ShowTransactions(int accountno) {
    FILE *fp = fopen(TRANSACTION_FILE, "rb");
    if (!fp) {
        printf("No transactions found.\n");
        return;
    }

    struct Transaction t;
    int count = 0;

    printf("\n--- Transactions for Account %d ---\n", accountno);
    while (fread(&t, sizeof(t), 1, fp)) {
        if (t.accountno == accountno) {
            printf("%d. %s\n", ++count, t.details);
        }
    }

    if (count == 0) {
        printf("No transactions found for this account.\n");
    }

    fclose(fp);
}
