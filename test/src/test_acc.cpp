#include <iostream>
#include <string.h> // for strcmp()
#include <random>   // for random_device, uniform_X_distribution
#include <universal/number/posit/posit.hpp>
#include <universal/internal/bitblock/bitblock.hpp>
// #include <omp.h>

#include <assert.h> // assert

#define N 16              // number of bits to test (PositN)
#define EXTRA N + 8       // superset of poist numbers to generate input values
#define SUPER 2 * (EXTRA) // extra precision bits to acomodate exact results
#define SAMPLES 1000000   // 1 million
// #define DEBUG
// #define DOUBLE_SAMPLES

using Posit_t = sw::universal::posit<N, 2>;
using Posit_HUB_t = sw::universal::posit<N + 1, 2>;
using Posit_extra_t = sw::universal::posit<EXTRA, 2>;
using Posit_super_t = sw::universal::posit<SUPER, 2>; // We need an extra size accumulator so that rounding does not affect the result, and we can truncate the result

template <typename Real_t1, typename Real_t2>
inline long double get_error(Real_t1 golden, Real_t2 computed)
{
    // if (abs(golden) >= 1){
    //     // Relative error
    //     return std::abs(((long double)(golden) - (long double)(computed))) / std::abs((long double)(golden));
    // }
    // else{
    //     // Error
    //     return ((long double)(golden) - (long double)(computed));
    // }
    // return ((long double)(golden) - (long double)(computed));    // Absolute error
    return ((long double)(golden) - (long double)(computed)) / std::abs((long double)(golden)); // relative error
    // return std::abs(((long double)(golden) - (long double)(computed))) / std::abs((long double)(golden));
}

template <typename Posit_in, typename Posit_out>
Posit_out to_posit(const Posit_in x)
{
    Posit_out p;

    if (x.nbits > p.nbits)
    {
        p.setbits((x.get().to_ullong()) >> (x.nbits - N)); // Truncate to have N-bits, and add ilsb = 1
        bool lsb = x.get()[x.nbits - N];
        bool rnd = x.get()[x.nbits - N - 1];
        bool stk = (((x.get() << (N + 1)).to_ullong()) == 0) ? false : true;
        if (rnd && (lsb || stk))
            p++;

        // Avoid overflow
        if (p.isnar() && !x.isnar())
        {
            if (x > 0)
                p--;
            else
                p++;
        }
        // Avoid underflow
        if (p.iszero() && !x.iszero())
        {
            if (x > 0)
                p++;
            else
                p--;
        }

        // Posit_out tmp = (Posit_out)((long double)(x));
        // // if ((Posit_out)((long double)(x)) != p)
        //     std::cout << "$$$$" << (x.get()<<(N+1)) <<"\n" << tmp <<  ", " << p << "\n" << x.get() << ", " << tmp.get() <<  ", " << p.get() << ", " << lsb << rnd << stk << std::endl;
        // assert(tmp == p);
    }
    else
    {
        // Only works fine when x.nbits <= 64
        sw::universal::internal::bitblock<p.nbits> raw_bits;
        raw_bits = x.get().to_ullong();
        raw_bits <<= (p.nbits - x.nbits);
        p.setBitblock(raw_bits); // Pad with 0's
    }
    return p;
}

