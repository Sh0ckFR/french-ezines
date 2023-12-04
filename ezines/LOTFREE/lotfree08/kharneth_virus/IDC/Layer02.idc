#include <idc.idc>

#define ROL(x,n) x<<n|(~(0xFFFFFFFF<<n)&x>>32-n)
#define RorB(x,n) x<<(8-n)|(~(0x80>>n-1)&x>>n)

static main() {
    	 auto J,address,count;	
    	 address = 0x401089;
    	 for (count = 0x30; count>0; count--) {
	 		 		 J = Byte(address);
					 J = ~J;
					 J = J + 0x33;
					 J = J - 0x81;
					 J = J - 0xC3;
					 J = ~J;
					 J = J & 0xFF;
					 J = RorB(J, 2);
					 J = J ^ count;
        	 PatchByte(address,J);
        	 address++;
   	 }
    Message("Code décrypté\n");
}

