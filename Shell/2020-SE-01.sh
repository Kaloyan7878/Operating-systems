#!/bin/bash
#Напишете shell скрипт, който получава два задължителни позиционни параметъра — име на файл (bar.csv) и име на директория.
#Директорията може да съдържа текстови файлове с имена от вида foobar.log, всеки от които съдържа "Licensed features..."
#данни за мрежово оборудване (физически интерфейси, VLANs, Hosts, Failover, VPN-3DES-AES, VPN Peers, VLAN Trunk Ports,
#тип лиценз, сериен номер, Activation Key).

#Имената на лог файловете определят hostname, а съдържанието им дава детайли за параметри на съответния хост.

#Скриптът трябва да генерира bar.csv (CSV файл) на база на log файловете в директорията, с колони: hostname,phy,vlans,hosts,
#failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key, без излишни trailing/leading интервали.


echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key\n" \
>> bar.csv
while IFS='\n' read -R FILE; do
    HOSTNAME="${FILE%.log}"
    echo $HOSTNAME >> bar.csv
    cat $FILE | tail -n +4 | awk -F '[ ]*:[ ]*' 'BEGIN { i=0; } { i++; } \
    i!=7 { print $2"," } i==8 { print $2; exit }' >> bar.csv
done < <(find $2 -name "*.log")
