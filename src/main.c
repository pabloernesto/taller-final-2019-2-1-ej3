#include <stdio.h>      // file wrangling
#include <arpa/inet.h>  // hton family

// fstat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DATAFILE "nros2bytes.dat"

int main(int argc, char **argv) {
  // read the entire thing to memory
  struct stat stats_buf;
  stat(DATAFILE, &stats_buf);

  const int n = stats_buf.st_size / 2;
  uint16_t file_buf[n];

  FILE *file = fopen(DATAFILE, "r");
  fread(file_buf, 2, n, file);
  fclose(file);

  // write with duplication
  file = fopen(DATAFILE, "w");
  uint16_t *ptr = file_buf;
  uint16_t *end = file_buf + n;
  for (; ptr != end; ptr++) {
    uint16_t x = ntohs(*ptr);
    fwrite(ptr, 2, 1, file);
    if (!(x % 3))
      fwrite(ptr, 2, 1, file);
  }
}
