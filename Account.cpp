#include "Account.h"

#include "Date.h"
#include "LibraryItem.h"
#include "LoanRecord.h"

#include <algorithm>
#include <functional>
#include <sstream>

std::string accountStateToString(AccountState state) {
    switch (state) {
        case AccountState::Active:
            return "Active";
        case AccountState::Frozen:
            return "Frozen";
        case AccountState::Closed:
            return "Closed";
    }

    return "Unknown";
}

std::string hashPassword(const std::string& password) {
    std::ostringstream output;
    output << std::hash<std::string>{}(password);
    return output.str();
}

Account::Account(int accountID,
                 const std::string& fullName,
                 const std::string& email,
                 const std::string& password,
                 bool librarian)
    : accountID(accountID),
      fullName(fullName),
      email(email),
      passwordHash(hashPassword(password)),
      accountState(AccountState::Active),
      librarian(librarian) {}

bool Account::login(const std::string& inputEmail, const std::string& password) const {
    return email == inputEmail && verifyPassword(password) && accountState != AccountState::Closed;
}

bool Account::verifyPassword(const std::string& password) const {
    return passwordHash == hashPassword(password);
}

void Account::resetPassword(const std::string& newPassword) {
    passwordHash = hashPassword(newPassword);
}

bool Account::checkIfLibrarian() const {
    return librarian;
}

int Account::getAccountID() const {
    return accountID;
}

const std::string& Account::getFullName() const {
    return fullName;
}

const std::string& Account::getEmail() const {
    return email;
}

AccountState Account::getAccountState() const {
    return accountState;
}

void Account::setAccountState(AccountState newState) {
    accountState = newState;
}

Patron::Patron(int accountID,
               const std::string& fullName,
               const std::string& email,
               const std::string& password)
    : Account(accountID, fullName, email, password, false), totalUnpaidFines(0.0) {}

double Patron::getTotalUnpaidFines() const {
    return totalUnpaidFines;
}

bool Patron::canCheckout() const {
    return accountState == AccountState::Active && totalUnpaidFines <= 0.0;
}

Date Patron::requestCheckout(LibraryItem& item,
                             std::vector<LoanRecord>& loans,
                             const std::string& recordID,
                             const Date& checkoutDate,
                             int loanDays) {
    Date dueDate = checkoutDate.addDays(loanDays);
    loans.emplace_back(recordID, accountID, item.getItemID(), checkoutDate, dueDate);
    item.updateStatus(ItemStatus::CheckedOut);
    return dueDate;
}

double Patron::returnItem(LibraryItem& item, LoanRecord& loan, const Date& returnDate) {
    double fee = loan.closeLoan(returnDate, item);
    addFine(fee);
    item.updateStatus(ItemStatus::Available);
    return fee;
}

void Patron::addFine(double amount) {
    if (amount > 0.0) {
        totalUnpaidFines += amount;
    }
}

bool Patron::payFine(double amount) {
    if (amount <= 0.0 || amount > totalUnpaidFines) {
        return false;
    }

    totalUnpaidFines -= amount;
    if (totalUnpaidFines < 0.01) {
        totalUnpaidFines = 0.0;
    }
    return true;
}

void Patron::waiveFine(double amount) {
    if (amount <= 0.0) {
        return;
    }

    totalUnpaidFines = std::max(0.0, totalUnpaidFines - amount);
}

Librarian::Librarian(int accountID,
                     const std::string& fullName,
                     const std::string& email,
                     const std::string& password,
                     const std::string& employeeID)
    : Account(accountID, fullName, email, password, true), employeeID(employeeID) {}

const std::string& Librarian::getEmployeeID() const {
    return employeeID;
}

bool Librarian::canRemoveItem(const LibraryItem& item) const {
    return item.getStatus() != ItemStatus::CheckedOut;
}

void Librarian::freezeAccount(Patron& patron) const {
    patron.setAccountState(AccountState::Frozen);
}

void Librarian::unfreezeAccount(Patron& patron) const {
    patron.setAccountState(AccountState::Active);
}

void Librarian::waivePatronFine(Patron& patron, double amount) const {
    patron.waiveFine(amount);
}
