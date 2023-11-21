/* Hack the planet
* prendre comme argument le nom du défaceur
* ainsi que le port du serveur HTP
*
* Une requête du style :
* GET /<URL> HTTP/1.1
* Host:<nom_du_serveur_web>
* --le reste de la requête est ignoré--
*
* doit renvoyer
* HTTP/1.1 200 OK
* Content-Length: <longueur de la réponse>
* Content-Type: text/plain
*
* <arg1> 0wns <nom_du_serveur_web>
* F34R M3 !!!
*
* (à titre d'exemple)
* Attention : Content-Length prend compte des retours charios
*/
import java.net.*;
import java.io.*;

public class HTP{

  public static void main(String [] args){
    try{
      if(args.length!=2){
        System.out.println("Usage: HTP <pseudo_du_hacker> <port>\n");
        System.exit(0);
      }

      String s=new String("");
      String eax=new String("");
      byte[] b=s.getBytes();
      boolean ok=false;
      ServerSocket serveur=new ServerSocket(Integer.parseInt(args[1]));
      Socket tunnel;
      while ((tunnel = serveur.accept()) != null) {
        ok=false;
        //Socket tunnel=serveur.accept();
        InputStream is=tunnel.getInputStream();
        InputStreamReader isr=new InputStreamReader(is);
        BufferedReader br=new BufferedReader(isr);
        OutputStream os=tunnel.getOutputStream();
        while((s=br.readLine()).length()!=0 && !ok){
          s=s.toLowerCase();
          System.out.println(s);
          if (s.startsWith("host")){
            ok=true;
            eax=s.substring((s.indexOf(':'))+1);
          }
        }
        s="HTTP/1.1 200 OK"+(char)0x0d+(char)0x0a+"Content-Type: text/html"+(char)0x0d+(char)0x0a;
        if(!ok) eax="Hacked by "+args[0];
        else eax=args[0]+" 0wns "+eax;
        eax="<center><h1>"+eax+"</h1></center>";
        s=s+"Content-Length: "+eax.length()+(char)0x0d+(char)0x0a+(char)0x0d+(char)0x0a;
        s=s+eax;
        os.write(s.getBytes());
        eax="";
        s="";
        os.close();
        /*br.close();
        isr.close();
        is.close();
        tunnel.close();
        serveur.close();*/
      }
    }
    catch(IOException e){
      System.out.println("Erreur "+e);	
    }	
  }
}