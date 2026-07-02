#!/bin/bash
echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key\n" \
>> bar.csv
while IFS='\n' read -R FILE; do
    HOSTNAME="${FILE%.log}"
    echo $HOSTNAME >> bar.csv
    cat $FILE | tail -n +4 | awk -F '[ ]*:[ ]*' 'BEGIN { i=0; } { i++; } \
    i!=7 { print $2"," } i==8 { print $2; exit }' >> bar.csv
done < <(find $2 -name "*.log")
