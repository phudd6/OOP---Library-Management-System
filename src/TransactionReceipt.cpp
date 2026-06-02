#include "TransactionReceipt.h"

#include <iomanip>
#include <sstream>

TransactionReceipt::TransactionReceipt(const std::string& receiptID,
                                       int patronID,
                                       double amount,
                                       const std::string& paymentMethod,
                                       const Date& paymentDate)
    : receiptID(receiptID),
      patronID(patronID),
      amount(amount),
      paymentMethod(paymentMethod),
      paymentDate(paymentDate) {}

const std::string& TransactionReceipt::getReceiptID() const {
    return receiptID;
}

std::string TransactionReceipt::getDetails() const {
    std::ostringstream output;
    output << "Receipt ID: " << receiptID << "\n"
           << "Patron ID: " << patronID << "\n"
           << "Amount Paid: $" << std::fixed << std::setprecision(2) << amount << "\n"
           << "Payment Method: " << paymentMethod << "\n"
           << "Payment Date: " << paymentDate.toString() << "\n";
    return output.str();
}
