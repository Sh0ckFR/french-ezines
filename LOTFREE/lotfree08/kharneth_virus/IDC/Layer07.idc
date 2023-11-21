#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x4010E8;
       for (i=0x1C;i>0;i--) {
           J = Byte(address);
           J = J - 0x75;
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

