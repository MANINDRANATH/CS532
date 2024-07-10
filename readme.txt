Project Title: Listing Sorter

Description:
This program reads data from a CSV file containing listing information, sorts the data by different attributes, and writes the sorted data back to new CSV files. The data is sorted by host name and price.

Files:
- sort.c: Contains the source code for the listing sorting application.
- Makefile: Contains commands for compiling the source code and cleaning up binary files.
- sorted_by_host_name.csv: Output file containing listings sorted by host name.
- sorted_by_price.csv: Output file containing listings sorted by price.

How to Compile and Run:
1. To compile the program, make sure you have GCC installed on your system.
2. Navigate to the directory containing the source code and Makefile.
3. Run the command 'make' to compile the program. This will create an executable named 'sort'.
4. To run the program, use the command './sort'.
5. To clean up compiled objects and executables, use the command 'make clean'.

Dependencies:
- GCC Compiler
- Standard C library

Usage:
After compiling, run the program with './sort' in the terminal. Ensure 'listings.csv' is present in the same directory as the executable for it to function correctly.

Output:
The program will create two files:
- 'sorted_by_host_name.csv' which contains the listings sorted by host name.
- 'sorted_by_price.csv' which contains the listings sorted by price.

Note:
The input CSV file should be named 'listings.csv' and must be located in the same directory as the executable. The CSV should be properly formatted without any corrupt data.
