// BookingSystem.cpp
#include "BookingSystem.hpp"
using namespace std;

// ========================== SHOW METHODS ==========================
Show::Show(const string &t) : time(t) {
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
            seats[i][j] = false;
}

void Show::printSeats() const {
    cout << BOLD << BLUE << "\n================== SCREEN ==================\n\n" << RESET;
    cout << CYAN << "       ";
    for (int i = 1; i <= COLS; ++i) cout << setw(3) << i;
    cout << RESET << "\n\n";

    for (int r = 0; r < ROWS; ++r) {
        cout << BOLD << YELLOW << "Row " << char('A' + r) << RESET << "  ";
        for (int c = 0; c < COLS; ++c) {
            if (seats[r][c])
                cout << RED  << "[X]" << RESET << " ";
            else
                cout << GREEN << "[ ]" << RESET << " ";
        }
        cout << "\n";
    }
    cout << BOLD << BLUE << "\n============== BOOK YOUR SEATS =============\n" << RESET;
}

bool Show::bookSeat(const string &raw) {
    string s = trim(raw);
    if (s.empty()) return false;
    // Normalize: keep only alnum
    string compact;
    for (char ch : s) if (isalnum((unsigned char)ch)) compact.push_back(ch);
    if (compact.size() < 2) return false;

    char row = toupper(compact[0]);
    if (row < 'A' || row >= char('A' + ROWS)) return false;

    string num = compact.substr(1);
    if (!is_number(num)) return false;

    int col = stoi(num);
    int r = row - 'A';
    int c = col - 1;
    if (c < 0 || c >= COLS) return false;
    if (seats[r][c]) return false;

    seats[r][c] = true;
    return true;
}

// ========================== MOVIE METHODS ==========================
Movie::Movie(const string &n) : name(n) {}

void Movie::addShow(const string &t) {
    showList.emplace_back(t);
}

void Movie::printShows() const {
    cout << BOLD << CYAN << "\n----------- Available Show Timings ----------\n" << RESET;
    for (size_t i = 0; i < showList.size(); ++i)
        cout << GREEN << "  " << (i + 1) << ". " << YELLOW << showList[i].time << RESET << "\n";
    cout << CYAN << "--------------------------------------------\n" << RESET;
}

// ====================== BOOKING SYSTEM =============================
BookingSystem::BookingSystem() {
    movies.emplace_back("Avengers: Endgame");
    movies.back().addShow("07:00 PM");
    movies.back().addShow("10:00 PM");

    movies.emplace_back("Leo");
    movies.back().addShow("06:30 PM");
    movies.back().addShow("09:45 PM");

    movies.emplace_back("KGF Chapter 2");
    movies.back().addShow("05:00 PM");
    movies.back().addShow("08:15 PM");
}

string BookingSystem::generateBookingID() {
    static int id = 5000;
    return to_string(++id);
}

string BookingSystem::nowDateTime() {
    time_t t = time(nullptr);
    char buf[64];
#if defined(_WIN32) || defined(_WIN64)
    tm tmv;
    localtime_s(&tmv, &t);
    strftime(buf, sizeof(buf), "%d-%b-%Y %I:%M %p", &tmv);
#else
    tm tmv;
    localtime_r(&t, &tmv);
    strftime(buf, sizeof(buf), "%d-%b-%Y %I:%M %p", &tmv);
#endif
    return string(buf);
}

void BookingSystem::pauseForEnter() {
    cout << BOLD << CYAN << "\nPress Enter to continue..." << RESET;
    string tmp;
    getline(cin, tmp);
}

// Display movies
void BookingSystem::viewMovies() const {
    cout << BOLD << BLUE << "\n=========== 🎬 NOW SHOWING 🎬 ===========\n" << RESET;
    for (size_t i = 0; i < movies.size(); ++i) {
        cout << GREEN << "  " << (i + 1) << ". " << RESET
             << BOLD << YELLOW << movies[i].name << RESET << "\n";
    }
    cout << BOLD << BLUE << "=========================================\n" << RESET;
}

