#include <iostream>
#include <fstream>
#include <regex>
#include <typeinfo>
#include <limits>
#include <algorithm>
#include "json/json.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;

Json::Value getCarRecords() {
	Json::Value root;
	std::ifstream file("cars.json");
	if (!file.is_open()) {
		cout << "Warning: 'cars.json' not found, starting with empty car list." << endl;
		return Json::Value(Json::arrayValue);
	}
	try {
		file >> root;
	} catch (...) {
		cout << "Error reading 'cars.json' - invalid format." << endl;
	}
	file.close();
	return root;
}

void setCarRecords(Json::Value root) {
	std::ofstream outFile("cars.json");
	outFile << root;
	outFile.close();
}

string getName() {
	string str;
	cout << "Enter Car's Name: ";
	getline(cin, str);
	if(str.empty()) {
		cout << "Enter a name!" << endl;
		return getName(); // Fixed: added return
	}
	return str;
}

int carOptions() {
	cout << "1. Car Enquiry" << endl;
	cout << "2. New Car" << endl;
	cout << "3. Modify Car" << endl;
	cout << "4. Remove Car" << endl;
	cout << "5. Show All Cars" << endl;
	cout << "6. Rent a Car" << endl;
	cout << "7. Change Rent Records" << endl;
	cout << "8. Show Rent Records" << endl;
	cout << "0. Exit" << endl;
	cout << "--- Choose any one option ---" << endl;

	int selectedOption = -1;
	while (true) {
		cout << "Enter one option: ";
		if (cin >> selectedOption && selectedOption >= 0 && selectedOption <= 8)
			break;
		cout << "Invalid input. Please enter a valid number between 0 and 8." << endl;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear leftover newline
	return selectedOption;
}

void checkCar() {
	cout << "--- Check Car ---" << endl;
	string str = getName();
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value list = getCarRecords();
	int len = list.size();

	for (int i = 0; i < len; i++) {
		string name = list[i]["name"].asString();
		string carName = name;
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		if (name == str) {
			string model = list[i]["model"].asString();
			string carNum = list[i]["carNumber"].asString();
			string rentPrice = list[i]["rentPrice"].asString();
			cout << "Car Name: " << carName << endl;
			cout << "Car Number: " << carNum << endl;
			cout << "Model: " << model << endl;
			cout << "Rent Price: $" << rentPrice << endl;
			return;
		}
	}
	cout << "Record Not Found." << endl;
}

void addCar() {
	string carName, carNumber, id, model, rentPrice;
	Json::Value records = getCarRecords();
	int numOfCars = records.size();
	id = std::to_string(numOfCars);
	carNumber = "C100" + std::to_string(numOfCars);

	cout << endl << "--- Provide Car Details ---" << endl;
	cout << "Car Name: ";
	getline(cin, carName);
	cout << "Car Model: ";
	getline(cin, model);
	cout << "Rent Price: ";
	getline(cin, rentPrice);

	Json::Value record;
	record["id"] = id;
	record["name"] = carName;
	record["carNumber"] = carNumber;
	record["model"] = model;
	record["rentPrice"] = rentPrice;

	records.append(record);
	setCarRecords(records);

	cout << carName << " Car added. " << endl;
	cout << "Car Number: " << carNumber << endl;
}

void updateCar() {
	cout << endl << "--- Update Car ---" << endl;
	string str = getName();
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value records = getCarRecords();
	int len = records.size();
	bool found = false;

	for (int i = 0; i < len; i++) {
		string value = records[i]["name"].asString();
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);
		if (str == value) {
			string name, model, rentPrice;
			cout << "New Name: ";
			getline(cin, name);
			cout << "Car Model: ";
			getline(cin, model);
			cout << "Rent Price: ";
			getline(cin, rentPrice);

			records[i]["name"] = name;
			records[i]["model"] = model;
			records[i]["rentPrice"] = rentPrice;

			found = true;
			break;
		}
	}
	if (found) {
		setCarRecords(records);
		cout << "Updated." << endl;
	} else {
		cout << "Record Not Found." << endl;
	}
}

void removeCar() {
	string carNum;
	cout << endl << "--- Remove Car ---" << endl;
	cout << "Car's Number to remove: ";
	getline(cin, carNum);
	std::transform(carNum.begin(), carNum.end(), carNum.begin(), ::tolower);

	Json::Value records = getCarRecords();
	Json::Value newRecords;
	bool carExists = false;

	for (int i = 0; i < records.size(); i++) {
		string value = records[i]["carNumber"].asString();
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);
		if (carNum == value) {
			carExists = true;
			continue;
		}
		newRecords.append(records[i]);
	}

	if (carExists) {
		setCarRecords(newRecords);
		cout << "Car Removed" << endl;
	} else {
		cout << "Car does not exist" << endl;
	}
}

