#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x401212;
       for (i=0x18;i>0;i--) {
           J = Byte(address);
           J = J ^ 0x79;
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

