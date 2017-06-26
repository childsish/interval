# interval

A C++ header library implementing an interval and interval set.

## Interval

The library implements an Interval class and an IntervalSet class. Intervals provide basic set and arithmetic functions,
 functions for converting position relative and absolute to the interval and a function to get the subsequence covered
 by the interval. The subsequence functionpermits any type that implements operator+ between the iterator and the
 start/stop types.
 
Intervals have been templated to allow the start and stop types to be any type that behaves like a
 number. If using a class, then comparator, arithmetic and shift operators must be implemented. The shift operators must
 return integers.

## IntervalSet
 
The IntervalSet has been implemented to provide fast lookup of intervals using interval binning. The `get` function will
 return all intervals in the set that overlap the query.

## Usage

Overlapping intervals.

```cpp
Interval<int> A(0, 2);
Interval<int> B(1, 3);
Interval<int> C(2, 4);

A.overlaps(B); // true
A.overlaps(C); // false
B.overlaps(C); // true
```

Custom position types.

```
class GenomicPosition {
public:
    const std::string chromosome;
    const int position;
    
    bool operator<(const GenomicPosition &that) const;
    bool operator==(const GenomicPosition &that) const;
    
    GenomicPosition operator+(const GenomicPosition &that) const;
    GenomicPosition operator-(const GenomicPosition &that) const;
    GenomicPosition operator*(const GenomicPosition &that) const;
    GenomicPosition operator/(const GenomicPosition &that) const;
    
    int operator<<(int amount) const;
    int operator>>(int amount) const;
}

Interval<GenomicPosition> A{{"chr1", 0}, {"chr1", 100000}};
Interval<GenomicPosition> B{{"chr1", 50000}, {"chr1", 150000}};

A.overlaps(B); // true
```

The `get_subsequence` function.

```
Interval<int> interval{3, 6};
interval.get_subsequence<std::string>("abcdefghij"); // "def"
interval.get_subsequence<std::vector<int>>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}); // {3, 4, 5}
```

The interval set.

```cpp
IntervalSet<int> set;
set.add(Interval<int>{0, 10});
set.add(Interval<int>{2, 12});
set.add(Interval<int>{4, 14});
set.add(Interval<int>{6, 16});
set.add(Interval<int>{8, 18});
set.add(Interval<int>{10, 20});

std::vector<Interval<int>> intervals = set.get(8, 11); // get all intervals
std::vector<Interval<int>> intervals = set.get(14, 18); // get intervals [6, 16), [8, 18), [10, 20)
```
