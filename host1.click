require(library /home/comnetsii/elements/routerport.click);

rp1 :: RouterPort(DEV veth1, IN_MAC 56:33:3c:9b:59:36, OUT_MAC 8e:2a:f4:69:f4:4d);

host::Host(MY_ADDRESS 1,GET 102, LENGTH 0,CONTENT_ID 101);

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
