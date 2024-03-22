#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

const char* FILENAME = "user_credentials.txt"; // File to store user credentials

// Function to check if a username already exists
bool usernameExists(const char* username) {
    ifstream file(FILENAME); // Open file for reading
    char storedUsername[100];
    char storedPassword[100];
    bool exists = false;

    while (file >> storedUsername >> storedPassword) {
        if (strcmp(storedUsername, username) == 0) {
            exists = true;
            break;
        }
    }
    file.close();

    return exists;
}

// Function to register a new user
void registerUser(const char* username, const char* password) {
    if (usernameExists(username)) {
        cout << "Error: Username already exists." << endl;
        return;
    }

    ofstream file(FILENAME, ios::app); // Open file in append mode
    if (file.is_open()) {
        file << username << " " << password << endl; // Write username and password to file
        file.close();
        cout << "User registered successfully." << endl;
    }
    else {
        cout << "Error: Unable to open file for writing." << endl;
    }
}

// Function to check if user credentials are valid
bool authenticateUser(const char* username, const char* password) {
    ifstream file(FILENAME); // Open file for reading
    char storedUsername[100];
    char storedPassword[100];
    bool found = false;

    while (file >> storedUsername >> storedPassword) {
        if (strcmp(storedUsername, username) == 0 && strcmp(storedPassword, password) == 0) {
            found = true;
            break;
        }
    }
    file.close();

    return found;
}

// Function to check if the file is empty
bool isFileEmpty() {
    ifstream file(FILENAME);
    return file.peek() == ifstream::traits_type::eof();
}

// Function to delete passwords for all domains
void deletePasswords(const char* username) {
    char filename[100];
    strncpy_s(filename, username, sizeof(filename));
    filename[sizeof(filename) - 1] = '\0'; // Ensure null-termination
    strncat_s(filename, "_credentials.txt", sizeof(filename) - strlen(filename) - 1);

    ofstream file(filename); // Open file for writing (truncates the file)
    file.close();
    cout << "Passwords deleted successfully." << endl;
}

// Function to encrypt a string using Caesar Cipher
string encryptCaesarCipher(const string& text, int shift) {
    string result = text;
    for (size_t i = 0; i < text.length(); ++i) {
        if (isalpha(text[i])) {
            if (isupper(text[i])) {
                result[i] = 'A' + (text[i] - 'A' + shift) % 26;
            }
            else {
                result[i] = 'a' + (text[i] - 'a' + shift) % 26;
            }
        }
    }
    return result;
}

// Function to decrypt a string encrypted using Caesar Cipher
string decryptCaesarCipher(const string& text, int shift) {
    string result = text;
    for (size_t i = 0; i < text.length(); ++i) {
        if (isalpha(text[i])) {
            if (isupper(text[i])) {
                result[i] = 'A' + (text[i] - 'A' - shift + 26) % 26; // Ensure positive result after modulo operation
            }
            else {
                result[i] = 'a' + (text[i] - 'a' - shift + 26) % 26; // Ensure positive result after modulo operation
            }
        }
    }
    return result;
}

// Function to add credentials to a user's file
void addCredentialsToFile(const char* username, const char* domain, const char* domainUsername, const char* domainPassword) {
    char filename[100];
    strncpy_s(filename, username, sizeof(filename));
    filename[sizeof(filename) - 1] = '\0'; // Ensure null-termination
    strncat_s(filename, "_credentials.txt", sizeof(filename) - strlen(filename) - 1);

    ofstream file(filename, ios::app); // Open file in append mode
    if (file.is_open()) {
        // Encrypt the password before storing it
        string encryptedPassword = encryptCaesarCipher(domainPassword, 3); // Encrypt using Caesar Cipher with shift 3

        file << "Domain: " << domain << endl;
        file << "Username: " << domainUsername << endl;
        file << "Password: " << encryptedPassword << endl << endl;
        file.close();
        cout << "Credentials added successfully." << endl;
    }
    else {
        cout << "Error: Unable to open file for writing." << endl;
    }
}

