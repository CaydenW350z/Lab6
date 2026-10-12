#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <fstream>

struct OrderItem {
    char code;
    int quantity;
    double unitPrice;
    double total;
    std::string name;
};

struct ItemSummary {
    std::string name;
    int quantity = 0;
    double sales = 0.0;
};

int getItemIndex(char code) {
    switch (code) {
    case 'W': return 0;
    case 'L': return 1;
    case 'S': return 2;
    case 'D': return 3;
    default: return -1;
    }
}

int main() {



    std::ofstream receipt_fout;
    receipt_fout.open("receipt.txt");

    std::ifstream Lab6_fin;
    Lab6_fin.open("Lab6.txt");

    if (!Lab6_fin) {
        std::cout << "Could not open Lab6.txt.\n";
        return 1;
    }

    std::string waterName;
    std::string leanName;
    std::string sodaName;
    std::string spriteName;
    double waterSmallPrice;
    double waterMediumPrice;
    double waterLargePrice;
    double waterXXLPrice;
    double leanSmallPrice;
    double leanMediumPrice;
    double leanLargePrice;
    double leanXXLPrice;
    double sodaSmallPrice;
    double sodaMediumPrice;
    double sodaLargePrice;
    double sodaXXLPrice;
    double spriteSmallPrice;
    double spriteMediumPrice;
    double spriteLargePrice;
    double spriteXXLPrice;

    std::getline(Lab6_fin, waterName);
    std::getline(Lab6_fin, leanName);
    std::getline(Lab6_fin, sodaName);
    std::getline(Lab6_fin, spriteName);

    Lab6_fin >> waterSmallPrice >> waterMediumPrice >> waterLargePrice >> waterXXLPrice
        >> leanSmallPrice >> leanMediumPrice >> leanLargePrice >> leanXXLPrice
        >> sodaSmallPrice >> sodaMediumPrice >> sodaLargePrice >> sodaXXLPrice
        >> spriteSmallPrice >> spriteMediumPrice >> spriteLargePrice >> spriteXXLPrice;

    const std::array<std::string, 4> itemNames = {
        waterName, leanName, sodaName, spriteName
    };
    std::array<ItemSummary, 4> itemSummaries = { {
        {waterName}, {leanName}, {sodaName}, {spriteName}
    } };
    std::string customerNameAccumulator;
    int totalCustomers = 0;
    char addAnotherCustomer = 'Y';

    while (addAnotherCustomer == 'Y' || addAnotherCustomer == 'y') {
        std::string customerName;
        std::cout << "Enter customer name: ";
        std::getline(std::cin >> std::ws, customerName);
        customerNameAccumulator += customerName + "\n";
        ++totalCustomers;

        std::cout << "--------------------- Menu ---------------------\n";
        std::cout << std::setw(12) << "Drinks" << std::setw(15) << "Small(S)" << std::setw(15)
            << "Medium(M)" << std::setw(15) << "Large(L)" << std::setw(10) << "XXL\n";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::setw(12) << waterName + " (W)" << std::setw(10) << "$" << std::setw(5) << waterSmallPrice << std::setw(10) << "$" << std::setw(5)
            << waterMediumPrice << std::setw(10) << "$" << std::setw(5) << waterLargePrice << std::setw(5) << "$" << std::setw(5) << waterXXLPrice << '\n';
        std::cout << std::setw(12) << leanName + " (L)" << std::setw(10) << "$" << std::setw(5) << leanSmallPrice << std::setw(10) << "$" << std::setw(5)
            << leanMediumPrice << std::setw(10) << "$" << std::setw(5) << leanLargePrice << std::setw(5) << "$" << std::setw(5) << leanXXLPrice << '\n';
        std::cout << std::setw(12) << sodaName + " (S)" << std::setw(10) << "$" << std::setw(5) << sodaSmallPrice << std::setw(10) << "$" << std::setw(5)
            << sodaMediumPrice << std::setw(10) << "$" << std::setw(5) << sodaLargePrice << std::setw(5) << "$" << std::setw(5) << sodaXXLPrice << '\n';
        std::cout << std::setw(12) << "McSprite (D)" << std::setw(10) << "$" << std::setw(5) << spriteSmallPrice << std::setw(10) << "$" << std::setw(5)
            << spriteMediumPrice << std::setw(10) << "$" << std::setw(5) << spriteLargePrice << std::setw(5) << "$" << std::setw(5) << spriteXXLPrice << "\n\n";

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
            double itemPrice = 0.0;
            if (itemCode == 'W') {
                if (itemSize == "S") itemPrice = waterSmallPrice;
                else if (itemSize == "M") itemPrice = waterMediumPrice;
                else if (itemSize == "L") itemPrice = waterLargePrice;
                else if (itemSize == "XXL") itemPrice = waterXXLPrice;
            }
            else if (itemCode == 'L') {
                if (itemSize == "S") itemPrice = leanSmallPrice;
                else if (itemSize == "M") itemPrice = leanMediumPrice;
                else if (itemSize == "L") itemPrice = leanLargePrice;
                else if (itemSize == "XXL") itemPrice = leanXXLPrice;
            }
            else if (itemCode == 'S') {
                if (itemSize == "S") itemPrice = sodaSmallPrice;
                else if (itemSize == "M") itemPrice = sodaMediumPrice;
                else if (itemSize == "L") itemPrice = sodaLargePrice;
                else if (itemSize == "XXL") itemPrice = sodaXXLPrice;
            }
            else if (itemCode == 'D') {
                if (itemSize == "S") itemPrice = spriteSmallPrice;
                else if (itemSize == "M") itemPrice = spriteMediumPrice;
                else if (itemSize == "L") itemPrice = spriteLargePrice;
                else if (itemSize == "XXL") itemPrice = spriteXXLPrice;
            }
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

        double discountedPrice = totalPrice;
        double arkansasStateTax;
        double faulknerCountyTax;
        double conwayMunicipalTax;
        if (isMember == 'y' || isMember == 'Y') {
            discountedPrice = totalPrice * 0.90;
        }
        else {
            discountedPrice = totalPrice;
        }
        const double salesDiscountMultiplier =
            (isMember == 'y' || isMember == 'Y') ? 0.90 : 1.0;
        for (const OrderItem& item : orderItems) {
            const int itemIndex = getItemIndex(item.code);
            itemSummaries[itemIndex].quantity += item.quantity;
            itemSummaries[itemIndex].sales += item.total * salesDiscountMultiplier;
        }
        arkansasStateTax = discountedPrice * 0.065;
        faulknerCountyTax = discountedPrice * 0.005;
        conwayMunicipalTax = discountedPrice * 0.02125;
        const double taxedTotal = discountedPrice + arkansasStateTax +
            faulknerCountyTax + conwayMunicipalTax;

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
         //real receipt
        std::cout << "\n-------------------- Store Receipt --------------------\n";


        //file receipt
        receipt_fout << "\n-------------------- Store Receipt --------------------\n";
        

       //real receipt
        std::cout << "Customer: " << customerName << '\n';
        //file receipt
        receipt_fout << "Customer: " << customerName << '\n';

          //real receipt
          std::cout << std::setw(24) << "Item" << std::setw(10) << "Qty"
              << std::setw(15) << "Unit Price" << std::setw(15) << "Total" << '\n';
          receipt_fout << std::setw(24) << "Item" << std::setw(10) << "Qty"
              << std::setw(15) << "Unit Price" << std::setw(15) << "Total" << '\n';

          for (const OrderItem& item : orderItems) {
            std::cout << std::setw(24) << item.name << std::setw(10) << item.quantity
                << std::setw(14) << "$" << std::fixed << std::setprecision(2)
                << item.unitPrice << std::setw(5) << "$" << item.total << '\n';
                //receipt file
            receipt_fout << std::setw(24) << item.name << std::setw(10) << item.quantity
                << std::setw(14) << "$" << std::fixed << std::setprecision(2)
                << item.unitPrice << std::setw(5) << "$" << item.total << '\n';
        }

        if (isMember == 'y' || isMember == 'Y') {
            std::cout << "Member discount applied: 10%\n";
            std::cout << "Discounted price: $" << discountedPrice << '\n';

            //file receipt
            receipt_fout << "Member discount applied: 10%\n";
            receipt_fout << "Discounted price: $" << discountedPrice << '\n';
        }
        
        std::cout << "State tax: $" << arkansasStateTax << '\n';
        std::cout << "County tax: $" << faulknerCountyTax << '\n';
        std::cout << "Conway City tax: $" << conwayMunicipalTax << '\n';
        std::cout << "Total after taxes: $" << taxedTotal << '\n';
        std::cout << "Tip: $" << tipAmount << '\n';
        std::cout << "Total with tip: $" << taxedTotal + tipAmount << '\n';
        //receipt file
        receipt_fout << "State tax: $" << arkansasStateTax << '\n';
        receipt_fout << "County tax: $" << faulknerCountyTax << '\n';
        receipt_fout << "Conway City tax: $" << conwayMunicipalTax << '\n';
        receipt_fout << "Total after taxes: $" << taxedTotal << '\n';
        receipt_fout << "Tip: $" << tipAmount << '\n';
        receipt_fout << "Total with tip: $" << taxedTotal + tipAmount << '\n';
        //real receipt
        int loyaltyPoints = static_cast<int>(taxedTotal / 3);
        std::string asterisks(loyaltyPoints, '*');
        std::cout << "Loyalty Points : " << asterisks << '\n';
        std::cout << "Cashier Note: " << note << '\n';
        std::cout << "--------------------- End of Receipt ---------------------\n";
        //receipt file
        receipt_fout << "Loyalty Points : " << asterisks << '\n';
        receipt_fout << "Cashier Note: " << note << '\n';
        receipt_fout << "--------------------- End of Receipt ---------------------\n";





        std::cout << "\nAdd another customer? (Y or N): ";
        std::cin >> addAnotherCustomer;
    }

    std::string itemCountAccumulator;
    
for (const ItemSummary& summary : itemSummaries) {
        itemCountAccumulator += summary.name + ": " +
            std::to_string(summary.quantity) + "\n";
    }
//real receipt
    std::cout << "\n-------------------- Sales Summary --------------------\n";
    std::cout << "Customers (" << totalCustomers << "):\n" << customerNameAccumulator;
    std::cout << "Total quantity sold by item:\n" << itemCountAccumulator;
    std::cout << "Total sales by item:\n";
    double totalSales = 0.0;
//receipt file
    receipt_fout << "\n-------------------- Sales Summary --------------------\n";
    receipt_fout << "Customers (" << totalCustomers << "):\n" << customerNameAccumulator;
    receipt_fout << "Total quantity sold by item:\n" << itemCountAccumulator;
    receipt_fout << "Total sales by item:\n";


    for (const ItemSummary& summary : itemSummaries) {
        std::cout << summary.name << ": $" << std::fixed << std::setprecision(2)
            << summary.sales << '\n';
        totalSales += summary.sales;
    }
    std::cout << "Total sales: $" << std::fixed << std::setprecision(2) << totalSales << '\n';
    std::cout << "Inventory:\n";
    std::cout << std::setw(24) << "Item" << std::setw(10) << "Start"
        << std::setw(10) << "Change" << std::setw(10) << "End" << '\n';
    std::cout << std::setw(24) << waterName << std::setw(10) << 50
        << std::setw(10) << -itemSummaries[0].quantity << std::setw(10) << 50 - itemSummaries[0].quantity << '\n';
    std::cout << std::setw(24) << leanName << std::setw(10) << 50
        << std::setw(10) << -itemSummaries[1].quantity << std::setw(10) << 50 - itemSummaries[1].quantity << '\n';
    std::cout << std::setw(24) << sodaName << std::setw(10) << 50
        << std::setw(10) << -itemSummaries[2].quantity << std::setw(10) << 50 - itemSummaries[2].quantity << '\n';
    std::cout << std::setw(24) << spriteName << std::setw(10) << 50
        << std::setw(10) << -itemSummaries[3].quantity << std::setw(10) << 50 - itemSummaries[3].quantity << '\n';
    std::cout << "-------------------------------------------------------\n";
//receipt file
    receipt_fout << "Total sales by item:\n";
    for (const ItemSummary& summary : itemSummaries) {
        receipt_fout << summary.name << ": $" << std::fixed << std::setprecision(2)
            << summary.sales << '\n';
    }
    receipt_fout << "Total sales: $" << std::fixed << std::setprecision(2) << totalSales << '\n';
    receipt_fout << "Inventory:\n";
    receipt_fout << std::setw(24) << "Item" << std::setw(10) << "Start"
        << std::setw(10) << "Change" << std::setw(10) << "End" << '\n';
    receipt_fout << std::setw(24) << waterName << std::setw(10) << 50
        << std::setw(10) << -itemSummaries[0].quantity << std::setw(10) << 50 - itemSummaries[0].quantity << '\n';
    receipt_fout << std::setw(24) << leanName << std::setw(10) << 50
        << std::setw(10) << -itemSummaries[1].quantity << std::setw(10) << 50 - itemSummaries[1].quantity << '\n';
    receipt_fout << std::setw(24) << sodaName << std::setw(10) << 50
        << std::setw(10) << -itemSummaries[2].quantity << std::setw(10) << 50 - itemSummaries[2].quantity << '\n';
    receipt_fout << std::setw(24) << spriteName << std::setw(10) << 50
        << std::setw(10) << -itemSummaries[3].quantity << std::setw(10) << 50 - itemSummaries[3].quantity << '\n';
    receipt_fout << "-------------------------------------------------------\n";
    return 0;
}