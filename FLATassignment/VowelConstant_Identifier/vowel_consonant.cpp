#include <iostream>
#include <string>
#include <cctype>
#include <fst/fstlib.h>

using namespace fst;

int main() {
    // 1. CREATE SYMBOL TABLES
    SymbolTable isyms("isyms");
    SymbolTable osyms("osyms");
    isyms.AddSymbol("<epsilon>", 0);
    osyms.AddSymbol("<epsilon>", 0);

    // Add vowels and consonants
    int v_id = osyms.AddSymbol("v");
    int c_id = osyms.AddSymbol("c");

    for (char ch = 'a'; ch <= 'z'; ++ch) {
        isyms.AddSymbol(std::string(1, ch));
    }

    // Save symbol tables for visualization
    isyms.WriteText("isyms.txt");
    osyms.WriteText("osyms.txt");

    // 2. BUILD THE FST
    StdVectorFst fst;
    int start_state = fst.AddState();
    int final_state = fst.AddState();
    fst.SetStart(start_state);
    fst.SetFinal(final_state, TropicalWeight::One());

    std::string vowels = "aeiou";
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        int in_id = isyms.Find(std::string(1, ch));
        int out_id = (vowels.find(ch) != std::string::npos) ? v_id : c_id;
        fst.AddArc(start_state, StdArc(in_id, out_id, TropicalWeight::One(), final_state));
    }

    fst.SetInputSymbols(&isyms);
    fst.SetOutputSymbols(&osyms);

    // Save FST
    fst.Write("vowel_consonant.fst");
    std::cout << "FST saved as vowel_consonant.fst" << std::endl;

    // 3. GET USER INPUT
    char user_char;
    std::cout << "Enter a single letter (a-z): ";
    std::cin >> user_char;
    user_char = tolower(user_char);

    int input_id = isyms.Find(std::string(1, user_char));
    if (input_id == kNoSymbol) {
        std::cout << "Invalid input!" << std::endl;
        return 1;
    }

    // 4. FIND OUTPUT
    std::string output;
    ArcIterator<StdVectorFst> it(fst, start_state);
    for (; !it.Done(); it.Next()) {
        const StdArc& arc = it.Value();
        if (arc.ilabel == input_id) {
            output = osyms.Find(arc.olabel);
            break;
        }
    }

    std::cout << "Output: " << output << std::endl;

    return 0;
}

