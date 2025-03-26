#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <limits>
#include <sstream>

using namespace std;

// Improved hash function using prime multiplication
int HashString(const string& value) {
    int hashCode = 0;
    for (char character : value) {
        hashCode = (hashCode * 31) + character; // Prime number multiplier for better distribution
    }
    return hashCode & 0x7fffffff; // Ensure positive value
}

int main() {
    unordered_multimap<int, string> passwordTable; // Using unordered_multimap for automatic collision handling
    ifstream passwordFile("10-million-password-list-top-10000.txt"); // Ensure correct file path

    if (!passwordFile) {
        cerr << "Error: Password file not found!" << endl;
        return 1;
    }

    string password;
    cout << "Reading passwords and hashing...\n";
    while (getline(passwordFile, password)) {
        int hashValue = HashString(password);
        passwordTable.insert({hashValue, password}); // Insert directly into multimap
    }
    passwordFile.close();

    cout << "Hashing complete. Enter a password or hashed value to find possible matches." << endl;

    string userInput;
    while (true) {
        cout << "\nEnter password or hashed value (or type -1 to exit): ";
        getline(cin, userInput);

        if (userInput == "-1") {
            break;
        }

        stringstream ss(userInput);
        int userHash;
        if (!(ss >> userHash)) { // If input is not a number, assume it's a password
            userHash = HashString(userInput);
        }

        auto range = passwordTable.equal_range(userHash);
        if (range.first != range.second) {
            cout << "Possible original passwords:\n";
            for (auto it = range.first; it != range.second; ++it) {
                cout << " - " << it->second << endl;
            }
        } else {
            cout << "Password not found in database." << endl;
        }
    }

    cout << "Goodbye!" << endl;
    return 0;
}
