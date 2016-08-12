#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

int main(int argc, char **argv) {
   FILE *other= 0;
   char const *error= 0, *oname;
   if (argc >= 2) {
      int argind;
      char const *arg;
      if (strlen(arg= argv[argind= 1]) >= 2 && *arg == '-') {
         if (strcmp(arg, "--")) {
            (void)fprintf(stderr, error= "Unknown option %s!\n", arg);
            goto cleanup;
         }
         ++argind;
      }
      if (argind + 1 != argc) {
         error=
            "Exactly one argument is required: The name of a binary file to"
            " bitwise XOR with binary stdin, yielding binary stdout."
         ;
         goto complain;
      }
      if (!(other= fopen(oname= argv[argind], "rb"))) {
         (void)fprintf(
            stderr, error= "Could not open '%s' for reading!\n", oname
         );
         goto cleanup;
      }
   }
   {
      int c;
      while ((c= getchar()) != EOF) {
         int co;
         if ((co= getc(other)) == EOF) {
            if (ferror(other)) {
               (void)fprintf(
                  stderr, error= "Error reading from '%s'!\n", oname
               );
               goto cleanup;
            }
            (void)fprintf(
                  stderr
               ,  error=
                  "Ran out of XOR-data: '%s' is shorter than standard input"
                  " stream!\n"
               , oname
            );
            goto cleanup;
         }
         assert((unsigned char)c <= UCHAR_MAX);
         assert((unsigned char)co <= UCHAR_MAX);
         c^= co;
         if (putchar(c) != c) {
            error= "Error writing to standard output stream!";
            goto complain;
         }
      }
      if (ferror(stdin)) {
         error= "Error reading from standard input stream!";
         goto complain;
      }
   }
   if (fflush(0)) {
      write_error:
      error= "Write error!";
      complain:
      (void)fprintf(stderr, "%s\n", error);
   }
   cleanup:
   if (other) {
      FILE *fh= other; other= 0;
      if (fclose(fh)) {
         (void)fprintf(stderr, error= "I/O error for '%s'!\n", oname);
         goto cleanup;
      }
   }
   return error ? EXIT_FAILURE : EXIT_SUCCESS;
}
