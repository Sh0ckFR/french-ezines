#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x401196;
       for (i=0x7C;i>0;i--) {
           J = Byte(address);
           J = J + 0x33;
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

