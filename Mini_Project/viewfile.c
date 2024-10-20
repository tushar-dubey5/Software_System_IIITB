#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define LoanStatus enum and Loan structure
typedef enum {
    PENDING,
    APPROVED,
    REJECTED
} LoanStatus;

typedef struct {
    int id;
    int user_id;
    double amount;
    LoanStatus status;
    int assigned_employee_id;
    time_t application_date;
    time_t decision_date;
} Loan;

// Function to convert LoanStatus to a string
const char* get_status_string(LoanStatus status) {
    switch (status) {
        case PENDING:
            return "Pending";
        case APPROVED:
            return "Approved";
        case REJECTED:
            return "Rejected";
        default:
            return "Unknown";
    }
}

// Function to print time in human-readable format
void print_time(time_t time) {
    struct tm *timeinfo = localtime(&time);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    printf("%s", buffer);
}

// Function to read and print loans from file
void print_loans(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    Loan loan;
    printf("ID\tUser ID\tAmount\t\tStatus\t\tAssigned Employee\tApplication Date\tDecision Date\n");
    printf("----------------------------------------------------------------------------------------------------\n");

    while (fread(&loan, sizeof(Loan), 1, file)) {
        printf("%d\t%d\tRs%.2f\t\t%s\t\t%d\t\t\t", 
               loan.id, loan.user_id, loan.amount, get_status_string(loan.status), loan.assigned_employee_id);

        print_time(loan.application_date); // Print application date
        printf("\t");
        if (loan.status != PENDING) {
            print_time(loan.decision_date); // Print decision date if available
        } else {
            printf("N/A");
        }
        printf("\n");
    }

    fclose(file);
}

int main() {
    const char *filename = "loans.dat";  // File containing loan data
    print_loans(filename);
    return 0;
}
