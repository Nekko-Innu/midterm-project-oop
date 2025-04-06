//Jhon Clive P. Buenafe, IT2A, Midterm pt2
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <limits>
#include <iomanip>
using namespace std;

// Nagva-validate kung tama ang pangalan (letters at spaces lang)
bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char ch : name) {
        if (!isalpha(ch) && ch != ' ') {
            return false;
        }
    }
    return true;
}

// Nagche-check kung lahat ng character ay digits
bool isDigits(const string& str) {
    if (str.empty()) return false;
    for (char ch : str) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}

// Ginagawang capital ang unang letter tapos small ang ibang letters
string capitalize(string input) {
    if (!input.empty()) {
        input[0] = toupper(input[0]);
        for (size_t i = 1; i < input.size(); ++i) {
            input[i] = tolower(input[i]);
        }
    }
    return input;
}

// Nililinis ang input buffer
void clearBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Nagpapahintay sa user bago ipagpatuloy
void pressToContinue() {
    cout << "Press Enter to Continue...";
    clearBuffer();
}

// Class para sa items ng inventory
class Item {
private:
    int productID;
    string name;
    int quantity;
    double price;
    string category;

public:
    // Constructor para sa bagong gawang Item
    Item(int id, string n, int q, double p, string c)
    : productID(id), name(capitalize(n)), quantity(q), 
      price(p), category(capitalize(c)) {}

    // Getter functions para makuha ang private data ni Item
    int getID() const { return productID; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }

    // Setter para i-update ang quantity at price ni Item
    void setQuantity(int q) { quantity = q; }
    void setPrice(double p) { price = p; }
};

// Class para sa buong Inventory
class Inventory {
private:
    vector<Item> items;
    int nextID = 1;

    // Manual na pag-sort by quantity
    void sortByQuantity(bool ascending) {   //true = ascending, false = descending.
        for (size_t i = 0; i < items.size(); i++) {     //bubble sort, i = number of passes, guamit ng size_t para ma avoid ang negatives maganda sa size at counting.
            for (size_t j = 0; j < items.size()-i-1; j++) {     //j = current item na chinecheck sa pass, -i-1 malaki na si i so di na sya kailangan na chinecheck
                bool swapNeeded = ascending ? 
                    (items[j].getQuantity() > items[j+1].getQuantity()):    //if ascending, check if quantity of item j ay greater than quantity of item j+1 yung next item.
                    (items[j].getQuantity() < items[j+1].getQuantity());    //if descending, check if quantity of item j is less than quantity of item j+1 yung next item.
                
                if (swapNeeded) { 
                    Item temp = items[j];
                    items[j] = items[j+1];
                    items[j+1] = temp;
                }
            }
        }
    }

    // Manual na pag-sort by price
    void sortByPrice(bool ascending) {
        for (size_t i = 0; i < items.size(); i++) {
            for (size_t j = 0; j < items.size()-i-1; j++) {
                bool swapNeeded = ascending ? 
                    (items[j].getPrice() > items[j+1].getPrice()) :
                    (items[j].getPrice() < items[j+1].getPrice());
                
                if (swapNeeded) {
                    Item temp = items[j];
                    items[j] = items[j+1];
                    items[j+1] = temp;
                }
            }
        }
    }

public:
    //Nagdadagdag ng bagong item sa inventory
    void addItem(string name, int quantity, double price, string category) {
        Item newItem(nextID++, name, quantity, price, category);
        items.push_back(newItem);
        cout << "\nItem added successfully!\n";
    }

    //Ina-update ang quantity o price ng isang item
    void updateItem(int id, char field, double newValue) {
        bool found = false; // Check para malaman kung nahanap yung item, kung hindi, mag print na "Item not found!"
        for (vector<Item>::iterator it = items.begin(); it != items.end(); ++it) {
            if (it->getID() == id) { // Kung match yung ID ng item
                found = true;
                string fieldName = (field == 'q') ? "Quantity" : "Price"; // Alamin kung anong field ang i-uupdate: quantity or price

                string oldValue = (field == 'q') ? to_string(it->getQuantity()) // Kunin ang dating value bago mag-update
                : to_string(it->getPrice());
                
                if (field == 'q') {
                    it->setQuantity(newValue); //Kapag (q)uantity, i-update yung quantity
                } else {
                    it->setPrice(newValue); //Kapag (p)rice, i-update yung price
                }
                
                cout << "\n" << fieldName << " of Item " << it->getName() //Pag ok na mag-update, mag print na ang name ng item
                     << " updated from " << oldValue 
                     << " to " << newValue << "\n";
                break;
            }
        }
        if (!found) cout << "\nItem not found!\n";
    }

