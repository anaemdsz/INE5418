import java.rmi.Naming;

public class Writer {

    public static void main(String[] args){
        ListInterface remoteList;
        Object element;
        
        try {
            remoteList = (ListInterface)Naming.lookup("rmi://localhost/RemoteList");
            remoteList.pushAt(0, "Primeiro Elemento");
            remoteList.pushAt(1, "Segundo Elemento");
            remoteList.pushAt(2, "Terceiro Elemento");
            element = remoteList.get(0);
            System.out.println("Tamanho da lista: " + remoteList.size());
            System.out.println("Elemento no índice 0: " + element.toString());
            element = remoteList.get(1);
            System.out.println("Elemento no índice 1: " + element.toString());
            element = remoteList.get(2);
            System.out.println("Elemento no índice 2: " + element.toString());
        } catch (Exception e) {
            System.out.println("Reader exception:" + e);
            return;
        }
    }
}
