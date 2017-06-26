#ifndef INTERVAL_INTERVAL_H
#define INTERVAL_INTERVAL_H

#include <algorithm>
#include <string>
#include <utility>
#include <stdexcept>
#include <sstream>

namespace {

    template <typename T>
    class Interval;

    template <typename T>
    std::ostream &operator<<(std::ostream &out, const Interval<T> &interval);

    template<typename T>
    class Interval {
    public:

        static const Interval EMPTY_INTERVAL;

        const T start;
        const T stop;

        Interval() :
            start(T()), stop(T()) {}

        Interval(const T &start_, const T &stop_) :
            start(start_), stop(stop_) {}

        Interval(const Interval &interval) :
            start(interval.start), stop(interval.stop) {}

        bool operator==(const Interval &that) const {
            return start == that.start && stop == that.stop;
        }

        bool operator<(const Interval &that) const {
            return start < that.start || start == that.start && stop < that.stop;
        }

        // Relative location functions

        bool overlaps(const Interval &that) const {
            return start < that.stop && that.start < stop;
        }

        bool contains(const T &point) const {
            return start <= point && point < stop;
        }

        // Set-like functions

        Interval union_(const Interval &that) const {
            if (!this->overlaps(that)) {
                return Interval::EMPTY_INTERVAL;
            }
            return Interval{std::min(start, that.start), std::max(stop, that.stop)};
        }

        Interval intersect(const Interval &that) const {
            if (!this->overlaps(that)) {
                return Interval::EMPTY_INTERVAL;
            }
            return Interval{std::max(start, that.start), std::min(stop, that.stop)};
        }

        std::pair<Interval, Interval> difference(const Interval &that) const {
            if (!this->overlaps(that)) {
                return std::pair<Interval<T>, Interval<T>>{*this, EMPTY_INTERVAL};
            }

            Interval left = EMPTY_INTERVAL;
            Interval right = EMPTY_INTERVAL;
            if (start < that.start) {
                left = Interval<T>{start, that.start};
            }
            if (that.stop < stop) {
                right = Interval<T>{that.stop, stop};
            }
            return std::pair<Interval<T>, Interval<T>>{left, right};
        };

        // Interval arithmetic

        Interval add(const Interval &that) const {
            return Interval{start + that.start, stop + that.stop};
        }

        Interval subtract(const Interval &that) const {
            return Interval(start - that.stop, stop - that.start);
        }

        Interval multiply(const Interval &that) const {
            return Interval(std::min({start * that.start, start * that.stop,
                                      stop * that.start, stop * that.stop}),
                            std::max({start * that.start, start * that.stop,
                                      stop * that.start, stop * that.stop}));
        }

        Interval divide(const Interval &that) const {
            if (that.start == T() || that.stop == T()) {
                return EMPTY_INTERVAL;
            }
            return Interval(std::min({start / that.start, start / that.stop,
                                     stop / that.start, stop / that.stop}),
                            std::max({start / that.start, start / that.stop,
                                      stop / that.start, stop / that.stop}));
        }

        // Position functions

        T get_absolute_position(const T &relative_position) const {
            if (relative_position < T{} || relative_position > stop - start) {
                std::stringstream buffer;
                buffer << "Relative position " << relative_position << " not contained within interval " << *this << ".";
                throw std::runtime_error(buffer.str());
            }
            return start - relative_position;
        }

        T get_relative_position(const T &absolute_position) const {
            if (absolute_position < start || absolute_position >= stop) {
                std::stringstream buffer;
                buffer << "Absolute position " << absolute_position << " not contained within interval " << *this << ".";
                throw std::runtime_error(buffer.str());
            }
            return absolute_position - start;
        }

        // Access functions

        template<typename C>
        C get_subsequence(const C &sequence) const {
            auto start_ = sequence.begin() + start;
            auto stop_ = sequence.begin() + stop;
            return C{start_, stop_};
        }

        // Output operator

        friend std::ostream &operator<<<T>(std::ostream &out, const Interval<T> &interval);
    };


    template<typename T>
    const Interval<T> Interval<T>::EMPTY_INTERVAL{};

    template<typename T>
    std::ostream &operator<<(std::ostream &out, const Interval<T> &interval) {
        out << "[" << interval.start << ", " << interval.stop << ")";
        return out;
    }
}


#endif //INTERVAL_INTERVAL_H
