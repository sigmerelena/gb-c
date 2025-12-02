#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ZERO_FLAG_BYTE_POSITION 7
#define SUBTRACT_FLAG_BYTE_POSITION 6
#define HALF_CARRY_FLAG_BYTE_POSITION 5
#define CARRY_FLAG_BYTE_POSITION 4

typedef struct {
  bool zero;
  bool subtract;
  bool half_carry;
  bool carry;
} FlagsRegister;

typedef struct {
  uint8_t a;
  uint8_t b;
  uint8_t c;
  uint8_t d;
  uint8_t e;
  uint8_t f;
  uint8_t h;
  uint8_t l;
} Registers;

uint8_t flags_to_byte(FlagsRegister flags) {
  return ((flags.zero ? 1 : 0) << ZERO_FLAG_BYTE_POSITION) |
         ((flags.subtract ? 1 : 0) << SUBTRACT_FLAG_BYTE_POSITION) |
         ((flags.half_carry ? 1 : 0) << HALF_CARRY_FLAG_BYTE_POSITION) |
         ((flags.carry ? 1 : 0) << CARRY_FLAG_BYTE_POSITION);
}

FlagsRegister byte_to_flags(uint8_t byte) {
  FlagsRegister flags;
  flags.zero = ((byte >> ZERO_FLAG_BYTE_POSITION) & 0x1) != 0;
  flags.subtract = ((byte >> SUBTRACT_FLAG_BYTE_POSITION) & 0x1) != 0;
  flags.half_carry = ((byte >> HALF_CARRY_FLAG_BYTE_POSITION) & 0x1) != 0;
  flags.carry = ((byte >> CARRY_FLAG_BYTE_POSITION) & 0x1) != 0;
  return flags;
}

uint16_t get_bc(const Registers *regs) {
  return ((uint16_t)regs->b << 8) | regs->c;
}

void set_bc(Registers *regs, uint16_t value) {
  regs->b = (value >> 8) & 0xFF; // high byte
  regs->c = value & 0xFF;        // low byte
}

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
  fclose(f);

  free(rom);

  return 0;
}
