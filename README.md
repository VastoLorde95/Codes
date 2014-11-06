Codes
=====
These are pieces of code and algorithms I have implemented which have helped me solve some competitive programming challenges. Feel free to use them.

Bits
======
Small pieces of code that perform small operations.

1. palindrome.cpp = given a number, it generates the next succesive palindrome in the number series

String Arithmetic
==================

string_arithmetic.cpp
This is a C++ implementation that handles numbers that are bigger than 64 bits long. It works by storing the numbers
as C++ strings. Custom functions have been defined that perform basic arithmetic operations like additon and subtraction
on the given numbers.

The following functions have been implemented for string arithmetic in string_operations.cpp:

1. add = adds the numbers in the two strings to produce a new string
2. sub = subtracts the numbers in the two strings
3. divde_by_2 = halves the value of the number in the given string
4. naive_multiply = grade school multiplication algo to multiply two string numbers, does not work for negative numbers
5. multiply = Karatsuba multiplication implementation to give the product of two string numbers, does not work for negative numbers.



*********************
Some Observations from this library:

If some you test it out then you wil realize that string is a highly ineffiecient data structure for storing data on which we want to compute arithmetic operations. 

Python 2.7 takes ~0.00006 seconds to multiply two numbers of 5000 and 1000 digits.
This library takes ~2.6 seconds for the same two numbers.

Python 2.7 takes ~0.000002 seconds to add two numbers of 5000 and 1000 digits.
This library takes ~0.006 seconds for the same two numbers.

That is ridiculously slow!! 
I will be studying how different libraries handle numbers greater than 64 bits.
I will be making a library in C++ to handle very large numbers like this and will be performing an analysis of C++ vs Python 2.7.

I speculate Python to be faster since it is written in C
