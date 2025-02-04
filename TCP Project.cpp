#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <regex>

using namespace std;

// User login class: handles user registration, login and menu selection.
class login {
    protected:
        string userName, userPass, userEmail;
        int userPhoneNumber,userIc;
    public:
        void userRegister();            // User registration
        bool userLogin();           // User Login Portal
        bool isUserLogin();         // Validate login information
        void selectMenu();          // Main menu selection
};

class hotelManagementPage : public login{
    public:
        void managementPage();
        void viewCustomerDetails();
        static void viewRoomBookings();
};

// User Dashboard class: inherits from the Login class and provides the user interface.
class userDashboard : public login{
    public:
        // Constructor: initialize user information
        userDashboard(string name, string pass,int ic,string email,int phoneNumber) {
            userName = name;
            userPass = pass;
            userIc = ic;
            userEmail = email;
            userPhoneNumber = phoneNumber;
        };
        void printUserDashboard();          // Display User Dashboard
        void userProfile();         // Manage user profiles
        void userBooking();         // Handle booking-related operations
        void updateCustomerList();          //Update customer file
};

// Booking class: stores the details of a single booking record
class booking {
public:
    string userName, roomNumber, date;
    int days;

    // Constructor: initializes booking information
    booking(const string& userName, const string& roomNumber, const string& date, int days)
        : userName(userName), roomNumber(roomNumber), date(date), days(days) {}

    // Static method: displays the title of the booking list
    static void displayBookingTitle(){
        system("cls");
        cout << "+--------------------------------------+" << endl;
        cout << "+----------- Your Bookings ------------+" << endl;
        cout << "+----+-----------+--------------+------+" << endl;
        cout << "| No.|Room Number|     Date     | Days |" << endl;
        cout << "+----+-----------+--------------+------+" << endl;
    }

    // Display details of individual bookings (formatted output)
    void displayBookingDetails() const {
        cout << " |" << setw(11) << roomNumber << "|" << setw(14) << date << "|" << setw(6) << days << "|" << endl ;
    }
};

// Booking management class: handles booking additions, deletions, changes and file operations.
class bookingManager{
    private:
        static vector<booking> bookings;            // Store the current user's booking list

        // Load the user's booking record from a file
        static void loadBookings(const string& userName) {
            bookings.clear();
            ifstream bookingFile(userName + "_bookings.txt");
            if (bookingFile.is_open()) {
                string roomNumber, date, time;
                while (getline(bookingFile, roomNumber)) {
                    getline(bookingFile, date);
                    string daysStr;
                    getline(bookingFile, daysStr);
                    int days = stoi(daysStr);           // Convert string to integer
                    bookings.push_back(booking(userName, roomNumber, date, days));

                }
                bookingFile.close();
            }
        }

        // Save the current booking list to a file
        static void saveBookings(const string& userName) {
            ofstream bookingFile(userName + "_bookings.txt", ios::trunc);
            if (bookingFile.is_open()) {
                for (const auto& b : bookings) {
                    bookingFile << b.roomNumber << endl
                                << b.date << endl
                                << to_string(b.days) << endl;           // Ensure it's written as a string

                }
                bookingFile.close();
            }
        }

        // Print booking receipt
        static void printReceipt(const string& userName, const string& roomNumber, const string& date, int days) {
            system("cls"); // Clear the console for a clean display

            cout << "+--------------------------------------+" << endl;
            cout << "|       JNTM Hotel Booking Center      |" << endl;
            cout << "+--------------------------------------+" << endl;
            cout << "|            Booking Receipt           |" << endl;
            cout << "+--------------------------------------+" << endl;
            cout << "| User Name     : " << setw(20) << left << userName << " |" << endl;
            cout << "| Room Number   : " << setw(20) << left << roomNumber << " |" << endl;
            cout << "| Booking Date  : " << setw(20) << left << date << " |" << endl;
            cout << "| Number of Days: " << setw(20) << left << days << " |" << endl;
            cout << "+--------------------------------------+" << endl;
            cout << "|    Thank you for booking with us!    |" << endl;
            cout << "+--------------------------------------+" << endl;
        }

