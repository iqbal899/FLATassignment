#include <iostream>
#include <string>
#include <algorithm>
#include <fst/fstlib.h>

using namespace fst;

int main() {
    // --- Terminal palindrome check ---
    std::string input;
    std::cout << "Enter a string over {a, b}: ";
    std::cin >> input;

    // Validate input
    for (char c : input) {
        if (c != 'a' && c != 'b') {
            std::cerr << "Error: Invalid character " << c << std::endl;
            return 1;
        }
    }

    std::string rev = input;
    std::reverse(rev.begin(), rev.end());

    if (input == rev)
        std::cout << "Palindrome" << std::endl;
    else
        std::cout << "Not a palindrome" << std::endl;

    // --- Build FST for visualization ---
    SymbolTable syms("symbols");
    syms.AddSymbol("<eps>", 0);
    syms.AddSymbol("a", 1);
    syms.AddSymbol("b", 2);

    StdVectorFst fst;
    int s = fst.AddState();  // single self-loop state
    fst.SetStart(s);
    fst.SetFinal(s, TropicalWeight::One());

    // Self-loops for 'a' and 'b'
    fst.AddArc(s, StdArc(syms.Find("a"), syms.Find("a"), TropicalWeight::One(), s));
    fst.AddArc(s, StdArc(syms.Find("b"), syms.Find("b"), TropicalWeight::One(), s));

    fst.SetInputSymbols(&syms);
    fst.SetOutputSymbols(&syms);

    // Save FST and symbols
    fst.Write("palindrome.fst");
    syms.WriteText("palindrome.syms");

    std::cout << "FST saved as 'palindrome.fst'" << std::endl;
    std::cout << "Symbol table saved as 'palindrome.syms'" << std::endl;

    return 0;
}

