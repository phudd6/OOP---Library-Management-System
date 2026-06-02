#ifndef LIBRARY_SYSTEM_H
#define LIBRARY_SYSTEM_H

#include "Account.h"
#include "LibraryItem.h"
#include "LoanRecord.h"
#include "TransactionReceipt.h"

#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

class LibrarySystem {
private:
    std::vector<std::unique_ptr<LibraryItem>> inventory;
    std::vector<std::unique_ptr<Account>> accounts;
    std::vector<LoanRecord> loans;
    std::vector<TransactionReceipt> receipts;
    std::map<std::string, std::queue<int>> waitlists;
    int nextAccountID;
    int nextLoanID;
    int nextReceiptID;

    void seedData();

    Account* findAccountByEmail(const std::string& email);
    Account* findAccountByID(int accountID);
    Patron* findPatronByID(int accountID);
    LibraryItem* findItemByID(const std::string& itemID);
    LoanRecord* findActiveLoan(int patronID, const std::string& itemID);

    bool emailExists(const std::string& email) const;
    bool itemExists(const std::string& itemID) const;
    bool isPatronOnWaitlist(const std::string& itemID, int patronID) const;
    int getWaitlistPosition(const std::string& itemID, int patronID) const;
    void notifyNextWaitlistedPatron(const std::string& itemID);
    std::string makeLoanID();
    std::string makeReceiptID();

    void startupMenu();
    void patronMenu(Patron& patron);
    void librarianMenu(Librarian& librarian);

    void loginFlow();
    void signUpFlow();
    void resetPasswordFlow();

    void viewCatalog() const;
    void searchCatalog() const;
    void patronBorrowFlow(Patron& patron);
    void patronReturnFlow(Patron& patron);
    void patronPayFineFlow(Patron& patron);
    void patronJoinWaitlistFlow(Patron& patron);

    void librarianAddItemFlow();
    void librarianRemoveItemFlow(const Librarian& librarian);
    void librarianFreezePatronFlow(const Librarian& librarian);
    void librarianUnfreezePatronFlow(const Librarian& librarian);
    void librarianMarkItemLostFlow();
    void librarianWaiveFineFlow(const Librarian& librarian);
    void librarianViewLoans() const;
    void librarianViewReceipts() const;
    void librarianViewWaitlists() const;

    static std::string readLine(const std::string& prompt);
    static std::string readRequiredLine(const std::string& prompt);
    static int readInt(const std::string& prompt);
    static int readPositiveInt(const std::string& prompt);
    static double readDouble(const std::string& prompt);
    static std::string toLower(std::string value);

public:
    LibrarySystem();
    void run();
};

#endif
