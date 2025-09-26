#include <iostream>
#include <string>
#include <vector>
#include <fst/fstlib.h>
#include <map>

using namespace fst;

// Map a Roman numeral character to its integer value
int RomanValue(char c) {
    switch(c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return -1; // invalid
    }
}

// Convert Roman numeral string to integer
int RomanToArabic(const std::string &roman) {
    int total = 0;
    int prev = 0;
    for (auto it = roman.rbegin(); it != roman.rend(); ++it) {
        int val = RomanValue(*it);
        if (val < 0) {
            std::cerr << "Invalid Roman numeral: " << *it << std::endl;
            return -1;
        }
        if (val < prev)
            total -= val;
        else
            total += val;
        prev = val;
    }
    return total;
}

// Optional: build FST mapping for visualization
StdVectorFst BuildRomanFST(SymbolTable &syms) {
    StdVectorFst fst;
    int start = fst.AddState();
    fst.SetStart(start);
    fst.SetFinal(start, TropicalWeight::One());

    std::vector<std::pair<char,int>> roman_map = {
        {'I',1}, {'V',5}, {'X',10}, {'L',50},
        {'C',100}, {'D',500}, {'M',1000}
    };

    for (auto [c,val] : roman_map) {
        int id = syms.AddSymbol(std::string(1,c));
        fst.AddArc(start, StdArc(id,id,TropicalWeight::One(), start));
    }

    fst.SetInputSymbols(&syms);
    fst.SetOutputSymbols(&syms);
    return fst;
}

int main() {
    std::string roman;
    std::cout << "Enter a Roman numeral (e.g., XIV): ";
    std::cin >> roman;

    int arabic = RomanToArabic(roman);
    if (arabic >= 0)
        std::cout << "Arabic numeral: " << arabic << std::endl;

    // Optional: build FST and save
    SymbolTable syms("symbols");
    syms.AddSymbol("<eps>",0);
    StdVectorFst roman_fst = BuildRomanFST(syms);
    roman_fst.Write("roman.fst");
    syms.WriteText("roman_syms.txt");

    return 0;
}

