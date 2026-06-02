#include "LibrarySystem.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

LibrarySystem::LibrarySystem() : nextAccountID(1004), nextLoanID(1), nextReceiptID(1) {
    seedData();
}

void LibrarySystem::run() {
    startupMenu();
}

void LibrarySystem::seedData() {
    accounts.push_back(std::make_unique<Librarian>(
        1, "Head Librarian", "librarian@library.com", "admin123", "EMP001"));
    accounts.push_back(std::make_unique<Patron>(
        1001, "Alex Johnson", "alex@example.com", "pass123"));
    accounts.push_back(std::make_unique<Patron>(
        1002, "Taylor Smith", "taylor@example.com", "pass123"));
    accounts.push_back(std::make_unique<Patron>(
        1003, "Morgan Overdue", "overdue@example.com", "pass123"));

    inventory.push_back(std::make_unique<Book>(
        "B001", "Introduction to Programming", 2022, "Shelf A1",
        "Diane Zak", "978-1111531003", 520, "Computer Science"));
    inventory.push_back(std::make_unique<Book>(
        "B002", "Pride and Prejudice", 1813, "Classics Shelf 1",
        "Jane Austen", "978-0141439518", 432, "Classic Literature"));
    inventory.push_back(std::make_unique<Book>(
        "B003", "Moby-Dick", 1851, "Classics Shelf 1",
        "Herman Melville", "978-1503280786", 720, "Classic Literature"));
    inventory.push_back(std::make_unique<Book>(
        "B004", "The Great Gatsby", 1925, "Classics Shelf 2",
        "F. Scott Fitzgerald", "978-0743273565", 180, "Classic Literature"));
    inventory.push_back(std::make_unique<Book>(
        "B005", "To Kill a Mockingbird", 1960, "Classics Shelf 2",
        "Harper Lee", "978-0061120084", 336, "Classic Literature"));
    inventory.push_back(std::make_unique<Book>(
        "B006", "1984", 1949, "Classics Shelf 2",
        "George Orwell", "978-0451524935", 328, "Dystopian Fiction"));
    inventory.push_back(std::make_unique<Book>(
        "B007", "Jane Eyre", 1847, "Classics Shelf 3",
        "Charlotte Bronte", "978-0141441146", 532, "Classic Literature"));
    inventory.push_back(std::make_unique<Book>(
        "B008", "Wuthering Heights", 1847, "Classics Shelf 3",
        "Emily Bronte", "978-0141439556", 416, "Classic Literature"));
    inventory.push_back(std::make_unique<Book>(
        "B009", "The Odyssey", -700, "Classics Shelf 4",
        "Homer", "978-0140268867", 541, "Epic Poetry"));
    inventory.push_back(std::make_unique<Book>(
        "B010", "Crime and Punishment", 1866, "Classics Shelf 4",
        "Fyodor Dostoevsky", "978-0143058144", 576, "Classic Literature"));
    inventory.push_back(std::make_unique<Book>(
        "B011", "War and Peace", 1869, "Classics Shelf 4",
        "Leo Tolstoy", "978-1400079988", 1296, "Classic Literature"));
    inventory.push_back(std::make_unique<Book>(
        "B012", "Frankenstein", 1818, "Classics Shelf 5",
        "Mary Shelley", "978-0486282114", 280, "Gothic Fiction"));
    inventory.push_back(std::make_unique<Book>(
        "B013", "Dracula", 1897, "Classics Shelf 5",
        "Bram Stoker", "978-0486411095", 418, "Gothic Fiction"));
    inventory.push_back(std::make_unique<Book>(
        "B014", "The Catcher in the Rye", 1951, "Classics Shelf 6",
        "J. D. Salinger", "978-0316769488", 277, "Classic Literature"));
    inventory.push_back(std::make_unique<Book>(
        "B015", "Hamlet", 1603, "Drama Shelf 1",
        "William Shakespeare", "978-0743477123", 342, "Drama"));
    inventory.push_back(std::make_unique<Book>(
        "B016", "The Iliad", -750, "Classics Shelf 4",
        "Homer", "978-0140275360", 683, "Epic Poetry"));
    inventory.push_back(std::make_unique<Periodical>(
        "P001", "Science Monthly", 2024, "Periodicals Rack",
        "Issue 42", 12, "Future Press"));
    inventory.push_back(std::make_unique<DigitalMedia>(
        "D001", "C++ Lecture Series", 2023, "Digital Catalog",
        "MP4", 850, "https://library.example.com/media/cpp"));

    LibraryItem* pride = findItemByID("B002");
    LibraryItem* mobyDick = findItemByID("B003");
    Date checkoutDate = Date::today().addDays(-30);
    Date dueDate = checkoutDate.addDays(14);

    if (pride != nullptr) {
        pride->updateStatus(ItemStatus::CheckedOut);
        loans.emplace_back(makeLoanID(), 1003, pride->getItemID(), checkoutDate, dueDate);
    }

    if (mobyDick != nullptr) {
        mobyDick->updateStatus(ItemStatus::CheckedOut);
        loans.emplace_back(makeLoanID(), 1003, mobyDick->getItemID(), checkoutDate, dueDate);
    }
}