void showCars() {
	cout << "--- List of Cars ---" << endl;
	Json::Value list = getCarRecords();
	cout << "No. | Car Name       | Car No. | Model | Rent Price (per day)" << endl;
	for (int i = 0; i < list.size(); i++) {
		string name = list[i]["name"].asString();
		string num = list[i]["carNumber"].asString();
		string model = list[i]["model"].asString();
		string rentPrice = list[i]["rentPrice"].asString();

		int padding = 14 - name.length();
		if (padding > 0) name += string(padding, ' ');

		cout << i + 1 << ".  " << name << "  " << num << "    " << model << "   $" << rentPrice << endl;
	}
}

// --- RENTAL SECTION ---
Json::Value getRentRecords() {
	Json::Value root;
	std::ifstream file("rents.json");
	if (!file.is_open()) {
		cout << "Warning: 'rents.json' not found, starting with empty rent records." << endl;
		return Json::Value(Json::arrayValue);
	}
	try {
		file >> root;
	} catch (...) {
		cout << "Error reading 'rents.json' - invalid format." << endl;
	}
	file.close();
	return root;
}

void setRentRecords(Json::Value root) {
	std::ofstream outFile("rents.json");
	outFile << root;
	outFile.close();
}

struct Renter {
	string name, phone, address;
};

void addRentRecord() {
	string carNumber, rentStartDate, rentEndDate, id;
	Renter r;

	Json::Value records = getRentRecords();
	id = std::to_string(records.size());

	cout << endl << "--- Provide Rent Details ---" << endl;
	cout << "Car Number: ";
	getline(cin, carNumber);
	cout << "Renter's Name: ";
	getline(cin, r.name);
	cout << "Renter's Phone: ";
	getline(cin, r.phone);
	cout << "Renter's Address: ";
	getline(cin, r.address);
	cout << "Rent Start Date: ";
	getline(cin, rentStartDate);
	cout << "Rent End Date: ";
	getline(cin, rentEndDate);

	Json::Value record;
	record["id"] = id;
	record["renter"]["name"] = r.name;
	record["renter"]["phone"] = r.phone;
	record["renter"]["address"] = r.address;
	record["car"]["carNumber"] = carNumber;
	record["rentStartDate"] = rentStartDate;
	record["rentEndDate"] = rentEndDate;

	records.append(record);
	setRentRecords(records);

	cout << endl << r.name << " rented a car. " << endl;
	cout << "Car Number: " << carNumber << endl;
}

void showRentRecords() {
	cout << "--- Rent Records ---" << endl;
	Json::Value list = getRentRecords();
	cout << "No. | Renter's Name     | Car No. | Rent End Date" << endl;
	for (int i = 0; i < list.size(); i++) {
		string name = list[i]["renter"]["name"].asString();
		string num = list[i]["car"]["carNumber"].asString();
		string rentEndDate = list[i]["rentEndDate"].asString();

		int padding = 18 - name.length();
		if (padding > 0) name += string(padding, ' ');

		cout << i + 1 << ".  " << name << "  " << num << "    " << rentEndDate << endl;
	}
}

void changeRentRecords() {
	string rentId, carNumber, rentStartDate, rentEndDate;
	Renter r;

	cout << endl << "--- Update Rent Details ---" << endl;
	cout << "Rent Id: ";
	getline(cin, rentId);
	std::transform(rentId.begin(), rentId.end(), rentId.begin(), ::tolower);

	Json::Value records = getRentRecords();
	for (int i = 0; i < records.size(); i++) {
		string id = records[i]["id"].asString();
		std::transform(id.begin(), id.end(), id.begin(), ::tolower);
		if (rentId == id) {
			cout << "--- Provide New Rent Details ---" << endl;
			cout << "New Car Number: ";
			getline(cin, carNumber);
			cout << "New Renter's Name: ";
			getline(cin, r.name);
			cout << "New Renter's Phone: ";
			getline(cin, r.phone);
			cout << "New Renter's Address: ";
			getline(cin, r.address);
			cout << "Rent Start Date: ";
			getline(cin, rentStartDate);
			cout << "Rent End Date: ";
			getline(cin, rentEndDate);

			records[i]["renter"]["name"] = r.name;
			records[i]["renter"]["phone"] = r.phone;
			records[i]["renter"]["address"] = r.address;
			records[i]["car"]["carNumber"] = carNumber;
			records[i]["rentStartDate"] = rentStartDate;
			records[i]["rentEndDate"] = rentEndDate;

			setRentRecords(records);
			cout << "Update Successful" << endl;
			return;
		}
	}
	cout << "Rent not found." << endl;
}

void actions(int& option) {
	switch(option) {
		case 1: checkCar(); break;
		case 2: addCar(); break;
		case 3: updateCar(); break;
		case 4: removeCar(); break;
		case 5: showCars(); break;
		case 6: addRentRecord(); break;
		case 7: changeRentRecords(); break;
		case 8: showRentRecords(); break;
	}
}

void home() {
	int option = carOptions();
	if (option == 0) exit(0);
	else actions(option);
}

int main() {
	cout << "*** WELCOME ***";
	string yesOrNo;
	while (true) {
		cout << endl << "--- Car Rental System ---" << endl;
		home();
		cout << endl << "Continue? (y/n): ";
		cin >> yesOrNo;
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (yesOrNo != "y" && yesOrNo != "Y") break;
	}
	cout << "Good Bye!" << endl;
}

