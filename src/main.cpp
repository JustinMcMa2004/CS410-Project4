#include <iostream>
#include "Product.h"

int main() {
    Product product(10);

    std::cout << "Package Management System" << std::endl;
    std::cout << "Has enough inventory for 5? "
              << (product.hasEnough(5) ? "Yes" : "No")
              << std::endl;

    return 0;
}