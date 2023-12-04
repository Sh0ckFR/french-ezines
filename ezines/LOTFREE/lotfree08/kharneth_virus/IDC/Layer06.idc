#include <idc.idc>

static main() {
       auto J,address,i;
       address = 0x401393;
       for (i=0x361;i>0;i--) {
           J = Byte(address);
           J = J ^ i;
           J = J ^ 0xFF;     // Not J
           PatchByte(address,J);
           address++;
       }
       Message("Code décrypté\n");
}