template <typename Posit_tt>
Posit_HUB_t to_posit_HUB(const Posit_tt x)
{
    Posit_HUB_t p;
    uint64_t ilsb = (x.iszero() || x.isnar()) ? 0ULL : 1ULL;      // Keep exceptions
    p.setbits((x.get().to_ullong()) >> (x.nbits - N - 1) | ilsb); // Truncate to have N-bits, and add ilsb = 1

    // Avoid overflow & underflow -- Not sure if necessary
    if ((p.isnar() && !x.isnar()) || (p.iszero() && !x.iszero()))
    {
        p++;
    }
    return p;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return -1;

    Posit_t a, b, r_posit;
    Posit_HUB_t a_HUB, b_HUB, r_HUB;
    Posit_extra_t extra_a, extra_b, extra_r;
    Posit_super_t a_SUPER, b_SUPER, r_gold;
    // const int M = std::max(N, 13);

    uint64_t ilsb_hub;

    // double a_rnd, b_rnd, r_gold;

    // random bitstring
    uint64_t a_rnd, b_rnd;

    // random numbers logic
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    /* Select the interval for random posits */
    // const uint64_t max_k = (2042ULL<<(N-12)) << (EXTRA-N);  // 2^30
    // const uint64_t min_k = (3ULL<<(N-12)) << (EXTRA-N);     // 2^-30
    const uint64_t max_k = (63ULL << (N - 7)) << (EXTRA - N); // 2^20
    const uint64_t min_k = (1ULL << (N - 7)) << (EXTRA - N);  // 2^-20
    // const uint64_t max_k = (123ULL<<(N-8)) << (EXTRA-N); // 2^15
    // const uint64_t min_k = (5ULL<<(N-8)) << (EXTRA-N);   // 2^-15
    // const uint64_t max_k = (27ULL<<(N-6)) << (EXTRA-N);    // 128
    // const uint64_t min_k = (5ULL<<(N-6)) << (EXTRA-N);    // 128^-1
    // const uint64_t max_k = ((3ULL<<(N-3ULL))-1ULL) << (EXTRA-N);    // max N-bit posit with all fraction bits, in EXTRA-bit format (pad with 0's)
    // const uint64_t min_k = ((1ULL<<(N-3ULL))<<(EXTRA-N));    // min N-bit posit with all fraction bits
    std::uniform_int_distribution<uint64_t> distr(min_k, max_k); // define the range, in EXTRA-bit format
    bool rand_bool;
    // std::uniform_int_distribution<uint64_t> distr(0, std::numeric_limits<uint64_t>::max()); // define the range

    const double mmax = (1ULL << 20);
    const double mmin = (1 / mmax);

    // #pragma omp parallel for
    for (uint64_t i = 0; i < SAMPLES; i++)
    {
        // Set random values as posits

        // Input a
        a_rnd = distr(gen);
        extra_a.setbits(a_rnd);
#ifdef DEBUG
        // std::cout << mmin << " < " << extra_a << " < " << mmax << " \n";
        assert(mmin <= extra_a);
        assert(extra_a <= mmax);
#endif
        rand_bool = std::uniform_int_distribution<>{0, 1}(gen);
        if (rand_bool)
        {
            extra_a = -extra_a;
        }
        // // double a_rnd2 = distr(gen);
        // // extra_a = a_rnd2;
        a = to_posit<Posit_extra_t, Posit_t>(extra_a);
        a_HUB = to_posit_HUB(extra_a);
        a_SUPER = to_posit<Posit_extra_t, Posit_super_t>(extra_a);

        // Input b
        b_rnd = distr(gen);
        extra_b.setbits(b_rnd);
#ifdef DEBUG
        // std::cout << mmin << " < " << extra_b << " < " << mmax << " \n";
        assert(mmin <= extra_b);
        assert(extra_b <= mmax);
#endif
        rand_bool = std::uniform_int_distribution<>{0, 1}(gen);
        if (rand_bool)
        {
            extra_b = -extra_b;
        }
        // // double b_rnd2 = distr(gen);
        // // extra_b = b_rnd2;
        b = to_posit<Posit_extra_t, Posit_t>(extra_b);
        b_HUB = to_posit_HUB(extra_b);
        b_SUPER = to_posit<Posit_extra_t, Posit_super_t>(extra_b);

        // Perform computation
        // We can assert that computation in double is as exact as fixed-point computation
        if (0 == strcmp(argv[1], "-m"))
        { // Multiplication
            // Golden (double)
            r_gold = a_SUPER * b_SUPER;
            // Posit (computed by library)
            r_posit = a * b;
            // Posit-HUB (need to compute in higher precision and truncate, no rounding)
            extra_r = (double(a_HUB) * double(b_HUB));
            r_HUB = to_posit_HUB(extra_r);
        }
        else if (0 == strcmp(argv[1], "-a"))
        { // Addition
            // Golden (double)
            r_gold = a_SUPER + b_SUPER;
            // Posit (computed by library)
            r_posit = a + b;
            // Posit-HUB (need to compute in higher precision and truncate, no rounding)
            extra_r = (double(a_HUB) + double(b_HUB));
            r_HUB = to_posit_HUB(extra_r);
        }
        else if (0 == strcmp(argv[1], "-c"))
        { // Conversion
            // Golden (double)
            r_gold = a_SUPER;
            // Posit (computed by library)
            r_posit = a;
            // Posit-HUB
            r_HUB = a_HUB;
        }
        else
        {
            // return -1;
        }

        // Print results
#ifdef DEBUG
        std::cout << "\n$$$$$\n"
                  << extra_a.get() << " " << extra_b.get() << " " << extra_r.get() << " \n";
        std::cout << a_rnd << " " << b_rnd << " " << extra_a << " " << extra_b << " \n";
        // std::cout << (a_rnd<6917529027641081856ULL) << " " << (b_rnd<6917529027641081856ULL) << " \n";
        std::cout << a.get() << " " << b.get() << " " << r_posit.get() << " \n";
        std::cout << a_HUB.get() << " " << b_HUB.get() << " " << r_HUB.get() << " \n";
        // std::cout << a_SUPER.get() << " " << b_SUPER.get() << " " << r_gold.get() << " \n";
        std::cout << r_gold << ", " << r_posit << ", " << r_HUB << " \n";
        std::cout << get_error(r_gold, r_posit) << "," << get_error(r_gold, r_HUB) << std::endl;
#endif
        std::cout << r_gold << "," << r_posit << "," << r_HUB << "," << get_error(r_gold, r_posit) << "," << get_error(r_gold, r_HUB) << "," << extra_a << "," << extra_b << std::endl;
    }
}
