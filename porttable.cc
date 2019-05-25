/*sends hello to neighbouring routers. Maintains a ports table.
  */

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <clicknet/ether.h>
#include <clicknet/ip.h>
#include <click/packet.hh>
//#include <click/glue.hh>
//#include <click/packet_anno.hh>
#include "porttable.hh"
#include "packet_new.hh"

#define UPDATE_PACKET 0
#define DELETE_PACKET 1
#define DATA_REQUEST_PACKET 2
#define DATA_RESPONSE_PACKET 3
#define ROUTING_UPDATE_PACKET 4
#define ACK_PACKET 5
#define HELLO_PACKET 6

CLICK_DECLS

/* Constructor */
PortTable::PortTable():hello_timer(this) {
    	hello_period = 5;
}


/* destructor */
PortTable::~PortTable(){
	
}

/* parser */
int PortTable::configure(Vector<String> &conf, ErrorHandler *errh) {

        if (cp_va_kparse(conf, this, errh,
            "MY_ADDRESS", cpkM, cpInteger, &myaddress,
            cpEnd) < 0) {

                  return -1;
        }
        return 0;
}

/* initializer */
int PortTable::initialize(ErrorHandler *errh){
    
    	hello_timer.initialize(this);
    	hello_timer.schedule_after_sec(1);
    	return 0;
}

/* Timer configurations */
void PortTable::run_timer(Timer* timer){

         send_hello();
         hello_timer.reschedule_after_sec(hello_period);

}

/* Function to send HELLO messages */
void PortTable::send_hello() {

     	// Broadcast hello to all ports.
     	int headroom = sizeof(click_ether);
     	WritablePacket *packet = Packet::make(headroom,0,sizeof(struct hello_packet), 0);
     	memset(packet->data(),0,packet->length());

     	struct hello_packet *format = (struct hello_packet*) packet->data();
//        struct header_common *header = (struct header_common *)packet->data();

           format-> header.type = HELLO_PACKET;
           format->header.src_address = myaddress;
          if (Packet *clone = packet->clone()){
              click_chatter("Broadcasting Hello Packet to Neighbouring Router");
                output(0).push(clone);
        }

        packet->kill();
}

/* Print ports table */
void PortTable::print_port_table(){

    click_chatter("-----Ports Table of router %d-------", myaddress);

	for (HashTable<int,int>::iterator iter = this->portmap.begin(); iter != this->portmap.end(); ++iter)
	{
    		click_chatter("DESTINATION: %d | PORT: %d", iter.key(), iter.value());
	}

        click_chatter("-----------------------");
 
}


void PortTable::push(int port, Packet *packet) {
        
	assert(packet);
	struct header_common *header = (struct header_common *)packet->data();

         if (header->type == HELLO_PACKET) {  // incoming hello will be used to update the port table

                // adding entry to the ports table
	        click_chatter("Received Hello from %u on port %d", header->src_address, port);
                this->portmap.set(header->src_address,port);
                this->print_port_table();

                packet->kill(); // kill received data packet as it is no longer needed

      	}

        else{   
		click_chatter("Wrong packet type");
		packet->kill();
	}
}


CLICK_ENDDECLS
EXPORT_ELEMENT(PortTable)