    //Tinatanggal ang item based sa ID
    void removeItem(int id) {
        bool found = false; // Check para malaman kung nahanap yung item, kung hindi, mag print na "Item not found!"
        for (size_t i = 0; i < items.size(); i++) { 
            if (items[i].getID() == id) { // Kung match yung ID ng item
                cout << "\nItem " << items[i].getName() << " removed from inventory\n"; //Print yung tatanggaling item
                items.erase(items.begin() + i); // Tinatanggal yung item
                found = true; 
                break; 
            }
        }
        if (!found) cout << "\nItem not found!\n";
    }

    //Display lahat ng item sa inventory
    void displayAll() const {
        cout << "\nCurrent Inventory:\n";
        cout << "====================================================================\n";
        cout << "|ID\t|Name\t\t|Quantity\t|Price\t\t|Category\n";
        cout << "====================================================================\n";
        
        for (size_t i = 0; i < items.size(); i++) {
            cout << items[i].getID() << "\t" 
                 << items[i].getName() << "\t\t"
                 << items[i].getQuantity() << "\t\t"
                 << fixed << setprecision(2) << items[i].getPrice() << "\t\t" 
                 //Fixed para mag print ng decimal, setprecision(2) para mag print ng 2 decimal, eto lang gamit sa header ni iomanip
                 << items[i].getCategory() << "\n";
        }
        cout << "====================================================================\n";
    }

    //Hahanap ng item gamit ang ID
    void searchItem(int id) const {
        bool found = false; // Check para malaman kung nahanap yung item, kung hindi, mag print na "Item not found!"
        for (size_t i = 0; i < items.size(); i++) { 
            if (items[i].getID() == id) { // Kung match yung ID ng item, print yung nahanap na item.
                cout << "\nItem found:\n";
                cout << "ID: " << items[i].getID() << "\n"
                     << "Name: " << items[i].getName() << "\n"
                     << "Quantity: " << items[i].getQuantity() << "\n"
                     << "Price: " << items[i].getPrice() << "\n"
                     << "Category: " << items[i].getCategory() << "\n";
                found = true;
                break;
            }
        }
        if (!found) cout << "\nItem not found!\n";
    }

    // Sinasagawa ang sorting batay sa field at order
    void sortItems(char field, bool ascending) { //Kapag (q)uantity, mag sort by quantity, kapag (p)rice, mag sort by price
        if (field == 'q') { //Kapag (q)uantity, else mag sort by (p)rice
            sortByQuantity(ascending);
        } else {
            sortByPrice(ascending);
        }
        cout << "\nInventory sorted!\n";
        displayAll();
    }

    // Ipinapakita ang items na mababa ang stock (<=5)
    void displayLowStock() const { //const pa di pwede mag update or baguhin yung inventory
        cout << "\nLow Stock Items (<=5):\n";
        cout << "====================================================================\n";
        cout << "|ID\t|Name\t\t|Quantity\t|Price\t\t|Category\n";
        cout << "====================================================================\n";
        
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i].getQuantity() <= 5) { //Check yung quantity ng item, kung <=5, print yung item
                cout << items[i].getID() << "\t" 
                     << items[i].getName() << "\t\t"
                     << items[i].getQuantity() << "\t\t"
                     << fixed << setprecision(2) << items[i].getPrice() << "\t\t" 
                     << items[i].getCategory() << "\n";
            }
        }
        cout << "====================================================================\n";
    }
};

// Kumukuha ng input na validated (optional)
string getInput(const string& prompt, bool (*validator)(const string&) = nullptr) { // validator - Optional function pointer para i-check kung valid ang input
    string input; // Variable na magho-hold ng user input
    while(true) {
        cout << prompt;
        getline(cin, input);
        if (input.empty()) { // Kung walang laman ang input, Error message, Balik sa umpisa ng loop
            cout << "Error: Input cannot be empty!\n";
            continue;
        }
        if (validator && !validator(input)) { //Check if valid input pag hindi, error message, ulit ng loop
            cout << "Error: Invalid input format!\n";
            continue;
        }
        break; //Pag valid yung input ni user, exit loop
    }
    return input; //return ang input ni user
}

