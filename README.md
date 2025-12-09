🎬 Movie Ticket Booking System (C++ – OOP)

A C++ console application that simulates an online movie ticket booking system. Designed with object-oriented programming, it features classes for Movie, Show, and BookingSystem, along with colored seat maps and booking history.

📌 Features

View movies and show timings

Seat map with color-coded availability

Book multiple seats at once

Validate seat format and availability

Generate a formatted ticket with Booking ID & timestamp

Save booking history to bookings.txt

Clean menu-driven user interface

🧠 Core OOP Concepts Used

Classes & Objects

Encapsulation

Composition

Data validation & sanitization

🎟️ Booking Flow

User selects a movie

Chooses a show timing

Seat layout displayed (green = available, red = booked)

User enters seats (A1, B2, etc.)

System validates and books them

Generates printed ticket

Saves record to bookings.txt

📄 Seat Representation

2D seat grid (A1, A2, B1, B2…)

ANSI color codes for visual interface

🛠️ Technologies Used

C++

OOP principles

File handling

String processing

▶️ Compilation
g++ main.cpp Movie.cpp Show.cpp BookingSystem.cpp -o ticket

./ticket