        // Print booking receipt
        static void printReceipt(const string& userName, const booking& b) {
            system("cls");
            cout << "+--------------------------------------+" << endl;
            cout << "|       JNTM Hotel Booking Center      |" << endl;
            cout << "+--------------------------------------+" << endl;
            cout << "|            Booking Receipt           |" << endl;
            cout << "+--------------------------------------+" << endl;
            cout << "| User Name     : " << setw(21) << left << b.userName << "|" << endl;
            cout << "| Room Number   : " << setw(21) << left << b.roomNumber << "|" << endl;
            cout << "| Booking Date  : " << setw(21) << left << b.date << "|" << endl;
            cout << "| Number of Days: " << setw(20) << left << b.days << " |" << endl;
            cout << "+--------------------------------------+" << endl;
            cout << "|    Thank you for booking with us!    |" << endl;
            cout << "+--------------------------------------+" << endl;
        }

    public:
        static bool isRoomAvailable(const string& roomNumber, const string& date) {
            ifstream roomFile("room.txt");
            string fileRoom, fileDate, fileUser;

            while (roomFile >> fileUser >> fileRoom >> fileDate) {
                if (fileRoom == roomNumber && fileDate == date) {
                    roomFile.close();
                    return false; // Room is already booked
                }
            }
            roomFile.close();
            return true; // Room is available
        }

        // Create a new booking (with input validation)
        static void bookService(const string& userName) {
            string date, roomNumber;
            int floor, room, days;

            regex datePattern(R"(^\d{2}/\d{2}/\d{4}$)");            // Matches DD/MM/YYYY

            system("cls");

            cout << "+--------------------------------------+" << endl;
            cout << "|             Make a Booking           |" << endl;
            cout << "+--------------------------------------+" << endl;

            // Floor input validation (1-8)
            do {
                cout << " Enter Floor (1-8)              : ";
                cin >> floor;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "+--------------------------------------+" << endl;
                    cout << "|            Invalid input.            |" << endl;
                    cout << "|Please enter a number between 1 and 8.|" << endl;
                    cout << "+--------------------------------------+" << endl;
                    floor = -1;
                } else if (floor < 1 || floor > 8) {
                    cout << "+--------------------------------------+" << endl;
                    cout << "|         Invalid Floor number.        |" << endl;
                    cout << "|          Please enter again.         |" << endl;
                    cout << "+--------------------------------------+" << endl;
                }
            } while ((floor < 1) || (floor > 8));

