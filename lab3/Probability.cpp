#include <iostream>
#include <set>
#include <random>

class DiscreteState {
private:
    int const state;

public:
    DiscreteState(int state) : state(state) { }

    //DiscreteState(DiscreteState const& other) : state(other.state) { }

    bool contains(int s) const {
        return s == state;
    }
};

class SegmentState {
private:
    int const beg, end;

public:
    SegmentState() : beg(0), end(-1) { }
    SegmentState(int beg, int end) : beg(beg), end(end) { }
    //SegmentState(SegmentState const& other) : beg(other.beg), end(other.end) { }

    bool contains(int s) const {
        return s >= beg && s <= end;
    }
};

class SetState {
private:
    std::set<int> const states;

public:
    SetState() : states() { }
    SetState(std::set<int> const& src) : states(src) { }
    //SetState(SetState const& other) : states(other.states) { }

    bool contains(int s) const {
        return states.count(s) > 0;
    }
};

template<class X, class Y>
class UnionState {
private:
    X const x;
    Y const y;
public:
    UnionState(X x, Y y) : x(x), y(y) 
    {}
    bool contains(int s) const {
        return x.contains(s) or y.contains(s);
    }
};

template<class X, class Y>
class SubstractState {
private:
    X const x;
    Y const y;
public:
    SubstractState(X x, Y y) : x(x), y(y)
    {}
    bool contains(int s) const {
        return x.contains(s) and !y.contains(s);
    }
};

template<class X, class Y>
class IntersectState {
private:
    X const x;
    Y const y;
public:
    IntersectState(X x, Y y) : x(x), y(y)
    {}
    bool contains(int s) const {
        return x.contains(s) and y.contains(s);
    }
};

class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count) : seed(seed), test_min(test_min), test_max(test_max), test_count(test_count) { }

    template <class T>
    float operator()(T const& s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min, test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good) / static_cast<float>(test_count);
    }
};

template<class T>
void measure(size_t n, T t) {
    ProbabilityTest tester(87, 0, 100, n);
    std::cout << n << ' ' << tester(t) << std::endl;
}

int main(int argc, const char* argv[]) {
    DiscreteState d(1);
    SegmentState s(0, 10);
    SetState ss({ 11, 13, 15, 17, 23});
    ProbabilityTest pt(10, 0, 100, 100000);
    UnionState<SegmentState, DiscreteState> sd(s, d);
    SubstractState<SegmentState, DiscreteState> s_d(s, d);
    IntersectState<SegmentState, DiscreteState> s_n_d(s, d);

    /*for (size_t i = 100; i < 100000; i += 100)
    {
        measure(i, s);
    }*/

    /*for (size_t i = 100; i < 100000; i += 100)
    {
        measure(i, ss);
    }*/

    measure(1000000, ss);


    std::cout << pt(d) << std::endl;
    std::cout << pt(s) << std::endl;
    std::cout << pt(sd) << std::endl;
    std::cout << pt(sd) << std::endl;
    std::cout << pt(s_d) << std::endl;
    std::cout << pt(s_n_d) << std::endl;

    return 0;
}
