/* LOTFREE 9 */
import java.io.ObjectInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.util.Random;
import crypto.Blowfish;

public class GetKey
{

    public GetKey()
    {
    }

    /* fonction pompee sur le web pour affichage hexa */
    public static String getHexString(byte[] b) throws Exception {
      String result = "";
      for (int i=0; i < b.length; i++) {
        result +="\\x";
        result += Integer.toString( ( b[i] & 0xff ) + 0x100, 16).substring( 1 );
      }
      return result;
    }

    /* the big one */
    public static void main(String args[]) throws IOException, ClassNotFoundException, java.lang.Exception
    {
      /* chargement des donnnees du client a destination du serveur */
      FileInputStream fis = new FileInputStream("client2server.dat");
      ObjectInputStream ois = new ObjectInputStream(fis);
      /* Lecture BigInteger */
      BigInteger cli = (BigInteger) ois.readObject(); /* keySharing.generateA() */
      System.out.println("BingInteger from Client");
      System.out.println(cli.toString());

      /* chargement des donnees du serveur a destination du client */
      FileInputStream fis2 = new FileInputStream("server2client.dat");
      ObjectInputStream ois2 = new ObjectInputStream(fis2);
      /* Lecture BigInteger */
      BigInteger srv = (BigInteger) ois2.readObject(); /* keySharing.generateA() */
      System.out.println("BingInteger from Server");
      System.out.println(cli.toString());

      BigInteger p = new BigInteger("72549841864194078899726837116892585496955575260843302046062234899404431414511");
      BigInteger g = new BigInteger("58759714624708089702199189818787533437405834872094654299502596364496752316901");


      BigInteger x = new BigInteger(256,new Random(1248705015814L)); /* == 1248705015814 == Mon Jul 27 16:30:15 CEST 2009 :) */

      /* si pK == srv => echange ok */
      BigInteger pK = g.modPow(x, p);
      System.out.println("PKey");
      System.out.println(pK.toString());

      BigInteger sK = srv.modPow(x, p);
      System.out.println("Temp Key");
      System.out.println(sK.toString());
      System.out.println(getHexString(sK.toByteArray()));
      
      byte retKey[] = new byte[16];
      System.arraycopy(sK.toByteArray(), 16, retKey, 0, 16);
      System.out.println("Blowfish Key");
      System.out.println(getHexString(retKey));
      
      Blowfish encryptor = new Blowfish(retKey);

      byte cryptedMessage[];
      System.out.println("== Donnees envoyees par le client ==");
      try
      {
        while((cryptedMessage = (byte[])ois.readObject()) != null)
          /* System.out.println(getHexString(encryptor.decrypt(cryptedMessage))); */
          System.out.println(new String(encryptor.decrypt(cryptedMessage), "UTF8"));
          /* System.out.println(encryptor.decrypt(cryptedMessage)); */
      }
      catch(ClassNotFoundException e)
      {
        e.printStackTrace();
      }
      catch(java.io.EOFException e)
      {
      }

      System.out.println("== Donnees envoyees par le serveur ==");
      try
      {
        while((cryptedMessage = (byte[])ois2.readObject()) != null)
          /* System.out.println(getHexString(encryptor.decrypt(cryptedMessage))); */
          System.out.println(new String(encryptor.decrypt(cryptedMessage), "UTF8"));
          /* System.out.println(encryptor.decrypt(cryptedMessage)); */
      }
      catch(ClassNotFoundException e)
      {
        e.printStackTrace();
      }
      catch(java.io.EOFException e)
      {
      }

      ois.close();
      fis.close();
      ois2.close();
      fis2.close();
    }
}
