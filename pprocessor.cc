#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <clicknet/ether.h>
#include <clicknet/ip.h>
#include <click/packet.hh>
#include "pprocessor.hh"
#include "packet_new.hh"

#define UPDATE_PACKET 0
#define DELETE_PACKET 1
#define DATA_REQUEST_PACKET 2
#define DATA_RESPONSE_PACKET 3
#define ROUTING_UPDATE_PACKET 4
#define ACK_PACKET 5
#define HELLO_PACKET 6

CLICK_DECLS

Processor::Processor() {
}

Processor::~Processor(){
	
}

int Processor::initialize(ErrorHandler *errh){
    return 0;
}


int Processor::configure(Vector<String> &conf, ErrorHandler *errh) {

        return 0;
}


void Processor::push(int port, Packet *p) {

	assert(p);

        struct header_common *header = (struct header_common *)p->data();

    		int n = noutputs();
    		for (int i = 0; i < n - 1; i++){ 
      			if (Packet *q = p->clone()){
				output(i).push(q);}
    	        }
		output(n-1).push(p);

}

CLICK_ENDDECLS
EXPORT_ELEMENT(Processor)
