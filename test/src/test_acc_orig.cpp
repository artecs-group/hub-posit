#include <iostream>
#include <string.h>   // for strcmp()
#include <random>   // for random_device, uniform_X_distribution
#include <universal/number/posit/posit.hpp>
#include <universal/internal/bitblock/bitblock.hpp>
// #include <omp.h>

#include <assert.h>   // assert

#define N 16
#define EXTRA N+8
#define SUPER 2*(EXTRA)
// #define SAMPLES 10000 // 0.01 million
#define SAMPLES 1000000 // 1 million
// #define SAMPLES 100000000 // 100 million
// #define SAMPLES 10000000000 // 1e10
// #define DEBUG
// #define DOUBLE_SAMPLES

using Posit_t = sw::universal::posit<N,2>;
using Posit_HUB_t = sw::universal::posit<N+1,2>;
using Posit_extra_t = sw::universal::posit<EXTRA,2>;
using Posit_super_t = sw::universal::posit<SUPER,2>;    // We need an extra size accumulator so that rounding does not affect the result, and we can truncate the result

template<typename Real_t1, typename Real_t2>
inline long double get_error(Real_t1 golden, Real_t2 computed){
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

template<typename Posit_in, typename Posit_out>
Posit_out to_posit(const Posit_in x){
    Posit_out p;

    if (x.nbits > p.nbits)
    {
        p.setbits((x.get().to_ullong())>>(x.nbits - N)); // Truncate to have N-bits, and add ilsb = 1
        bool lsb = x.get()[x.nbits - N];
        bool rnd = x.get()[x.nbits - N - 1];
        bool stk = (((x.get()<<(N+1)).to_ullong()) == 0) ? false : true;
        if (rnd && (lsb || stk)) p++;

        // Avoid overflow
        if (p.isnar() && !x.isnar()){
            if (x > 0)
                p--;
            else
                p++;
        }
        // Avoid underflow
        if (p.iszero() && !x.iszero()){
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
    else {
        // Only works fine when x.nbits <= 64
        sw::universal::internal::bitblock<p.nbits> raw_bits;
        raw_bits = x.get().to_ullong();
        raw_bits<<=(p.nbits - x.nbits);
        p.setBitblock(raw_bits); // Pad with 0's
    }
    return p;
}

template<typename Posit_tt>
Posit_HUB_t to_posit_HUB(const Posit_tt x){
    Posit_HUB_t p;
    uint64_t ilsb = (x.iszero() || x.isnar()) ? 0ULL : 1ULL; // Keep exceptions
    p.setbits((x.get().to_ullong())>>(x.nbits - N - 1) | ilsb); // Truncate to have N-bits, and add ilsb = 1

    // Avoid overflow & underflow -- Not sure if necessary
    if ((p.isnar() && !x.isnar()) || (p.iszero() && !x.iszero())){
        p++;
    }
    return p;
}

int main(int argc, char *argv[]) {
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
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    // std::uniform_int_distribution<int> distr(- std::numeric_limits<int>::max(), std::numeric_limits<int>::max()); // define the range
    // const uint64_t max_k = (1ULL<<(EXTRA-1ULL)) + ((1ULL<<(EXTRA-1ULL))-1ULL);//((1ULL<<63)-1) + (1ULL<<63);

    // const uint64_t max_k = (1ULL<<(EXTRA-2ULL));    // 1 in posit
    // const uint64_t min_k = 0ULL;
    // const uint64_t max_k = (3ULL<<(EXTRA-3ULL));    // 16 in posit
    
    // const uint64_t max_k = ((1ULL<<(N-1ULL))-1ULL) << (EXTRA-N);    // max of N-bit posit, in EXTRA-bit format
    // const uint64_t min_k = (1ULL<<(EXTRA-N));    // min of N-bit posit
    // HUB affects only to the fraction
    // const uint64_t max_k = ((1ULL<<(N-1ULL))-(1ULL+4ULL)) << (EXTRA-N);    // max N-bit posit with fraction bits, in EXTRA-bit format (pad with 0's)
    // const uint64_t min_k = (4ULL<<(EXTRA-N));    // min N-bit posit with fraction bits
    const uint64_t max_k = (2042ULL<<(N-12)) << (EXTRA-N);  // 2^30
    const uint64_t min_k = (3ULL<<(N-12)) << (EXTRA-N);     // 2^-30
    // const uint64_t max_k = (63ULL<<(N-7)) << (EXTRA-N);  // 2^20
    // const uint64_t min_k = (1ULL<<(N-7)) << (EXTRA-N);   // 2^-20
    // const uint64_t max_k = (123ULL<<(N-8)) << (EXTRA-N); // 2^15
    // const uint64_t min_k = (5ULL<<(N-8)) << (EXTRA-N);   // 2^-15
    // const uint64_t max_k = (27ULL<<(N-6)) << (EXTRA-N);    // 128
    // const uint64_t min_k = (5ULL<<(N-6)) << (EXTRA-N);    // 128^-1
    // const uint64_t max_k = ((3ULL<<(N-3ULL))-1ULL) << (EXTRA-N);    // max N-bit posit with all fraction bits, in EXTRA-bit format (pad with 0's)
    // const uint64_t min_k = ((1ULL<<(N-3ULL))<<(EXTRA-N));    // min N-bit posit with all fraction bits
    std::uniform_int_distribution<uint64_t> distr(min_k, max_k); // define the range, in EXTRA-bit format
    bool rand_bool;
    // std::uniform_int_distribution<uint64_t> distr(0, std::numeric_limits<uint64_t>::max()); // define the range
    
    // const long int max_k = (1ULL<<(EXTRA-2ULL));
    // std::uniform_int_distribution<long int> distr(- max_k, max_k); // define the range
    // int32_t a_fx, b_fx;

    // std::uniform_real_distribution<double> distr(-10.0, 10.0); // define the range

    // const double mmax = (1ULL << 47);
    // const double mmin = (1/(2*mmax));
    const double mmax = (1ULL << 20);
    const double mmin = (1/mmax);
    // const double mmax = (1ULL << 4);
    // const double mmin = (1/mmax);

// #pragma omp parallel for
    for (uint64_t i = 0; i < SAMPLES; i++){
        // Set random values as posits
        // a_fx = distr(gen);
        // b_fx = distr(gen);
        // a_rnd = (double)(a_fx) / (1ULL<<31);
        // b_rnd = (double)(b_fx) / (1ULL<<31);
        // a_rnd = (double)(distr(gen)) / (1ULL<<31);
        // b_rnd = (double)(distr(gen)) / (1ULL<<31);

        // Input a
        a_rnd = distr(gen);
        extra_a.setbits(a_rnd);
#ifdef DEBUG
        // std::cout << mmin << " < " << extra_a << " < " << mmax << " \n";
        assert(mmin <= extra_a);
        assert(extra_a <= mmax);
#endif
        rand_bool = std::uniform_int_distribution<>{0, 1}(gen);
        if (rand_bool){
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
        if (rand_bool){
            extra_b = -extra_b;
        }
        // // double b_rnd2 = distr(gen);
        // // extra_b = b_rnd2;
        b = to_posit<Posit_extra_t, Posit_t>(extra_b);
        b_HUB = to_posit_HUB(extra_b);
        b_SUPER = to_posit<Posit_extra_t, Posit_super_t>(extra_b);

        // std::cout << extra.get() << " " << a.get() << " " << a_HUB.get() <<  " " << a_SUPER.get() << std::endl;


        // Perform computation
        // We can assert that computation in double is as exact as fixed-point computation
        if (0 == strcmp(argv[1], "-m")){    // Multiplication
            // Golden (double)
            r_gold = a_SUPER * b_SUPER;
            // Posit (computed by library)
            r_posit = a * b;
            // Posit-HUB (need to compute in higher precision and truncate, no rounding)
            extra_r = (double(a_HUB) * double(b_HUB));
            r_HUB = to_posit_HUB(extra_r);
        }
        else if (0 == strcmp(argv[1], "-a")){   // Addition
            // Golden (double)
            r_gold = a_SUPER + b_SUPER;
            // Posit (computed by library)
            r_posit = a + b;
            // Posit-HUB (need to compute in higher precision and truncate, no rounding)
            extra_r = (double(a_HUB) + double(b_HUB));
            r_HUB = to_posit_HUB(extra_r);
        }
        else if (0 == strcmp(argv[1], "-c")){   // Conversion
            // Golden (double)
            r_gold = a_SUPER;
            // Posit (computed by library)
            r_posit = a;
            // Posit-HUB
            r_HUB = a_HUB;
        }
        else {
            // return -1;
        }


        // Print results
#ifdef DEBUG
    // if (1e5 < abs(r_gold)){
    // if (((1e1 < std::abs(get_error(r_gold, r_posit))) || (1e1 < get_error(r_gold, r_HUB))) && (abs(r_gold) < 1e15)){
        std::cout << "\n$$$$$\n" << extra_a.get() << " " << extra_b.get() << " " << extra_r.get() << " \n";
        std::cout << a_rnd << " " << b_rnd << " " << extra_a << " " << extra_b << " \n";
        // std::cout << (a_rnd<6917529027641081856ULL) << " " << (b_rnd<6917529027641081856ULL) << " \n";
        std::cout << a.get() << " " << b.get() << " " << r_posit.get() << " \n";
        std::cout << a_HUB.get() << " " << b_HUB.get() << " " << r_HUB.get() << " \n";
        // std::cout << a_SUPER.get() << " " << b_SUPER.get() << " " << r_gold.get() << " \n";
        std::cout << r_gold << ", " << r_posit << ", " << r_HUB << " \n";
        std::cout << get_error(r_gold, r_posit) <<  "," << get_error(r_gold, r_HUB) << std::endl;
    //     return -1;
    // }
#endif
        // std::cout << get_error(r_gold, r_posit) <<  "," << get_error(r_gold, r_HUB) << std::endl;
        // std::cout << "\n\n" << (a_rnd<6917529027641081856ULL) << " " << (b_rnd<6917529027641081856ULL) << " " << extra_a << " " << extra_b << " \n";

// // #pragma omp single
// #pragma omp critical
// {
        std::cout << r_gold <<  "," << r_posit <<  "," << r_HUB << "," << get_error(r_gold, r_posit) <<  "," << get_error(r_gold, r_HUB) <<  "," << extra_a <<  "," << extra_b << std::endl;
// }

//     std::stringstream buf;
//     buf << r_gold <<  "," << r_posit <<  "," << r_HUB << "," << get_error(r_gold, r_posit) <<  "," << get_error(r_gold, r_HUB) <<  "," << extra_a <<  "," << extra_b << "\n";

// #pragma omp critical
//     std::cout << buf.rdbuf();

    }

    
//         a = a_rnd;
//         ilsb_a = (a.iszero() || a.isnar()) ? 0ULL : 1ULL; // Keep exceptions
//         // a_HUB.setbits(((a.get().to_ullong())<<1) | ilsb_a);
//         extra = a_rnd;
//         a_HUB.setbits((extra.get().to_ullong())>>(EXTRA-N-1) | ilsb_a); // truncate rightmost bits

//         b = b_rnd;
//         ilsb_b = (b.iszero() || b.isnar()) ? 0ULL : 1ULL; // Keep exceptions
//         // b_HUB.setbits(((b.get().to_ullong())<<1) | ilsb_b);
//         extra = b_rnd;
//         b_HUB.setbits((extra.get().to_ullong())>>(EXTRA-N-1) | ilsb_b); // truncate rightmost bits


//         // Perform computation
//         // We can assert that computation in double is as exact as fixed-point computation
//         if (0 == strcmp(argv[1], "-m")){    // Multiplication
//             // Golden (double)
//             r_gold = a_rnd * b_rnd;
//             // Posit (computed by library)
//             r_posit = a * b;
//             // Posit-HUB (need to compute in higher precision and truncate, no rounding)
//             extra = (double(a_HUB) * double(b_HUB));
// #if defined(DEBUG) && !defined(DOUBLE_SAMPLES)
//             int64_t rr = (int64_t)(a_fx) * (int64_t)(b_fx);
//             double rrr = (double)(rr) / (1ULL<<62);
//             // std::cout << "$$$$\n" << a_fx <<  ", " << b_fx << std::endl << a_rnd <<  ", " << b_rnd << std::endl << r_gold <<  ", " << rrr << std::endl;
//             assert(r_gold == rrr);
// #endif
//         }
//         else if (0 == strcmp(argv[1], "-a")){   // Addition
//             // Golden (double)
//             r_gold = a_rnd + b_rnd;
//             // Posit (computed by library)
//             r_posit = a + b;
//             // Posit-HUB (need to compute in higher precision and truncate, no rounding)
//             extra = (double(a_HUB) + double(b_HUB));

//             // if (std::abs(r_gold) >= 1.0){
//             //     continue;
//             // }
// #if defined(DEBUG) && !defined(DOUBLE_SAMPLES)
//             int32_t rr = (int32_t)(a_fx) + (int32_t)(b_fx);
//             double rrr = (double)(rr) / (1ULL<<31);
//             // std::cout << "$$$$\n" << a_fx <<  ", " << b_fx << std::endl << a_rnd <<  ", " << b_rnd << std::endl << r_gold <<  ", " << rrr << std::endl;
//             assert((r_gold == rrr) || (std::abs(r_gold) >= 1.0));   // Exclude results which produce overflow
// #endif
//         }
//         else if (0 == strcmp(argv[1], "-c")){   // Conversion
//             // Golden (double)
//             r_gold = a_rnd;
//             // Posit (computed by library)
//             r_posit = a;
//             // Posit-HUB (need to compute in higher precision and truncate, no rounding)
//             extra = (double(a_HUB));
//         }
//         else {
//             return -1;
//         }

//         // // Exclude results which produce overflow for 32-bit fixed-point arithmetic
//         // if (std::abs(r_gold) >= 1.0){
//         //     continue;
//         // }

//         ilsb_b = (extra.iszero() || extra.isnar()) ? 0ULL : 1ULL; // Keep exceptions
//         r_HUB.setbits((extra.get().to_ullong())>>(EXTRA-N-1) | ilsb_b); // truncate rightmost bits

//         // // Avoid overflow
//         // if (r_HUB.isnar() && !extra.isnar()){
//         //     if (extra > 0)
//         //         r_HUB--;
//         //     else
//         //         r_HUB++;
//         // }
//         // // Avoid underflow
//         // if (r_HUB.iszero() && !extra.iszero()){
//         //     if (extra > 0)
//         //         r_HUB++;
//         //     else
//         //         r_HUB--;
//         // }

//         // Avoid overflow & underflow
//         // same as above, since correction is only needed at negative overflow and positive underflow
//         if ((r_HUB.isnar() && !extra.isnar()) || (r_HUB.iszero() && !extra.iszero())){
//             r_HUB++;
//         }

//         // Print results
// #ifdef DEBUG
// #ifndef DOUBLE_SAMPLES
//         std::cout << a_fx <<  ", " << b_fx << std::endl;
// #endif
//         std::cout << std::setprecision(15) << a_rnd << " " << b_rnd << std::endl;
//         std::cout << std::setprecision(15) << a << " " << b <<  " \n" << a_HUB << " " << b_HUB <<   " \n" << (Posit_extra_t)a_rnd << " " << (Posit_extra_t)b_rnd << std::endl;
        
//         std::cout << a.get() << "   " << b.get() <<  " \n" << a_HUB.get() << "  " << b_HUB.get() << " \n" << ((Posit_extra_t)a_rnd).get() << "  " << ((Posit_extra_t)b_rnd).get() << std::endl;
//         std::cout << a.get().to_ullong() - (a_HUB.get().to_ullong()>>1UL) << " " << b.get().to_ullong() - (b_HUB.get().to_ullong()>>1UL) << std::endl;
//         // std::cout << std::setprecision(10) << r_gold << " " << r_posit << " " << r_HUB << std::endl;
//         std::cout << r_posit.get() << " " << r_HUB.get() << std::endl;
//         std::cout << std::setprecision(10) << r_gold << " " << r_posit <<  " " << r_HUB <<  " -- " << get_error(r_gold, r_posit) <<  " -- " << get_error(r_gold, r_HUB) << std::endl;
//         std::cout << std::endl;
// #else
//         std::cout << get_error(r_gold, r_posit) <<  "," << get_error(r_gold, r_HUB) << std::endl;
// #endif
//     }

}
