#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Define a structure to store bus information
struct Bus {
    int busNumber;
    char source[50];
    char destination[50];
    int totalSeats;
    int availableSeats;
    float fare;
};

// Define a structure to store user login information
struct User {
    char username[50];
    char password[50];
};

// Forward declaration of the calculateDiscount function
float calculateDiscount(float originalFare, int age);

// Function to set the background color of the console using system("color")
void setBackgroundColor(char colorCode) {
    char colorCommand[10];
    sprintf(colorCommand, "color %cF", colorCode);  // 'F' sets white text
    system(colorCommand);
}

// Function to display the main menu
void displayMainMenu() {
    printf("\n=== Main Menu ===\n");
    printf("1. Login\n");
    printf("2. Exit\n");
    printf("Enter your choice: ");
}

// Function to display the user menu
void displayUserMenu() {
    printf("\n=== User Menu ===\n");
    printf("1. Book a Ticket\n");
    printf("2. Cancel a Ticket\n");
    printf("3. Check Bus Status\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

// Function to perform user login
int loginUser(struct User users[], int numUsers, char username[], char password[]) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return i; // Return the index of the logged-in user
        }
    }
    return -1; // Return -1 if login fails
}

// Function to calculate the discount based on age
float calculateDiscount(float originalFare, int age) {
    float discount = 0.0;

    // Example discount conditions
    if (age < 12) {
        discount = 0.5; // 50% discount for children
    } else if (age >= 60) {
        discount = 0.3; // 30% discount for senior citizens
    }

    // Calculate the discounted fare
    float discountedFare = originalFare * (1 - discount);
    return discountedFare;
}

// Function to book tickets with discount option
void bookTicket(struct Bus buses[], int numBuses) {
    printf("\nEnter Bus Number: ");
    int busNumber;
    scanf("%d", &busNumber);

    // Find the bus with the given busNumber
    int busIndex = -1;
    for (int i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            busIndex = i;
            break;
        }
    }

    if (busIndex == -1) {
        setBackgroundColor('4');
        printf("Bus with Bus Number %d not found.\n", busNumber);
    } else {
        printf("Enter Number of Seats: ");
        int seatsToBook;
        if (scanf("%d", &seatsToBook) != 1) {
            setBackgroundColor('4');
            printf("Invalid input for number of seats.\n");
            return;
        }

        if (buses[busIndex].availableSeats < seatsToBook) {
            setBackgroundColor('4');
            printf("Sorry, only %d seats are available.\n", buses[busIndex].availableSeats);
        } else {
            int eligibleDiscountPassengers;
            printf("Enter number of passengers eligible for discount (age < 12 or age >= 60): ");
            if (scanf("%d", &eligibleDiscountPassengers) != 1 || eligibleDiscountPassengers > seatsToBook) {
                setBackgroundColor('4');
                printf("Invalid input for number of discount passengers.\n");
                return;
            }

            float totalDiscountedFare = 0.0;
            for (int i = 0; i < eligibleDiscountPassengers; i++) {
                int age;
                printf("Enter age of passenger %d: ", i + 1);
                if (scanf("%d", &age) != 1) {
                    setBackgroundColor('4');
                    printf("Invalid input for age.\n");
                    return;
                }

                // If age is less than 12, ask for the smart card number
                if (age < 12) {
                    char smartCard[20];
                    printf("Enter Smart Card Number for passenger %d: ", i + 1);
                    scanf("%s", smartCard);
                    printf("Smart Card Number for passenger %d: %s\n", i + 1, smartCard);
                } 
                // If age is 60 or greater, ask for the CNIC
                else if (age >= 60) {
                    char cnic[20];
                    printf("Enter CNIC for passenger %d: ", i + 1);
                    scanf("%s", cnic);
                    printf("CNIC for passenger %d: %s\n", i + 1, cnic);
                }

                // Calculate the discounted fare for this passenger
                float discountedFare = calculateDiscount(buses[busIndex].fare, age);
                printf("Discounted Fare for passenger %d: %.2f\n", i + 1, discountedFare);
                totalDiscountedFare += discountedFare;
            }

            // Calculate the fare for the remaining passengers (those not eligible for discount)
            float remainingFare = buses[busIndex].fare * (seatsToBook - eligibleDiscountPassengers);
            float totalFare = totalDiscountedFare + remainingFare;

            // Update available seats
            buses[busIndex].availableSeats -= seatsToBook;

            setBackgroundColor('2');
            printf("Booking successful! %d seats booked on Bus Number %d.\n", seatsToBook, busNumber);
            printf("Total Fare: %.2f\n", totalFare);
        }
    }
}

