#ifndef INTERVAL_INTERVALSET_H
#define INTERVAL_INTERVALSET_H

#include <unordered_map>
#include <vector>

#include "Interval.h"


template<typename T>
class IntervalSet {
public:

    void add(Interval<T> interval) {
        _bins[_get_bin(interval)].push_back(interval);
    }

    std::vector<Interval<T>> get(const Interval<T> &interval) const {
        std::vector<Interval<T>> intervals;
        std::vector<int> bins = _get_overlapping_bins(interval);
        for (auto bin : bins) {
            if (_bins.find(bin) == _bins.end()) {
                continue;
            }
            for (auto candidate: _bins.at(bin)) {
                if (interval.overlaps(candidate)) {
                    intervals.push_back(candidate);
                }
            }
        }
        return intervals;
    }

private:

    static const int MAX_BIN = ((1<<18)-1)/7;
    
    std::unordered_map<int, std::vector<Interval<T>>> _bins;

    /**
     * Get a bin for an interval.
     * Adapted from https://samtools.github.io/hts-specs/SAMv1.pdf. Binning steps are interval lengths of 16384, 131072,
     * 1048576, 8388608, 67108864
     * @param interval
     * @return
     */
    int _get_bin(const Interval<T> &interval) const {
        T stop = interval.stop - 1;
        if (interval.start>>14 == stop>>14) return ((1<<15)-1)/7 + (interval.start>>14);
        if (interval.start>>17 == stop>>17) return ((1<<12)-1)/7 + (interval.start>>17);
        if (interval.start>>20 == stop>>20) return ((1<<9)-1)/7  + (interval.start>>20);
        if (interval.start>>23 == stop>>23) return ((1<<6)-1)/7  + (interval.start>>23);
        if (interval.start>>26 == stop>>26) return ((1<<3)-1)/7  + (interval.start>>26);
        return 0;
    }

    /**
     * Get the overlapping bins for an interval.
     * Adapted from https://samtools.github.io/hts-specs/SAMv1.pdf
     * @param intervals
     * @return
     */
    std::vector<int> _get_overlapping_bins(const Interval<T> &interval) const {
        std::vector<int> bins;
        int k;
        T stop = interval.stop - 1;
        bins.push_back(0);
        for (k =    1 + (interval.start>>26); k <=    1 + (stop>>26); ++k) bins.push_back(k);
        for (k =    9 + (interval.start>>23); k <=    9 + (stop>>23); ++k) bins.push_back(k);
        for (k =   73 + (interval.start>>20); k <=   73 + (stop>>20); ++k) bins.push_back(k);
        for (k =  585 + (interval.start>>17); k <=  585 + (stop>>17); ++k) bins.push_back(k);
        for (k = 4681 + (interval.start>>14); k <= 4681 + (stop>>14); ++k) bins.push_back(k);
        return bins;
    }
};

#endif //INTERVAL_INTERVALSET_H
