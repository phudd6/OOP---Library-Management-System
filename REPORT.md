# Library Management Program Report

## 1. Project Overview

This project is a console-based Library Management Program written in C++17 for an Object-Oriented Programming final group project. The program allows users to manage a library catalog, borrow and return items, calculate overdue fines, manage patron accounts, and record payment transactions.

The system supports two main user roles:

- **Patron**: can view/search the catalog, borrow items, return items, view fines, and pay fines.
- **Librarian**: can add/remove library items, freeze/unfreeze patron accounts, mark items as lost, waive fines, view loan records, and view transaction receipts.

The project uses object-oriented programming principles such as inheritance, abstraction, encapsulation, polymorphism, and composition.

## 2. Objectives

The main objectives of this project are:

- Build a working library management system using C++.
- Apply OOP concepts in a practical software design.
- Use inheritance to model different types of library items and accounts.
- Use polymorphism for item-specific detail display and late-fee calculation.
- Track checkout records, return records, late fees, and fine payments.
- Provide a menu-driven interface that can be demonstrated from the terminal.

## 3. Development Environment

- **Language**: C++17
- **Interface**: Console / terminal
- **Storage**: In-memory storage using `std::vector`, `std::map`, and `std::queue`
- **Compiler command**:

```bash
g++ -std=c++17 src/*.cpp -o library_app
```

To run the program:

```bash
./library_app
```

## 4. System Features

### 4.1 Account Features

The system supports account creation, login, and password reset.

Implemented account features:

- Login using email and password.
- Sign up as a new patron.
- Reset password by providing the old password.
- Store password values as hashed strings instead of plain text.
- Identify whether the logged-in user is a patron or librarian.
- Support account states: `Active`, `Frozen`, and `Closed`.

### 4.2 Patron Features

Patrons can:

- View all catalog items.
- Search catalog items by title or item ID.
- Borrow available items.
- Return borrowed items.
- Automatically receive late fees for overdue physical items.
- Pay fines.
- Receive a transaction receipt after payment.
- Join a waitlist for an unavailable checked-out item.

Borrowing is blocked if:

- The account is frozen.
- The patron has unpaid fines.
- The selected item is unavailable.

### 4.3 Librarian Features

Librarians can:

- View the full catalog.
- Add a new book, periodical, or digital media item.
- Remove available or lost items.
- Prevent removal of checked-out items.
- Freeze patron accounts.
- Unfreeze patron accounts.
- Mark items as lost.
- Waive patron fines.
- View all loan records.
- View all transaction receipts.
- View item waitlists.

### 4.4 Library Item Features

The catalog supports three item types:

- `Book`
- `Periodical`
- `DigitalMedia`

Each item type has its own details and late-fee behavior.

Late fee rules:

| Item Type | Fee Rule |
| --- | --- |
| Book | `$0.50` per overdue day |
| Periodical | `$0.25` per overdue day |
| DigitalMedia | `$0.00` per overdue day |

Item statuses are represented using an enum:

```cpp
enum class ItemStatus {
    Available,
    CheckedOut,
    Lost
};
```

This is safer than storing item statuses as raw strings because it prevents spelling mistakes in status comparisons.

## 5. OOP Design

### 5.1 Class Structure

```text
LibraryItem
  -> Book
  -> Periodical
  -> DigitalMedia

Account
  -> Patron
  -> Librarian

LoanRecord
TransactionReceipt
Date
LibrarySystem
```

### 5.2 LibraryItem Hierarchy

`LibraryItem` is an abstract base class. It stores attributes shared by all library items:

- `itemID`
- `title`
- `publicationYear`
- `status`
- `location`

It also declares two important virtual functions:

- `getDetails()`
- `calculateLateFee(int daysOverdue)`

These functions are overridden by `Book`, `Periodical`, and `DigitalMedia`.

This demonstrates **abstraction** and **polymorphism**. The program can treat all catalog items as `LibraryItem` objects while still calling the correct subclass behavior at runtime.

### 5.3 Book Class

`Book` extends `LibraryItem` and adds:

- `author`
- `isbn`
- `pageCount`
- `genre`

Books calculate late fees at `$0.50` per overdue day.

### 5.4 Periodical Class

`Periodical` extends `LibraryItem` and adds:

- `issueNumber`
- `volume`
- `publisher`

Periodicals calculate late fees at `$0.25` per overdue day.

### 5.5 DigitalMedia Class

`DigitalMedia` extends `LibraryItem` and adds:

