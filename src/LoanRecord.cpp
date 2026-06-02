#include "LoanRecord.h"

#include <iomanip>
#include <sstream>

LoanRecord::LoanRecord(const std::string& recordID,
                       int patronID,
                       const std::string& itemID,
                       const Date& checkoutDate,
                       const Date& dueDate)
    : recordID(recordID),
      patronID(patronID),
      itemID(itemID),
      checkoutDate(checkoutDate),
      dueDate(dueDate),
      returnDate(),
      returned(false),
      lateFee(0.0) {}

bool LoanRecord::isOverdue() const {
    return !returned && Date::today() > dueDate;
}

bool LoanRecord::isOverdueOn(const Date& date) const {
    return !returned && date > dueDate;
}

double LoanRecord::closeLoan(const Date& actualReturnDate, const LibraryItem& item) {
    returnDate = actualReturnDate;
    returned = true;

    int daysOverdue = dueDate.daysUntil(actualReturnDate);
    lateFee = item.calculateLateFee(daysOverdue);
    return lateFee;
}

const std::string& LoanRecord::getRecordID() const {
    return recordID;
}

int LoanRecord::getPatronID() const {
    return patronID;
}

const std::string& LoanRecord::getItemID() const {
    return itemID;
}

const Date& LoanRecord::getCheckoutDate() const {
    return checkoutDate;
}

const Date& LoanRecord::getDueDate() const {
    return dueDate;
}

bool LoanRecord::isReturned() const {
    return returned;
}

double LoanRecord::getLateFee() const {
    return lateFee;
}

std::string LoanRecord::getDetails() const {
    std::ostringstream output;
    output << "Record ID: " << recordID << "\n"
           << "Patron ID: " << patronID << "\n"
           << "Item ID: " << itemID << "\n"
           << "Checkout Date: " << checkoutDate.toString() << "\n"
           << "Due Date: " << dueDate.toString() << "\n"
           << "Returned: " << (returned ? "Yes" : "No") << "\n";

    if (returned) {
        output << "Return Date: " << returnDate.toString() << "\n"
               << "Late Fee: $" << std::fixed << std::setprecision(2) << lateFee << "\n";
    }

    return output.str();
}
