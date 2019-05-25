require(library /home/comnetsii/elements/routerport.click);

rp1 :: RouterPort(DEV veth6, IN_MAC fa:66:cd:09:5c:7c, OUT_MAC 02:42:88:54:a7:2c);
rp2 :: RouterPort(DEV veth9, IN_MAC 96:e9:27:65:50:93, OUT_MAC ba:ad:46:da:04:b0);
rp3 :: RouterPort(DEV veth21, IN_MAC 3a:cd:8b:43:d4:02, OUT_MAC 4e:9c:07:a9:77:d2);

//ptable :: PortTable(MY_ADDRESS 503);
rtable :: RoutingTable(MY_ADDRESS 503);
forwarder :: Forward(ROUTING_ELEMENT rtable,MY_ADDRESS 503);

broadcast::Processor();


pc1 :: PacketClassifier();
pc2 :: PacketClassifier();
pc3 :: PacketClassifier();
// FIRST RP INPUT

rp1 -> Queue(65535)->Unqueue->pc1;
rp2 -> Queue(65535)->Unqueue->pc2;
rp3 -> Queue(65535)->Unqueue->pc3;

pc1[0]->Discard;
pc1[1]->Discard;
pc1[2]->[0]forwarder;
pc1[3]->[0]forwarder;
pc1[4]->[0]rtable;
pc1[5]->[0]forwarder;
pc1[6] -> Discard; 

pc2[0]->Discard;
pc2[1]->Discard;
pc2[2]->[1]forwarder;
pc2[3]->[1]forwarder;
pc2[4]->[1]rtable;
pc2[5]->[1]forwarder;
pc2[6] -> Discard;

pc3[0]->Discard;
pc3[1]->Discard;
pc3[2]->[2]forwarder;
pc3[3]->[2]forwarder;
pc3[4]->[2]rtable;
pc3[5]->[2]forwarder;
pc3[6] -> Discard;


//ptable[0] -> broadcast;

rtable[0] -> broadcast;

broadcast[0]-> rp1;
broadcast[1]-> rp2;
broadcast[2]-> rp3;

forwarder[0]-> rp1;
forwarder[1]-> rp2;
forwarder[2]-> rp3;

