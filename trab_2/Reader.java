import java.rmi.*;

public class Reader {

    public static void main(String[] args){
        ListInterface remoteList;
        
        try {
            remoteList = (ListInterface)Naming.lookup("rmi://localhost/RemoteList");
            for (int i = 0; i < remoteList.size(); i ++) {
                System.out.println("Elemento no indice " + i + ": " + remoteList.get(i).toString());
            }
        } catch (Exception e) {
            System.out.println("Reader exception:" + e);
            System.exit(1);
        }
    }
}
