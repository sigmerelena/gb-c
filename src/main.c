#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "where rom?, use: %s game.rom\n", argv[0]);
    return 1;
  }
  FILE *f = fopen(argv[1], "rb");
  if (!f) {
    fprintf(stderr, "error opening rom\n");
    return 1;
  }
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);

  unsigned char *rom = malloc(size);
  if (!rom) {
    fprintf(stderr, "malloc failed\n");
    fclose(f);
    return 1;
  }

  if (fread(rom, 1, size, f) != size) {
    fprintf(stderr, "fread failed\n");
    free(rom);
    fclose(f);
    return 1;
  }
  printf("loaded rom %s (%ld bytes)\n", argv[1], size);

  fseek(f, 0x0104, SEEK_SET);
  for (int i = 0; i < 48; i++) {
    int byte = fgetc(f);
    if (byte == EOF)
      break;
    printf("%02X ", byte);
    if ((i + 1) % 16 == 0)
      printf("\n");
  }
  fclose(f);
  printf("\n");
  free(rom);

  return 0;
}
