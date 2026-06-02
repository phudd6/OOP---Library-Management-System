#ifndef TRANSACTION_RECEIPT_H
#define TRANSACTION_RECEIPT_H

#include "Date.h"

#include <string>

class TransactionReceipt {
private:
    std::string receiptID;
    int patronID;
    double amount;
    std::string paymentMethod;
    Date paymentDate;

public:
    TransactionReceipt(const std::string& receiptID,
                       int patronID,
                       double amount,
                       const std::string& paymentMethod,
                       const Date& paymentDate);

    const std::string& getReceiptID() const;
    std::string getDetails() const;
};

#endif
