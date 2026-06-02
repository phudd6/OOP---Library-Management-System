#ifndef LOAN_RECORD_H
#define LOAN_RECORD_H

#include "Date.h"
#include "LibraryItem.h"

#include <string>

class LoanRecord {
private:
    std::string recordID;
    int patronID;
    std::string itemID;
    Date checkoutDate;
    Date dueDate;
    Date returnDate;
    bool returned;
    double lateFee;

public:
    LoanRecord(const std::string& recordID,
               int patronID,
               const std::string& itemID,
               const Date& checkoutDate,
               const Date& dueDate);

    bool isOverdue() const;
    bool isOverdueOn(const Date& date) const;
    double closeLoan(const Date& actualReturnDate, const LibraryItem& item);

    const std::string& getRecordID() const;
    int getPatronID() const;
    const std::string& getItemID() const;
    const Date& getCheckoutDate() const;
    const Date& getDueDate() const;
    bool isReturned() const;
    double getLateFee() const;
    std::string getDetails() const;
};

#endif