            // Room input validation (1-6)
            do {
                cout << " Enter Room (1-6)               : ";
                cin >> room;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "+--------------------------------------+" << endl;
                    cout << "|            Invalid input.            |" << endl;
                    cout << "|Please enter a number between 1 and 6.|" << endl;
                    cout << "+--------------------------------------+" << endl;
                    room = -1;
                } else if (room < 1 || room > 6) {
                    cout << "+--------------------------------------+" << endl;
                    cout << "|          Invalid Room number.        |" << endl;
                    cout << "|          Please enter again.         |" << endl;
                    cout << "+--------------------------------------+" << endl;
                }
            } while ((room < 1) || (room > 6));

            roomNumber = to_string(floor) + "00" + to_string(room);

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Date input validation (format and logic)
            while (true) {
                cout << " Enter Booking Date (DD/MM/YYYY): ";
                getline(cin, date);

                if (regex_match(date, datePattern)) {
                    int day, month, year;
                    char separator;
                    stringstream ss(date);
                    ss >> day >> separator >> month >> separator >> year;

                    if (month < 1 || month > 12) {
                        cout << "+--------------------------------------+" << endl;
                        cout << "|            Invalid month.            |" << endl;
                        cout << "|    Month must be between 1 and 12.   |" << endl;
                        cout << "+--------------------------------------+" << endl;
                        continue;
                    }

                    int maxDays = 31;
                    if (month == 4 || month == 6 || month == 9 || month == 11) {
                        maxDays = 30;
                    } else if (month == 2) {
                        maxDays = 28;
                    }

                    if (day >= 1 && day <= maxDays) {
                        break;
                    } else {
                        cout << "+--------------------------------------+" << endl;
                        cout << "|             Invalid day.             |" << endl;
                        cout << "|     Day must be between 1 and " << maxDays << ".    |" << endl;
                        cout << "+--------------------------------------+" << endl;
                    }
                } else {
                    cout << "+--------------------------------------+" << endl;
                    cout << "|Invalid date format. Please try again.|" << endl;
                    cout << "+--------------------------------------+" << endl;
                }
            }

            if (!isRoomAvailable(roomNumber, date)) {
                cout << "+--------------------------------------+" << endl;
                cout << "|     Sorry, this room is already      |" << endl;
                cout << "|       booked on " << date << " !         |" << endl;
                cout << "+--------------------------------------+" << endl;
                system("pause");
                return;
            }

            // Days input validation (format and logic)
            while (true) {
                cout << " Enter Booking Days (1-6)    : ";
                cin >> days;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "+--------------------------------------+" << endl;
                    cout << "|            Invalid input.            |" << endl;
                    cout << "|Please enter a number between 1 and 6.|" << endl;
                    cout << "+--------------------------------------+" << endl;
                    continue;
                }

                if (days >= 1 && days <= 6) {
                    break;
                }

                cout << "+--------------------------------------+" << endl;
                cout << "|          Invalid input.              |" << endl;
                cout << "|   Please enter a number from 1 to 6. |" << endl;
                cout << "+--------------------------------------+" << endl;
            }

            ofstream roomFile("room.txt", ios::app);
            roomFile << userName << " " << roomNumber << " " << date << endl;
            roomFile.close();

            ofstream bookingFile(userName + "_bookings.txt", ios::app);
            if (bookingFile.is_open()) {
                bookingFile << roomNumber << endl << date << endl << days << endl;
                bookingFile.close();
                cout << "+--------------------------------------+" << endl;
                cout << "|          Booking successful!         |" << endl;
                cout << "+--------------------------------------+" << endl;
            } else {
                cout << "+--------------------------------------+" << endl;
                cout << "|  Error: Unable to save the booking.  |" << endl;
                cout << "+--------------------------------------+" << endl;
            }
            printReceipt(userName, roomNumber, date, days);
            system("pause");
        }

        // View the user's booking list
        static void viewBooking(const string& userName){
            loadBookings(userName);

            char choice;
            if (bookings.empty()) {
                system("cls");
                cout << "+--------------------------------------+" << endl;
                cout << "|       You have no bookings yet.      |" << endl;
                cout << "+--------------------------------------+" << endl;
                system("pause");
                return;
            }

            booking::displayBookingTitle();

            for (int i = 0; i < bookings.size(); ++i){
                cout << "|" << setw(3) << i+1 ;
                bookings[i].displayBookingDetails();
            }
            cout << "+----+-----------+--------------+------+" << endl;

            while (true){
                cout << " Do you want to check the details of booking? (y/n): ";
                cin >> choice;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "+--------------------------------------+" << endl;
                    cout << "|   Invalid choice. Please try again.  |" << endl;
                    cout << "+--------------------------------------+" << endl;
                }else{
                    if (choice == 'y' || choice == 'n') {
                        break; // Exit the loop if the input is valid
                    } else {
                        cout << "+--------------------------------------+" << endl;
                        cout << "|     Please only enter 'y' or 'n'.    |" << endl;
                        cout << "+--------------------------------------+" << endl;
                    }
                }
            }

            if ((choice == 'y') || (choice == 'Y')) {
                size_t bookingIndex;

                while (true) {
                    cout << " Enter the booking number (1-" << bookings.size() << "): ";
                    cin >> bookingIndex;

                    if (cin.fail() || bookingIndex < 1 || bookingIndex > bookings.size()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "+--------------------------------------+" << endl;
                        cout << "|             Invalid input.           |" << endl;
                        cout << "|      Please enter a valid number.    |" << endl;
                        cout << "+--------------------------------------+" << endl;
                    } else {
                        break;          // Valid input
                    }
                }

            const booking& selectedBooking = bookings[bookingIndex - 1];
            printReceipt(userName,selectedBooking);

            system("pause");
            }
        }

        // Canceling a specific booking
        static void cancelBooking(const string& userName) {
            loadBookings(userName);

            if (bookings.empty()) {
                system("cls");
                cout << "+--------------------------------------+" << endl;
                cout << "|    You have no bookings to cancel.   |" << endl;
                cout << "+--------------------------------------+" << endl;
                system("pause");
                return;
            }

            booking::displayBookingTitle();

            for (int i = 0; i < bookings.size(); ++i){
                cout << "|" << setw(3) << i+1 ;
                bookings[i].displayBookingDetails();
            }
            cout << "+----+-----------+--------------+------+" << endl;

            cout << " Enter the number of the booking to cancel: ";
            int cancelChoice;
            cin >> cancelChoice;

            if (cancelChoice < 1 || cancelChoice > bookings.size()) {
                cout << "+--------------------------------------+" << endl;
                cout << "|            Invalid choice.           |" << endl;
                cout << "+--------------------------------------+" << endl;
                system("pause");
                return;
            }

            bookings.erase(bookings.begin() + cancelChoice - 1);
            saveBookings(userName);

            cout << "+--------------------------------------+" << endl;
            cout << "|    Booking successfully canceled.    |" << endl;
            cout << "+--------------------------------------+" << endl;
            system("pause");
        }
};

