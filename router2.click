require(library /home/comnetsii/elements/routerport.click);

rp1 :: RouterPort(DEV veth4, IN_MAC 5e:da:16:2d:43:bb, OUT_MAC 86:4e:85:93:21:26);
rp2 :: RouterPort(DEV veth7, IN_MAC de:cf:f4:9e:73:67, OUT_MAC 6a:14:c2:83:3a:ca);
rp3 :: RouterPort(DEV veth10, IN_MAC ba:ad:46:da:04:b0, OUT_MAC 96:e9:27:65:50:93);

//ptable :: PortTable(MY_ADDRESS 502);
rtable :: RoutingTable(MY_ADDRESS 502);
forwarder :: Forward(ROUTING_ELEMENT rtable,MY_ADDRESS 502);

broadcast::Processor();

pc1 :: PacketClassifier();
pc2 :: PacketClassifier();
pc3 :: PacketClassifier();

rp1->Queue(65535)->Unqueue->pc1;
rp2->Queue(65535)->Unqueue->pc2;
rp3->Queue(65535)->Unqueue->pc3;

pc1[0]->Discard;
pc1[1]->Discard;
pc1[2]->[0]forwarder;
pc1[3]->[0]forwarder;
pc1[4]->[0]rtable;
pc1[5]->[0]forwarder;
pc1[6]->Discard;

pc2[0]->Discard;
pc2[1]->Discard;
pc2[2]->[1]forwarder;
pc2[3]->[1]forwarder;
pc2[4]->[1]rtable;
pc2[5]->[1]forwarder;
pc2[6]->Discard;

pc3[0]->Discard;
pc3[1]->Discard;
pc3[2]->[2]forwarder;
pc3[3]->[2]forwarder;
pc3[4]->[2]rtable;
pc3[5]->[2]forwarder;
pc3[6]->Discard;

//ptable[0]->broadcast;

rtable[0]->broadcast;

broadcast[0]->rp1;
broadcast[1]->rp2;
broadcast[2]->rp3;

forwarder[0]->rp1;
forwarder[1]->rp2;
forwarder[2]->rp3;
