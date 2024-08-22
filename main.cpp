#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm> // for std::remove

using namespace std;

class Car {
private:
    string carId;
    string brand;
    string model;
    double basePricePerDay;
    bool available;

public:
    Car(string carId, string brand, string model, double basePricePerDay)
        : carId(carId), brand(brand), model(model), basePricePerDay(basePricePerDay), available(true) {}

    string getCarId() const {
        return carId;
    }

    string getBrand() const {
        return brand;
    }

    string getModel() const {
        return model;
    }

    double calculatePrice(int rentalDays) const {
        return basePricePerDay * rentalDays;
    }

    void rent() {
        available = false;
    }

    void returnCar() {
        available = true;
    }

    bool isAvailable() const {
        return available;
    }
};

class Customer {
private:
    string customerId;
    string name;

public:
    Customer(string customerId, string name) : customerId(customerId), name(name) {}

    string getCustomerId() const {
        return customerId;
    }

    string getName() const {
        return name;
    }
};

class Rental {
private:
    Car car;
    Customer customer;
    int days;

public:
    Rental(Car car, Customer customer, int days) : car(car), customer(customer), days(days) {}

    const Car& getCar() const {
        return car;
    }

    const Customer& getCustomer() const {
        return customer;
    }

    int getDays() const {
        return days;
    }
};

class CarRentalSystem {
private:
    vector<Car> cars;
    vector<Customer> customers;
    vector<Rental> rentals;

public:
    void addCar(const Car& car) {
        cars.push_back(car);
    }

    void addCustomer(const Customer& customer) {
        customers.push_back(customer);
    }

    void rentCar(Car &car, const Customer& customer, int days) {
        if (car.isAvailable()) {
            car.rent();
            rentals.push_back(Rental(car, customer, days));
        } else {
            cout << "Car is not available for rent" << endl;
        }
    }

    void returnCar(Car &car) {
        car.returnCar();
        auto it = remove_if(rentals.begin(), rentals.end(), [&car](const Rental& rental) {
            return rental.getCar().getCarId() == car.getCarId();
        });

        if (it != rentals.end()) {
            rentals.erase(it, rentals.end());
            cout << "Car returned successfully." << endl;
        } else {
            cout << "Car was not rented" << endl;
        }
    }

    void menu() {
        int choice;
        while (true) {
            cout << "===== Car Rental System =====" << endl;
            cout << "1. Rent a Car" << endl;
            cout << "2. Return a Car" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume newline

            if (choice == 1) {
                string customerName;
                cout << "\n== Rent a Car ==\n" << endl;
                cout << "Enter your name: ";
                getline(cin, customerName);

                cout << "\nAvailable Cars:" << endl;
                for (const auto &car : cars) {
                    if (car.isAvailable()) {
                        cout << car.getCarId() << " - " << car.getBrand() << " " << car.getModel() << endl;
                    }
                }

                string carId;
                int rentalDays;
                cout << "\nEnter the car ID you want to rent: ";
                getline(cin, carId);

                cout << "Enter the number of days for rental: ";
                cin >> rentalDays;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume newline

                Customer newCustomer("CUS" + to_string(customers.size() + 1), customerName);
                addCustomer(newCustomer);

                Car *selectedCar = nullptr;
                for (auto &car : cars) {
                    if (car.getCarId() == carId && car.isAvailable()) {
                        selectedCar = &car;
                        break;
                    }
                }

                if (selectedCar) {
                    double totalPrice = selectedCar->calculatePrice(rentalDays);
                    cout << "\n== Rental Information ==\n" << endl;
                    cout << "Customer ID: " << newCustomer.getCustomerId() << endl;
                    cout << "Customer Name: " << newCustomer.getName() << endl;
                    cout << "Car: " << selectedCar->getBrand() << " " << selectedCar->getModel() << endl;
                    cout << "Rental Days: " << rentalDays << endl;
                    printf("Total Price: $%.2f\n", totalPrice);

                    string confirm;
                    cout << "\nConfirm rental (Y/N): ";
                    getline(cin, confirm);

                    if (confirm == "Y" || confirm == "y") {
                        rentCar(*selectedCar, newCustomer, rentalDays);
                        cout << "\nCar rented successfully." << endl;
                    } else {
                        cout << "\nRental canceled." << endl;
                    }
                } else {
                    cout << "\nInvalid car selection or car not available for rent." << endl;
                }
            } else if (choice == 2) {
                string carId;
                cout << "\n== Return a Car ==\n" << endl;
                cout << "Enter the car ID you want to return: ";
                getline(cin, carId);

                Car *carToReturn = nullptr;
                for (auto &car : cars) {
                    if (car.getCarId() == carId && !car.isAvailable()) {
                        carToReturn = &car;
                        break;
                    }
                }

                if (carToReturn) {
                    Customer *customer = nullptr;
                    for (const auto &rental : rentals) {
                        if (rental.getCar().getCarId() == carToReturn->getCarId()) {
                            customer = new Customer(rental.getCustomer());
                            break;
                        }
                    }

                    if (customer) {
                        returnCar(*carToReturn);
                        cout << "Car returned successfully by " << customer->getName() << endl;
                        delete customer;
                    } else {
                        cout << "Car was not rented or rental information is missing." << endl;
                    }
                } else {
                    cout << "Invalid car ID or car is not rented." << endl;
                }
            } else if (choice == 3) {
                break;
            } else {
                cout << "Invalid choice. Please enter a valid option." << endl;
            }
        }

        cout << "\nThank you for using the Car Rental System!" << endl;
    }
};

int main() {
    CarRentalSystem rentalSystem;

    Car car1("C001", "Toyota", "Camry", 60.0);
    Car car2("C002", "Honda", "Accord", 70.0);
    Car car3("C003", "Mahindra", "Thar", 150.0);
    rentalSystem.addCar(car1);
    rentalSystem.addCar(car2);
    rentalSystem.addCar(car3);

    rentalSystem.menu();

    return 0;
}
