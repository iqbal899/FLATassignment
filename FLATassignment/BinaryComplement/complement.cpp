#include <iostream>
#include <string>
#include <fst/fstlib.h>

using namespace fst;

// Convert FST to string
std::string FstToString(const StdVectorFst& fst, const SymbolTable& syms) {
    std::string result = "";
    if (fst.NumStates() == 0) return result;

    int state = fst.Start();
    while (fst.NumArcs(state) > 0) {
        ArcIterator<StdVectorFst> arc_it(fst, state);
        const StdArc& arc = arc_it.Value();
        std::string sym = syms.Find(arc.olabel);
        if (!sym.empty() && sym != "<epsilon>")
            result += sym;
        state = arc.nextstate;
    }
    return result;
}

// Build linear input FST from string
StdVectorFst StringToFst(const std::string& str, const SymbolTable& syms) {
    StdVectorFst fst;
    fst.AddState();
    fst.SetStart(0);
    int cur = 0;

    for (char c : str) {
        int next = fst.AddState();
        int id = syms.Find(std::string(1, c));
        if (id == kNoSymbol) {
            std::cerr << "Error: Unsupported symbol: " << c << std::endl;
            return StdVectorFst();
        }
        fst.AddArc(cur, StdArc(id, id, TropicalWeight::One(), next));
        cur = next;
    }
    fst.SetFinal(cur, TropicalWeight::One());
    fst.SetInputSymbols(&syms);
    fst.SetOutputSymbols(&syms);
    return fst;
}

int main() {
    // 1. SYMBOL TABLE
    SymbolTable syms("symbols");
    syms.AddSymbol("<epsilon>", 0);
    int id0 = syms.AddSymbol("0");
    int id1 = syms.AddSymbol("1");

    // Save symbol table to file for visualization
    syms.WriteText("symbols.txt");
    std::cout << "Symbol table saved as symbols.txt" << std::endl;

    // 2. COMPLEMENT FST WITH LOOP
    StdVectorFst comp_fst;
    int s = comp_fst.AddState();
    comp_fst.SetStart(s);
    comp_fst.SetFinal(s, TropicalWeight::One());

    // Loop transitions: 0->1, 1->0
    comp_fst.AddArc(s, StdArc(id0, id1, TropicalWeight::One(), s));
    comp_fst.AddArc(s, StdArc(id1, id0, TropicalWeight::One(), s));

    comp_fst.SetInputSymbols(&syms);
    comp_fst.SetOutputSymbols(&syms);

    // Save complement FST
    comp_fst.Write("binary_complement.fst");
    std::cout << "Complement FST saved as binary_complement.fst" << std::endl;

    // 3. USER INPUT
    std::string input;
    std::cout << "Enter a binary string: ";
    std::getline(std::cin >> std::ws, input);

    StdVectorFst input_fst = StringToFst(input, syms);
    if (input_fst.NumStates() == 0) return 1;

    // 4. COMPOSE INPUT WITH COMPLEMENT FST
    StdVectorFst result;
    Compose(input_fst, comp_fst, &result);

    // Save result FST
    result.Write("binary_result.fst");
    std::cout << "Result FST saved as binary_result.fst" << std::endl;

    // 5. EXTRACT OUTPUT STRING
    std::string output = FstToString(result, syms);
    std::cout << "Original:   " << input << "\n";
    std::cout << "Complement: " << output << "\n";

    return 0;
}

