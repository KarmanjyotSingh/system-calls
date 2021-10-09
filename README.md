# system-calls

____

>Implemented system calls in C as a programming assignment of 
>**course** : Operating Systems and Networks ( Monsoon 21 )

## Given Task

- Given an input file , to produce an output file , reversing the input file , using only the system calls in C , the use of `printf` , `scanf` , and other standard `C-library` functions not being permitted.

- The program also outputs the percentage of the task completed.

- Commands used : `read` , `write` , `lseek` , `stat` , `free` , `fflush` , `open` , `mkdir` , `perror` , etc.

## Features 

- Works with both relative and absolute paths for the given input file path.
  
- Handles files quite greater than the actual **RAM** size.

## Execution Instructions :

### q1 

>firstly compile the main program using `gcc q1.c` to create an executable `a.out`.
>
>In your console , execute the command `./a.out <input_file_path>/input_file.txt`.
>
>The reversed file gets created in the directory `Assignment` directory created in the `cwd` as `1_input.txt`.

### q2 

>firstly compile the main program using `gcc q2.c` to create an executable `a.out`.
>
>In your console , execute the command `./a.out <input_file_path>/input_file.txt <num_partitions>  <nth_partition>`.
>
>The file with only given partition block reversed gets created in the directory `Assignment` directory created in the `cwd` as `2_input.txt`.
>
>
>**Assumptions**
>1. The file size was assumed to be divisible by the `<num_partitions>` 
>2. both `<num_partitions>` and ` <nth_partition>` were assumed to be single digit integers.

____
