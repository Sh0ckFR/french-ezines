#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x40128D;
       for (i=0x16;i>0;i--) {
           J = Byte(address);
           J = J - 0x45;
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

