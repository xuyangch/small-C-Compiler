# SmallC Compiler
A compiler that converts a C-like program into MIPS assembly code. It has 6 kinds of optimization technique including arithmetic identities, strength reduction, constant folding, constant propagation, assign elimination, and unreachable function elimination for code generation.
# Build the Compiler
To build the project, you need to goto the project directory and type the following in command line:
```
bison -dv smallc.y
gcc -c smallc.tab.c
flex smallc.lex
gcc -c lex.yy.c
gcc -o scc smallc.tab.o lex.yy.o -lm
```
??: You cannot use yacc to build the smallc.y because I use some of the unique feature of bison in my project!
# Run the Compiler
To run the smallc compiler, simply type:
`./scc "Source file name" "Output file name"`
The Output file name should have the suffix ".s". Then the three-address-code will be generated to a file called InterCode and the corresponding MIPS code will be generated to Output file.
To run the MIPS code, choose Load File in SPIM simulator and run.

# Examples
I provide three types of example programs, all of which have suffix ".sc".
## Error Test
example for error test is in the folder error_test, to run the error test program, simply type:
`
./scc error_test/error_N.sc "Output file name"
`
Here N ranges from 1~9 representing the 9 semantic analysis in the report.

## Optimization Test
Example for optimization test is in the folder opt_test, to run the optimization test program, simply type:
`
./scc opt_test/optim_N.sc "Output file name"
`
Here N ranges from 1~6 representing the 6 optimizations in the report

## Sample Test
Example for sample test is in the folder sample_test, to run the optimization test program, simply type:
`
./scc sample_test/"Input file name" "Output file name"
`
Here Input file name are:
| Input File Name | Description |
| ------ | ------ |
| arith_test.sc | For arithmetic calculation. Just run the file and SPIM will give the answer. |
| test_struct.sc | For structure test. Just run the file and SPIM will give the answer. |
| hex_oct_test.sc | For hex and oct number test. Just run the file and SPIM will give the answer. |
| eight_queens.sc | This program calculates the number of solutions of 8 queens problem. Just run the file and SPIM will give the answer. |
| fib.sc | This program calculates the n fibonacci number. You can enter n in SPIM and see the result. |
| hanoi.sc | This program prints the solution for n hanoi tower. You can enter n in SPIM and see the result. |
| loop_test.sc | for loop test. Just run the file and SPIM will give the answer. |
| sort.sc | This program sorts an array and print the sorted array. Just run the file and SPIM will give the answer. |

# Documents
A detailed description of the compiler can be found [here](https://github.com/chyacinth/small-C-Compiler/blob/master/project_report.pdf)