#include <stdio.h>      // file wrangling
#include <arpa/inet.h>  // hton family

// fstat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DATAFILE "nros2bytes.dat"

// copy the character at pos into (pos + offset)
// this operation does not preserve the current file position
static void scoot_byte(FILE *file, size_t pos, size_t offset) {
  fseek(file, pos, SEEK_SET);
  char c = fgetc(file);
  offset--;                       // fgetc advances the file by one position
  fseek(file, offset, SEEK_CUR);
  fputc(c, file);
}

// copy [pos, pos+n) to [pos+offset, pos+n+offset), using constant memory
// this operation does not preserve the current file position
void scoot(FILE *file, size_t n, size_t pos, size_t offset) {
  while (n > 0) {
    n--;
    scoot_byte(file, pos+n, offset);
  }
}

int main(int argc, char **argv) {
  struct stat stats_buf;
  stat(DATAFILE, &stats_buf);
  int n = stats_buf.st_size / 2;    // how many unprocessed shorts there are

  FILE *file = fopen(DATAFILE, "r+");
  for (; n; n--) {
    uint16_t x;
    fread(&x, sizeof(x), 1, file);
    uint16_t dx = ntohs(x);         // standing for "decoded x"
    if (!(dx % 3)) {
      size_t pos = ftell(file);
      size_t bytes_left = (n-1) * sizeof(x);
      scoot(file, bytes_left, pos, sizeof(x));
      fseek(file, pos, SEEK_SET);
      fwrite(&x, 2, 1, file);
    }
  }
}