Account* LibrarySystem::findAccountByEmail(const std::string& email) {
    for (const auto& account : accounts) {
        if (account->getEmail() == email) {
            return account.get();
        }
    }
    return nullptr;
}

Account* LibrarySystem::findAccountByID(int accountID) {
    for (const auto& account : accounts) {
        if (account->getAccountID() == accountID) {
            return account.get();
        }
    }
    return nullptr;
}

Patron* LibrarySystem::findPatronByID(int accountID) {
    Account* account = findAccountByID(accountID);
    if (account == nullptr || account->checkIfLibrarian()) {
        return nullptr;
    }
    return dynamic_cast<Patron*>(account);
}

LibraryItem* LibrarySystem::findItemByID(const std::string& itemID) {
    for (const auto& item : inventory) {
        if (item->getItemID() == itemID) {
            return item.get();
        }
    }
    return nullptr;
}

LoanRecord* LibrarySystem::findActiveLoan(int patronID, const std::string& itemID) {
    for (auto& loan : loans) {
        if (!loan.isReturned() && loan.getPatronID() == patronID && loan.getItemID() == itemID) {
            return &loan;
        }
    }
    return nullptr;
}

bool LibrarySystem::emailExists(const std::string& email) const {
    return std::any_of(accounts.begin(), accounts.end(), [&](const auto& account) {
        return account->getEmail() == email;
    });
}

bool LibrarySystem::itemExists(const std::string& itemID) const {
    return std::any_of(inventory.begin(), inventory.end(), [&](const auto& item) {
        return item->getItemID() == itemID;
    });
}

bool LibrarySystem::isPatronOnWaitlist(const std::string& itemID, int patronID) const {
    auto match = waitlists.find(itemID);
    if (match == waitlists.end()) {
        return false;
    }

    std::queue<int> queueCopy = match->second;
    while (!queueCopy.empty()) {
        if (queueCopy.front() == patronID) {
            return true;
        }
        queueCopy.pop();
    }

    return false;
}

int LibrarySystem::getWaitlistPosition(const std::string& itemID, int patronID) const {
    auto match = waitlists.find(itemID);
    if (match == waitlists.end()) {
        return 0;
    }

    std::queue<int> queueCopy = match->second;
    int position = 1;
    while (!queueCopy.empty()) {
        if (queueCopy.front() == patronID) {
            return position;
        }
        queueCopy.pop();
        ++position;
    }

    return 0;
}

void LibrarySystem::notifyNextWaitlistedPatron(const std::string& itemID) {
    auto match = waitlists.find(itemID);
    if (match == waitlists.end() || match->second.empty()) {
        return;
    }

    int patronID = match->second.front();
    Patron* patron = findPatronByID(patronID);
    if (patron == nullptr) {
        return;
    }

    std::cout << "Waitlist notice: " << patron->getFullName()
              << " (Account ID " << patronID << ") is next in line for item "
              << itemID << ".\n";
}

