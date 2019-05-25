require(library /home/comnetsii/elements/routerport.click);

rp1 :: RouterPort(DEV veth14, IN_MAC 12:18:e0:f8:03:a6, OUT_MAC 96:43:06:2e:ad:a3);

host :: Host(FILENAME 102.dat,MY_ADDRESS 3,LENGTH 1,CONTENT_ID 102);
//broadcast::Processor();


pc1 :: PacketClassifier();

rp1->pc1;

pc1[0]->Discard;
pc1[1]->Discard;
pc1[2]->[0]host;
pc1[3]->[0]host;
pc1[4]->Discard;
pc1[5]->[0]host;
pc1[6]->Discard;

host[0]->rp1;

