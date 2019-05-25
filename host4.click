require(library /home/comnetsii/elements/routerport.click);

rp1 :: RouterPort(DEV veth16, IN_MAC ea:29:8a:78:3a:c9, OUT_MAC 46:ee:5e:59:81:bb);

host :: Host(FILENAME 101.dat,MY_ADDRESS 4,LENGTH 1,CONTENT_ID 101);
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

