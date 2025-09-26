# FLATassignment
FST using CPP for assignment

---

# Finite State Transducer (FST) Projects

This repository contains several FST-based projects implemented in C++ using [OpenFST](http://www.openfst.org/). Each project demonstrates the use of **Finite State Transducers** for different computational tasks.

---

## Projects Included

1. **Digit to Word Converter**

   * Converts a string of digits (`0-9`) into their corresponding English words.
   * Example: `123` → `one two three`.
   * Generates a **visualizable FST** using OpenFST tools.

2. **Roman Numeral to Arabic Converter**

   * Converts a Roman numeral string to its Arabic (decimal) equivalent.
   * Example: `XIV` → `14`.
   * Includes optional FST visualization for Roman numeral symbols.

3. **Palindrome Checker over {a, b}**

   * Checks if a string containing only `a` and `b` is a palindrome.
   * Example: `abba` → `Palindrome`, `abab` → `Not a palindrome`.
   * Can generate an FST representing input and transitions for visualization.

4. **Vowel/Consonant Identifier**

   * Identifies vowels and consonants in a given string.
   * Example: `hello` → `h:C e:V l:C l:C o:V`
   * Can be visualized using an FST to represent transitions.

5. **Binary Complement Generator**

   * Generates the 1’s complement of a binary string.
   * Example: `1010` → `0101`
   * Uses FST transitions to flip each binary digit.

---

## Requirements

* C++ compiler (`g++`)
* [OpenFST](http://www.openfst.org/)
* Graphviz (`dot`) for PDF visualization

---

## Compilation

Compile each program separately with the OpenFST library:

```bash
g++ digit_to_word.cpp -o digit_to_word -lfst
g++ roman.cpp -o roman -lfst
g++ palindrome.cpp -o palindrome -lfst
g++ vowel_consonant.cpp -o vowel_consonant -lfst
g++ binary_complement.cpp -o binary_complement -lfst
```

---

## Running the Programs

### 1. Digit to Word Converter

```bash
./digit_to_word
```

Example:

```
Enter a string of digits (e.g., 1234): 234
Original: 234
Converted: two three four
```

Generate PDF of FST:

```bash
fstdraw --isymbols=digit.syms --osymbols=word.syms digit_to_word.fst | dot -Tpdf > digit_to_word.pdf
xdg-open digit_to_word.pdf
```

---

### 2. Roman Numeral to Arabic

```bash
./roman
```

Example:

```
Enter a Roman numeral (e.g., XIV): XIX
Arabic numeral: 19
```

Generate PDF of FST:

```bash
fstdraw --isymbols=roman_syms.txt --osymbols=roman_syms.txt roman.fst | dot -Tpdf > roman.pdf
xdg-open roman.pdf
```

---

### 3. Palindrome Checker over {a, b}

```bash
./palindrome
```

Example:

```
Enter a string over {a, b}: abba
Palindrome
```

Generate PDF of FST:

```bash
fstdraw --isymbols=palindrome_syms.txt --osymbols=palindrome_syms.txt palindrome.fst | dot -Tpdf > palindrome.pdf
xdg-open palindrome.pdf
```

---

### 4. Vowel/Consonant Identifier

```bash
./vowel_consonant
```

Example:

```
Enter a letter: o
output : v
```

Generate PDF of FST:

```bash
fstdraw --isymbols=vowel_consonant_syms.txt --osymbols=vowel_consonant_syms.txt vowel_consonant.fst | dot -Tpdf > vowel_consonant.pdf
xdg-open vowel_consonant.pdf
```

---

### 5. Binary Complement Generator

```bash
./binary_complement
```

Example:

```
Enter a binary string: 1010
Complement: 0101
```

Generate PDF of FST:

```bash
fstdraw --isymbols=binary.syms --osymbols=binary.syms binary_complement.fst | dot -Tpdf > binary_complement.pdf
xdg-open binary_complement.pdf
```

---

This README covers all the projects  focusing on the terminal output and FST visualization.

---