- `fileFormat`
- `fileSize`
- `downloadLink`

Digital media items do not charge late fees because access can be considered automatically controlled.

### 5.6 Account Hierarchy

`Account` is the base class for users of the system. It stores:

- `accountID`
- `fullName`
- `email`
- `passwordHash`
- `accountState`
- `isLibrarian`

`Patron` and `Librarian` inherit from `Account`.

This demonstrates **inheritance** and **encapsulation** because common user fields and login behavior are stored in the base class, while role-specific behavior is implemented in derived classes.

### 5.7 Patron Class

`Patron` adds:

- `totalUnpaidFines`

Patron behavior includes:

- Checking whether the patron is eligible to borrow.
- Requesting checkout.
- Returning items.
- Paying fines.
- Receiving fines.
- Having fines waived by a librarian.

### 5.8 Librarian Class

`Librarian` adds:

- `employeeID`

Librarian behavior includes:

- Checking whether an item can be removed.
- Freezing patron accounts.
- Unfreezing patron accounts.
- Waiving patron fines.

### 5.9 LoanRecord Class

`LoanRecord` stores information about a checkout transaction:

- `recordID`
- `patronID`
- `itemID`
- `checkoutDate`
- `dueDate`
- `returnDate`
- `returned`
- `lateFee`

It can check overdue status and close a loan when an item is returned.

### 5.10 TransactionReceipt Class

`TransactionReceipt` records fine payments. It stores:

- `receiptID`
- `patronID`
- `amount`
- `paymentMethod`
- `paymentDate`

This class makes the payment feature more complete because the system creates an actual receipt object rather than only printing a message.

### 5.11 Date Class

The `Date` class supports:

- Getting the current date.
- Parsing dates.
- Adding days.
- Comparing dates.
- Calculating day differences.

The system uses this class to calculate due dates and overdue fines.

### 5.12 LibrarySystem Class

`LibrarySystem` coordinates the whole program. It stores:

- Catalog inventory
- Accounts
- Loan records
- Transaction receipts

It also controls the console menus and connects user input to the correct object behavior.

## 6. Data Storage

The program uses in-memory storage. This means data is stored in vectors while the program is running:

```cpp
std::vector<std::unique_ptr<LibraryItem>> inventory;
std::vector<std::unique_ptr<Account>> accounts;
std::vector<LoanRecord> loans;
std::vector<TransactionReceipt> receipts;
```

The data resets when the program exits. This design was chosen to keep the project focused on OOP design and core system behavior instead of file parsing or database management.

The waitlist feature uses a queue:

```cpp
std::map<std::string, std::queue<int>> waitlists;
```

The `std::map` connects an item ID to its waitlist, and the `std::queue<int>` stores patron account IDs in first-come-first-served order. This is a meaningful use of a queue because the first patron who joins an item's waitlist should be the first patron allowed to borrow it when the item becomes available.

## 7. Seed Data

The program starts with seeded accounts and catalog items so it can be tested immediately.

### 7.1 Seeded Accounts

| Role | Email | Password | Purpose |
| --- | --- | --- | --- |
| Librarian | `librarian@library.com` | `admin123` | Demonstrates librarian functions |
| Patron | `alex@example.com` | `pass123` | Regular patron account |
| Patron | `taylor@example.com` | `pass123` | Regular patron account |
| Patron | `overdue@example.com` | `pass123` | Account with overdue books for testing fines |

### 7.2 Seeded Catalog

The catalog includes sample project materials and famous literature titles, including:

- Pride and Prejudice
- Moby-Dick
- The Great Gatsby
- To Kill a Mockingbird
- 1984
- Jane Eyre
- Wuthering Heights
- The Odyssey
- Crime and Punishment
- War and Peace
- Frankenstein
- Dracula
- The Catcher in the Rye
- Hamlet
- The Iliad

Ancient publication years are displayed as BCE, for example:

```text
Year: 700 BCE
```

## 8. Main Workflows

### 8.1 Login Workflow

1. User selects login.
2. User enters email and password.
3. System checks account credentials.
4. If the user is a librarian, the librarian menu opens.
5. If the user is a patron, the patron menu opens.
6. If login fails, an error message is displayed.

### 8.2 Checkout Workflow

1. Patron selects borrow item.
2. Patron enters item ID.
3. System checks account status and unpaid fines.
4. System checks item availability.
5. System creates a `LoanRecord`.
6. System changes item status to `CheckedOut`.
7. System displays the due date.

### 8.3 Return Workflow

