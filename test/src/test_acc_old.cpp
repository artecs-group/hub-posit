#include <iostream>
#include <string.h>   // for strcmp()
#include <random>   // for random_device, uniform_X_distribution
#include <universal/number/posit/posit.hpp>

#include <assert.h>   // assert

#define N 32
#define EXTRA 2*(N)
#define SAMPLES 10000//00 // 1 million
// #define DEBUG
#define DOUBLE_SAMPLES


template<typename Real_t>
long double get_error(long double golden, Real_t computed){
    return ((long double)(golden) - (long double)(computed));
}

int main(int argc, char *argv[]) {
    if (argc != 2)
        return -1;

    using Posit_t = sw::universal::posit<N,2>;
    using Posit_HUB_t = sw::universal::posit<N+1,2>;
    using Posit_extra_t = sw::universal::posit<EXTRA,2>;    // We need an extra size accumulator so that rounding does not affect the result, and we can truncate the result

    Posit_t a, b, r_posit;
    Posit_HUB_t a_HUB, b_HUB, r_HUB;
    Posit_extra_t extra;
    // const int M = std::max(N, 13);

    uint64_t ilsb_a, ilsb_b;

    double a_rnd, b_rnd;
    long double r_gold;

    // random numbers logic
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

#ifdef DOUBLE_SAMPLES
    std::uniform_real_distribution<double> distr(-1000000.0, 1000000.0); // define the range
    // std::uniform_real_distribution<double> distr(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max()); // define the range

    for (uint64_t i = 0; i < SAMPLES; i++){
        // Set random values as posits
        a_rnd = distr(gen);
        b_rnd = distr(gen);
#else
    std::uniform_int_distribution<int> distr(- std::numeric_limits<int>::max(), std::numeric_limits<int>::max()); // define the range
    int32_t a_fx, b_fx;

    for (uint64_t i = 0; i < SAMPLES; i++){
        // Set random values as posits
        a_fx = distr(gen);
        b_fx = distr(gen);
        a_rnd = (double)(a_fx) / (1ULL<<31);
        b_rnd = (double)(b_fx) / (1ULL<<31);
        // a_rnd = (double)(distr(gen)) / (1ULL<<31);
        // b_rnd = (double)(distr(gen)) / (1ULL<<31);
#endif
        a = a_rnd;
        ilsb_a = (a.iszero() || a.isnar()) ? 0ULL : 1ULL; // Keep exceptions
        // a_HUB.setbits(((a.get().to_ullong())<<1) | ilsb_a);
        extra = a_rnd;
        a_HUB.setbits((extra.get().to_ullong())>>(EXTRA-N-1) | ilsb_a); // truncate rightmost bits

        b = b_rnd;
        ilsb_b = (b.iszero() || b.isnar()) ? 0ULL : 1ULL; // Keep exceptions
        // b_HUB.setbits(((b.get().to_ullong())<<1) | ilsb_b);
        extra = b_rnd;
        b_HUB.setbits((extra.get().to_ullong())>>(EXTRA-N-1) | ilsb_b); // truncate rightmost bits


        // Perform computation
        // We can assert that computation in double is as exact as fixed-point computation
        if (0 == strcmp(argv[1], "-m")){    // Multiplication
            // Golden (double)
            r_gold = a_rnd * b_rnd;
            // Posit (computed by library)
            r_posit = a * b;
            // Posit-HUB (need to compute in higher precision and truncate, no rounding)
            extra = (double(a_HUB) * double(b_HUB));
#if defined(DEBUG) && !defined(DOUBLE_SAMPLES)
            int64_t rr = (int64_t)(a_fx) * (int64_t)(b_fx);
            double rrr = (double)(rr) / (1ULL<<62);
            // std::cout << "$$$$\n" << a_fx <<  ", " << b_fx << std::endl << a_rnd <<  ", " << b_rnd << std::endl << r_gold <<  ", " << rrr << std::endl;
            assert(r_gold == rrr);
#endif
        }
        else if (0 == strcmp(argv[1], "-a")){   // Addition
            // Golden (double)
            r_gold = (long double)(a_rnd) + (long double)(b_rnd);
            // Posit (computed by library)
            r_posit = a + b;
            // Posit-HUB (need to compute in higher precision and truncate, no rounding)
            extra = (double(a_HUB) + double(b_HUB));

            // if (std::abs(r_gold) >= 1.0){
            //     continue;
            // }
#if defined(DEBUG) && !defined(DOUBLE_SAMPLES)
            int32_t rr = (int32_t)(a_fx) + (int32_t)(b_fx);
            double rrr = (double)(rr) / (1ULL<<31);
            // std::cout << "$$$$\n" << a_fx <<  ", " << b_fx << std::endl << a_rnd <<  ", " << b_rnd << std::endl << r_gold <<  ", " << rrr << std::endl;
            assert((r_gold == rrr) || (std::abs(r_gold) >= 1.0));   // Exclude results which produce overflow
#endif
        }
        else if (0 == strcmp(argv[1], "-c")){   // Conversion
            // Golden (double)
            r_gold = a_rnd;
            // Posit (computed by library)
            r_posit = a;
            // Posit-HUB (need to compute in higher precision and truncate, no rounding)
            extra = (double(a_HUB));
        }
        else {
            return -1;
        }

        // // Exclude results which produce overflow for 32-bit fixed-point arithmetic
        // if (std::abs(r_gold) >= 1.0){
        //     continue;
        // }

        ilsb_b = (extra.iszero() || extra.isnar()) ? 0ULL : 1ULL; // Keep exceptions
        r_HUB.setbits((extra.get().to_ullong())>>(EXTRA-N-1) | ilsb_b); // truncate rightmost bits

        // // Avoid overflow
        // if (r_HUB.isnar() && !extra.isnar()){
        //     if (extra > 0)
        //         r_HUB--;
        //     else
        //         r_HUB++;
        // }
        // // Avoid underflow
        // if (r_HUB.iszero() && !extra.iszero()){
        //     if (extra > 0)
        //         r_HUB++;
        //     else
        //         r_HUB--;
        // }

        // Avoid overflow & underflow
        // same as above, since correction is only needed at negative overflow and positive underflow
        if ((r_HUB.isnar() && !extra.isnar()) || (r_HUB.iszero() && !extra.iszero())){
            r_HUB++;
        }

        // Print results
#ifdef DEBUG
#ifndef DOUBLE_SAMPLES
        std::cout << a_fx <<  ", " << b_fx << std::endl;
#endif
        std::cout << std::setprecision(15) << a_rnd << " " << b_rnd << std::endl;
        std::cout << std::setprecision(15) << a << " " << b <<  " \n" << a_HUB << " " << b_HUB <<   " \n" << (Posit_extra_t)a_rnd << " " << (Posit_extra_t)b_rnd << std::endl;
        
        std::cout << a.get() << "   " << b.get() <<  " \n" << a_HUB.get() << "  " << b_HUB.get() << " \n" << ((Posit_extra_t)a_rnd).get() << "  " << ((Posit_extra_t)b_rnd).get() << std::endl;
        std::cout << a.get().to_ullong() - (a_HUB.get().to_ullong()>>1UL) << " " << b.get().to_ullong() - (b_HUB.get().to_ullong()>>1UL) << std::endl;
        // std::cout << std::setprecision(10) << r_gold << " " << r_posit << " " << r_HUB << std::endl;
        std::cout << r_posit.get() << " " << r_HUB.get() << std::endl;
        std::cout << std::setprecision(10) << r_gold << " " << r_posit <<  " " << r_HUB <<  " -- " << get_error(r_gold, r_posit) <<  " -- " << get_error(r_gold, r_HUB) << std::endl;
        std::cout << std::endl;
#else
        std::cout << get_error(r_gold, r_posit) <<  "," << get_error(r_gold, r_HUB) << std::endl;
#endif
    }

}