#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x4012A3;
       for (i=0xF0;i>0;i--) {
           J = Byte(address);
           J = J + 0xD6;
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