// Static member initialisation
vector<booking> bookingManager::bookings;

// ------------------------- method implementation -------------------------

void hotelManagementPage::viewRoomBookings() {
        system("cls");
        ifstream roomFile("room.txt");
        if (!roomFile.is_open()) {
            cout << "+--------------------------------------+" << endl;
            cout << "|       No booking records found.      |" << endl;
            cout << "+--------------------------------------+" << endl;
            return;
        }
        int count;
        string roomNumber, date, userName;
        cout << "+--------------------------------------+" << endl;
        cout << "|------ Room Booking Information ------|" << endl;
        cout << "+---+--------------+------+------------+" << endl;
        cout << "|No.|   Username   | Room |    Date    |" << endl;
        cout << "+---+--------------+------+------------+" << endl;

        while (getline(roomFile, userName) && getline(roomFile, roomNumber) && getline(roomFile, date)) {
            cout << "|" << setw(3) << count
                 << "|" << setw(13) << userName
                 << " |" << setw(5) << roomNumber
                 << " | " << setw(10) << date << " |" << endl;
            count++;
        }
        cout << "+---+--------------+------+------------+" << endl;

        roomFile.close();

        system("pause");
    }

void hotelManagementPage::viewCustomerDetails(){
    // View Customer Details
    system("cls");
    ifstream file("customers.txt");
    if (!file.is_open()) {
        cout << "+--------------------------------------+" << endl;
        cout << "|  Error: Could not open customer file |" << endl;
        cout << "+--------------------------------------+" << endl;
        system("pause");
    }

    // Print table header
    cout << "+-----------------------------------------------------------+" << endl;
    cout << "|------------------ Customer Information -------------------|" << endl;
    cout << "+-----+---------------------+----------------+--------------+" << endl;
    cout << "| No. |       Username      |       IC       | Phone Number |" << endl;
    cout << "+-----+---------------------+----------------+--------------+" << endl;

    string username, ic, email, phone;
    int count = 1;

    getline(file,username);
    getline(file,ic);
    getline(file,phone);

    while (getline(file, username) && getline(file, ic) && getline(file, phone)) {
        cout << "|" << setw(4) << count
             << " |" << setw(20) << username
             << " |" << setw(15) << ic
             << " |" << setw(13) << phone << " |" << endl;
        count++;
    }

    cout << "+-----+---------------------+----------------+--------------+" << endl;

    file.close();

    system("pause");
    managementPage();
}