// Function to cancel tickets
void cancelTicket(struct Bus buses[], int numBuses) {
    printf("\nEnter Bus Number: ");
    int busNumber;
    scanf("%d", &busNumber);

    // Find the bus with the given busNumber
    int busIndex = -1;
    for (int i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            busIndex = i;
            break;
        }
    }

    if (busIndex == -1) {
        setBackgroundColor('4');
        printf("Bus with Bus Number %d not found.\n", busNumber);
    } else {
        printf("Enter Number of Seats to Cancel: ");
        int seatsToCancel;
        if (scanf("%d", &seatsToCancel) != 1) {
            setBackgroundColor('4');
            printf("Invalid input for number of seats to cancel.\n");
            return;
        }

        if (seatsToCancel > (buses[busIndex].totalSeats - buses[busIndex].availableSeats)) {
            setBackgroundColor('4');
            printf("Error: You can't cancel more seats than were booked.\n");
        } else {
            buses[busIndex].availableSeats += seatsToCancel;
            setBackgroundColor('2');
            printf("Cancellation successful! %d seats canceled on Bus Number %d.\n", seatsToCancel, busNumber);
        }
    }
}

// Function to check bus status
void checkBusStatus(struct Bus buses[], int numBuses) {
    printf("\nEnter Bus Number: ");
    int busNumber;
    scanf("%d", &busNumber);

    // Find the bus with the given busNumber
    int busIndex = -1;
    for (int i = 0; i < numBuses; i++) {
        if (buses[i].busNumber == busNumber) {
            busIndex = i;
            break;
        }
    }

    if (busIndex != -1) {
        setBackgroundColor('2');
        printf("\nBus Number: %d\n", buses[busIndex].busNumber);
        printf("Source: %s\n", buses[busIndex].source);
        printf("Destination: %s\n", buses[busIndex].destination);
        printf("Total Seats: %d\n", buses[busIndex].totalSeats);
        printf("Available Seats: %d\n", buses[busIndex].availableSeats);
        printf("Fare: %.2f\n", buses[busIndex].fare);
    } else {
        setBackgroundColor('4');
        printf("Bus with Bus Number %d not found.\n", busNumber);
    }
}

int main() {
    // Initialize user data with updated names
    struct User users[5] = {
        {"tahasheikh", "pass1"}, {"areesha", "pass2"}, {"sheherbano", "pass3"}, {"uliya", "pass4"}, {"umer", "pass5"},
    };
    int numUsers = 5;

    // Initialize bus data
    struct Bus buses[3] = {
        {101, "karachi", "lahore", 50, 50, 500.0},
        {102, "islamabad", "muree", 40, 40, 400.0},
        {103, "rawalpindi", "karachi", 30, 30, 300.0},
    };
    int numBuses = 3;

    int loggedInUserId = -1; // Index of the logged-in user

    while (1) {
        if (loggedInUserId == -1) {
            displayMainMenu();
            int choice;
            if (scanf("%d", &choice) != 1) {
                setBackgroundColor('4');
                printf("Invalid input.\n");
                continue;
            }

            if (choice == 1) {
                char username[50];
                char password[50];

                printf("Enter Username: ");
                if (scanf("%s", username) != 1) {
                    setBackgroundColor('4');
                    printf("Invalid input for username.\n");
                    continue;
                }
                printf("Enter Password: ");
                if (scanf("%s", password) != 1) {
                    setBackgroundColor('4');
                    printf("Invalid input for password.\n");
                    continue;
                }

                loggedInUserId = loginUser(users, numUsers, username, password);
                if (loggedInUserId == -1) {
                    setBackgroundColor('6'); // Set orange background for error
                    printf("Login failed. Please check your username and password.\n");
                } else {
                    setBackgroundColor('5');
                    printf("Login successful. Welcome, %s!\n", username);
                }
            } else if (choice == 2) {
                setBackgroundColor('1'); // Set blue background for exit
                printf("Exiting the program.\n");
                break;
            } else {
                setBackgroundColor('4');
                printf("Invalid choice. Please try again.\n");
            }
        } else {
            displayUserMenu();
            int userChoice;
            if (scanf("%d", &userChoice) != 1) {
                setBackgroundColor('4');
                printf("Invalid input.\n");
                continue;
            }

            switch (userChoice) {
                case 1:
                    bookTicket(buses, numBuses);
                    break;
                case 2:
                    cancelTicket(buses, numBuses);
                    break;
                case 3:
                    checkBusStatus(buses, numBuses);
                    break;
                case 4:
                    setBackgroundColor('1'); // Set blue background for exit
                    printf("Logging out.\n");
                    loggedInUserId = -1;
                    break;
                default:
                    setBackgroundColor('4');
                    printf("Invalid choice. Please try again.\n");
            }
        }
    }

    return 0;
}
