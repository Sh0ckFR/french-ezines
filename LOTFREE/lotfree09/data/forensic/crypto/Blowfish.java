package crypto;

import java.io.PrintStream;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import javax.crypto.*;
import javax.crypto.spec.SecretKeySpec;
import javax.crypto.IllegalBlockSizeException;

public class Blowfish
{

    public Blowfish(byte key[])
    {
        encryptCipher = null;
        decryptCipher = null;
        SecretKeySpec skeySpec = new SecretKeySpec(key, "Blowfish");
        try
        {
            encryptCipher = Cipher.getInstance("Blowfish");
            decryptCipher = Cipher.getInstance("Blowfish");
        }
        catch(NoSuchAlgorithmException e)
        {
            e.printStackTrace();
        }
        catch(NoSuchPaddingException e)
        {
            e.printStackTrace();
        }
        try
        {
            encryptCipher.init(1, skeySpec);
            decryptCipher.init(2, skeySpec);
        }
        catch(InvalidKeyException e)
        {
            e.printStackTrace();
        }
    }

    public byte[] decrypt(byte ciphertext[])
    {
        if(decryptCipher == null)
        {
            System.out.println("Error : DecryptCipher not initialised");
            return null;
        }
        try
        {
          return decryptCipher.doFinal(ciphertext);
        }
        catch(IllegalBlockSizeException e)
        {
          e.printStackTrace();
        }
        catch(javax.crypto.BadPaddingException e)
        {
          e.printStackTrace();
        }
        return null;
    }

    public byte[] encrypt(byte plaintext[])
    {
        if(encryptCipher == null)
        {
            System.out.println("Error : EncryptCipher not initialised");
            return null;
        }
        try
        {
          return encryptCipher.doFinal(plaintext);
        }
        catch(IllegalBlockSizeException e)
        {
          e.printStackTrace();
        }
        catch(javax.crypto.BadPaddingException e)
        {
          e.printStackTrace();
        }
        return null;
    }

    private Cipher encryptCipher;
    private Cipher decryptCipher;
}
