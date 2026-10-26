#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <ostream>
#include <string>
#include <vector>
#include <fstream>


/*
"Real receipt" refers to the receipt that is printed to the console for the customer to see.

"Receipt file" refers to the receipt that is written to the "receipt.txt" file for record-keeping purposes.

*/




//structure for order item
struct OrderItem {
    char code;
    int quantity;
    double unitPrice;
    double total;
    std::string name;
};
//structure for item summary
struct ItemSummary {
    std::string name;
    int quantity = 0;
    double sales = 0.0;
};

//definition for item code function
int getItemIndex(char code) {
    switch (code) {
    case 'W': return 0;
    case 'L': return 1;
    case 'S': return 2;
    case 'D': return 3;
    default: return -1;
    }
}

// Prints one drink row in the menu.
void printDrinkRow(const std::string& name, double smallPrice, double mediumPrice,
    double largePrice, double xxlPrice) {
    std::cout << std::setw(12) << name << std::setw(10) << "$" << std::setw(5) << smallPrice
        << std::setw(10) << "$" << std::setw(5) << mediumPrice << std::setw(10) << "$"
        << std::setw(5) << largePrice << std::setw(5) << "$" << std::setw(5) << xxlPrice << '\n';
}

// Reads the four drink names and their prices from the input file.
void readCatalog(std::ifstream& file, std::array<std::string, 4>& names,
    std::array<std::array<double, 4>, 4>& prices) {
    for (std::string& name : names) {
        std::getline(file, name);
    }

    for (std::array<double, 4>& drinkPrices : prices) {
        file >> drinkPrices[0] >> drinkPrices[1] >> drinkPrices[2] >> drinkPrices[3];
    }
}

// Gets a drink's price using its code and size.
double getItemPrice(char itemCode, const std::string& itemSize,
    const std::array<std::array<double, 4>, 4>& prices) {
    const int itemIndex = getItemIndex(itemCode);
    int sizeIndex = -1;

    if (itemSize == "S") sizeIndex = 0;
    else if (itemSize == "M") sizeIndex = 1;
    else if (itemSize == "L") sizeIndex = 2;
    else if (itemSize == "XXL") sizeIndex = 3;

    if (itemIndex == -1 || sizeIndex == -1) {
        return 0.0;
    }

    return prices[itemIndex][sizeIndex];
}

// Prints the drink menu and its prices.
void displayMenu(const std::array<std::string, 4>& names,
    const std::array<std::array<double, 4>, 4>& prices) {
    std::cout << "--------------------- Menu ---------------------\n";
    std::cout << std::setw(12) << "Drinks" << std::setw(15) << "Small(S)" << std::setw(15)
        << "Medium(M)" << std::setw(15) << "Large(L)" << std::setw(10) << "XXL\n";
    std::cout << std::fixed << std::setprecision(2);

    printDrinkRow(names[0] + " (W)", prices[0][0], prices[0][1], prices[0][2], prices[0][3]);
    printDrinkRow(names[1] + " (L)", prices[1][0], prices[1][1], prices[1][2], prices[1][3]);
    printDrinkRow(names[2] + " (S)", prices[2][0], prices[2][1], prices[2][2], prices[2][3]);
    printDrinkRow("McSprite (D)", prices[3][0], prices[3][1], prices[3][2], prices[3][3]);
    std::cout << '\n';
}
//structure for the receipt totals
struct CheckoutTotals {
    double discountedPrice;
    double stateTax;
    double countyTax;
    double municipalTax;
    double taxedTotal;
};

// Calculates the member discount and taxes for an order.
CheckoutTotals calculateCheckoutTotals(double totalPrice, char isMember) {
    CheckoutTotals totals;
    if (isMember == 'y' || isMember == 'Y') {
        totals.discountedPrice = totalPrice * 0.90;
    }
    else {
        totals.discountedPrice = totalPrice;
    }

    totals.stateTax = totals.discountedPrice * 0.065;
    totals.countyTax = totals.discountedPrice * 0.005;
    totals.municipalTax = totals.discountedPrice * 0.02125;
    totals.taxedTotal = totals.discountedPrice + totals.stateTax +
        totals.countyTax + totals.municipalTax;
    return totals;
}

