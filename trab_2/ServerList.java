import java.rmi.*;

public class ServerList {
    public static void main (String[] argv) {
        try {
            Naming.rebind("rmi://localhost/RemoteList", new RemoteList());
        } catch (Exception e) {
            System.out.println("ServerList exception:" + e);
            System.exit(1);
        }
    }
}