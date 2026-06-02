# Main

Topic: Library Management Program

Class Book

# LibrabryItem

**`ATTRIBUTES`** 

`<protected> itemID: String` 

`<protected> title: String` 

`<protected> publicationYear: Integer` 

`<protected> status: String (e.g., "Available", "Checked Out", "Lost")` 

`<protected> location: String`

**`FUNCTIONS`**

**`updateStatus(newStatus: String) : void`**

1. `System receives the newStatus string passed from the LibraryCatalog.`  
2. `System overwrites the current status attribute with the new value.`  
3. `System saves the updated state.`

**`getDetails() : String`** `(Abstract / Virtual)`

1. `System collects the shared attributes (itemID, title, publicationYear, status).`  
2. `System formats them into a readable string block.`  
3. `Subclasses will override this to add their specific attributes before returning the final string.`

**`calculateLateFee(daysOverdue: Integer) : Float`** `(Abstract / Virtual)`

1. `System receives the daysOverdue integer.`  
2. `System applies the specific daily fine formula (which will be defined differently in each subclass like Book or DigitalMedia).`  
3. `System returns the calculated Float amount.`

# Book

**`ATTRIBUTES`** 

`<private> author: String` 

`<private> isbn: String` 

`<private> pageCount: Integer`

`<private> genre: String`

**`FUNCTIONS`**

**`getDetails() : String`** `(Override)`

1. `System calls the parent class LibraryItem::getDetails() to retrieve the base information.`  
2. `System appends the Book-specific attributes (author, isbn, pageCount) to the result.`  
3. `System returns the complete formatted string.`

**`calculateLateFee(daysOverdue: Integer) : Float`** `(Override)`

1. `System checks the standard daily fee rate for physical books (e.g., $0.50/day).`  
2. `System multiplies daysOverdue by the daily rate.`  
3. `System returns the total fee as a Float.`

**`getGenre() : String`**

1. `System returns the genre of the Book object`

# Periodical

`ATTRIBUTES` 

`<private> issueNumber: String` 

`<private> volume: Integer` 

`<private> publisher: String`

**`FUNCTIONS`**

**`getDetails() : String`** `(Override)`

1. `System calls the parent class LibraryItem::getDetails() to retrieve the base information.`  
2. `System appends the Periodical-specific attributes (issueNumber, volume, publisher) to the result.`  
3. `System returns the complete formatted string.`

**`calculateLateFee(daysOverdue: Integer) : Float`** `(Override)`

1. `System checks the standard daily fee rate for periodicals/magazines (e.g., $0.25/day, usually lower than books).`  
2. `System multiplies daysOverdue by the daily rate.`  
3. `System returns the total fee as a Float.`

# DigitalMedia

`ATTRIBUTES` 

`<private> fileFormat: String` 

`<private> fileSize: Integer` 

`<private> downloadLink: String`

**`FUNCTIONS`**

**`getDetails() : String`** `(Override)`

1. `System calls the parent class LibraryItem::getDetails() to retrieve the base information.`  
2. `System appends the DigitalMedia-specific attributes (fileFormat, fileSize, downloadLink) to the result.`  
3. `System returns the complete formatted string.`

**`calculateLateFee(daysOverdue: Integer) : Float`** `(Override)`

1. `System recognizes the item category as DigitalMedia.`  
2. `System sets the late fee formula (Note: Digital items often do not incur late fees because access can be automatically revoked when the due date passes, so the daily rate can be set to $0.00).`  
3. `System returns the total fee as a Float (e.g., 0.0).`

# LoanRecord

`ATTRIBUTES` 

`<private> recordID: String` 

`<private> patronID: String` 

`<private> itemID: String` 

`<private> checkoutDate: Date` 

`<private> dueDate: Date` 

`<private> returnDate: Date (Initially Null)`

**`FUNCTIONS`**

**`isOverdue() : Boolean`**

1. `System fetches the current system time/date.`  
2. `System checks if returnDate is null (meaning the item is still not returned).`  
3. `System compares the current date with the dueDate.`  
4. `System returns true if the current date is strictly greater than the dueDate, otherwise false.`

**`closeLoan(returnDate: Date) : void`**

1. `System receives the returnDate when the Patron returns the item.`  
2. `System overwrites the null returnDate attribute with the actual date.`  
3. `System calls calculateLateFee() from the associated LibraryItem if isOverdue() returns true.`  
4. `System finalizes the record, making it a permanent historical log.`

# Account

`ATTRIBUTES`

`<protected> accountID` (Integer)

`<protected> fullName` (String)

`<protected> email` (String)

`<protected> passwordHash` (String \- never store plain text passwords\!)

`<protected> accountState` (Enum: Active, Frozen, Closed)

`<protected> isLibrarian(Boolean)`

**`FUNCTIONS`**

**`login(email, password): Boolean`**

