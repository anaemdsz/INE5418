import java.rmi.*;

public interface ListInterface extends Remote {
    public void pushAt(int index, Object element) throws RemoteException;
    public Object get(int index) throws RemoteException;
    public Object pop(int index) throws RemoteException;
    public int size() throws RemoteException;
}
