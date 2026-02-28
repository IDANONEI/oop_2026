#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "VectorUtils.h"


TEST_CASE("MultiplyElementsByMin")
{
    SECTION("Empty vector stays empty")
    {
        std::vector<double> v;
        MultiplyElementsByMin(v);
        REQUIRE(v.empty());
    }

    SECTION("Single element")
    {
        std::vector<double> v = {5.0};
        MultiplyElementsByMin(v);
        REQUIRE(v == std::vector<double>{25.0});
    }

    SECTION("Typical case with negative minimum")
    {
        std::vector<double> v = {2.0, 4.0, -1.5};
        MultiplyElementsByMin(v);
        REQUIRE(v == std::vector<double>{-3.0, -6.0, 2.25});
    }

    SECTION("All positive numbers")
    {
        std::vector<double> v = {2.0, 2.0, 3.0};
        MultiplyElementsByMin(v);
        REQUIRE(v == std::vector<double>{4.0, 4.0, 6.0});
    }
}

TEST_CASE("SortedVector")
{
    SECTION("Empty test")
    {
        std::vector<double> v;
        SortedVector(v);
        REQUIRE(v.empty());
    }

    SECTION("Sorted test")
    {
        std::vector<double> v = {3.0, 1.0, 2.0, -5.0};
        SortedVector(v);
        REQUIRE(v == std::vector<double>{-5.0, 1.0, 2.0, 3.0});
    }

}


TEST_CASE("PrintSortedVector")
{
    SECTION("Prints sorted")
    {
        std::vector<double> v = {3.1415927, 2.3, 1.0};
        std::ostringstream out;

        PrintSortedVector(v, out);

        REQUIRE(out.str() == "1.000 2.300 3.142");
    }

    SECTION("Not modify the original vector")
    {
        std::vector<double> v = {3.0, 1.0, 2.0};
        std::vector<double> original = v;

        std::ostringstream out;
        PrintSortedVector(v, out);

        REQUIRE(v == original);
    }
}

TEST_CASE("ReadNumbers reads")
{
    SECTION("All numbers from stream")
    {
        std::istringstream in("1.5 2.0\n-3\t4.25");
        auto v = ReadNumbers(in);
        REQUIRE(v == std::vector<double>{1.5, 2.0, -3.0, 4.25});
    }

    SECTION("ERROR input")
    {
        std::istringstream in("1.5 abc 3.0");
        REQUIRE_THROWS(ReadNumbers(in));
    }

    SECTION("Error token")
    {
        std::istringstream in("12abc 3.0");
        REQUIRE_THROWS(ReadNumbers(in));
    }

}
