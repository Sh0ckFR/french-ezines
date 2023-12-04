#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x40122A;
       for (i=0x16;i>0;i--) {
           J = Byte(address);
           J = J ^ 0xC6;
           J = J + i;
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

