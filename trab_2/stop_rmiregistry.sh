service="rmiregistry"

# Verifica se rmi registry ja esta em execucao
#if pgrep -x "$service" >/dev/null
if ps -C "$service" >/dev/null
then
    echo "$service em execucao. Reiniciando o $service"
    pkill $service
    sleep 1
fi