// Asks for a tip and calculates the amount for the selected option.
double getTipAmount(double taxedTotal) {
    int tip;
    double tipAmount = 0.0;
    std::cout << "\nHow much would you like to tip?\n";
    std::cout << "1. 10%, 2. 15% 3. 25% 4. Custom Amount\n";
    std::cin >> tip;

    switch (tip) {
    case 1:
        tipAmount = taxedTotal * 0.10;
        break;
    case 2:
        tipAmount = taxedTotal * 0.15;
        break;
    case 3:
        tipAmount = taxedTotal * 0.25;
        break;
    case 4:
        std::cout << "Enter custom tip amount: $";
        std::cin >> tipAmount;
        break;
    default:
        std::cout << "Invalid number. No tip will be added.\n";
        break;
    }
    return tipAmount;
}

// Prints a customer's receipt to the selected output. ( file or console)
void printCustomerReceipt(std::ostream& output, const std::string& customerName,
    const std::vector<OrderItem>& orderItems, char isMember,
    const CheckoutTotals& totals, double tipAmount, const std::string& note) {
    output << "\n-------------------- Store Receipt --------------------\n";
    output << "Customer: " << customerName << '\n';
    output << std::setw(24) << "Item" << std::setw(10) << "Qty"
        << std::setw(15) << "Unit Price" << std::setw(15) << "Total" << '\n';

    for (const OrderItem& item : orderItems) {
        output << std::setw(24) << item.name << std::setw(10) << item.quantity
            << std::setw(14) << "$" << std::fixed << std::setprecision(2)
            << item.unitPrice << std::setw(5) << "$" << item.total << '\n';
    }

    if (isMember == 'y' || isMember == 'Y') {
        output << "Member discount applied: 10%\n";
        output << "Discounted price: $" << totals.discountedPrice << '\n';
    }
    output << "State tax: $" << totals.stateTax << '\n';
    output << "County tax: $" << totals.countyTax << '\n';
    output << "Conway City tax: $" << totals.municipalTax << '\n';
    output << "Total after taxes: $" << totals.taxedTotal << '\n';
    output << "Tip: $" << tipAmount << '\n';
    output << "Total with tip: $" << totals.taxedTotal + tipAmount << '\n';

    const int loyaltyPoints = static_cast<int>(totals.taxedTotal / 3);
    const std::string asterisks(loyaltyPoints, '*');
    output << "Loyalty Points : " << asterisks << '\n';
    output << "Cashier Note: " << note << '\n';
    output << "--------------------- End of Receipt ---------------------\n";
}

// Prints the sales and inventory summary to the selected output. (file or console)
void printSalesSummary(std::ostream& output,
    const std::array<ItemSummary, 4>& itemSummaries,
    const std::string& customerNameAccumulator, int totalCustomers,
    bool isReceiptFile) {
    std::string itemCountAccumulator;
    for (const ItemSummary& summary : itemSummaries) {
        itemCountAccumulator += summary.name + ": " +
            std::to_string(summary.quantity) + "\n";
    }

    output << "\n-------------------- Sales Summary --------------------\n";
    output << "Customers (" << totalCustomers << "):\n" << customerNameAccumulator;
    output << "Total quantity sold by item:\n" << itemCountAccumulator;
    output << "Total sales by item:\n";


    // Keep the existing extra heading in the receipt file.
    if (isReceiptFile) {
        output << "Total sales by item:\n";
    }

    double totalSales = 0.0;
    for (const ItemSummary& summary : itemSummaries) {
        output << summary.name << ": $" << std::fixed << std::setprecision(2)
            << summary.sales << '\n';
        totalSales += summary.sales;
    }

    output << "Total sales: $" << std::fixed << std::setprecision(2) << totalSales << '\n';
    output << "Inventory:\n";
    output << std::setw(24) << "Item" << std::setw(10) << "Start"
        << std::setw(10) << "Change" << std::setw(10) << "End" << '\n';
    for (const ItemSummary& summary : itemSummaries) {
        output << std::setw(24) << summary.name << std::setw(10) << 50
            << std::setw(10) << -summary.quantity << std::setw(10)
            << 50 - summary.quantity << '\n';
    }
    output << "-------------------------------------------------------\n";
}