1. Patron selects return item.
2. Patron enters item ID.
3. System finds the active loan record.
4. System calculates overdue days.
5. System calculates late fee using the specific item type.
6. System closes the loan.
7. System changes item status to `Available`.
8. System adds any fine to the patron account.

### 8.4 Fine Payment Workflow

1. Patron selects pay fine.
2. System displays current unpaid fines.
3. Patron enters payment amount and method.
4. System validates the payment amount.
5. System deducts the fine.
6. System creates a `TransactionReceipt`.
7. System displays the receipt.

### 8.5 Waitlist Workflow

1. Patron selects join item waitlist.
2. Patron enters the item ID.
3. System checks that the item exists.
4. System only allows waitlisting for checked-out items.
5. System adds the patron ID to the item's `std::queue`.
6. When the item is returned, the system shows which patron is next.
7. When the item is available, only the patron at the front of the queue can borrow it first.

This demonstrates the FIFO behavior of a queue.

### 8.6 Librarian Item Management Workflow

Librarians can add new items by choosing the item type and entering required fields. They can remove items only if the item is not currently checked out. They can also mark an item as lost, which updates its status to `Lost`.

### 8.7 Librarian Account Management Workflow

Librarians can freeze and unfreeze patron accounts. A frozen account cannot borrow new items. Librarians can also waive patron fines.

## 9. Error Handling Approach

The project mainly uses direct validation with `if` statements instead of `throw` and `catch`. This is appropriate for most of the current console-menu errors because invalid user input is expected and frequent. Examples include wrong menu choices, missing item IDs, invalid payment amounts, or trying to borrow an unavailable item.

For this project, regular validation is clearer than exceptions because these are normal workflow cases, not unexpected program failures. Exceptions would be more useful for serious problems such as file loading errors, corrupted saved data, or failed database operations. Since this version uses in-memory storage and simple menus, conditional validation keeps the code easier to read and demonstrate.

## 10. Testing

