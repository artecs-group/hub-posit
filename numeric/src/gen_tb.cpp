#include <iostream>
#include <string.h>
#include <universal/number/posit/posit.hpp>

#define N 12
#define EXTRA 2*(N)
// #define DEBUG

int main(int argc, char *argv[]) {
    if (argc != 2)
        return -1;

    using Posit_t = sw::universal::posit<N,2>;
    using Posit_HUB_t = sw::universal::posit<N+1,2>;
    using Posit_extra_t = sw::universal::posit<EXTRA,2>;    // We need an extra size accumulator so that rounding does not affect the result, and we can truncate the result

    Posit_t a, b, r;
    Posit_HUB_t a_HUB, b_HUB, r_HUB;
    Posit_extra_t extra;
    // const int M = std::max(N, 13);

    // Mult test
#if 0
    a = 1.0; b = -1.0;
    a_HUB.setbits(((a.get().to_ullong())<<1) | 1ULL);
    b_HUB.setbits(((b.get().to_ullong())<<1) | 1ULL);

    r_HUB = a_HUB * b_HUB;

    extra = (double(a_HUB)*double(b_HUB));

    // r.setbits((r_HUB.get().to_ullong())>>1);
    r.setbits((extra.get().to_ullong())>>EXTRA-N); // truncate bits


#ifdef DEBUG
    std::cout << "Posit:" << std::endl;
    std::cout << a <<  " " << b <<  " " << r << std::endl;
    std::cout << a.get() <<  " " << b.get() <<  " " << r.get() << std::endl;
    std::cout << "Posit-HUB:" << std::endl;
    std::cout << a_HUB <<  " " << b_HUB <<  " " << extra << std::endl;
    std::cout << a_HUB.get() <<  " " << b_HUB.get() <<  " " << extra.get() << std::endl;
#else
    std::cout << a.get() <<  " " << b.get() <<  " " << r.get() << std::endl;
#endif

#else
    uint64_t ilsb_a, ilsb_b;
    for (uint64_t i = 0; i < (1ULL << N); i++){
        a.setbits(i);
        ilsb_a = (a.iszero() || a.isnar()) ? 0ULL : 1ULL; // Keep exceptions
        a_HUB.setbits((i<<1) | ilsb_a);
        for (uint64_t j = 0; j < (1ULL << N); j++){
            b.setbits(j);
            ilsb_b = (b.iszero() || b.isnar()) ? 0ULL : 1ULL; // Keep exceptions
            b_HUB.setbits((j<<1) | ilsb_b);

            // Perform computation
            if (0 == strcmp(argv[1], "-m")){
                extra = (double(a_HUB) * double(b_HUB));
            }
            else if (0 == strcmp(argv[1], "-a")){
                extra = (double(a_HUB) + double(b_HUB));
            }
            else {
                return -1;
            }


            r.setbits((extra.get().to_ullong())>>EXTRA-N); // truncate rightmost bits

            // // Avoid overflow
            // if (r.isnar() && !extra.isnar()){
            //     if (extra > 0)
            //         r--;
            //     else
            //         r++;
            // }
            // // Avoid underflow
            // if (r.iszero() && !extra.iszero()){
            //     if (extra > 0)
            //         r++;
            //     else
            //         r--;
            // }

            // Avoid overflow & underflow
            // same as above, since correction is only needed at negative overflow and positive underflow
            if ((r.isnar() && !extra.isnar()) || (r.iszero() && !extra.iszero())){
                r++;
            }

#ifdef DEBUG
            std::cout << "\nPosit:" << std::endl;
            if (0 == strcmp(argv[1], "-m")){
                std::cout << a <<  " " << b <<  " " << a*b << std::endl;
                std::cout << a.get() <<  " " << b.get() <<  " " << (a*b).get() << std::endl;
            }
            else if (0 == strcmp(argv[1], "-a")){
                std::cout << a <<  " " << b <<  " " << a+b << std::endl;
                std::cout << a.get() <<  " " << b.get() <<  " " << (a+b).get() << std::endl;
            }
            std::cout << "Posit-HUB:" << std::endl;
            std::cout << std::setprecision(10) << a_HUB <<  " " << b_HUB <<  " " << extra << std::endl;
            std::cout << a_HUB.get() <<  " " << b_HUB.get() <<  " " << extra.get() <<  " " << r.get() << std::endl;
#else
            std::cout << a.get() <<  " " << b.get() <<  " \n1 " << r.get() << std::endl;
#endif
        }
    }
#endif

}