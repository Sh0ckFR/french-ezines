// Decompiled by Jad v1.5.8e. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.geocities.com/kpdus/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   DiffieHellman.java

package crypto;

import java.math.BigInteger;
import java.util.Random;

public class DiffieHellman
{

    public DiffieHellman()
    {
        p = new BigInteger("72549841864194078899726837116892585496955575260843302046062234899404431414511");
        g = new BigInteger("58759714624708089702199189818787533437405834872094654299502596364496752316901");
    }

    public BigInteger generateA()
    {
        rand = new Random(System.currentTimeMillis());
        do
            a = new BigInteger(256, rand);
        while(a.compareTo(p) >= 0);
        return g.modPow(a, p);
    }

    public void setB(BigInteger b)
    {
        k = b.modPow(a, p);
    }

    public byte[] getKey()
    {
        byte retKey[] = new byte[16];
        System.arraycopy(k.toByteArray(), 16, retKey, 0, 16);
        return retKey;
    }

    private Random rand;
    private BigInteger p;
    private BigInteger g;
    private BigInteger a;
    private BigInteger k;
}