//Check ng integer input mula sa user
int getIntInput(const string& prompt) {
    string input = getInput(prompt, isDigits); // Kukunin ang input, dapat puro digits lang
    return stoi(input); // Convert string to integer gamit ang stoi (string to int)
}

//Check ng double input mula sa user
double getDoubleInput(const string& prompt) { //getInput na may custom validator para sa double values
    string input = getInput(prompt, [](const string& str) {
        bool hasDot = false; //Check para sa decimal point
        for (char c : str) {
            if (!isdigit(c)) {
                if (c == '.' && !hasDot) {
                    hasDot = true; //Pwede ang 1st decimal point
                } else {
                    return false; //Ivalid pag more than 1 decimal point or ibang char.
                }
            }
        }
        return true; // Valid kung digits lang at 1 decimal point
    });
    return stod(input); // Convert string to double gamit ang stod (string to double)
}

// Ipinapakita ang menu ng programa
void displayMainMenu() { 
    cout << "\n============================";
    cout << "\nInventory Management System";
    cout << "\n============================";
    cout << "\n========== Menu ============\n\n";
    cout << "1 - Add Item\n";
    cout << "2 - Update Item\n";
    cout << "3 - Remove Item\n";
    cout << "4 - Display All Items\n";
    cout << "5 - Search Item\n";
    cout << "6 - Sort Items\n";
    cout << "7 - Display Low Stock Items\n";
    cout << "8 - Exit\n";
    cout << "============================\n";
}

// Main function: dito nagaganap ang user interaction
int main() {
    Inventory inventory; // Inventory object

    while(true) { // Main loop para paulit-ulit ang menu hangga't hindi nag-e-exit
        displayMainMenu(); //Call ang main menu
        int choice = getIntInput("Enter choice (1-8): "); 
        bool skipPress = false; //Ginagamit para i-skip ang "Press any key" prompt

        switch (choice) {
            case 1: {
                char another;
                do {
                    string name = getInput("Item name: ", isValidName);
                    int qty = getIntInput("Quantity: ");
                    double price = getDoubleInput("Price: ");
                    string category = getInput("Category: ");
                    inventory.addItem(name, qty, price, category);

                    cout << "Add another item (y/n)? ";
                    cin >> another;
                    clearBuffer();
                } while (another == 'y' || another == 'Y');
                skipPress = true;
                break;
            }
            case 2: {
                char another;
                do {
                    int id = getIntInput("Enter ID: ");
                    char field;
                    cout << "Update (q)uantity or (p)rice? ";
                    cin >> field;
                    clearBuffer();
                    
                    if (field == 'q' || field == 'p') {
                        double value = (field == 'q') ? 
                            getIntInput("New quantity: ") :
                            getDoubleInput("New price: ");
                        inventory.updateItem(id, field, value);
                    } else {
                        cout << "Invalid choice!\n";
                    }

                    cout << "Update another item (y/n)? ";
                    cin >> another;
                    clearBuffer();
                } while (another == 'y' || another == 'Y');
                skipPress = true;
                break;
            }
            case 3: {
                char another;
                do {
                    int id = getIntInput("Enter ID to remove: ");
                    inventory.removeItem(id);

                    cout << "Remove another item (y/n)? ";
                    cin >> another;
                    clearBuffer();
                } while (another == 'y' || another == 'Y');
                skipPress = true;
                break;
            }
            case 4:
                inventory.displayAll();
                break;
            case 5: {
                char another;
                do {
                    int id = getIntInput("Enter ID to search: ");
                    inventory.searchItem(id);

                    cout << "Search another item (y/n)? ";
                    cin >> another;
                    clearBuffer();
                } while (another == 'y' || another == 'Y');
                skipPress = true;
                break;
            }
            case 6: {
                char field;
                cout << "Sort by (q)uantity or (p)rice? ";
                cin >> field;
                clearBuffer();
                
                char order;
                cout << "(a)scending or (d)escending? ";
                cin >> order;
                clearBuffer();
                
                inventory.sortItems(field, (order == 'a'));
                break;
            }
            case 7:
                inventory.displayLowStock();
                break;
            case 8:
                cout << "\nExiting...\n";
                return 0;
            default:
                cout << "Invalid choice!\n";
        }

        if (!skipPress) {
            pressToContinue();
        }
    }
}