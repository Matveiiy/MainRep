#include "types.h"
int main() {
    std::cout << "here\n";
    Parser parser("example.fl");
    parser.parse();
    return 0;
}