/* LOTFREE 9 */
import java.io.ObjectInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.util.GregorianCalendar;
import java.util.Date;
import java.util.Random;

public class BruteForce
{

    public BruteForce()
    {
    }

    public static void main(String args[]) throws IOException, ClassNotFoundException
    {
      FileInputStream fis = new FileInputStream("client2server.dat");
      ObjectInputStream ois = new ObjectInputStream(fis);
      BigInteger plop = (BigInteger) ois.readObject(); /* keySharing.generateA() */
      ois.close();
      fis.close();
      System.out.println("BingInteger from stream");
      System.out.println(plop.toString());

      BigInteger p = new BigInteger("72549841864194078899726837116892585496955575260843302046062234899404431414511");
      BigInteger g = new BigInteger("58759714624708089702199189818787533437405834872094654299502596364496752316901");

      GregorianCalendar gc1 = new GregorianCalendar(2009, 06, 27, 16, 28, 0);
      GregorianCalendar gc2 = new GregorianCalendar(2009, 06, 27, 16, 32, 0);
      long l1 = gc1.getTimeInMillis();
      long l2 = gc2.getTimeInMillis();
      System.out.println(l1+" => "+l2);

      /* l1 = 1248726600000L; */
      /* teste 1248727212986 */

      int ok = 0;
      while(1==1)
      {
        BigInteger x = new BigInteger(256,new Random(l1));
        if(plop.compareTo(g.modPow(x, p))==0)
        {
          System.out.println("Trouve: "+l1);
          /* == 1248705015814 == Mon Jul 27 16:30:15 CEST 2009 :) */
          break;
        }
        if(l1 >= l2)
        {
          System.out.println("End");
          break;
        }
        System.out.println(l1+"++");
        l1++;
      }
    }
}
