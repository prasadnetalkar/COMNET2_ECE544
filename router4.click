require(library /home/comnetsii/elements/routerport.click);

rp1 :: RouterPort(DEV veth8, IN_MAC 6a:14:c2:83:3a:ca, OUT_MAC de:cf:f4:9e:73:67);
rp2 :: RouterPort(DEV veth11, IN_MAC e2:3e:1b:01:65:55, OUT_MAC 4e:45:fe:36:b5:aa);

//ptable :: PortTable(MY_ADDRESS 504);
rtable :: RoutingTable(MY_ADDRESS 504);
forwarder :: Forward(ROUTING_ELEMENT rtable,MY_ADDRESS 504);

broadcast::Processor();

pc1 :: PacketClassifier();
pc2 :: PacketClassifier();

rp1->Queue(65535)->Unqueue->pc1;
rp2->Queue(65535)->Unqueue->pc2;

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

//ptable[0]->broadcast;

rtable[0]->broadcast;

broadcast[0]->rp1;
broadcast[1]->rp2;

forwarder[0]->rp1;
forwarder[1]->rp2;

