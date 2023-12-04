#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x400042;
       for (i=0x9;i>0;i--) {
           J = Byte(address);
           J = J ^ 0xC5;
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

