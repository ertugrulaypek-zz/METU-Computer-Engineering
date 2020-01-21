s=$(getent ahosts "s" | cut -d " " -f 1 | uniq)
r1=$(getent ahosts "r1" | cut -d " " -f 1 | uniq)
r3=$(getent ahosts "r3" | cut -d " " -f 1 | uniq)
d=$(getent ahosts "d" | cut -d " " -f 1 | uniq)

s_adapter=$(ip route get $s | grep -Po '(?<=(dev )).*(?= src| proto)')
r1_adapter=$(ip route get $r1 | grep -Po '(?<=(dev )).*(?= src| proto)')
r3_adapter=$(ip route get $r3 | grep -Po '(?<=(dev )).*(?= src| proto)')
d_adapter=$(ip route get $d | grep -Po '(?<=(dev )).*(?= src| proto)')


sudo tc qdisc add dev $s_adapter root netem loss 5% delay 3ms 
sudo tc qdisc add dev $d_adapter root netem loss 5% delay 3ms 
