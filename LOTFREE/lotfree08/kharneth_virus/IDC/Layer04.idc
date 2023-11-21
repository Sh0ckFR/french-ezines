#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x401104;
       for (i=0x92;i>0;i--) {
           J = Byte(address);
           J = J + i;
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

