import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.rmi.*;
import java.rmi.server.*;

public class RemoteList extends UnicastRemoteObject implements ListInterface {

    private List<Object> list;

    protected RemoteList() throws RemoteException {
        list = Collections.synchronizedList(new ArrayList<Object>());
    }

    public void pushAt(int index, Object element) throws RemoteException {
        list.add(index, element);
    }

    public void push(Object element) throws RemoteException {
        pushAt(list.size() - 1, element);
    }

    public Object pop(int index) throws RemoteException {
        return list.remove(index);
    }
    
    public Object get(int index) throws RemoteException {
        return list.get(index);
    }

    public int size() throws RemoteException {
        return list.size();
    }
}
