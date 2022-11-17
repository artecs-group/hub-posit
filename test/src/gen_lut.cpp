#include <iostream>
#include <universal/number/posit/posit.hpp>

#define N 8
// #define CSV

int main() {
    using Posit_t = sw::universal::posit<N,2>;
    using Posit_HUB_t = sw::universal::posit<N+1,2>;

    Posit_t a = 0;
    Posit_HUB_t a_HUB = 0;
    const int M = std::max(N, 13);

#ifdef CSV
    std::cout << "i,Posit,Posit-HUB" << std::endl;
#else
    std::cout << std::setw(3+2) << " " << std::setw(M) << "Posit" << std::setw(M+1) << " Posit-HUB" << std::endl;
#endif

    uint64_t ilsb;
    for (uint64_t i = 0; i < (1ULL << N); i++){
        a.setbits(i);
        ilsb = (a.iszero() || a.isnar()) ? 0ULL : 1ULL; // Keep exceptions
        a_HUB.setbits((i<<1) | ilsb);

#ifdef CSV
        std::cout << i << "," << a <<  "," << a_HUB << std::endl;
#else
        // std::cout << std::setw(3) << i << "  " << a.get() <<  " " << a_HUB.get() << std::endl;
        std::cout << std::setw(3) << i << "  " << std::setw(M) << a <<  " " << std::setw(M) << a_HUB << std::endl;
#endif
    }
}