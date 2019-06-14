    //
// Created by ilia on 13.06.19.
//

#include <gtest/gtest.h>

TEST(BigPortal, local_portal)
{
    int value = 777;
    struct priv{
        static int get_val(const int & value) { return value; }
    };
    EXPECT_EQ(value, priv::get_val(value));
}



