// Grover's searching
// Source: ./examples/grover.cpp
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>
#include "qpp.h"

using namespace qpp;

int main()
{
    idx n = 4; // number of qubits
    std::cout << ">> Grover on n = " << n << " qubits" << std::endl;

    std::vector<idx> dims(n, 2); // local dimensions
    // number of elements in the database
    idx N = std::round(std::pow(2, n));
    std::cout << ">> Database size: " << N << std::endl;

    // mark an element randomly
    idx marked = randidx(0, N - 1);
    std::cout << ">> Marked state: " << marked << " -> ";
    std::cout << disp(n2multiidx(marked, dims), " ") << std::endl;

    ket psi = mket(n2multiidx(0, dims)); // computational |0>^\otimes n

    // apply H^\otimes n, no aliasing
    psi = (kronpow(gt.H, n) * psi).eval();

    cmat G = 2 * prj(psi) - gt.Id(N); // Diffusion operator

    // number of queries
    idx nqueries = std::ceil(pi * std::sqrt((double) N) / 4.);
    std::cout << ">> We run " << nqueries << " queries" << std::endl;
    for (idx i = 0; i < nqueries; ++i)
    {
        psi(marked) = -psi(marked); // apply the oracle first, no aliasing
        psi = (G * psi).eval(); // then the diffusion operator, no aliasing
    }

    // we now measure the state in the computational basis
    auto measured = measure(psi, gt.Id(N));
    std::cout << ">> Probability of the marked state: "
              << std::get<1>(measured)[marked] << std::endl;
    std::cout << ">> Probability of all results: ";
    std::cout << disp(std::get<1>(measured), ", ") << std::endl;

    // sample
    std::cout << ">> Let's sample..." << std::endl;
    idx result = std::get<0>(measured);
    if (result == marked)
        std::cout << ">> Hooray, we obtained the correct result: ";
    else
        std::cout << ">> Not there yet... we obtained: ";
    std::cout << result << " -> ";
    std::cout << disp(n2multiidx(result, dims), " ") << std::endl;
}
