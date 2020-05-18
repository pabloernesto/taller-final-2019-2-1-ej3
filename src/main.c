#include <stdio.h>      // file wrangling
#include <arpa/inet.h>  // hton family

// fstat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DATAFILE "nros2bytes.dat"

static inline int is_divisible(uint16_t x) {
  x = ntohs(x);
  return x % 3 == 0;
}

static inline uint16_t read_still(FILE *f) {
  uint16_t x;
  fread(&x, sizeof(x), 1, f);
  fseek(f, -sizeof(x), SEEK_CUR);
  return x;
}

static inline void write_still(FILE *f, uint16_t x) {
  fwrite(&x, sizeof(x), 1, f);
  fseek(f, -sizeof(x), SEEK_CUR);
}

int main(int argc, char **argv) {
  // keep a write pointer to the end of the file
  FILE *w_handle = fopen(DATAFILE, "r+");
  fseek(w_handle, 0, SEEK_END);

  // keep track of final file size
  FILE *r_handle = fopen(DATAFILE, "r");
  for (uint16_t x; fread(&x, sizeof(x), 1, r_handle); ) {
    // duplicated items take up extra space
    if (is_divisible(x))
      fseek(w_handle, sizeof(x), SEEK_CUR);
  }

  // copy into final positions
  // r_handle just finished reading all items, so it at EOF
  while (ftell(r_handle) != ftell(w_handle)) {
    uint16_t x;
    fseek(r_handle, -sizeof(x), SEEK_CUR);
    x = read_still(r_handle);

    const int ncopies = is_divisible(x) ? 2 : 1;
  
    for (int i = 0; i < ncopies; i++) {
      fseek(w_handle, -sizeof(x), SEEK_CUR);
      write_still(w_handle, x);
    }
  }
}