std::string LibrarySystem::makeLoanID() {
    std::ostringstream output;
    output << "L" << std::setfill('0') << std::setw(4) << nextLoanID++;
    return output.str();
}

std::string LibrarySystem::makeReceiptID() {
    std::ostringstream output;
    output << "R" << std::setfill('0') << std::setw(4) << nextReceiptID++;
    return output.str();
}

void LibrarySystem::startupMenu() {
    bool running = true;

    while (running) {
        std::cout << "\n=== Library Management Program ===\n"
                  << "1. Login\n"
                  << "2. Sign up\n"
                  << "3. Reset password\n"
                  << "4. Exit\n";

        int choice = readInt("Choose an option: ");
        switch (choice) {
            case 1:
                loginFlow();
                break;
            case 2:
                signUpFlow();
                break;
            case 3:
                resetPasswordFlow();
                break;
            case 4:
                running = false;
                std::cout << "Goodbye.\n";
                break;
            default:
                std::cout << "Invalid option.\n";
                break;
        }
    }
}

void LibrarySystem::loginFlow() {
    std::string email = readRequiredLine("Email: ");
    std::string password = readRequiredLine("Password: ");

    Account* account = findAccountByEmail(email);
    if (account == nullptr || !account->login(email, password)) {
        std::cout << "Login failed. Check your email and password.\n";
        return;
    }

    std::cout << "Welcome, " << account->getFullName() << ".\n";

    if (account->checkIfLibrarian()) {
        Librarian* librarian = dynamic_cast<Librarian*>(account);
        if (librarian != nullptr) {
            librarianMenu(*librarian);
        }
    } else {
        Patron* patron = dynamic_cast<Patron*>(account);
        if (patron != nullptr) {
            patronMenu(*patron);
        }
    }
}

void LibrarySystem::signUpFlow() {
    std::string fullName = readRequiredLine("Full name: ");
    std::string email = readRequiredLine("Email: ");

    if (emailExists(email)) {
        std::cout << "An account with that email already exists.\n";
        return;
    }

    std::string password = readRequiredLine("Password: ");
    accounts.push_back(std::make_unique<Patron>(nextAccountID++, fullName, email, password));
    std::cout << "Account created. You can now log in.\n";
}

void LibrarySystem::resetPasswordFlow() {
    std::string email = readRequiredLine("Email: ");
    Account* account = findAccountByEmail(email);

    if (account == nullptr) {
        std::cout << "No account found for that email.\n";
        return;
    }

    std::string oldPassword = readRequiredLine("Old password: ");
    if (!account->verifyPassword(oldPassword)) {
        std::cout << "Old password is incorrect.\n";
        return;
    }

    std::string newPassword = readRequiredLine("New password: ");
    account->resetPassword(newPassword);
    std::cout << "Password reset successfully.\n";
}

void LibrarySystem::patronMenu(Patron& patron) {
    bool loggedIn = true;

    while (loggedIn) {
        std::cout << "\n=== Patron Menu ===\n"
                  << "Account ID: " << patron.getAccountID()
                  << " | State: " << accountStateToString(patron.getAccountState())
                  << " | Fines: $" << std::fixed << std::setprecision(2)
                  << patron.getTotalUnpaidFines() << "\n"
                  << "1. View catalog\n"
                  << "2. Search catalog\n"
                  << "3. Borrow item\n"
                  << "4. Return item\n"
                  << "5. Pay fine\n"
                  << "6. Join item waitlist\n"
                  << "7. Logout\n";

        int choice = readInt("Choose an option: ");
        switch (choice) {
            case 1:
                viewCatalog();
                break;
            case 2:
                searchCatalog();
                break;
            case 3:
                patronBorrowFlow(patron);
                break;
            case 4:
                patronReturnFlow(patron);
                break;
            case 5:
                patronPayFineFlow(patron);
                break;
            case 6:
                patronJoinWaitlistFlow(patron);
                break;
            case 7:
                loggedIn = false;
                break;
            default:
                std::cout << "Invalid option.\n";
                break;
        }
    }
}

