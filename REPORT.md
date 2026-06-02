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
g++ -std=c++17 *.cpp -o library_app
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
g++ -std=c++17 -Wall -Wextra -pedantic *.cpp -o library_app
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

## 12. Limitations

Current limitations:

- Data is not saved after the program exits.
- Password hashing uses `std::hash`, which is acceptable for this educational project but not secure enough for real systems.
- The program is console-based and does not include a graphical interface.
- Email and item ID validation is basic.
- The system does not currently support advanced reports, reservations, or renewal workflows.
- Error handling mostly uses normal validation instead of exception handling.

## 13. Possible Future Improvements

Possible future improvements include:

- Add file persistence for accounts, catalog items, loans, and receipts.
- Add duplicate ISBN checking for books.
- Add item renewal and reservation features.
- Add a stronger password hashing approach.
- Add case-insensitive search for email and item IDs.
- Add exception handling for future file/database operations.
- Add a graphical user interface.
- Add unit tests for each class.

## 14. Conclusion

The Library Management Program successfully implements a practical OOP system in C++. It demonstrates inheritance, encapsulation, abstraction, polymorphism, and composition through a working library application. The project includes role-based menus, multiple item types, checkout and return workflows, overdue fine calculation, account management, transaction receipts, and seeded data for easy demonstration.

Overall, the project meets the main requirements of a library management system while showing clear object-oriented design and implementation.
