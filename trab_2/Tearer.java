import java.rmi.Naming;

public class Tearer {

    public static void main(String[] args){
        ListInterface remoteList;
        Object element;
        
        try {
            remoteList = (ListInterface)Naming.lookup("rmi://localhost/RemoteList");

            for (int i = 5; i > 0; i--) {
                if (remoteList.size() > 0) {
                    element = remoteList.pop(remoteList.size()-1);
                    System.out.println("Removendo um elemento");
                }
            }

        System.out.println("Tamanho da lista: " + remoteList.size());
        } catch (Exception e) {
            System.out.println("Reader exception:" + e);
            return;
        }
        
    }
}
