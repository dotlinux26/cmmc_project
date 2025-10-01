# C-- (C Minus Minus) A FUNNY PROJECT OF C++

**C-- (C minus minus)** is an experimental open-source project inspired by C++.
The goal is to simplify syntax and common operations, making the language more beginner-friendly while reducing common errors such as missing semicolons.

---

## Overview
C-- keeps the power of C++ but tries to remove unnecessary complexity. 
The project is currently in **beta**, so bugs are expected. Feedback and contributions from the community are welcome.

---

## Features
- Simplified syntax based on C++.
- Some common mistakes (like missing `;`) are tolerated.
- Open for community contribution and improvements.

---

## Usage
The compiler auto translates `.cmm` files into `.cpp`, then builds them into executables.

###Easy to use with this syntax:
```
./cmmc input.cmm [--run] [--keep-cpp]
```
###Options:
- `--keep-cpp`: keep the generated `.cpp` file.
- `--run`: run the compiled binary immediately.
###Note:
./cmmc : for linux
./cmmc.exe : for window

###Self build: 
- Linux 
```
g++ cmmc.cpp -o cmmc
```
```
x86_64-w64-mingw32-g++ cmmc.cpp -o cmmc.exe -static-libgcc -static-libstdc++
```
(Cross-compile for Windows)
- Window
```
g++ cmmc.cpp -o cmmc.exe
```
---

## Download v1.0.0

You can download the first stable release (v1.0.0) of C-- compiler:

- [Release v1.0.0](https://github.com/dotlinux26/cmmc_project/releases/tag/v1.0.0)

---

## Screenshots / Demonstration
![Compiler Demo](docs/demo.png)
![Compiler Demo](docs/demo2.png)

---

## Contributing
This project is **open source** and welcomes:
- Bug reports
- Feature suggestions
- Pull requests for improvements

---

## License

-This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).


# C-- Language Syntax Guide

## 1. Variable Declaration
int a = 5        // integer
float b = 3.2    // floating point
string s = "hi"  // string
- No need for `;` at the end of the line.
- Data types are similar to C++ (`int`, `float`, `string`...).

---

## 2. Functions
int sumab(int a, int b) {
    return a + b
}
- Functions take parameters and return a value.
- No need for `;` after `return`.

---

## 3. Console Output
duma "Hello world\n"      // print string
dume a                    // print variable value
duma "Value = " a b "\n"  // concatenate string + variable directly
- `duma` = start printing.
- `dume` = print variable value.
- Concatenate string and variable by writing them side by side, no `+` or `<<` needed.

---

## 4. Input
- Basic C-- has no standard input, you can write custom input functions.
- Example of printing a prompt + variable:
duma "input a:" dume a

---

## 5. Operators and Expressions
a + b
a - b
a * b
a / b
a % b
a > b
a < b
a == b
a != b
- Standard arithmetic and comparison operators.
- Ternary operator is supported:
duma "a > b = " (a > b ? "true" : "false") "\n"

---

## 6. String Concatenation
duma "Hello " "world" "!"        // => Hello world!
duma "Result: " (a + b) "\n"     // print arithmetic result
- Consecutive strings automatically concatenate.
- Variables or expressions placed next to strings will be printed.

---

## 7. Comments
// single-line comment
/* 
   multi-line comment
*/
- Comments follow standard C/C++ syntax.

---

## 8. Complete Example Program
#include <iostream>

int sumab(int a, int b) {
    return a + b
}

int main() {
    int a = 6
    int b = 4

    duma "input a:" dume a
    duma "input b:" dume b

    int sum = sumab(a, b)

    duma "condition a > b = " (a > b ? "true" : "false") "\n"

    duma "Result of " a " + " b " = " sum "\n"

    duma "Other test: " (a * b) "\n"
    duma "Bye!\n"
}
- Running this program will output:
input a:6
input b:4
condition a > b = true
Result of 6 + 4 = 10
Other test: 24
Bye!

---

## 9. Notes
1. No need for `;` at the end of lines.
2. `duma` / `dume` replace `std::cout`.
3. Concatenate variables and strings by writing them side by side, or use `(a + b)` to calculate first.
4. Functions and operators are similar to C++ but C-- is more flexible.
5. Comments still use `//` or `/* ... */`.


