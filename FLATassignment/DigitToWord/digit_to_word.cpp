#include <iostream>
#include <string>
#include <vector>
#include <fst/fstlib.h>

using namespace fst;

// Convert FST to string by following the shortest path (needed after composition)
std::string FstToString(const StdVectorFst& fst, const SymbolTable& syms) {
    if (fst.NumStates() == 0) return "";

    // Use ShortestPath to ensure the composed FST is a linear path
    StdVectorFst shortest;
    ShortestPath(fst, &shortest); 

    std::string result = "";
    int state = shortest.Start();
    while (shortest.NumArcs(state) > 0) {
        ArcIterator<StdVectorFst> arc_it(shortest, state);
        const StdArc &arc = arc_it.Value();
        std::string sym = syms.Find(arc.olabel);
        
        if (!sym.empty() && sym != "<epsilon>") {
            // Add a space after each word, except possibly the last one.
            // A more complex FST could handle spacing better, but this programmatic approach works.
            if (!result.empty()) {
                result += " ";
            }
            result += sym;
        }
        state = arc.nextstate;
    }
    return result;
}

// Create linear FST from user string (the input tape)
StdVectorFst StringToFst(const std::string& str, const SymbolTable& syms) {
    StdVectorFst fst;
    fst.AddState();
    fst.SetStart(0);
    int cur = 0;

    for (char c : str) {
        int next = fst.AddState();
        int id = syms.Find(std::string(1, c));
        if (id == kNoSymbol) {
            std::cerr << "Unsupported symbol in input: " << c << std::endl;
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
    // Input symbols (digits) and Output symbols (words)
    SymbolTable isyms("isyms_digit");
    SymbolTable osyms("osyms_word");
    
    // Add epsilon symbols first (ID 0)
    isyms.AddSymbol("<epsilon>", 0);
    osyms.AddSymbol("<epsilon>", 0);

    // Mappings: [Digit Char] -> [Word String]
    std::vector<std::pair<std::string, std::string>> mappings = {
        {"0", "zero"}, {"1", "one"}, {"2", "two"}, {"3", "three"}, 
        {"4", "four"}, {"5", "five"}, {"6", "six"}, {"7", "seven"}, 
        {"8", "eight"}, {"9", "nine"}
    };

    // Build Digit-to-Word Converter FST
    StdVectorFst converter;
    
    // Use a single, self-looping state for arbitrary length strings
    int s = converter.AddState();
    converter.SetStart(s);
    converter.SetFinal(s, TropicalWeight::One());

    for (const auto& mapping : mappings) {
        // Add the digit character to the input symbol table
        int in_id = isyms.AddSymbol(mapping.first);
        
        // Add the word string to the output symbol table
        int out_id = osyms.AddSymbol(mapping.second);
        
        // Create the self-loop arc: Input=Digit, Output=Word, Next State=s
        converter.AddArc(s, StdArc(in_id, out_id, TropicalWeight::One(), s));
    }
    
    converter.SetInputSymbols(&isyms);
    converter.SetOutputSymbols(&osyms);

    // --- User Input ---
    std::string user_input;
    std::cout << "Enter a string of digits (e.g., 1234): ";
    std::getline(std::cin >> std::ws, user_input);

    // 1. Build FST for input string (using input symbols)
    StdVectorFst input_fst = StringToFst(user_input, isyms);
    if (input_fst.NumStates() == 0) return 1;

    // 2. Compose
    StdVectorFst composed;
    Compose(input_fst, converter, &composed);

    // 3. Extract output string
    std::string output = FstToString(composed, osyms);

    std::cout << "Original:  " << user_input << std::endl;
    std::cout << "Converted: " << output << std::endl;

    // --- Save FST and proper symbol tables (using WriteText for fstdraw compatibility) ---
    converter.Write("digit_to_word.fst");
    isyms.WriteText("digit.syms");
    osyms.WriteText("word.syms"); 

    std::cout << "FST saved as 'digit_to_word.fst'" << std::endl;
    std::cout << "Symbol tables saved for visualization." << std::endl;

    return 0;
}