// Function to retrieve credentials from a user's file
void retrieveCredentialsFromFile(const char* username) {
    char filename[100];
    strncpy_s(filename, username, sizeof(filename));
    filename[sizeof(filename) - 1] = '\0'; // Ensure null-termination
    strncat_s(filename, "_credentials.txt", sizeof(filename) - strlen(filename) - 1);

    ifstream file(filename); // Open file for reading
    if (file.is_open()) {
        cout << "Credentials for " << username << ":\n";
        char line[100];
        while (file.getline(line, sizeof(line))) {
            // Check if the line contains a password and decrypt it
            if (strncmp(line, "Password: ", 10) == 0) {
                string encryptedPassword = line + 10; // Extract the password part
                string decryptedPassword = decryptCaesarCipher(encryptedPassword, 3); // Decrypt using Caesar Cipher with shift 3
                cout << "Password: " << decryptedPassword << endl; // Display the decrypted password
            }
            else {
                cout << line << endl;
            }
        }
        file.close();
    }
    else {
        cout << "Error: Unable to open file for reading." << endl;
    }
}

// Function to generate a random password
string generateRandomPassword(int length) {
    const string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string password;
    srand(time(nullptr));
    for (int i = 0; i < length; ++i) {
        password += charset[rand() % charset.length()];
    }
    return password;
}

int main() {
    char choice;
    bool running = true;
    while (running) {
        cout << "Welcome to the Password Manager!\n";
        cout << "Would you like to sign up (s) or login (l)? ";
        cin >> choice;

        if (choice == 's' || choice == 'S') {
            // Ask user to register
            cout << "\nSign Up\n";
            char username[100];
            char password[100];
            cout << "Enter username: ";
            cin.ignore(); // Clear input buffer
            cin.getline(username, sizeof(username));
            cout << "Enter password: ";
            cin.getline(password, sizeof(password));

            // Register user
            registerUser(username, password);
        }
        else if (choice == 'l' || choice == 'L') {
            // Prompt user to log in
            cout << "\nLogin\n";
            char username[100];
            char password[100];
            cout << "Enter username: ";
            cin.ignore(); // Clear input buffer
            cin.getline(username, sizeof(username));
            cout << "Enter password: ";
            cin.getline(password, sizeof(password));

            // Check if credentials are valid
            if (authenticateUser(username, password)) {
                cout << "\nLogin successful. Welcome, " << username << "!" << endl;

                // User authentication successful, present menu options
                while (true) {
                    cout << "\nMenu:\n";
                    cout << "1. Add credentials for a website\n";
                    cout << "2. Retrieve credentials for all websites\n";
                    cout << "3. Delete saved passwords\n";
                    cout << "4. Logout\n";
                    cout << "5. Close the application\n";
                    cout << "Choose an option: ";
                    cin >> choice;

                    if (choice == '1') {
                        char domain[100], domainUsername[100];
                        cout << "\nEnter domain: ";
                        cin.ignore(); // Clear input buffer
                        cin.getline(domain, sizeof(domain));
                        cout << "Enter username for " << domain << ": ";
                        cin.getline(domainUsername, sizeof(domainUsername));

                        char passwordChoice;
                        bool validChoice = false;
                        do {
                            cout << "Do you want to generate a random password? (y/n): ";
                            cin >> passwordChoice;
                            if (passwordChoice == 'y' || passwordChoice == 'Y') {
                                string generatedPassword = generateRandomPassword(12); // Generate a 12-character random password
                                addCredentialsToFile(username, domain, domainUsername, generatedPassword.c_str());
                                cout << "Your generated password is: " << generatedPassword;
                                validChoice = true;
                            }
                            else if (passwordChoice == 'n' || passwordChoice == 'N') {
                                char domainPassword[100];
                                cout << "Enter password for " << domain << ": ";
                                cin >> domainPassword;
                                addCredentialsToFile(username, domain, domainUsername, domainPassword);
                                validChoice = true;
                            }
                            else {
                                cout << "Please select either 'y' or 'n'." << endl;
                            }
                        } while (!validChoice);
                    }
                    else if (choice == '2') {
                        retrieveCredentialsFromFile(username);
                    }
                    else if (choice == '3') {
                        deletePasswords(username);
                    }
                    else if (choice == '4') {
                        cout << "Logged out successfully." << endl;
                        break; // Break the inner loop to return to signup/login page
                    }
                    else if (choice == '5') {
                        running = false;
                        break; // Break the inner loop and exit the program
                    }
                    else {
                        cout << "Invalid choice." << endl;
                    }
                }
            }
            else {
                cout << "\nError: Invalid username or password." << endl;
            }
        }
        else {
            cout << "Invalid choice. Please choose 's' or 'l'." << endl;
        }
    }

    return 0;
}

