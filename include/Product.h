#ifndef PRODUCT_H
#define PRODUCT_H

class Product {
    private:
    int quantity;

    public:
    Product(int initialQuantity);

    bool hasEnough(int requested);

};

#endif

