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
- duma similar std::cout.
```
duma "Hello world\n"      // print string
duma "Value = " a "\n"    
// print variable concatenated with string
```
- `duma` = start printing.
- You can concatenate strings and variables by writing them side by side, no `+` or `<<` needed.

---

## 4. Input
dume = input from user, similar to `std::cin`.

### Single variable:
int a

duma "Enter value for a: "
dume a

duma "You entered: " a "\n"

### Multiple variables (using duma):
```
int x
int y
int z

duma "Enter 3 values: "
dume x,y,z

duma "Values are: " x " " y " " z "\n"

- You can input **1 variable** or **multiple variables** separated by `,`.
- Variables will receive values in the order they are listed.
```
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
    int a
    int b
    int c

    duma "Enter 3 values for a, b, c: "
    dume a,b,c

    int sum = sumab(a, b)

    duma "Condition a > b = " (a > b ? "true" : "false") "\n"

    duma "Result of " a " + " b " = " sum "\n"
    duma "Third value entered: " c "\n"

    duma "Other test: " (a * b) "\n"
    duma "Bye!\n"
}
- Running this program will output something like:
Enter 3 values for a, b, c:
2 5 9
Condition a > b = false
Result of 2 + 5 = 7
Third value entered: 9
Other test: 10
Bye!

---

## 9. Notes
1. No need for `;` at the end of lines.
2. `duma` = output, `dume` = input.
3. Concatenate variables and strings by writing them side by side, or use `(a + b)` to calculate first.
4. Functions and operators are similar to C++ but C-- is more flexible.
5. Comments use `//` or `/* ... */`.
6. `dume` can take **1 variable** or **multiple variables separated by `,`**.
