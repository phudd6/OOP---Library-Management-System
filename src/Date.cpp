#include "Date.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

Date::Date() : year(1970), month(1), day(1) {}

Date::Date(int year, int month, int day) : year(year), month(month), day(day) {}

Date Date::today() {
    std::time_t now = std::time(nullptr);
    std::tm localTime{};

#if defined(_WIN32)
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif

    return Date(localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday);
}

bool Date::parse(const std::string& input, Date& output) {
    int parsedYear = 0;
    int parsedMonth = 0;
    int parsedDay = 0;
    char dash1 = '\0';
    char dash2 = '\0';

    std::istringstream stream(input);
    if (!(stream >> parsedYear >> dash1 >> parsedMonth >> dash2 >> parsedDay)) {
        return false;
    }

    if (dash1 != '-' || dash2 != '-' || parsedYear < 1900 || parsedMonth < 1 ||
        parsedMonth > 12 || parsedDay < 1 || parsedDay > 31) {
        return false;
    }

    std::tm tmDate{};
    tmDate.tm_year = parsedYear - 1900;
    tmDate.tm_mon = parsedMonth - 1;
    tmDate.tm_mday = parsedDay;
    tmDate.tm_isdst = -1;

    std::time_t normalized = std::mktime(&tmDate);
    if (normalized == -1) {
        return false;
    }

    std::tm* checked = std::localtime(&normalized);
    if (checked == nullptr || checked->tm_year != parsedYear - 1900 ||
        checked->tm_mon != parsedMonth - 1 || checked->tm_mday != parsedDay) {
        return false;
    }

    output = Date(parsedYear, parsedMonth, parsedDay);
    return true;
}

std::string Date::toString() const {
    std::ostringstream output;
    output << std::setfill('0') << std::setw(4) << year << "-"
           << std::setw(2) << month << "-" << std::setw(2) << day;
    return output.str();
}

Date Date::addDays(int days) const {
    std::tm tmDate{};
    tmDate.tm_year = year - 1900;
    tmDate.tm_mon = month - 1;
    tmDate.tm_mday = day + days;
    tmDate.tm_isdst = -1;

    std::mktime(&tmDate);
    return Date(tmDate.tm_year + 1900, tmDate.tm_mon + 1, tmDate.tm_mday);
}

int Date::daysUntil(const Date& other) const {
    std::tm start{};
    start.tm_year = year - 1900;
    start.tm_mon = month - 1;
    start.tm_mday = day;
    start.tm_isdst = -1;

    std::tm end{};
    end.tm_year = other.year - 1900;
    end.tm_mon = other.month - 1;
    end.tm_mday = other.day;
    end.tm_isdst = -1;

    std::time_t startTime = std::mktime(&start);
    std::time_t endTime = std::mktime(&end);
    constexpr int secondsPerDay = 60 * 60 * 24;

    return static_cast<int>(std::difftime(endTime, startTime) / secondsPerDay);
}

bool Date::operator>(const Date& other) const {
    return other.daysUntil(*this) > 0;
}

bool Date::operator<(const Date& other) const {
    return daysUntil(other) > 0;
}

bool Date::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
}