1. `User choose option 1 to log in.`  
2. `System ask users to fill in email and password.`  
3. `Check if the email exists or not, then check the password.`  
4. `If successful, System calls checkIfLibrarian() on the logged-in account.`  
5. `If it returns True, System displays the Librarian Menu (giving access to addLibraryItem, removeLibraryItem, etc.).`  
6. `If it returns False, System displays the Patron Menu (giving access to requestCheckout, returnItem, etc.).`  
7. `If login fails, System returns an error message.`

**`signUp(email, password):`** 

1. `User choose option 2 to sign up`  
2. `System ask user to enter email and password to sign up for an account`  
3. `Check for duplicate emails, if yes -> require user to enter another email`  
4. `A new account is created and stored in an array`

**`resetPassword(newPassword):`** 

1. `User choose option 3 to reset password`  
2. `User enter an email -> check if the email exists`  
3. `Ask user to enter old password, then enter new password`  
4. `The System replace the old password with the new password`

**`checkIfLibrarian():`**

1. `System check if the account is the same as the librarian account.`  
2. `Return True or False`

# Patron

**`ATTRIBUTES`**  
`<private> totalUnpaidFines`

**`FUNCTIONS`**

**`requestCheckout(item: LibraryItem) : void`**

1. `User choose the option to borrow an item and provide the itemID.`  
2. `System check the LibraryItem status (must be "Available") and verify the User account (must be "Active" and have no unpaid fines).`  
3. `If valid, System creates a new LoanRecord with the current date and calculates the dueDate, then updates the item's status to "Checked Out".`  
4. `System confirms success, stores the record, and displays the return due date to the user; otherwise, it returns a specific error message (e.g., "Item not available" or "Unpaid fines exist").`

**`returnItem(item: LibraryItem) : void`**

1. `User choose the option to return a borrowed item and enter the itemID.`  
2. `System locates the active LoanRecord matching this itemID and the current User.`  
3. `System compares the current date against the dueDate to calculate if there is a late fee.`  
4. `System updates the item status back to "Available", records the return date, closes the loan, and informs the user of success (along with any fine incurred).`

**`payFine(amount: Float) : void`**

1. `User check their totalUnpaidFines and choose the option to pay.`  
2. `System ask user to enter the payment amount and select a payment method`  
3. `Check if the entered amount is valid (greater than 0 and not exceeding the total fine).`  
4. `System deducts the paid amount from the user's total fines, generates a TransactionReceipt, and confirms successful payment.`

# Librarian

`ATTRIBUTES`

`<private> employeeID(string)`

**`FUNCTIONS`**

**`addLibraryItem(itemDetails):`**

1. `User (Librarian) choose the option to add a new item to the system.`  
2. `System ask user to select the item category (Book, Periodical, DigitalMedia) and fill in the required details (Title, Year, Author, etc.).`  
3. `Check for duplicate unique identifiers (like ISBN or ItemID). If a duplicate exists -> require user to enter a different ID.`  
4. `System creates the specific new object and stores it in the main inventory array.`

**`removeLibraryItem(itemID):`**

1. `User (Librarian) choose the option to delete an item and enter the itemID.`  
2. `System search for the itemID in the inventory.`  
3. `Check if the item is currently "Checked Out". If yes -> block deletion and return a warning message.`  
4. `If the item is "Available" or "Lost", System removes the item from the array and confirms successful deletion.`

**`freezeAccount(patronID):`**

1. `User (Librarian) choose the option to manage users and select an account to freeze.`  
2. `System ask for confirmation and a reason for freezing the account.`  
3. `System locates the User in the userDatabase array.`  
4. `System changes the accountState to "Frozen" and prevents this user from borrowing future items until un-frozen.`

**`waiveFine(patronID: String) : void`**

1. `User (Librarian) choose the option to waive a fine and input the patronID.`  
2. `System searches the userDatabase array to locate the specific Patron account and displays their current totalUnpaidFines.`  
3. `System ask user (Librarian) to enter the amount to waive (or choose "Waive All") and optionally input a reason for the waiver.`  
4. `System deducts the specified amount from the Patron's totalUnpaidFines (or sets it directly to 0.0) without requiring any payment method.`  
5. `System logs the action (for administrative tracking) and displays a success confirmation message.`

# LibraryCatolog

`ATTRIBUTES` 

`<private> inventory: Array<LibraryItem>`

`<private> userDatabase: Array<Account>` 

`<private> activeLoans: Array<LoanRecord>`

**`FUNCTIONS`**

**`searchByTitle(keyword: String) : Array<LibraryItem>`**

1. `User inputs a keyword in the terminal.`  
2. `System creates an empty temporary array to store results.`  
3. `System iterates through every LibraryItem in the inventory array.`  
4. `System compares the keyword against the title attribute of each item (ignoring case sensitivity).`  
5. `If a match is found, System adds the item to the temporary array.`  
6. `System returns the populated array to be displayed to the User.`