void hotelManagementPage::managementPage() {
    system("cls");
    int choice;
    cout << "+--------------------------------------+" << endl;
    cout << "|           Management Page            |" << endl;
    cout << "+--------------------------------------+" << endl;
    cout << "| 1. View Customer Details             |" << endl;
    cout << "| 2. View Room Details                 |" << endl;
    cout << "| 3. Logout                            |" << endl;
    cout << "+--------------------------------------+" << endl;
    cout << " Enter your choice: " ;
    cin >> choice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "+--------------------------------------+" << endl;
        cout << "|     Please enter a valid number.     |" << endl;
        cout << "+--------------------------------------+" << endl;
        system("pause");
    }

    switch (choice) {
    case 1:
        viewCustomerDetails();
        break;
    case 2:
        viewRoomBookings();
        managementPage();
        break;
    case 3:
        cout << "+--------------------------------------+" << endl;
        cout << "|             Logging out...           |" << endl;
        cout << "+--------------------------------------+" << endl;
        system("pause");
        selectMenu();
        break;
    default:
        cout << "+--------------------------------------+" << endl;
        cout << "|   Invalid choice. Please try again.  |" << endl;
        cout << "+--------------------------------------+" << endl;
        system("pause");
    }
}

// Validate user login information (includes password retry mechanism)
bool login::isUserLogin() {

    system("cls");
    cout << "+--------------------------------------+" << endl;
    cout << "|              User Login              |" << endl;
    cout << "+--------------------------------------+" << endl;
    cout << " Enter Name    : ";
    cin.ignore();
    getline(cin, userName);

    if (userName == "ManagerAdam") {
        int attemptCount = 0;
        while (attemptCount < 3) { // Allow up to 3 attempts for password
            cout << " Enter Password: ";
            string userPass;
            getline(cin, userPass);

            if (userPass.empty()) {
                cout << "+--------------------------------------+" << endl;
                cout << "|     Password cannot be empty.        |" << endl;
                cout << "+--------------------------------------+" << endl;
                continue;
            }

            // Check management credentials
            if (userPass == "123456") {
                hotelManagementPage management;
                management.managementPage();
                return true;
            } else {
                attemptCount++;
                if (attemptCount < 3) {
                    cout << "+--------------------------------------+" << endl;
                    cout << "|          Incorrect password.         |" << endl;
                    cout << "|       You have " << (3 - attemptCount) << " attempts left.      |" << endl;
                    cout << "+--------------------------------------+" << endl;
                }
            }
        }
        cout << "+--------------------------------------+" << endl;
        cout << "|  Incorrect password entered 3 times. |" << endl;
        cout << "|    Please re-enter your username.    |" << endl;
        cout << "+--------------------------------------+" << endl;
        system("pause");
        return false;
    }

    //(username input and file checking logic)
    ifstream read(userName + ".txt");
    if (!read.is_open()) {
        cout << "+--------------------------------------+" << endl;
        cout << "|        Error: User not found.        |" << endl;
        cout << "+--------------------------------------+" << endl;
        system("pause");
        return false;
    }

    string enterName, enterPass, userIc;
    getline(read,enterName);
    getline(read,enterPass);
    getline(read,userIc);
    getline(read, userEmail);
    read >> userPhoneNumber;

    read.close();

    int attemptCount = 0;

    while (attemptCount < 3) { // Allow up to 3 attempts for password
        cout << " Enter Password: ";
        string userPass;
        getline(cin, userPass);

        //(password verification logic)
        if (userPass.empty()) {
            cout << "+--------------------------------------+" << endl;
            cout << "|     Password cannot be empty.        |" << endl;
            cout << "+--------------------------------------+" << endl;
            continue;
        }

        if (userPass == enterPass) {
            return true;            // Successful login
            break;
        } else {
            attemptCount++;
            if (attemptCount < 3) {
                cout << "+--------------------------------------+" << endl;
                cout << "|          Incorrect password.         |" << endl;
                cout << "|       You have " << (3 - attemptCount) << " attempts left.      |" << endl;
                cout << "+--------------------------------------+" << endl;
            }
        }
    }
    cout << "+--------------------------------------+" << endl;
    cout << "|  Incorrect password entered 3 times. |" << endl;
    cout << "|    Please re-enter your username.    |" << endl;
    cout << "+--------------------------------------+" << endl;
    system("pause");
    return false;           // Failed login after 3 attempts
}

