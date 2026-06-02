#include "LibraryItem.h"

#include <algorithm>
#include <sstream>

namespace {
std::string formatPublicationYear(int year) {
    if (year < 0) {
        return std::to_string(-year) + " BCE";
    }

    return std::to_string(year);
}
}

std::string itemStatusToString(ItemStatus status) {
    switch (status) {
        case ItemStatus::Available:
            return "Available";
        case ItemStatus::CheckedOut:
            return "Checked Out";
        case ItemStatus::Lost:
            return "Lost";
    }

    return "Unknown";
}

LibraryItem::LibraryItem(const std::string& itemID,
                         const std::string& title,
                         int publicationYear,
                         ItemStatus status,
                         const std::string& location)
    : itemID(itemID),
      title(title),
      publicationYear(publicationYear),
      status(status),
      location(location) {}

void LibraryItem::updateStatus(ItemStatus newStatus) {
    status = newStatus;
}

const std::string& LibraryItem::getItemID() const {
    return itemID;
}

const std::string& LibraryItem::getTitle() const {
    return title;
}

ItemStatus LibraryItem::getStatus() const {
    return status;
}

std::string LibraryItem::baseDetails() const {
    std::ostringstream output;
    output << "ID: " << itemID << "\n"
           << "Title: " << title << "\n"
           << "Year: " << formatPublicationYear(publicationYear) << "\n"
           << "Status: " << itemStatusToString(status) << "\n"
           << "Location: " << location << "\n";
    return output.str();
}

Book::Book(const std::string& itemID,
           const std::string& title,
           int publicationYear,
           const std::string& location,
           const std::string& author,
           const std::string& isbn,
           int pageCount,
           const std::string& genre)
    : LibraryItem(itemID, title, publicationYear, ItemStatus::Available, location),
      author(author),
      isbn(isbn),
      pageCount(pageCount),
      genre(genre) {}

std::string Book::getDetails() const {
    std::ostringstream output;
    output << baseDetails()
           << "Type: Book\n"
           << "Author: " << author << "\n"
           << "ISBN: " << isbn << "\n"
           << "Pages: " << pageCount << "\n"
           << "Genre: " << genre << "\n";
    return output.str();
}

double Book::calculateLateFee(int daysOverdue) const {
    return std::max(0, daysOverdue) * 0.50;
}

std::string Book::getType() const {
    return "Book";
}

std::string Book::getGenre() const {
    return genre;
}

Periodical::Periodical(const std::string& itemID,
                       const std::string& title,
                       int publicationYear,
                       const std::string& location,
                       const std::string& issueNumber,
                       int volume,
                       const std::string& publisher)
    : LibraryItem(itemID, title, publicationYear, ItemStatus::Available, location),
      issueNumber(issueNumber),
      volume(volume),
      publisher(publisher) {}

std::string Periodical::getDetails() const {
    std::ostringstream output;
    output << baseDetails()
           << "Type: Periodical\n"
           << "Issue Number: " << issueNumber << "\n"
           << "Volume: " << volume << "\n"
           << "Publisher: " << publisher << "\n";
    return output.str();
}

double Periodical::calculateLateFee(int daysOverdue) const {
    return std::max(0, daysOverdue) * 0.25;
}

std::string Periodical::getType() const {
    return "Periodical";
}

DigitalMedia::DigitalMedia(const std::string& itemID,
                           const std::string& title,
                           int publicationYear,
                           const std::string& location,
                           const std::string& fileFormat,
                           int fileSize,
                           const std::string& downloadLink)
    : LibraryItem(itemID, title, publicationYear, ItemStatus::Available, location),
      fileFormat(fileFormat),
      fileSize(fileSize),
      downloadLink(downloadLink) {}

std::string DigitalMedia::getDetails() const {
    std::ostringstream output;
    output << baseDetails()
           << "Type: Digital Media\n"
           << "File Format: " << fileFormat << "\n"
           << "File Size: " << fileSize << " MB\n"
           << "Download Link: " << downloadLink << "\n";
    return output.str();
}

double DigitalMedia::calculateLateFee(int daysOverdue) const {
    (void)daysOverdue;
    return 0.0;
}

std::string DigitalMedia::getType() const {
    return "DigitalMedia";
}
