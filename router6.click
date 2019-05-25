require(library /home/comnetsii/elements/routerport.click);

rp1 :: RouterPort(DEV veth15, IN_MAC 46:ee:5e:59:81:bb, OUT_MAC ea:29:8a:78:3a:c9);
rp2 :: RouterPort(DEV veth19, IN_MAC 6e:4d:5c:6c:b1:a2, OUT_MAC f6:ae:f9:53:b0:51);
rp3 :: RouterPort(DEV veth22, IN_MAC 4e:9c:07:a9:77:d2, OUT_MAC 3a:cd:8b:43:d4:02);

//ptable :: PortTable(MY_ADDRESS 506);
rtable :: RoutingTable(MY_ADDRESS 506);
forwarder :: Forward(ROUTING_ELEMENT rtable,MY_ADDRESS 506);

broadcast::Processor();


pc1 :: PacketClassifier();
pc2 :: PacketClassifier();
pc3 :: PacketClassifier();
// FIRST RP INPUT

rp1 -> Queue(65535)->Unqueue->pc1;
rp2 -> Queue(65535)->Unqueue->pc2;
rp3 -> Queue(65535)->Unqueue->pc3;

pc1[0]->[0]rtable;
pc1[1]->[0]rtable;
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

