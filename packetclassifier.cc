#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/timer.hh>
#include <click/packet.hh>

#include "packetclassifier.hh" 
#include "packet_new.hh"

#define UPDATE_PACKET 0
#define DELETE_PACKET 1
#define DATA_REQUEST_PACKET 2
#define DATA_RESPONSE_PACKET 3
#define ROUTING_UPDATE_PACKET 4
#define ACK_PACKET 5
#define HELLO_PACKET 6

CLICK_DECLS 

PacketClassifier::PacketClassifier() {
}

PacketClassifier::~PacketClassifier(){
	
}

int PacketClassifier::initialize(ErrorHandler *errh){
    return 0;
}

void PacketClassifier::push(int port, Packet *packet) {

	assert(packet);
	struct header_common *header = (struct header_common *)packet->data();

	if(header->type == UPDATE_PACKET) {
		output(0).push(packet);

	} else if(header->type == DELETE_PACKET) {
		output(1).push(packet);

	} else if(header->type == DATA_REQUEST_PACKET) {
		output(2).push(packet);

	} else if(header->type == DATA_RESPONSE_PACKET) {
        output(3).push(packet);

    } 
	else if(header->type == ROUTING_UPDATE_PACKET) {
        output(4).push(packet);
    
	} 
	else if(header->type == ACK_PACKET) {
        output(5).push(packet);

    } 
	else if(header->type == HELLO_PACKET) {
        output(6).push(packet);

    }
	else 
	{
		packet->kill();
	}

}


CLICK_ENDDECLS 
EXPORT_ELEMENT(PacketClassifier)
