#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x401271;
       for (i=0x1C;i>0;i--) {
           J = Byte(address);
           J = J ^ 0xA2;
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

