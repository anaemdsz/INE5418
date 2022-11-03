service="rmiregistry"

# Verifica se rmi registry ja esta em execucao
#if pgrep -x "$service" >/dev/null
if ps -C "$service" >/dev/null
then
    pkill $service
    sleep 1
fi &&

rmiregistry &

echo "Iniciando o servidor"
java -Djava.security.policy=server.policy ServerList
