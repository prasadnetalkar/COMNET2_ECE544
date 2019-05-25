require(library /home/comnetsii/elements/routerport.click);

rp2 :: RouterPort(DEV veth13, IN_MAC 96:43:06:2e:ad:a3, OUT_MAC 12:18:e0:f8:03:a6);
rp3 :: RouterPort(DEV veth20, IN_MAC f6:ae:f9:53:b0:51, OUT_MAC 6e:4d:5c:6c:b1:a2);

//ptable :: PortTable(MY_ADDRESS 505);
rtable :: RoutingTable(MY_ADDRESS 505);
forwarder :: Forward(ROUTING_ELEMENT rtable,MY_ADDRESS 505);

broadcast::Processor();


pc2 :: PacketClassifier();
pc3 :: PacketClassifier();
// FIRST RP INPUT

rp2 -> Queue(65535)->Unqueue->pc2;
rp3 -> Queue(65535)->Unqueue->pc3;

pc2[0]->[0]rtable;
pc2[1]->[0]rtable;
pc2[2]->[0]forwarder;
pc2[3]->[0]forwarder;
pc2[4]->[0]rtable;
pc2[5]->[0]forwarder;
pc2[6] -> Discard;

pc3[0]->Discard;
pc3[1]->Discard;
pc3[2]->[1]forwarder;
pc3[3]->[1]forwarder;
pc3[4]->[1]rtable;
pc3[5]->[1]forwarder;
pc3[6] -> Discard;


//ptable[0] -> broadcast;

rtable[0] -> broadcast;

broadcast[0]-> rp2;
broadcast[1]-> rp3;

forwarder[0]-> rp2;
forwarder[1]-> rp3;