void LibrarySystem::librarianMenu(Librarian& librarian) {
    bool loggedIn = true;

    while (loggedIn) {
        std::cout << "\n=== Librarian Menu ===\n"
                  << "Employee ID: " << librarian.getEmployeeID() << "\n"
                  << "1. View catalog\n"
                  << "2. Add library item\n"
                  << "3. Remove library item\n"
                  << "4. Freeze patron account\n"
                  << "5. Unfreeze patron account\n"
                  << "6. Mark item lost\n"
                  << "7. Waive patron fine\n"
                  << "8. View loans\n"
                  << "9. View receipts\n"
                  << "10. View waitlists\n"
                  << "11. Logout\n";

        int choice = readInt("Choose an option: ");
        switch (choice) {
            case 1:
                viewCatalog();
                break;
            case 2:
                librarianAddItemFlow();
                break;
            case 3:
                librarianRemoveItemFlow(librarian);
                break;
            case 4:
                librarianFreezePatronFlow(librarian);
                break;
            case 5:
                librarianUnfreezePatronFlow(librarian);
                break;
            case 6:
                librarianMarkItemLostFlow();
                break;
            case 7:
                librarianWaiveFineFlow(librarian);
                break;
            case 8:
                librarianViewLoans();
                break;
            case 9:
                librarianViewReceipts();
                break;
            case 10:
                librarianViewWaitlists();
                break;
            case 11:
                loggedIn = false;
                break;
            default:
                std::cout << "Invalid option.\n";
                break;
        }
    }
}

void LibrarySystem::viewCatalog() const {
    if (inventory.empty()) {
        std::cout << "The catalog is empty.\n";
        return;
    }

    std::cout << "\n=== Catalog ===\n";
    for (const auto& item : inventory) {
        std::cout << item->getDetails() << "-------------------------\n";
    }
}