// Book tickets
void BookingSystem::bookTicket() {
    cout << BOLD << CYAN << "\n--- Book Tickets ---\n" << RESET;
    viewMovies();

    cout << BOLD << "Enter movie number: " << RESET;
    string in;
    getline(cin, in);
    if (in.empty()) { getline(cin, in); } // handle previous newline
    in = trim(in);
    if (!is_number(in)) {
        cout << RED << "Invalid input. Movie number must be numeric.\n" << RESET;
        return;
    }
    int m = stoi(in);
    if (m < 1 || m > (int)movies.size()) {
        cout << RED << "Invalid movie selection.\n" << RESET;
        return;
    }
    Movie &mv = movies[m - 1];

    mv.printShows();
    cout << BOLD << "Enter show number: " << RESET;
    getline(cin, in);
    in = trim(in);
    if (!is_number(in)) {
        cout << RED << " Invalid input. Show number must be numeric.\n" << RESET;
        return;
    }
    int s = stoi(in);
    if (s < 1 || s > (int)mv.showList.size()) {
        cout << RED << " Invalid show selection.\n" << RESET;
        return;
    }
    Show &sh = mv.showList[s - 1];

    // Show seats
    sh.printSeats();

    vector<string> booked;
    while (true) {
        cout << BOLD << "Enter seat to book (e.g. A4) or DONE to finish: " << RESET;
        string seat;
        getline(cin, seat);
        seat = trim(seat);
        if (seat.empty()) continue;
        string up = upper(seat);
        if (up == "DONE") break;

        if (sh.bookSeat(seat)) {
            cout << GREEN << "✔ Seat " << upper(seat) << " booked successfully!\n" << RESET;
            booked.push_back(upper(seat));
        } else {
            cout << RED << " Seat invalid or already booked. Try again.\n" << RESET;
        }
    }

    if (booked.empty()) {
        cout << YELLOW << "No seats selected. Booking cancelled.\n" << RESET;
        return;
    }

    string seatsCombined;
    for (size_t i = 0; i < booked.size(); ++i) {
        seatsCombined += booked[i];
        if (i + 1 < booked.size()) seatsCombined += ",";
    }

    string bookingID = generateBookingID();
    string timeNow = nowDateTime();

    // Use static QR file (no generation)
    string qrFile = STATIC_QR_FILE;

    // Print polished ticket/bill
    cout << BOLD << MAGENTA
     << "\n==============================================\n"
     << "                 🎫 BOOKING RECEIPT            \n"
     << "==============================================\n"
     << RESET;

    cout << BOLD << CYAN  << "Booking ID     : " << RESET << GREEN << bookingID      << RESET << "\n";
    cout << BOLD << CYAN  << "Date/Time      : " << RESET << YELLOW << timeNow       << RESET << "\n";
    cout << BOLD << CYAN  << "Movie          : " << RESET << BLUE   << mv.name       << RESET << "\n";
    cout << BOLD << CYAN  << "Show Time      : " << RESET << BLUE   << sh.time       << RESET << "\n";
    cout << BOLD << CYAN  << "Seats          : " << RESET << GREEN  << seatsCombined << RESET << "\n";
    cout << BOLD << CYAN  << "Amount         : " << RESET << YELLOW << "₹" << (PRICE * (int)booked.size()) << RESET << "\n";

    cout << BOLD << MAGENTA
     << "----------------------------------------------\n"
     << "        ✔ Enjoy Your Movie! 🍿🎬               \n"
     << "==============================================\n"
     << RESET;


    // Save booking record
    ofstream ofs("bookings.txt", ios::app);
    if (ofs) {
        ofs << bookingID << "|" << timeNow << "|" << mv.name << "|" << sh.time
            << "|" << seatsCombined << "|" << (PRICE * (int)booked.size()) << "|" << qrFile << "\n";
    }

    cout << GREEN << "Saved booking to bookings.txt\n" << RESET;
    pauseForEnter();
}

void BookingSystem::viewBookings() const {
    cout << BOLD << CYAN << "\n---- BOOKING HISTORY ----\n" << RESET;
    ifstream ifs("bookings.txt");
    if (!ifs) {
        cout << YELLOW << "No bookings found.\n" << RESET;
        return;
    }
    string line;
    while (getline(ifs, line)) {
        cout << line << "\n";
    }
    cout << BOLD << "-------------------------\n" << RESET;
}

// Main menu
void BookingSystem::run() {
    while (true) {
        cout << BOLD << CYAN
             << "\n============================================\n"
             << "         🎬 MOVIE TICKET BOOKING SYSTEM      \n"
             << "============================================\n" << RESET;

        cout << GREEN  << "  1. 🎞️  View Movies\n"   << RESET;
        cout << CYAN   << "  2. 🎟️  Book Tickets\n"   << RESET;
        cout << YELLOW << "  3. 📄  View Booking History\n" << RESET;
        cout << RED    << "  4. ❌  Exit\n" << RESET;

        cout << BOLD << "\n👉 Enter your choice: " << RESET;
        string choice;
        getline(cin, choice);
        choice = trim(choice);
        if (choice.empty()) { getline(cin, choice); choice = trim(choice); } // handle extra newline

        if (!is_number(choice)) {
            cout << RED << "Invalid input. Please enter a number (1-4).\n" << RESET;
            continue;
        }

        int c = stoi(choice);
        if (c == 1) viewMovies();
        else if (c == 2) bookTicket();
        else if (c == 3) viewBookings();
        else if (c == 4) {
            cout << GREEN << "\nThank you for using the Movie Booking System! 👋\n" << RESET;
            break;
        } else {
            cout << RED << "Please choose between 1 and 4.\n" << RESET;
        }
    }
}