// User registration: verifying the uniqueness of the user name and saving the information
void login::userRegister() {
    system("cls");

    cout << "+--------------------------------------+" << endl;
    cout << "|         Register a new user          |" << endl;
    cout << "+--------------------------------------+" << endl;

    // (input logic and file saving)
    while (true) {

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << " Please Enter Username    : ";
        getline(cin, userName);

        ifstream checkFile(userName + ".txt");          // Check if the username already exists by checking the file
        if (checkFile.is_open()) {
            cout << "+--------------------------------------+" << endl;
            cout << "|         Username already taken.      |" << endl;
            cout << "|     Please choose another username.  |" << endl;
            cout << "+--------------------------------------+" << endl;
            checkFile.close();
        } else {
            break;          // Username is available, exit the loop
        }
    }

    cout << " Please Enter Password    : ";
    getline(cin, userPass);

    cout << " Please Enter IC Number   : ";          // For IC, Email, and Phone Number, accept empty input
    string icInput;
    getline(cin, icInput);
    if (!icInput.empty()) {
        userIc = stoi(icInput);             // Convert string to integer if provided
    } else {
        userIc = 0;             // Set default value if left empty
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << " Please Enter Email       : ";
    getline(cin, userEmail);
    if (userEmail.empty()) {
        userEmail = "Not Provided";             // Use a default value if left empty
    }

    cout << " Please Enter Phone Number: ";
    string phoneInput;
    getline(cin, phoneInput);
    if (!phoneInput.empty()) {
        userPhoneNumber = stoi(phoneInput);             // Convert string to integer if provided
    }else {
        userPhoneNumber = 0;
    }

    // Save to individual user file
    ofstream file(userName + ".txt");
    if (file.is_open()) {
        file << userName << endl
             << userPass << endl
             << userIc << endl
             << userEmail << endl
             << userPhoneNumber;
        file.close();
    }

    // Save to central customer file
    ofstream customerFile("customers.txt", ios::app);
    if (customerFile.is_open()) {
        customerFile << userName << endl
                     << userIc << endl
                     << userPhoneNumber << endl;
        customerFile.close();
    }

    cout << "+--------------------------------------+" << endl;
    cout << "|        Registration successful!      |" << endl;
    cout << "+--------------------------------------+" << endl;
    system("pause");
    system("cls");
}

// User Login Portal
bool login::userLogin() {
    if (isUserLogin()) {
        userDashboard dashboard(userName, userPass, userIc, userEmail, userPhoneNumber);
        dashboard.printUserDashboard();              // Move to the user dashboard after successful login
        return true;
    } else {
        cout << "+--------------------------------------+" << endl;
        cout << "|    Invalid login. Please try again.  |" << endl;
        cout << "+--------------------------------------+" << endl;
        return false;
    }
}

void userDashboard::updateCustomerList() {
    ifstream inFile("customers.txt");
    vector<string> customerRecords;
    string line;

    // Read all existing customers
    while (getline(inFile, line)) {
        if (line.find(userName) == string::npos) {
            customerRecords.push_back(line); // Keep old records
        }
    }
    inFile.close();

    // Rewrite `customers.txt` with updated data
    ofstream outFile("customers.txt", ios::trunc);
    for (const string& record : customerRecords) {
        outFile << record << endl;
    }

    // Add updated user information
    outFile << userName << endl
            << userIc << endl
            << userPhoneNumber << endl;
    outFile.close();
}

// Manage user profiles
void userDashboard::userProfile(){
    system("cls");
    char opt;
    cout << "+--------------------------------------+" << endl;
    cout << "|          Profile Information         |" << endl;
    cout << "+--------------------------------------+" << endl;
    cout << " Name        : " << userName << endl;
    cout << " Password    : (hidden)" << endl;
    cout << " Ic Number   : " << userIc << endl;
    cout << " Email       : " << userEmail << endl;
    cout << " Phone Number: " << userPhoneNumber << endl << endl;

    while (true){
        cout << " Do you want to make any changes?(y/n): ";
        cin >> opt;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "+--------------------------------------+" << endl;
            cout << "|   Invalid choice. Please try again.  |" << endl;
            cout << "+--------------------------------------+" << endl;
        }else{
            if (opt == 'y' || opt == 'n') {
                break;          // Exit the loop if the input is valid
            } else {
                cout << "+--------------------------------------+" << endl;
                cout << "|     Please only enter 'y' or 'n'.    |" << endl;
                cout << "+--------------------------------------+" << endl;
            }
        }
    }

    cout << endl;
    if (opt == 'y' || opt == 'Y') {
        // Allow user to update their information
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string icInput;
        cout << " Please Enter New IC Number   : ";          // For IC, Email, and Phone Number, accept empty input
        getline(cin, icInput);
        if (!icInput.empty()) {
            userIc = stoi(icInput);           // Convert string to integer if provided
        } else {
            userIc = 0;             // Set default value if left empty
        }

        cout << " Please Enter New Email       : ";
        getline(cin, userEmail);
        if (userEmail.empty()) {
            userEmail = "Not Provided";             // Use a default value if left empty
        }

        cout << " Please Enter New Phone Number: ";
        string phoneInput;
        getline(cin, phoneInput);
        if (!phoneInput.empty()) {
            userPhoneNumber = stoi(phoneInput);             // Convert string to integer if provided
        } else {
            userPhoneNumber = 0;            // Set default value if left empty
        }

        updateCustomerList();

        ofstream file(userName + ".txt", ios::trunc);           // Open the file in truncate mode to overwrite the content
        if (file.is_open()) {
            file << userName << endl << userPass << endl << userIc << endl << userEmail << endl << userPhoneNumber;
            file.close();
            cout << "+--------------------------------------+" << endl;
            cout << "|      Profile updated successfully!   |" << endl;
            cout << "+--------------------------------------+" << endl;
            system("pause");
        } else {
            cout << "+--------------------------------------+" << endl;
            cout << "|Error: Unable to save profile updates.|" << endl;
            cout << "+--------------------------------------+" << endl;
        }
    }else {
        cout << "+--------------------------------------+" << endl;
        cout << "|            No changes made.          |" << endl;
        cout << "+--------------------------------------+" << endl;
        system("pause");
    }
};

