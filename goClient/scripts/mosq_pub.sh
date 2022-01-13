#!/bin/bash

telemetrymsg="0 1.23 2.34 4.5 2 3 4 50 1 0 0 0 0 0 2 15 15 15"

if [ $# -ge 1 ]
then
    # First variable is message
    msg=${1}
    if [ $msg == "default" ]
    then
        msg=$telemetrymsg
        echo "Publishing telemetry request data"
    fi

    echo "Mosquitto publishing $msg message every second"

    while true
    do
        mosquitto_pub -h localhost -u user -P pass -t 'raptor/telemetry' -m "$msg" -r
        sleep 1
    done
else
    echo "Usage mosq_pub.sh <message>"
fi
