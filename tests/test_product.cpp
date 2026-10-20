#include <gtest/gtest.h>
#include "Product.h"

TEST(ProductTest, DetectsEnoughInventory) {

    // Arrange
    Product product(10);

    // Act + Assert
    EXPECT_TRUE(product.hasEnough(5));
    EXPECT_TRUE(product.hasEnough(10));
    EXPECT_FALSE(product.hasEnough(12));
}