// Handle booking-related operations
void userDashboard::userBooking(){
    while (true) {
        system("cls");

        cout << "+--------------------------------------+" << endl;
        cout << "|              Booking Menu            |" << endl;
        cout << "+--------------------------------------+" << endl;
        cout << "| 1. Make a Booking                    |" << endl;
        cout << "| 2. View Bookings                     |" << endl;
        cout << "| 3. Cancel Bookings                   |" << endl;
        cout << "| 4. Back to Dashboard                 |" << endl;
        cout << "+--------------------------------------+" << endl;
        cout << " Enter your choice: ";
        int option;
        while (true) {
            cin >> option;

            // Check if the input is invalid
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "+--------------------------------------+" << endl;
                cout << "|             Invalid input.           |" << endl;
                cout << "|      Please enter a valid number.    |" << endl;
                cout << "+--------------------------------------+" << endl;
            } else {
                // Valid input
                break;          // Exit the loop if the input is valid
            }
        }

        switch (option) {
        case 1:
            bookingManager::bookService(userName);
            break;
        case 2:
            bookingManager::viewBooking(userName);
            break;
        case 3:
            bookingManager::cancelBooking(userName);
            break;
        case 4:
            cout << "+--------------------------------------+" << endl;
            cout << "|     Returning to the dashboard...    |" << endl;
            cout << "+--------------------------------------+" << endl;
            return;
        default:
            cout << "+--------------------------------------+" << endl;
            cout << "|   Invalid choice. Please try again.  |" << endl;
            cout << "+--------------------------------------+" << endl;
        }
    }
};

