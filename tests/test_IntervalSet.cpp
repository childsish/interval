#include "gtest/gtest.h"

#include "IntervalSet.h"


class IntervalSetTest: public ::testing::Test {
protected:
    Interval<int> a{0, 10000};
    Interval<int> b{0, 100000};
    Interval<int> c{90000, 210000};
    Interval<int> d{200000, 500000};
    Interval<int> e{600000, 610000};
    Interval<int> f{600000, 10000000};
    Interval<int> g{0, 10000000};
};

TEST_F(IntervalSetTest, test_add) {
    IntervalSet<int> set;

    set.add(a);
    set.add(b);
    set.add(c);
    set.add(d);
    set.add(e);
    set.add(f);
    set.add(g);
}

TEST_F(IntervalSetTest, test_get) {
    IntervalSet<int> set;
    std::vector<Interval<int>> intervals;

    set.add(a);
    set.add(b);
    set.add(d);
    set.add(e);
    set.add(f);

    intervals = set.get(c);
    EXPECT_EQ(intervals.size(), 2);
    EXPECT_EQ(std::min(intervals[0], intervals[1]), b);
    EXPECT_EQ(std::max(intervals[0], intervals[1]), d);
    intervals = set.get(g);
    EXPECT_EQ(intervals.size(), 5);
}
