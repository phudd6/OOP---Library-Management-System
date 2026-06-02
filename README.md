# Library Management Program

Console-based C++17 OOP project for managing library items, patrons, librarians,
loans, returns, account state, fines, receipts, and item waitlists.

## Compile

```bash
g++ -std=c++17 src/*.cpp -o library_app
```

Run:

```bash
./library_app
```

## Seeded Accounts

| Role | Email | Password | Notes |
| --- | --- | --- | --- |
| Librarian | `librarian@library.com` | `admin123` | Can add/remove items, freeze/unfreeze patrons, mark items lost, waive fines, and view loans/receipts. |
| Patron | `alex@example.com` | `pass123` | Regular patron account. |
| Patron | `taylor@example.com` | `pass123` | Regular patron account. |
| Patron | `overdue@example.com` | `pass123` | Has two already-overdue checked-out books for testing returns and late fees. |

## Demo Suggestions

1. Log in as `overdue@example.com`.
2. Return `B002` or `B003`.
3. Confirm that the returned book becomes available and a late fee is added.
4. Try borrowing another item before paying fines to confirm borrowing is blocked.
5. Pay part or all of the fine and confirm that a transaction receipt is printed.
6. Join a waitlist for a checked-out item such as `B002`.
7. Log in as the librarian and view waitlists, receipts, waive fines, mark an item lost, or freeze/unfreeze accounts.

## Implemented OOP Features

- `LibraryItem` is an abstract base class.
- `Book`, `Periodical`, and `DigitalMedia` override detail display and late-fee behavior.
- `Account` is a base class for `Patron` and `Librarian`.
- `Patron` owns checkout, return, fine payment, and fine tracking behavior.
- `Librarian` owns account-management and item-removal permission behavior.
- `LoanRecord` tracks checkout date, due date, return date, and late fee.
- `TransactionReceipt` records fine payments with receipt ID, patron ID, amount, method, and payment date.
- `Date` supports current date, day arithmetic, parsing, and day differences.
- Item status uses an `ItemStatus` enum with `Available`, `CheckedOut`, and `Lost`.
- Ancient publication years display as BCE instead of negative numbers.
- Item waitlists use `std::queue<int>` to store patron account IDs in first-come-first-served order.

## Project Folders

- `src/` contains all C++ source and header files.
- `assets/` is reserved for UML diagrams and other report images.
- Root-level Markdown files contain project documentation and the final report.

## Class Structure

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

## Data Structures Used

- `std::vector` stores accounts, catalog items, loan records, and receipts.
- `std::unique_ptr` safely stores polymorphic `Account` and `LibraryItem` objects.
- `std::queue<int>` stores item waitlists using FIFO order.
- `std::map<std::string, std::queue<int>>` maps each item ID to its waitlist.

## Seeded Catalog

The catalog includes project-focused sample items plus 15 famous literature works:

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
