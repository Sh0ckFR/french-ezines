#include <idc.idc>

#define RolB(x,n) x << n | (~(0xFF<<n) & x >> (8-n))
#define RorB(x,n) x << (8-n) | (~(0x80 >> n-1) & x>>n)

static main() {
    	 auto J,address,i;	
    	 address = 0x401073;
    	 for (i=0x16;i>0;i--) {
	 		 		 J = Byte(address);
					 J = J - 0x45;
					 J = J & 0xFF;
					 J = RorB(J, 5);
					 J = J ^ 0x56;
					 J = J - i;
					 J = J + 0x78;
					 J = J + 0xB2;
					 J = J + 0x68;
					 J = J ^ 0xFF; // Not
					 J = J ^ i;
					 J = J & 0xFF;
					 J = RolB(J, 4);
					 J = J - i;
        	 PatchByte(address,J);
        	 address++;
   	 }
    Message("Code décrypté\n");
}

