#include <vnigma/vnigma.h>

#include <iostream>

int main(int, char*[])
{
    auto sum = vnigma::add(1, 1);
    std::cout << sum << std::endl;
    return 0;
}
