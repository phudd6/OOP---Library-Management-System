#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

class Date;
class LibraryItem;
class LoanRecord;

enum class AccountState {
    Active,
    Frozen,
    Closed
};

std::string accountStateToString(AccountState state);
std::string hashPassword(const std::string& password);

class Account {
protected:
    int accountID;
    std::string fullName;
    std::string email;
    std::string passwordHash;
    AccountState accountState;
    bool librarian;

public:
    Account(int accountID,
            const std::string& fullName,
            const std::string& email,
            const std::string& password,
            bool librarian);
    virtual ~Account() = default;

    bool login(const std::string& inputEmail, const std::string& password) const;
    bool verifyPassword(const std::string& password) const;
    void resetPassword(const std::string& newPassword);
    bool checkIfLibrarian() const;

    int getAccountID() const;
    const std::string& getFullName() const;
    const std::string& getEmail() const;
    AccountState getAccountState() const;
    void setAccountState(AccountState newState);
};

class Patron : public Account {
private:
    double totalUnpaidFines;

public:
    Patron(int accountID,
           const std::string& fullName,
           const std::string& email,
           const std::string& password);

    double getTotalUnpaidFines() const;
    bool canCheckout() const;
    Date requestCheckout(LibraryItem& item,
                         std::vector<LoanRecord>& loans,
                         const std::string& recordID,
                         const Date& checkoutDate,
                         int loanDays);
    double returnItem(LibraryItem& item, LoanRecord& loan, const Date& returnDate);
    void addFine(double amount);
    bool payFine(double amount);
    void waiveFine(double amount);
};

class Librarian : public Account {
private:
    std::string employeeID;

public:
    Librarian(int accountID,
              const std::string& fullName,
              const std::string& email,
              const std::string& password,
              const std::string& employeeID);

    const std::string& getEmployeeID() const;
    bool canRemoveItem(const LibraryItem& item) const;
    void freezeAccount(Patron& patron) const;
    void unfreezeAccount(Patron& patron) const;
    void waivePatronFine(Patron& patron, double amount) const;
};

#endif
