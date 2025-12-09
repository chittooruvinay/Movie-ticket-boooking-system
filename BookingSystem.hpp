// BookingSystem.hpp
#ifndef BOOKINGSYSTEM_HPP
#define BOOKINGSYSTEM_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <random>
#include <iomanip>
#include <algorithm>
#include <cctype>

// ===== COLORS (ANSI) =====
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// CONFIG
constexpr int ROWS = 6;            // A-F
constexpr int COLS = 10;           // 1-10
constexpr int PRICE = 250;         // ₹ per ticket
const std::string STATIC_QR_FILE = "my_qr.png"; // your static QR file

// Helper utilities
static inline std::string trim(const std::string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}
static inline std::string upper(const std::string &s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), [](unsigned char c){ return std::toupper(c); });
    return r;
}
static inline bool is_number(const std::string &s) {
    if (s.empty()) return false;
    for (char c : s) if (!std::isdigit((unsigned char)c)) return false;
    return true;
}

class Show {
public:
    std::string time;
    bool seats[ROWS][COLS];

    Show(const std::string &t);

    void printSeats() const;
    bool bookSeat(const std::string &s);
};

class Movie {
public:
    std::string name;
    std::vector<Show> showList;

    Movie(const std::string &n);

    void addShow(const std::string &t);
    void printShows() const;
};

class BookingSystem {
private:
    std::vector<Movie> movies;

    std::string generateBookingID();
    std::string nowDateTime();
    void pauseForEnter();

public:
    BookingSystem();
    void run();
    void viewMovies() const;
    void bookTicket();
    void viewBookings() const;
};

#endif // BOOKINGSYSTEM_HPP
