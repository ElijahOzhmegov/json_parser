    //
// Created by ilia on 13.06.19.
//

#include <gtest/gtest.h>

TEST(Generic, local_class)
{
    int value = 777;
    struct priv{
        static int get_val(const int & value) { return value; }
    };
    EXPECT_EQ(value, priv::get_val(value));
}



