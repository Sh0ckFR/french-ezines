#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x401240;
       for (i=0x1B;i>0;i--) {
           J = Byte(address);
           J = J ^ 0x2B;
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

