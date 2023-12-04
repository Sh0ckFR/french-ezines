#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x4010B9;
       for (i=0x2F;i>0;i--) {
           J = Byte(address);
           J = J ^ 0x45;
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

