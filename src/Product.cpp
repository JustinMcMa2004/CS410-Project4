#include "Product.h"

Product::Product(int initialQuantity){
    quantity = initialQuantity;

}

bool Product::hasEnough(int requested) {
    return requested <= quantity;
}