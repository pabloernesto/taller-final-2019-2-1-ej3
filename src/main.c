#include <stdio.h>      // file wrangling
#include <arpa/inet.h>  // hton family

// fstat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DATAFILE "nros2bytes.dat"

int main(int argc, char **argv) {
  // keep a write pointer to the end of the file
  FILE *w_handle = fopen(DATAFILE, "r+");
  fseek(w_handle, 0, SEEK_END);

  // keep track of final file size
  FILE *r_handle = fopen(DATAFILE, "r");
  for (uint16_t x; fread(&x, sizeof(x), 1, r_handle); ) {
    uint16_t dx = ntohs(x);         // "decoded x", because of ~ENDIANNESS~
    // duplicated items take up extra space
    if (!(dx % 3))
      fseek(w_handle, sizeof(x), SEEK_CUR);
  }

  // copy into final positions
  // r_handle just finished reading all items, so it at EOF
  while (ftell(r_handle) != ftell(w_handle)) {
    uint16_t x;
    fseek(r_handle, -sizeof(x), SEEK_CUR);

    fread(&x, sizeof(x), 1, r_handle);
    fseek(r_handle, -sizeof(x), SEEK_CUR);          // reading moves the file

    uint16_t dx = ntohs(x);                         // "decoded x"
    const int ncopies = dx % 3 ? 1 : 2;
  
    fseek(w_handle, -sizeof(x), SEEK_CUR);
    fwrite(&x, 2, 1, w_handle);
    if (ncopies == 2) {
      // NOTE: writing two items with a single fwrite() does not result on two
      //       copies of x. fwrite writes x, keeps going, and writes the next
      //       thing on the stack (dx)
      fseek(w_handle, -2*sizeof(x), SEEK_CUR);      // writing moves the file
      fwrite(&x, 2, 1, w_handle);
    }
    fseek(w_handle, -sizeof(x), SEEK_CUR);          // writing moves the file
  }
}