The project was compiled with strict warnings:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/*.cpp -o library_app
```

The compile completed successfully without warnings.

### 10.1 Play-Tested Features

The following features were play-tested:

- Login
- Sign up
- Password reset
- Patron catalog view
- Patron catalog search
- Borrow available item
- Return non-overdue item
- Return overdue item
- Late fee generation
- Fine payment
- Transaction receipt generation
- Joining an item waitlist
- Queue priority when borrowing a waitlisted item
- Librarian view waitlists
- Librarian add book
- Librarian add periodical
- Librarian add digital media
- Librarian remove available item
- Librarian blocked from removing checked-out item
- Librarian freeze patron
- Frozen patron blocked from borrowing
- Librarian unfreeze patron
- Librarian waive fine
- Librarian mark item lost
- Librarian view loans
- Librarian view receipts
- BCE year display
- Late-fee formulas for book, periodical, and digital media

### 10.2 Example Demo Scenario

To demonstrate overdue fines:

1. Log in as:

```text
Email: overdue@example.com
Password: pass123
```

2. Choose return item.
3. Return item `B002` or `B003`.
4. The system adds a late fee.
5. Choose pay fine.
6. Enter an amount and payment method.
7. The system prints a transaction receipt.

## 11. OOP Principles Used

### 11.1 Encapsulation

Class attributes are private or protected, and outside code accesses them through public methods. For example, `Patron` keeps `totalUnpaidFines` private and provides methods such as `payFine`, `addFine`, and `waiveFine`.

### 11.2 Inheritance

The project uses inheritance in two major areas:

- `Book`, `Periodical`, and `DigitalMedia` inherit from `LibraryItem`.
- `Patron` and `Librarian` inherit from `Account`.

### 11.3 Polymorphism

`LibraryItem` defines virtual methods that subclasses override:

- `getDetails()`
- `calculateLateFee()`

This allows the system to store different item types together while still calling the correct behavior for each type.

### 11.4 Abstraction

`LibraryItem` is an abstract base class because it represents a general item but cannot be used directly. Specific item types must implement their own details and fine calculations.

### 11.5 Composition

`LibrarySystem` contains collections of accounts, items, loan records, and receipts. `LoanRecord` also uses `Date` objects to represent checkout, due, and return dates.

## 12. Technical Implementation Of The UML Design In C++

This section explains how the UML design was converted into the actual C++ implementation. The purpose of this section is to focus on the technical side of the project: how classes were represented in code, how UML relationships became C++ relationships, how object-oriented programming concepts were applied, and how the main system workflows were implemented using C++ features and data structures.

The UML diagram worked as the blueprint for the system. In the final program, each major UML class was translated into a C++ class. Class attributes in the UML became private or protected data members. UML operations became public member functions. UML inheritance relationships became C++ inheritance using the `public` inheritance keyword. UML associations were implemented through object references, pointers, vectors, queues, and other standard library containers.

The implementation separates the project into header files and source files inside the `src/` folder. Header files such as `src/LibraryItem.h`, `src/Account.h`, and `src/LibrarySystem.h` contain class declarations. Source files such as `src/LibraryItem.cpp`, `src/Account.cpp`, and `src/LibrarySystem.cpp` contain the function definitions. This structure makes the program easier to read because the class interface is separated from the implementation details.

### 12.1 Overall File Organization

The project is organized into several C++ files. Each file has a specific responsibility:

| File | Main Purpose |
| --- | --- |
| `src/main.cpp` | Starts the program by creating a `LibrarySystem` object and calling `run()` |
| `src/LibrarySystem.h/.cpp` | Controls menus, stores system data, and connects user input to class behavior |
| `src/LibraryItem.h/.cpp` | Defines the abstract item class and item subclasses |
| `src/Account.h/.cpp` | Defines account, patron, and librarian behavior |
| `src/LoanRecord.h/.cpp` | Tracks checkout, due date, return date, and late fee information |
| `src/TransactionReceipt.h/.cpp` | Records fine payment receipt data |
| `src/Date.h/.cpp` | Provides date operations used by loans, returns, and receipts |
| `assets/` | Stores UML diagrams and report images |

This file organization follows a common C++ design pattern. Each major class or group of closely related classes has a header file and a source file. The header file tells the rest of the program what the class can do, while the source file explains how the class does it.

The `src/main.cpp` file is intentionally simple:

```cpp
int main() {
    LibrarySystem system;
    system.run();
    return 0;
}
```

This keeps startup logic separate from system logic. The `main()` function does not handle menus, accounts, catalog items, or loans directly. Instead, it delegates responsibility to the `LibrarySystem` class.

### 12.2 Implementation Of The LibraryItem UML Hierarchy

In the UML design, `LibraryItem` is the parent class for all item types. In C++, this was implemented as an abstract base class. The class contains the attributes that are shared by every library item:

- `itemID`
- `title`
- `publicationYear`
- `status`
- `location`

The C++ implementation uses protected data members for these shared fields:

```cpp
class LibraryItem {
protected:
    std::string itemID;
    std::string title;
    int publicationYear;
    ItemStatus status;
    std::string location;
};
```

The fields are protected instead of public. This supports encapsulation because outside code cannot directly modify these values. At the same time, derived classes such as `Book`, `Periodical`, and `DigitalMedia` can still access the shared item information when building their own detailed output.

The UML design also described abstract or virtual functions. In C++, these were implemented as pure virtual functions:

```cpp
virtual std::string getDetails() const = 0;
virtual double calculateLateFee(int daysOverdue) const = 0;
virtual std::string getType() const = 0;
```

The `= 0` syntax makes the method pure virtual. This means `LibraryItem` cannot be instantiated directly. The program cannot create a generic library item because a generic item does not know its own details or late-fee formula. Instead, the program must create a specific item type, such as a book, periodical, or digital media item.

This is a direct C++ implementation of abstraction in the UML diagram. The base class defines what all library items must be able to do, but it does not define every detail itself.

### 12.3 Implementation Of Book, Periodical, And DigitalMedia

The UML subclasses `Book`, `Periodical`, and `DigitalMedia` were implemented as C++ classes that inherit from `LibraryItem`.

The inheritance relationship is shown in code like this:

```cpp
class Book : public LibraryItem {
    // Book-specific attributes and methods
};
```

This means `Book` is a specialized version of `LibraryItem`. A `Book` has all the shared item attributes from `LibraryItem`, but it also has additional attributes:

- `author`
- `isbn`
- `pageCount`
- `genre`

The same idea is used for `Periodical` and `DigitalMedia`. `Periodical` adds issue number, volume, and publisher. `DigitalMedia` adds file format, file size, and download link.

Each subclass overrides `getDetails()` so it can show both shared information and subclass-specific information. For example, a book displays title, status, location, author, ISBN, page count, and genre. A digital media item displays title, status, location, file format, file size, and download link.

The subclasses also override `calculateLateFee()`. This is one of the clearest examples of polymorphism in the project:

| Class | Late Fee Formula |
| --- | --- |
| `Book` | `daysOverdue * 0.50` |
| `Periodical` | `daysOverdue * 0.25` |
| `DigitalMedia` | `0.00` |

The important technical point is that the rest of the system can call:

```cpp
item.calculateLateFee(daysOverdue);
```

without needing to know whether the item is a book, periodical, or digital media item. C++ chooses the correct overridden function at runtime. This is runtime polymorphism.

### 12.4 Polymorphic Storage With unique_ptr

The UML design includes many different item types, but the system needs to store them in one catalog. In C++, objects of different derived classes cannot be stored directly in the same `std::vector` unless they are accessed through a common base type. To solve this, the project stores catalog items using base-class pointers:

```cpp
std::vector<std::unique_ptr<LibraryItem>> inventory;
```

This line is important because it combines several C++ concepts:

- `std::vector` provides dynamic array storage.
- `std::unique_ptr` manages memory automatically.
- `LibraryItem` is the base class used for polymorphism.

Using `std::unique_ptr<LibraryItem>` allows the vector to store `Book`, `Periodical`, and `DigitalMedia` objects together. For example, the program can push a `Book` into the inventory like this:

```cpp
inventory.push_back(std::make_unique<Book>(...));
```

Even though the vector type is `std::unique_ptr<LibraryItem>`, the actual object can be a `Book`. When `getDetails()` or `calculateLateFee()` is called, C++ uses virtual dispatch to call the correct subclass method.

This approach also avoids manual memory management. Without `std::unique_ptr`, the program would need to use `new` and `delete`, which could easily cause memory leaks. `std::unique_ptr` automatically destroys the object when it is removed from the vector or when the program ends.

### 12.5 Item Status Implementation With enum class

The original design described item status as a string, such as `"Available"`, `"Checked Out"`, or `"Lost"`. In the C++ implementation, this was improved by using an enum:

```cpp
enum class ItemStatus {
    Available,
    CheckedOut,
    Lost
};
```

This is more reliable than using raw strings. If the system used strings, a typing mistake like `"CheckedOut"` instead of `"Checked Out"` could cause logic errors. With an enum, the compiler checks that only valid status values are used.

For display purposes, the enum is converted back into readable text using:

```cpp
std::string itemStatusToString(ItemStatus status);
```

This design separates internal logic from display formatting. Internally, the program uses safe enum values. Externally, the user sees readable text such as `Available`, `Checked Out`, and `Lost`.

### 12.6 Implementation Of The Account UML Hierarchy

The UML design also includes an account hierarchy. In C++, `Account` is the base class, and `Patron` and `Librarian` inherit from it.

The base class stores shared account attributes:

```cpp
class Account {
protected:
    int accountID;
    std::string fullName;
    std::string email;
    std::string passwordHash;
    AccountState accountState;
    bool librarian;
};
```

This follows the same design idea as `LibraryItem`. Common data is placed in the parent class, while role-specific data is placed in child classes.

The account state is represented using another enum:

```cpp
enum class AccountState {
    Active,
    Frozen,
    Closed
};
```

This enum is used to control patron behavior. For example, a frozen account cannot borrow items. The use of an enum makes account state checks safer and easier to understand.

The `Account` class also contains login-related behavior:

- `login(...)`
- `verifyPassword(...)`
- `resetPassword(...)`
- `checkIfLibrarian()`

Passwords are stored as hashed values using `std::hash<std::string>`. This is not meant to be production-level security, but it satisfies the design requirement that plain text passwords should not be stored directly.

### 12.7 Patron Class Implementation

The `Patron` class represents a normal library user. It inherits all shared account information from `Account` and adds:

```cpp
double totalUnpaidFines;
```

The patron class includes behavior that belongs specifically to patrons:

- `canCheckout()`
- `requestCheckout(...)`
- `returnItem(...)`
- `addFine(...)`
- `payFine(...)`
- `waiveFine(...)`

The `canCheckout()` method checks whether the account is active and whether the patron has unpaid fines. This method is used before allowing a checkout:

```cpp
bool Patron::canCheckout() const {
    return accountState == AccountState::Active && totalUnpaidFines <= 0.0;
}
```

The checkout process is implemented through `requestCheckout(...)`. This method creates a due date, adds a new loan record, and changes the item status to checked out. Keeping this behavior in `Patron` matches the UML design because requesting checkout is a patron operation.

The return process is implemented through `returnItem(...)`. This method closes the loan, calculates the late fee, adds the fee to the patron account, and changes the item status back to available.

This design keeps patron-specific responsibilities inside the `Patron` class instead of placing all behavior directly in the menu code.

### 12.8 Librarian Class Implementation

The `Librarian` class represents staff-level users. It inherits from `Account` and adds:

```cpp
std::string employeeID;
```

Librarian behavior includes:

- `canRemoveItem(...)`
- `freezeAccount(...)`
- `unfreezeAccount(...)`
- `waivePatronFine(...)`

The `canRemoveItem(...)` method prevents deletion of checked-out items:

```cpp
bool Librarian::canRemoveItem(const LibraryItem& item) const {
    return item.getStatus() != ItemStatus::CheckedOut;
}
```

This prevents a logic error where an item could be deleted while a patron still has an active loan for it.

The librarian class also controls account management operations. Freezing and unfreezing accounts updates the patron's `AccountState`. Waiving a fine calls the patron's fine-related method. This design keeps librarian permissions inside the librarian role.

### 12.9 Role-Based Menu Integration

The `LibrarySystem` class controls the program menus. After login, the program checks whether the account is a librarian:

```cpp
if (account->checkIfLibrarian()) {
    Librarian* librarian = dynamic_cast<Librarian*>(account);
    librarianMenu(*librarian);
} else {
    Patron* patron = dynamic_cast<Patron*>(account);
    patronMenu(*patron);
}
```

The `dynamic_cast` is used because accounts are stored through base-class pointers:

```cpp
std::vector<std::unique_ptr<Account>> accounts;
```

Since the actual object may be a `Patron` or a `Librarian`, `dynamic_cast` safely converts the base pointer into the correct derived type after the role is known.

The menu system demonstrates how UML operations become user-facing workflows. For example, the patron menu calls methods related to borrowing, returning, paying fines, and joining waitlists. The librarian menu calls methods related to adding items, removing items, freezing accounts, viewing records, and marking items lost.

### 12.10 LoanRecord Implementation

The UML design includes `LoanRecord` to track item checkout history. In C++, `LoanRecord` stores:

- `recordID`
- `patronID`
- `itemID`
- `checkoutDate`
- `dueDate`
- `returnDate`
- `returned`
- `lateFee`

This class is responsible for tracking whether a loan is active or closed. It also calculates the late fee when the loan is closed:

```cpp
double LoanRecord::closeLoan(const Date& actualReturnDate, const LibraryItem& item) {
    returnDate = actualReturnDate;
    returned = true;

    int daysOverdue = dueDate.daysUntil(actualReturnDate);
    lateFee = item.calculateLateFee(daysOverdue);
    return lateFee;
}
```

This method shows an important connection between classes. `LoanRecord` does not need to know the specific item type. It receives a `LibraryItem` reference and calls `calculateLateFee()`. If the item is a book, the book fee formula is used. If the item is digital media, the fee is zero. This is another example of polymorphism.

### 12.11 Date Class Implementation

The `Date` class supports the date calculations needed by loans and receipts. It stores:

- `year`
- `month`
- `day`

Important methods include:

- `today()`
- `parse(...)`
- `toString()`
- `addDays(...)`
- `daysUntil(...)`

When an item is checked out, the checkout date is the current date and the due date is calculated by adding 14 days:

```cpp
Date checkoutDate = Date::today();
Date dueDate = checkoutDate.addDays(14);
```

When an item is returned, the system compares the return date to the due date. If the return date is after the due date, the number of overdue days is used to calculate a late fee.

The `Date` class also supports readable output. For example, dates are displayed in `YYYY-MM-DD` format. This keeps date formatting logic separate from the menu and loan classes.

### 12.12 TransactionReceipt Implementation

The UML design mentioned that a receipt should be generated when a patron pays a fine. In the C++ implementation, this was represented by the `TransactionReceipt` class.

The class stores:

- `receiptID`
- `patronID`
- `amount`
- `paymentMethod`
- `paymentDate`

When a patron pays a fine, the system creates a new receipt:

```cpp
receipts.emplace_back(makeReceiptID(), patron.getAccountID(), amount, method, Date::today());
```

The receipt is stored in:

```cpp
std::vector<TransactionReceipt> receipts;
```

This allows librarians to view receipt history during the same program session. The receipt class also has a `getDetails()` method that formats the receipt information for display.

This implementation improves the technical completeness of the project because the receipt is not just a printed message. It is an actual object stored by the system.

### 12.13 LibrarySystem As The Controller Class

The `LibrarySystem` class acts as the central controller of the program. It does not replace the other classes, but it coordinates them.

It stores the main system data:

```cpp
std::vector<std::unique_ptr<LibraryItem>> inventory;
std::vector<std::unique_ptr<Account>> accounts;
std::vector<LoanRecord> loans;
std::vector<TransactionReceipt> receipts;
std::map<std::string, std::queue<int>> waitlists;
```

It also contains helper functions for searching:

- `findAccountByEmail(...)`
- `findAccountByID(...)`
- `findPatronByID(...)`
- `findItemByID(...)`
- `findActiveLoan(...)`

These helper methods keep the menu code simpler. Instead of repeating search loops everywhere, the system uses named functions. This makes the code easier to maintain and easier to explain.

The `LibrarySystem` class also contains seeded data. The seeded accounts and catalog items allow the program to be tested immediately after compiling. This is useful for a final project demonstration because the instructor can log in and test features without creating all data manually.

### 12.14 Data Structures Used In The Implementation

The project uses several standard C++ data structures. Each one was chosen for a specific purpose.

| Data Structure | Used For | Reason |
| --- | --- | --- |
| `std::vector` | Inventory, accounts, loans, receipts | Simple dynamic storage with easy iteration |
| `std::unique_ptr` | Polymorphic item and account storage | Automatic memory management |
| `std::map` | Mapping item IDs to waitlists | Associates each item with a queue |
| `std::queue` | Item waitlists | First-in, first-out reservation behavior |
| `std::string` | Text fields such as title, email, and IDs | Standard C++ text storage |

The most common container is `std::vector`. It is used because most operations involve looping through all records and finding a matching ID. For a small educational project, vector storage is simple and effective.

The waitlist feature uses a queue because waitlists should behave in first-come-first-served order. The first patron to join the waitlist should be the first patron allowed to borrow the item when it becomes available.

The waitlist storage is:

```cpp
std::map<std::string, std::queue<int>> waitlists;
```

The map key is the item ID. The queue value stores patron account IDs. This design allows each item to have its own waitlist.

### 12.15 Queue-Based Waitlist Implementation

The queue waitlist is one of the clearest data-structure features in the project. When a patron tries to join the waitlist for a checked-out item, the system stores the patron's account ID in the queue:

```cpp
std::queue<int>& waitlist = waitlists[itemID];
waitlist.push(patron.getAccountID());
```

The `push()` operation adds the patron to the back of the queue. If another patron joins later, that patron is placed behind the first patron.

When an item is returned, the system checks the queue and announces the next patron:

```cpp
int patronID = match->second.front();
```

The `front()` operation checks the patron at the front of the queue without removing them. This represents the person who has priority.

When the first waitlisted patron borrows the item, the system removes that patron from the queue:

```cpp
waitlistMatch->second.pop();
```

The `pop()` operation removes the front patron after their turn has been used.

The borrowing logic also prevents another patron from skipping the queue. If the item is available but a waitlist exists, the system checks whether the current patron is at the front. If not, the borrow request is rejected and the patron is told to join the waitlist.

This feature demonstrates that the queue is not only included for appearance. It is actually used to control program behavior.

### 12.16 Checkout Workflow Implementation

The checkout workflow connects several classes together:

1. The patron chooses borrow item.
2. `LibrarySystem` reads the item ID.
3. `LibrarySystem` finds the item.
4. The system checks patron eligibility using `patron.canCheckout()`.
5. The system checks item status.
6. The system checks waitlist priority.
7. `Patron::requestCheckout()` creates the loan and updates item status.

The checkout flow uses both validation and object behavior. `LibrarySystem` handles menu input and object lookup. `Patron` handles the actual checkout operation.

This separation is important because it prevents the menu from containing all business logic. The menu coordinates the process, but the role class still owns the role-specific behavior.

### 12.17 Return And Late Fee Workflow Implementation

The return workflow also connects multiple classes:

1. The patron chooses return item.
2. `LibrarySystem` finds the item.
3. `LibrarySystem` finds the active loan.
4. `Patron::returnItem()` closes the loan.
5. `LoanRecord::closeLoan()` calculates overdue days.
6. `LibraryItem::calculateLateFee()` chooses the correct fee formula.
7. The patron's unpaid fine balance is updated.
8. The item status is changed back to available.
9. The waitlist system announces the next patron if one exists.

This workflow demonstrates the benefit of polymorphism. The return logic does not need separate `if` statements for books, periodicals, and digital media. Instead, each item class defines its own fine calculation.

### 12.18 Input Validation And Error Handling

The project mainly uses normal validation with `if` statements instead of `throw` and `catch`. This was an intentional design choice.

Most errors in this project are expected user-input cases. Examples include:

- Entering a wrong menu option.
- Trying to borrow a checked-out item.
- Trying to borrow while the account is frozen.
- Trying to pay more than the fine balance.
- Searching for an item that does not exist.
- Trying to remove a checked-out item.

These are not unexpected system failures. They are normal possibilities in a menu-based program. Therefore, direct validation is clearer than exception handling.

For example:

```cpp
if (item == nullptr) {
    std::cout << "Item not found.\n";
    return;
}
```

This is easy to understand and easy to demonstrate. Exceptions would be more appropriate if the project used file loading, databases, or external services. For example, a future version that loads catalog data from a file might use `throw` and `catch` if the file is missing or corrupted.

### 12.19 How The UML Relationships Were Converted To C++

The UML diagram contains different types of relationships. Each relationship was implemented using a suitable C++ mechanism.

| UML Concept | C++ Implementation |
| --- | --- |
| Class | `class` declarations in `.h` files |
| Attribute | private or protected data member |
| Operation | public member function |
| Inheritance | `class Child : public Parent` |
| Abstract method | pure virtual function with `= 0` |
| Association | pointers, references, IDs, or containers |
| Multiplicity | `std::vector`, `std::map`, or `std::queue` |
| State | `enum class` |

For example, the UML inheritance relationship from `LibraryItem` to `Book` became:

```cpp
class Book : public LibraryItem
```

The UML relationship between patrons and loans was implemented through patron IDs stored inside `LoanRecord`. The system searches for active loans by matching both patron ID and item ID. This avoids duplicating full patron objects inside each loan.

### 12.20 Technical Testing Of The Implementation

The implementation was tested by compiling with strict warnings:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/*.cpp -o library_app
```

Strict compilation is important because it checks for many common C++ issues. The program compiled successfully.

The following technical behaviors were play-tested:

- Polymorphic item display through `getDetails()`.
- Different late-fee formulas for each item type.
- Checkout status changes from available to checked out.
- Return status changes from checked out to available.
- Overdue fine calculation.
- Transaction receipt creation.
- Frozen account borrowing prevention.
- Checked-out item removal prevention.
- Lost item status update.
- Queue-based waitlist priority.
- Exact item ID search and keyword title search.

The queue waitlist test confirmed that if Alex joins the waitlist before Taylor, Alex receives borrowing priority when the item becomes available. Taylor is blocked until Alex has the chance to borrow. This confirms that the queue is functioning as a first-in-first-out data structure.

### 12.21 Technical Conclusion

The UML design was successfully implemented in C++ by translating UML classes into C++ classes, UML attributes into class fields, UML operations into member functions, and UML inheritance relationships into C++ inheritance. Abstract UML behavior was implemented using pure virtual functions. Polymorphism was implemented through virtual methods and base-class pointers. Collections and multiplicity were implemented using STL containers such as `std::vector`, `std::map`, and `std::queue`.

Overall, the technical implementation shows how an object-oriented design can become a working C++ program. The project does not only model the UML diagram structurally; it also implements the expected behaviors through real workflows such as login, checkout, return, fine payment, account management, item management, receipt creation, and queue-based waitlisting.

## 13. Limitations

Current limitations:

- Data is not saved after the program exits.
- Password hashing uses `std::hash`, which is acceptable for this educational project but not secure enough for real systems.
- The program is console-based and does not include a graphical interface.
- Email and item ID validation is basic.
- The system does not currently support advanced reports, reservations, or renewal workflows.
- Error handling mostly uses normal validation instead of exception handling.

## 14. Possible Future Improvements

Possible future improvements include:

- Add file persistence for accounts, catalog items, loans, and receipts.
- Add duplicate ISBN checking for books.
- Add item renewal and reservation features.
- Add a stronger password hashing approach.
- Add case-insensitive search for email and item IDs.
- Add exception handling for future file/database operations.
- Add a graphical user interface.
- Add unit tests for each class.

## 15. Conclusion

The Library Management Program successfully implements a practical OOP system in C++. It demonstrates inheritance, encapsulation, abstraction, polymorphism, and composition through a working library application. The project includes role-based menus, multiple item types, checkout and return workflows, overdue fine calculation, account management, transaction receipts, and seeded data for easy demonstration.

Overall, the project meets the main requirements of a library management system while showing clear object-oriented design and implementation.