void LibrarySystem::searchCatalog() const {
    std::string query = toLower(readRequiredLine("Enter item ID or title keyword: "));
    bool found = false;
    bool allowTitleSearch = query.length() >= 2;

    if (!allowTitleSearch) {
        std::cout << "Single-character searches only match exact item IDs. "
                  << "Use at least 2 characters for title keyword search.\n";
    }

    for (const auto& item : inventory) {
        bool idMatches = toLower(item->getItemID()) == query;
        bool titleMatches = allowTitleSearch &&
                            toLower(item->getTitle()).find(query) != std::string::npos;

        if (idMatches || titleMatches) {
            std::cout << item->getDetails() << "-------------------------\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "No matching item found.\n";
    }
}

void LibrarySystem::patronBorrowFlow(Patron& patron) {
    if (!patron.canCheckout()) {
        if (patron.getAccountState() != AccountState::Active) {
            std::cout << "Your account is not active, so you cannot borrow items.\n";
        } else {
            std::cout << "You must pay unpaid fines before borrowing more items.\n";
        }
        return;
    }

    std::string itemID = readRequiredLine("Item ID to borrow: ");
    LibraryItem* item = findItemByID(itemID);

    if (item == nullptr) {
        std::cout << "Item not found.\n";
        return;
    }

    if (item->getStatus() != ItemStatus::Available) {
        std::cout << "Item is not available. You can join its waitlist from the patron menu.\n";
        return;
    }

    auto waitlistMatch = waitlists.find(itemID);
    if (waitlistMatch != waitlists.end() && !waitlistMatch->second.empty()) {
        int nextPatronID = waitlistMatch->second.front();
        if (nextPatronID != patron.getAccountID()) {
            int position = getWaitlistPosition(itemID, patron.getAccountID());
            std::cout << "This item is reserved for the next waitlisted patron.\n";
            if (position > 0) {
                std::cout << "Your waitlist position is " << position << ".\n";
            } else {
                std::cout << "Join the waitlist to reserve a future turn.\n";
            }
            return;
        }

        waitlistMatch->second.pop();
        if (waitlistMatch->second.empty()) {
            waitlists.erase(waitlistMatch);
        }
        std::cout << "Waitlist reservation confirmed. You are next for this item.\n";
    }

    Date checkoutDate = Date::today();
    Date dueDate = patron.requestCheckout(*item, loans, makeLoanID(), checkoutDate, 14);

    std::cout << "Checkout successful. Due date: " << dueDate.toString() << "\n";
}

void LibrarySystem::patronReturnFlow(Patron& patron) {
    std::string itemID = readRequiredLine("Item ID to return: ");
    LibraryItem* item = findItemByID(itemID);

    if (item == nullptr) {
        std::cout << "Item not found.\n";
        return;
    }

    LoanRecord* loan = findActiveLoan(patron.getAccountID(), itemID);
    if (loan == nullptr) {
        std::cout << "No active loan found for this item on your account.\n";
        return;
    }

    Date returnDate = Date::today();
    double fee = patron.returnItem(*item, *loan, returnDate);

    std::cout << "Return successful.\n";
    if (fee > 0.0) {
        std::cout << "Late fee added: $" << std::fixed << std::setprecision(2) << fee << "\n";
    } else {
        std::cout << "No late fee.\n";
    }

    notifyNextWaitlistedPatron(itemID);
}

void LibrarySystem::patronPayFineFlow(Patron& patron) {
    std::cout << "Current unpaid fines: $" << std::fixed << std::setprecision(2)
              << patron.getTotalUnpaidFines() << "\n";

    if (patron.getTotalUnpaidFines() <= 0.0) {
        std::cout << "No fines to pay.\n";
        return;
    }

    double amount = readDouble("Payment amount: ");
    std::string method = readRequiredLine("Payment method: ");

    if (!patron.payFine(amount)) {
        std::cout << "Invalid payment amount.\n";
        return;
    }

    receipts.emplace_back(makeReceiptID(), patron.getAccountID(), amount, method, Date::today());
    const TransactionReceipt& receipt = receipts.back();

    std::cout << "Payment accepted by " << method << ". Receipt generated.\n"
              << receipt.getDetails()
              << "Remaining fines: $" << std::fixed << std::setprecision(2)
              << patron.getTotalUnpaidFines() << "\n";
}

void LibrarySystem::patronJoinWaitlistFlow(Patron& patron) {
    if (patron.getAccountState() != AccountState::Active) {
        std::cout << "Your account is not active, so you cannot join waitlists.\n";
        return;
    }

    std::string itemID = readRequiredLine("Item ID to waitlist: ");
    LibraryItem* item = findItemByID(itemID);

    if (item == nullptr) {
        std::cout << "Item not found.\n";
        return;
    }

    if (item->getStatus() == ItemStatus::Available) {
        std::cout << "This item is currently available. Borrow it instead of joining a waitlist.\n";
        return;
    }

    if (item->getStatus() == ItemStatus::Lost) {
        std::cout << "This item is marked lost and cannot accept waitlist requests.\n";
        return;
    }

    if (isPatronOnWaitlist(itemID, patron.getAccountID())) {
        std::cout << "You are already on this waitlist at position "
                  << getWaitlistPosition(itemID, patron.getAccountID()) << ".\n";
        return;
    }

    std::queue<int>& waitlist = waitlists[itemID];
    waitlist.push(patron.getAccountID());

    std::cout << "Waitlist request added. Your position is "
              << waitlist.size() << ".\n";
}

void LibrarySystem::librarianAddItemFlow() {
    std::cout << "\nItem type:\n"
              << "1. Book\n"
              << "2. Periodical\n"
              << "3. Digital media\n";

    int type = readInt("Choose item type: ");

    if (type < 1 || type > 3) {
        std::cout << "Invalid item type.\n";
        return;
    }

    std::string itemID = readRequiredLine("Item ID: ");

    if (itemExists(itemID)) {
        std::cout << "An item with that ID already exists.\n";
        return;
    }

    std::string title = readRequiredLine("Title: ");
    int year = readInt("Publication year: ");
    std::string location = readRequiredLine("Location: ");

    switch (type) {
        case 1: {
            std::string author = readRequiredLine("Author: ");
            std::string isbn = readRequiredLine("ISBN: ");
            int pageCount = readPositiveInt("Page count: ");
            std::string genre = readRequiredLine("Genre: ");
            inventory.push_back(std::make_unique<Book>(
                itemID, title, year, location, author, isbn, pageCount, genre));
            std::cout << "Book added.\n";
            break;
        }
        case 2: {
            std::string issueNumber = readRequiredLine("Issue number: ");
            int volume = readPositiveInt("Volume: ");
            std::string publisher = readRequiredLine("Publisher: ");
            inventory.push_back(std::make_unique<Periodical>(
                itemID, title, year, location, issueNumber, volume, publisher));
            std::cout << "Periodical added.\n";
            break;
        }
        case 3: {
            std::string fileFormat = readRequiredLine("File format: ");
            int fileSize = readPositiveInt("File size in MB: ");
            std::string downloadLink = readRequiredLine("Download link: ");
            inventory.push_back(std::make_unique<DigitalMedia>(
                itemID, title, year, location, fileFormat, fileSize, downloadLink));
            std::cout << "Digital media added.\n";
            break;
        }
        default:
            std::cout << "Invalid item type.\n";
            break;
    }
}

void LibrarySystem::librarianRemoveItemFlow(const Librarian& librarian) {
    std::string itemID = readRequiredLine("Item ID to remove: ");

    auto match = std::find_if(inventory.begin(), inventory.end(), [&](const auto& item) {
        return item->getItemID() == itemID;
    });

    if (match == inventory.end()) {
        std::cout << "Item not found.\n";
        return;
    }

    if (!librarian.canRemoveItem(**match)) {
        std::cout << "Cannot remove an item that is currently checked out.\n";
        return;
    }

    waitlists.erase(itemID);
    inventory.erase(match);
    std::cout << "Item removed.\n";
}

void LibrarySystem::librarianFreezePatronFlow(const Librarian& librarian) {
    int patronID = readInt("Patron account ID to freeze: ");
    Patron* patron = findPatronByID(patronID);

    if (patron == nullptr) {
        std::cout << "Patron not found.\n";
        return;
    }

    std::string reason = readRequiredLine("Reason for freezing account: ");
    std::string confirmation = toLower(readRequiredLine("Type YES to confirm: "));

    if (confirmation != "yes") {
        std::cout << "Freeze cancelled.\n";
        return;
    }

    librarian.freezeAccount(*patron);
    std::cout << "Account frozen. Reason recorded: " << reason << "\n";
}

void LibrarySystem::librarianUnfreezePatronFlow(const Librarian& librarian) {
    int patronID = readInt("Patron account ID to unfreeze: ");
    Patron* patron = findPatronByID(patronID);

    if (patron == nullptr) {
        std::cout << "Patron not found.\n";
        return;
    }

    std::string reason = readRequiredLine("Reason for unfreezing account: ");
    std::string confirmation = toLower(readRequiredLine("Type YES to confirm: "));

    if (confirmation != "yes") {
        std::cout << "Unfreeze cancelled.\n";
        return;
    }

    librarian.unfreezeAccount(*patron);
    std::cout << "Account unfrozen. Reason recorded: " << reason << "\n";
}

void LibrarySystem::librarianMarkItemLostFlow() {
    std::string itemID = readRequiredLine("Item ID to mark lost: ");
    LibraryItem* item = findItemByID(itemID);

    if (item == nullptr) {
        std::cout << "Item not found.\n";
        return;
    }

    std::string reason = readRequiredLine("Reason for marking lost: ");
    std::string confirmation = toLower(readRequiredLine("Type YES to confirm: "));

    if (confirmation != "yes") {
        std::cout << "Mark lost cancelled.\n";
        return;
    }

    item->updateStatus(ItemStatus::Lost);
    waitlists.erase(itemID);
    std::cout << "Item marked as lost. Reason recorded: " << reason << "\n";
}

void LibrarySystem::librarianWaiveFineFlow(const Librarian& librarian) {
    int patronID = readInt("Patron account ID: ");
    Patron* patron = findPatronByID(patronID);

    if (patron == nullptr) {
        std::cout << "Patron not found.\n";
        return;
    }

    std::cout << "Current unpaid fines: $" << std::fixed << std::setprecision(2)
              << patron->getTotalUnpaidFines() << "\n";

    if (patron->getTotalUnpaidFines() <= 0.0) {
        std::cout << "This patron has no fines.\n";
        return;
    }

    std::string choice = toLower(readRequiredLine("Enter amount or ALL: "));
    double amount = 0.0;

    if (choice == "all") {
        amount = patron->getTotalUnpaidFines();
    } else {
        std::istringstream stream(choice);
        if (!(stream >> amount) || amount <= 0.0 || amount > patron->getTotalUnpaidFines()) {
            std::cout << "Invalid waiver amount.\n";
            return;
        }
    }

    std::string reason = readRequiredLine("Reason for waiver: ");
    librarian.waivePatronFine(*patron, amount);
    std::cout << "Waived $" << std::fixed << std::setprecision(2) << amount
              << ". Reason recorded: " << reason << "\n";
}

void LibrarySystem::librarianViewLoans() const {
    if (loans.empty()) {
        std::cout << "No loan records exist.\n";
        return;
    }

    std::cout << "\n=== Loan Records ===\n";
    for (const auto& loan : loans) {
        std::cout << loan.getDetails() << "-------------------------\n";
    }
}

void LibrarySystem::librarianViewReceipts() const {
    if (receipts.empty()) {
        std::cout << "No transaction receipts exist.\n";
        return;
    }

    std::cout << "\n=== Transaction Receipts ===\n";
    for (const auto& receipt : receipts) {
        std::cout << receipt.getDetails() << "-------------------------\n";
    }
}

void LibrarySystem::librarianViewWaitlists() const {
    if (waitlists.empty()) {
        std::cout << "No item waitlists exist.\n";
        return;
    }

    std::cout << "\n=== Item Waitlists ===\n";
    for (const auto& entry : waitlists) {
        std::cout << "Item ID: " << entry.first << "\n";

        std::queue<int> queueCopy = entry.second;
        int position = 1;
        while (!queueCopy.empty()) {
            Account* account = nullptr;
            for (const auto& storedAccount : accounts) {
                if (storedAccount->getAccountID() == queueCopy.front()) {
                    account = storedAccount.get();
                    break;
                }
            }

            std::cout << position << ". Account ID " << queueCopy.front();
            if (account != nullptr) {
                std::cout << " - " << account->getFullName();
            }
            std::cout << "\n";

            queueCopy.pop();
            ++position;
        }

        std::cout << "-------------------------\n";
    }
}

std::string LibrarySystem::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::string LibrarySystem::readRequiredLine(const std::string& prompt) {
    while (true) {
        std::string input = readLine(prompt);
        if (!input.empty()) {
            return input;
        }

        std::cout << "This field cannot be empty.\n";
    }
}

int LibrarySystem::readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int value = 0;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter a valid whole number.\n";
    }
}

int LibrarySystem::readPositiveInt(const std::string& prompt) {
    while (true) {
        int value = readInt(prompt);
        if (value > 0) {
            return value;
        }

        std::cout << "Enter a number greater than zero.\n";
    }
}

double LibrarySystem::readDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        double value = 0.0;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter a valid number.\n";
    }
}

std::string LibrarySystem::toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return value;
}
