#ifndef DATE_H
#define DATE_H

#include <string>

class Date {
private:
    int year;
    int month;
    int day;

public:
    Date();
    Date(int year, int month, int day);

    static Date today();
    static bool parse(const std::string& input, Date& output);

    std::string toString() const;
    Date addDays(int days) const;
    int daysUntil(const Date& other) const;

    bool operator>(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
};

#endif
