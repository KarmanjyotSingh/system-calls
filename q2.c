#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
/* bytes per write to be adjusted according to the file size rest done*/
#define bytesPerRead 1024 
#define bytesPerWrite 1024
/* utility function to print the error message */
void showerror(char *errmsg)
{
    write(1, errmsg, strlen(errmsg));
}
/* utility function for printing the % file transferred onto the terminal*/
void printPercentage(off_t file_size, int size_moved)
{
    char *bufferstring = malloc((15) * sizeof(char));
    //system("clear");
    // printf("file size :%lld\n bytesMoved : %lld\n ", file_size, size_moved);
    sprintf(bufferstring, "\r%.2f%%", (((double)size_moved / (double)file_size)) * 100);
    write(1, bufferstring, strlen(bufferstring));
    fflush(stdout);
    free(bufferstring);
}
/*utility function to reverse parts of string*/
void reversearr(char *org, char *rev, int moveSize)
{
    for (int i = 0; i < moveSize; i++)
    {
        rev[i] = org[moveSize - i - 1];
    }
    rev[moveSize] = '\0';
    // fixing the null character at the end of the string //
}
char *getFileName(char *filePath)
{
    if (filePath == NULL) // if an invalid file path is provided
        return NULL;
    char *x = (char *)malloc((strlen(filePath) + 1) * sizeof(char));
    strcpy(x, filePath);
    char *cpy = strstr(x, "/");
    if (cpy == NULL) // file name itself provided ==> it is in the current working directory
        return x;
    else
    {
        int lft = 0;
        do
        {
            lft = strlen(cpy) + 1;
            // printf("lft is : %d\n", lft); //
            x = &x[strlen(x) - lft + 2]; // rearranging the pointer to the string (pointer de-referencing) //
            // printf("x is : %s\n", x); //
            cpy = strstr(x, "/");
            // printf("cpy is : %s\n", cpy); //
        } while (cpy); // until cpy doesnt become null //
    }
    return x;
}
char *getPath(char *filePath)
{
    if (filePath == NULL)
        return NULL;
    char *x = (char *)malloc((strlen(filePath) + 1) * sizeof(char));
    strcpy(x, filePath);
    char *fileName = getFileName(x);
    char *chk = strstr(x, fileName);
    if (chk)
    {
        strcpy(chk, "");
        // .... printf("%s\n", x); ....  to check //
        return x;
    }
    else
        return NULL;
}
int main(int argc, char *argv[])
{
    // .... in case input isnt provided .... //
    if (argc != 4)
    {
        if (argc == 1) // incase of only "./a.out"
        {
            char errmsg[] = "error: no input provided ! \n";
            showerror(errmsg);
        }
        else //in case wrong number of arguments are provided
        {
            char errmsg[] = "error: wrong number of inputs provided ! \n";
            showerror(errmsg);
            // write(FILE_DESC,ERR_MSG,NO-BYTES-TO-SHOW/WRITE) //
        }
        return 1; // exit the program //
    }

    /* ....Correct number of arguments provided.... */

    /* ....Correct number of arguments provided.... */
    int src = open(argv[1], O_RDONLY);
    // ....opening the source file only in the read mode from the input file path provided .... //
    /* .... lseeek is used for random access to any part of the program file ....
       .... lseek(int fd, int offset, int origin) - returns the offset value from the beginning of the file ....
    */
    if (src < 0)
    {
        char errmsg[] = "invalid input file!\n"; // show the error
        showerror(errmsg);
    }
    int num_prts = argv[2][0] - '0';
    if (num_prts < 0 || num_prts > 9)
    {
        char errmsg[] = "invalid integer input given\n"; // show the error
        showerror(errmsg);
        return 1;
    }
    int part_no = argv[3][0] - '0';
    if (part_no < 0 || part_no > 9)
    {
        char errmsg[] = "invalid integer input given\n"; // show the error
        showerror(errmsg);
        return 1;
    }

    // .... valid file provided, main code starts .... //
    char *inputFile = getFileName(argv[1]);
    char *inputPath = getPath(argv[1]);

    struct stat st;
    stat(argv[1], &st);
    long long int sz_src = st.st_size;
    int strlenfile = strlen(inputFile); // length of the file name

    /* .... to create Assignment directory in the current working directory .... */
    char dirname[] = "Assignment";
    int dir = mkdir(dirname, 0700);
    // printf("%d\n", dir);
    // in case of failure, -1 ==> directory already exists 0 ==> success new directory created
    if (dir == 1)
    {
        perror("error");
        return 1; // exit the program
    }
    /* directory created */
    char *outputFile = (char *)malloc((strlenfile + 3) * sizeof(char));
    sprintf(outputFile, "Assignment/2_%s", inputFile);
    //printf("%s", outputFile);
    int dst = open(outputFile, O_CREAT | O_RDWR | O_TRUNC, 0600);
    // create an empty file  //
    if (dst < 0)
        perror("error");

    /* Output file created*/
    lseek(src, -1, SEEK_END); // ignoring the null character at the end of the file, and setting the file pointer //
    lseek(dst, 0, SEEK_SET);  // setting the file pointer to the beginning of  the file

    int sz_block = sz_src / num_prts;        // dividing file into blocks of sizes
    int num_block = sz_block / bytesPerRead; // num of blocks
    int sz_mv = sz_block - num_block * bytesPerRead;
    int byteMoved = 0;
    char *org = (char *)malloc((bytesPerRead + 1) * sizeof(char));
    char *wrt = (char *)malloc((bytesPerRead + 1) * sizeof(char));
    // incase the block size is too large toh we shall be reading this in chunks //
    int x = 0;
    if (part_no > num_prts || part_no < 1)
    {
        showerror("Invalid Input , 1 <= part_number <= number_of_parts\n");
        return 1;
    }
    while (1)
    {
        printPercentage(sz_block, byteMoved);
        // print % on the terminal//
        lseek(src, (part_no - 1) * sz_block + (num_block * bytesPerRead), SEEK_SET);
        // position the file pointer //
        int r = read(src, org, sz_mv);
        // read from the file //
        if (r != 0)
            reversearr(org, wrt, sz_mv);
        // reverse the read string //
        if (r > 0)
            write(dst, wrt, sz_mv);
        // write to the file //
        byteMoved += r;
        num_block--;
        if (num_block == -1) // finished writing all the blocks //
            break;
        sz_mv = bytesPerRead;
    }
    printPercentage(sz_block, byteMoved);
    int clsrc = close(src);
    if (clsrc == -1)
    {
        perror("");
        return 1;
    }
    int clsdst = close(dst);
    if (clsdst == -1)
    {
        perror("");
        return 1;
    }
    return 0;
}
