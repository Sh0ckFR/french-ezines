//un telnet en Java
import java.io.*;
import java.net.*;

public class Telnet
{
    Socket s;
    BufferedReader br;
    BufferedReader br_socket;
    PrintStream ps;
    String data;

    public Telnet(String host,int port)
    {
	try{
	    s=new Socket(host,port);
            br=new BufferedReader(new InputStreamReader(System.in));
            br_socket=new BufferedReader(new InputStreamReader(s.getInputStream()));
            ps=new PrintStream(s.getOutputStream());
            data=new String();
	}
	catch(Exception e){System.out.println("erreur: "+e);}
    }

    public void run()
    {
        try{
            while(true){
                data=br.readLine();
                ps.println(data);
                data=br_socket.readLine();
                System.out.println(data);
            }
        }
        catch(IOException ioe){System.out.println("erreur: "+ioe);}

    }

    public static void main(String []args)
    {
	int p;
	if(args.length!=2){
	    System.out.println("Usage:"+
			       " java Telnet <host> <port>");
	    System.exit(0);
	}
	try{
	    p=Integer.parseInt(args[1]);
	    Telnet t=new Telnet(args[0],p);
            t.run();
	}
	catch(Exception e){System.out.println("erreur: "+e);}
    }
}
