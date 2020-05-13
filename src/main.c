#include <stdio.h>      // file wrangling
#include <arpa/inet.h>  // hton family

/* ftruncate */
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv) {
  FILE *r_handle = fopen("nros2bytes.dat", "r");
  FILE *w_handle = fopen("nros2bytes.dat", "r+");

  int x;
  while (fread(&x, 2, 1, r_handle)) {
    int x_int = ntohs(x);
    if (x_int % 3)
      continue;
    fwrite(&x, 2, 1, w_handle);
  }
  ftruncate(fileno(w_handle), ftell(w_handle));
}