//start of main function
int main() {


//opening the output file for receipt
    std::ofstream receipt_fout;
    receipt_fout.open("receipt.txt");

    //opening the input file for reading prices and item names
    std::ifstream Lab6_fin;
    Lab6_fin.open("Lab6.txt");


//makes sure the file is open
    if (!Lab6_fin) {
        std::cout << "Could not open Lab6.txt.\n";
        return 1;
    }


    std::array<std::string, 4> itemNames;
    std::array<std::array<double, 4>, 4> itemPrices;


    // Load the drink names and prices from the input file.
    readCatalog(Lab6_fin, itemNames, itemPrices);

    std::array<ItemSummary, 4> itemSummaries;
    for (int i = 0; i < 4; ++i) {
        itemSummaries[i].name = itemNames[i];
    }


    std::string customerNameAccumulator;
    int totalCustomers = 0;
    char addAnotherCustomer = 'Y';

    while (addAnotherCustomer == 'Y' || addAnotherCustomer == 'y') {
        std::string customerName;
        std::cout << "Enter customer name: ";
        std::getline(std::cin >> std::ws, customerName);
        customerNameAccumulator += customerName + "\n";
        ++totalCustomers;

        // Show the available drinks and prices.
        displayMenu(itemNames, itemPrices);

        char isMember = 'n';
        std::cout << "Is the customer a member? (y for yes, n for no): ";
        std::cin >> isMember;

        std::vector<OrderItem> orderItems;
        double totalPrice = 0.0;
        char checkout = 'N';
        while (checkout == 'N' || checkout == 'n') {
            char itemCode;
            std::string itemSize;
            int itemQuantity;

            std::cout << "Please make your selection using the item codes (W, L, S, D): ";
            std::cin >> itemCode;
            if (itemCode >= 'a' && itemCode <= 'z') {
                itemCode = static_cast<char>(itemCode - 'a' + 'A');
            }

            std::cout << "Please choose your size (S, M, L, XXL): ";
            std::cin >> itemSize;
            for (char& character : itemSize) {
                if (character >= 'a' && character <= 'z') {
                    character = static_cast<char>(character - 'a' + 'A');
                }
            }

            std::cout << "Enter the quantity of the item: ";
            std::cin >> itemQuantity;

            const int itemIndex = getItemIndex(itemCode);

            // Get the price for the selected drink and size.
            double itemPrice = getItemPrice(itemCode, itemSize, itemPrices);

            //input validation for item code, size, and quantity
            if (itemIndex == -1 || itemPrice == 0.0 || itemQuantity <= 0) {
                std::cout << "Invalid item, size, or quantity. Please enter a valid selection.\n";
                continue;
            }

            const double itemTotal = itemQuantity * itemPrice;
            totalPrice += itemTotal;
            orderItems.push_back({ itemCode, itemQuantity, itemPrice, itemTotal, itemNames[itemIndex] });

            std::cout << "Would you like to checkout this customer's order? (Y or N): ";
            std::cin >> checkout;
        }

        std::string note;
        std::cout << "Enter Cashier Note: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, note);

        // Calculate the member discount and taxes for the order.
        const CheckoutTotals checkoutTotals = calculateCheckoutTotals(totalPrice, isMember);
        const double salesDiscountMultiplier =
            (isMember == 'y' || isMember == 'Y') ? 0.90 : 1.0;
        for (const OrderItem& item : orderItems) {
            const int itemIndex = getItemIndex(item.code);
            itemSummaries[itemIndex].quantity += item.quantity;
            itemSummaries[itemIndex].sales += item.total * salesDiscountMultiplier;
        }
        // Ask for a tip based on the order total after taxes.
        const double tipAmount = getTipAmount(checkoutTotals.taxedTotal);

        // Print the receipt on the console.
        printCustomerReceipt(std::cout, customerName, orderItems, isMember,
            checkoutTotals, tipAmount, note);
        // Write the same receipt to the output file.
        printCustomerReceipt(receipt_fout, customerName, orderItems, isMember,
            checkoutTotals, tipAmount, note);





        std::cout << "\nAdd another customer? (Y or N): ";
        std::cin >> addAnotherCustomer;
    }

    // Print the final sales and inventory summary on the console.
    printSalesSummary(std::cout, itemSummaries, customerNameAccumulator,
        totalCustomers, false);
    // Write the final sales and inventory summary to the output file.
    printSalesSummary(receipt_fout, itemSummaries, customerNameAccumulator,
        totalCustomers, true);
    return 0;
}