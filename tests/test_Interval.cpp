#include "gtest/gtest.h"

#include <sstream>
#include "Interval.h"


class IntervalTest: public ::testing::Test {
protected:
    Interval<int> a{2, 5};  /*   [  ]   */
    Interval<int> b{0, 1};  /* []       */
    Interval<int> c{1, 3};  /*  [ ]     */
    Interval<int> d{3, 4};  /*    []    */
    Interval<int> e{4, 6};  /*     [ ]  */
    Interval<int> f{6, 7};  /*       [] */
};


TEST_F(IntervalTest, test_equality) {
    EXPECT_EQ(Interval<int>(0, 0), Interval<int>(0, 0));
    EXPECT_EQ(Interval<int>::EMPTY_INTERVAL, Interval<int>(0, 0));
}

TEST_F(IntervalTest, test_less_than) {
    EXPECT_LT(Interval<int>(0, 10), Interval<int>(5, 6));
    EXPECT_LT(Interval<int>(0, 5), Interval<int>(0, 10));
}

TEST_F(IntervalTest, test_overlaps) {
    EXPECT_FALSE(a.overlaps(b));
    EXPECT_TRUE(a.overlaps(c));
    EXPECT_TRUE(a.overlaps(d));
    EXPECT_TRUE(a.overlaps(e));
    EXPECT_FALSE(a.overlaps(f));
}

TEST_F(IntervalTest, test_union) {
    EXPECT_EQ(a.union_(b), Interval<int>::EMPTY_INTERVAL);
    EXPECT_EQ(a.union_(c), Interval<int>(1, 5));
    EXPECT_EQ(a.union_(d), a);
    EXPECT_EQ(a.union_(e), Interval<int>(2, 6));
    EXPECT_EQ(a.union_(f), Interval<int>::EMPTY_INTERVAL);
}

TEST_F(IntervalTest, test_intersect) {
    EXPECT_EQ(a.intersect(b), Interval<int>::EMPTY_INTERVAL);
    EXPECT_EQ(a.intersect(c), Interval<int>(2, 3));
    EXPECT_EQ(a.intersect(d), d);
    EXPECT_EQ(a.intersect(e), Interval<int>(4, 5));
    EXPECT_EQ(a.intersect(f), Interval<int>::EMPTY_INTERVAL);
}

TEST_F(IntervalTest, test_get_subsequence) {
    std::vector<char> seq1 = {'a', 'b', 'c', 'd', 'e', 'f', 'h', 'i'};
    std::vector<char> seq1_a = {'c', 'd', 'e'};
    EXPECT_EQ(a.get_subsequence<std::vector<char>>(seq1), seq1_a);
    EXPECT_EQ(a.get_subsequence<std::string>("abcdefghi"), "cde");
}

TEST_F(IntervalTest, test_output_stream) {
    std::stringstream output;
    output << Interval<int>(0, 10);
    EXPECT_EQ(output.str(), "[0, 10)");
}
