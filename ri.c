typedef void (*function)();
char program[32];
int main() {
  char *t = program;
  unsigned i, n;
  for (;;) {
    for (i = 3; i; i--) {
      n = getch() - '0';
      if (n > 7) (*(function)program)();
      *t = *t * 8 + n;
    }
    t++;
  }
}
