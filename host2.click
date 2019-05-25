require(library /home/comnetsii/elements/routerport.click);

rp1 :: RouterPort(DEV veth18, IN_MAC 5e:7f:98:37:2b:7c, OUT_MAC 1e:5a:ad:f7:92:bd);

host :: Host(FILENAME 101.dat,MY_ADDRESS 2,LENGTH 1,CONTENT_ID 101);
//broadcast::Processor();


pc1 :: PacketClassifier();

rp1->Queue(65000)->Unqueue->pc1;

pc1[0]->Discard;
pc1[1]->Discard;
pc1[2]->[0]host;
pc1[3]->[0]host;
pc1[4]->Discard;
pc1[5]->[0]host;
pc1[6]->Discard;

host[0]->rp1;