**`searchByAuthor(keyword: String) : Array<LibraryItem>`**

1. `User chooses the "Search by Author" option and inputs a keyword (author's name).`  
2. `System creates an empty temporary array to store the search results.`  
3. `System iterates through every LibraryItem in the inventory array.`  
4. `System checks if the current item belongs to the Book category (since only Books have the author attribute).`  
5. `If it is a Book, System compares the keyword against the item's author attribute (ignoring case sensitivity).`  
6. `If a match is found, System appends the item to the temporary array.`  
7. `System returns the populated array to be displayed to the User (or a "No items found" message if empty).`

**`searchByGenre(keyword: String) : Array<LibraryItem>`**

1. `User chooses the "Search by Genre" option and inputs a keyword (e.g., "Science Fiction", "Technology").`  
2. `System creates an empty temporary array to store the search results.`  
3. `System iterates through every LibraryItem in the inventory array.`  
4. `System compares the keyword against the item's genre attribute (ignoring case sensitivity).`  
5. `If a match is found, System appends the item to the temporary array.`  
6. `System returns the populated array to be displayed to the User (or a "No items found" message if empty).`

**`generateOverdueReport() : Array<LoanRecord>`**

1. `User (Librarian) selects the option to generate an overdue report.`  
2. `System iterates through the activeLoans array.`  
3. `System calls the isOverdue() function on every single LoanRecord.`  
4. `System collects all records that return true into a new report array.`  
5. `System displays the compiled report, showing Patron IDs, Item IDs, and days overdue.`

**`processCheckout(patronID: String, itemID: String) : void`**

1. `User (Librarian/Patron) choose the checkout option and input both patronID and itemID.`  
2. `System searches the userDatabase array to find the Patron and the inventory array to find the LibraryItem.`  
3. `Check the validation rules:`  
   * `The Patron's accountState must be "Active".`  
   * `The Patron's totalUnpaidFines must be 0.`  
   * `The LibraryItem's status must be "Available".`  
4. `If all checks pass, System instantiates a new LoanRecord object, recording the patronID, itemID, current checkoutDate, and calculating the dueDate (e.g., current date + 14 days).`  
5. `System updates the LibraryItem's status to "Checked Out".`  
6. `System appends the new LoanRecord to the activeLoans array and adds the itemID to the Patron's borrowedItemsList.`  
7. `System displays a success message with the due date; if any check fails in step 3, System aborts the process and returns a specific error message (e.g., "Account Frozen", "Item Currently Unavailable", "Outstanding Fines").`

# MenuManager

`ATTRIBUTES`

 `<private> catalog: LibraryCatalog` 

**`FUNCTIONS`**

**`runSystem() : void`**

1. `System starts the application and initializes the catalog object.`  
2. `System enters an infinite loop (while true) to keep the program running.`  
3. `System calls showLoginMenu().`  
4. `If the User chooses the option to exit from the login menu, System breaks the loop, saves any necessary data, and terminates the program safely.`

**`showLoginMenu() : void`**

1. `System displays the main screen with options: [1] Login, [2] Sign Up, [3] Reset Password, [4] Exit.`  
2. `User inputs their choice.`  
3. `If User chooses [1], System asks for email and password.`  
4. `System calls catalog.login(email, password).`  
5. `If login is successful, System retrieves the current account and calls checkIfLibrarian().`  
6. `If checkIfLibrarian() is True, System transitions to showLibrarianMenu(). If False, System transitions to showPatronMenu().`

**`showLibrarianMenu(currentUser: Account) : void`**

1. `System displays "--- LIBRARIAN MENU ---" and presents administrative options (e.g., [1] Add Item, [2] Remove Item, [3] Manage Users, [4] Logout).`  
2. `User (Librarian) inputs a choice.`  
3. `If User chooses an action (e.g., [1]), System prompts for necessary input (like item details) and calls the corresponding function in catalog (e.g., catalog.addLibraryItem()).`  
4. `After the action completes, System pauses to let the user read the result, then loops back to step 1 to show the menu again.`  
5. `If User chooses [4] Logout, System clears the currentUser session and returns to showLoginMenu().`

**`showPatronMenu(currentUser: Account) : void`**

1. `System displays "--- PATRON MENU ---" and presents user options (e.g., [1] Search Catalog, [2] Borrow Item, [3] Return Item, [4] Pay Fines, [5] Logout).`  
2. `User (Patron) inputs a choice.`  
3. `If User chooses an action (e.g., [2]), System prompts for necessary input (like itemID) and calls the corresponding function in catalog (e.g., catalog.processCheckout()).`  
4. `After the action completes, System pauses to let the user read the result, then loops back to step 1 to show the menu again.`  
5. `If User chooses [5] Logout, System clears the currentUser session and returns to showLoginMenu()`

