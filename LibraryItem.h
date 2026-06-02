#ifndef LIBRARY_ITEM_H
#define LIBRARY_ITEM_H

#include <string>

enum class ItemStatus {
    Available,
    CheckedOut,
    Lost
};

std::string itemStatusToString(ItemStatus status);

class LibraryItem {
protected:
    std::string itemID;
    std::string title;
    int publicationYear;
    ItemStatus status;
    std::string location;

    std::string baseDetails() const;

public:
    LibraryItem(const std::string& itemID,
                const std::string& title,
                int publicationYear,
                ItemStatus status,
                const std::string& location);
    virtual ~LibraryItem() = default;

    void updateStatus(ItemStatus newStatus);
    const std::string& getItemID() const;
    const std::string& getTitle() const;
    ItemStatus getStatus() const;

    virtual std::string getDetails() const = 0;
    virtual double calculateLateFee(int daysOverdue) const = 0;
    virtual std::string getType() const = 0;
};

class Book : public LibraryItem {
private:
    std::string author;
    std::string isbn;
    int pageCount;
    std::string genre;

public:
    Book(const std::string& itemID,
         const std::string& title,
         int publicationYear,
         const std::string& location,
         const std::string& author,
         const std::string& isbn,
         int pageCount,
         const std::string& genre);

    std::string getDetails() const override;
    double calculateLateFee(int daysOverdue) const override;
    std::string getType() const override;
    std::string getGenre() const;
};

class Periodical : public LibraryItem {
private:
    std::string issueNumber;
    int volume;
    std::string publisher;

public:
    Periodical(const std::string& itemID,
               const std::string& title,
               int publicationYear,
               const std::string& location,
               const std::string& issueNumber,
               int volume,
               const std::string& publisher);

    std::string getDetails() const override;
    double calculateLateFee(int daysOverdue) const override;
    std::string getType() const override;
};

class DigitalMedia : public LibraryItem {
private:
    std::string fileFormat;
    int fileSize;
    std::string downloadLink;

public:
    DigitalMedia(const std::string& itemID,
                 const std::string& title,
                 int publicationYear,
                 const std::string& location,
                 const std::string& fileFormat,
                 int fileSize,
                 const std::string& downloadLink);

    std::string getDetails() const override;
    double calculateLateFee(int daysOverdue) const override;
    std::string getType() const override;
};

#endif