// Display User Dashboard
void userDashboard::printUserDashboard() {
    while (true) {
        int option;
        system("cls");

        cout << "+--------------------------------------+" << endl;
        cout << "|             User Dashboard           |" << endl;
        cout << "+--------------------------------------+" << endl;
        cout << "| 1. View Profile                      |" << endl;
        cout << "| 2. Booking                           |" << endl;
        cout << "| 3. Logout                            |" << endl;
        cout << "+--------------------------------------+" << endl;
        cout << " Enter your choice: ";
        cin >> option;

        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "+--------------------------------------+" << endl;
            cout << "|             Invalid input.           |" << endl;
            cout << "|      Please enter a valid number.    |" << endl;
            cout << "+--------------------------------------+" << endl;
            continue;
        }

        switch (option) {
        case 1:
            userProfile();
            break;
        case 2:
            userBooking();
            system("pause");
            break;
        case 3:
            cout << "+--------------------------------------+" << endl;
            cout << "|             Logging out...           |" << endl;
            cout << "+--------------------------------------+" << endl;
            system("pause");
            return; // Exit the dashboard and return to the main menu
        default:
            cout << "+--------------------------------------+" << endl;
            cout << "|   Invalid choice. Please try again.  |" << endl;
            cout << "+--------------------------------------+" << endl;
            system("pause");
        }
    }
}

// Main menu selection
void login::selectMenu() {
    while (true) {
        system("cls");
        int choice;

        // Display menu options
        cout << "+--------------------------------------+" << endl;
        cout << "| Welcome to JNTM Hotel Booking Center |" << endl;
        cout << "+--------------------------------------+" << endl;
        cout << "| 1. Register New User                 |" << endl;
        cout << "| 2. Login                             |" << endl;
        cout << "| 3. Exit                              |" << endl;
        cout << "+--------------------------------------+" << endl;
        cout << " Enter your choice: " ;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "+--------------------------------------+" << endl;
            cout << "|     Please enter a valid number.     |" << endl;
            cout << "+--------------------------------------+" << endl;
            continue;
        }

        switch (choice) {
        case 1:
            userRegister();
            break;
        case 2:
            userLogin();
            break;
        case 3:
            cout << "+--------------------------------------+" << endl;
            cout << "|               Goodbye!               |" << endl;
            cout << "+--------------------------------------+" << endl;
            return; // Exit the menu
        default:
            cout << "+--------------------------------------+" << endl;
            cout << "|   Invalid choice. Please try again.  |" << endl;
            cout << "+--------------------------------------+" << endl;
            system("pause");
        }
    }
}

// Main function entry
int main() {

    login user;
    user.selectMenu();

    return 0;
}
