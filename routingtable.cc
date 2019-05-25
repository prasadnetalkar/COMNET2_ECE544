#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <clicknet/ether.h>
#include <clicknet/ip.h>
#include <click/packet.hh>
#include "routingtable.hh"
#include "packet_new.hh"
#include <click/glue.hh>

#define UPDATE_PACKET 0
#define DELETE_PACKET 1
#define DATA_REQUEST_PACKET 2
#define DATA_RESPONSE_PACKET 3
#define ROUTING_UPDATE_PACKET 4
#define ACK_PACKET 5
#define HELLO_PACKET 6

CLICK_DECLS


/* constructor */
RoutingTable::RoutingTable():print_timer(this){
   print_period=5;
}


/* destructor */
RoutingTable::~RoutingTable(){
	
}


/* Initializer */
int RoutingTable::initialize(ErrorHandler *errh){
    print_timer.initialize(this);
    print_timer.schedule_after_sec(1);
    return 0;
}

/* parser */
int RoutingTable::configure(Vector<String> &conf, ErrorHandler *errh) {

        if (cp_va_kparse(conf, this, errh,"MY_ADDRESS", cpkM, cpInteger, &myaddress,cpEnd) < 0) {

                  return -1;
        }
      
        return 0;
}

/* Timer configurations */
void RoutingTable::run_timer(Timer* timer){

         print_table();
         print_timer.schedule_after_sec(print_period);

}

/* print routing table */
void RoutingTable::print_table(){
    
     click_chatter("-----CONTENT TABLE %d-------", myaddress);

        for (HashTable<int,struct contentTableInfo*>::iterator iter = this->contentTable.begin(); iter != this->contentTable.end(); ++iter)
        {
                click_chatter("CONTENT: %d | HOP: %d | PORT: %d", iter.key(), iter.value()->hopcount,iter.value()->portnum);
        }

        click_chatter("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
		
		click_chatter("-----HOST TABLE %d-------", myaddress);

        for (HashTable<int,struct hostTableInfo*>::iterator iter = this->hostTable.begin(); iter != this->hostTable.end(); ++iter)
        {
                click_chatter("Host: %d | HOP: %d | PORT: %d", iter.key(), iter.value()->hopcount,iter.value()->portnum);
        }

        click_chatter("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");

}

int RoutingTable::get_forward_port(int destination){
               
    if(destination > 100){
        HashTable<int,struct contentTableInfo*>::iterator iter = this->contentTable.find(destination);   
            if(iter != this->contentTable.end()) {

                    click_chatter("&&&&&&&content table look up&&&&&& port = %d" ,iter.value()->portnum);
                     return iter.value()->portnum;
               }
               else
                      return -1;

}else {
        HashTable<int,struct hostTableInfo*>::iterator iter = this->hostTable.find(destination);  
           if(iter != this->hostTable.end()) {
                      
                      click_chatter("&&&&&&&host table look up&&&&&& port = %d" ,iter.value()->portnum);
                      return iter.value()->portnum;
               }
               else
                      return -1;
    }
}

   
    

/* Push configuration for incoming packets */
void RoutingTable::push(int port, Packet *packet) {

        assert(packet);
        struct header_common *cheader = (struct header_common *)packet->data();
        if (cheader->type == UPDATE_PACKET || cheader->type == DELETE_PACKET){ // update/delete packet 
            struct update_packet *update_pkt = (struct update_packet*) packet->data();
            click_chatter("Received an update/delete packet from %d",cheader->src_address); 
	    struct contentTableInfo* ct= new struct contentTableInfo(); 
	    struct hostTableInfo* ht= new struct hostTableInfo();
	    ht->hopcount=0;
	    ht->portnum=port;
	    this->hostTable.set(cheader->src_address,ht);
            ct->hopcount=0;
            ct->portnum=port;
            for (int i = 0; i < update_pkt->length; i++) {
                this->contentTable.set(update_pkt->addresses[i],ct);
            }
            Vector<uint16_t> ids;
            ids.push_back(cheader->src_address);
            for (int i = 0;i <update_pkt->length;i++){
                ids.push_back(update_pkt->addresses[i]);}
            
            int len = ids.size();
            struct routing_update_packet* update_p;       
            size_t size = sizeof (struct routing_update_packet) +len * sizeof (uint16_t);
            WritablePacket *pkt = Packet::make(size);
            struct header_common *cheader = (struct header_common *)pkt->data();
            update_p = reinterpret_cast<struct routing_update_packet *> (pkt->data());
            cheader->type = ROUTING_UPDATE_PACKET;
            cheader->src_address = uint16_t(myaddress);
            update_p->length = len;
            update_p->hop_count = 0;
            for (int i = 0; i < len; i++) {
                update_p->addresses[i] = ids[i];
            } 
            click_chatter("Broadcasting Routing Update Packet");
            checked_output_push(0, pkt);
            packet->kill();}
        else if(cheader->type == ROUTING_UPDATE_PACKET){
            struct routing_update_packet *rupdate_pkt = (struct routing_update_packet*) packet->data();
            if(cheader->src_address == myaddress){
              packet->kill();}
            else{
                rupdate_pkt->hop_count = rupdate_pkt->hop_count + 1;
                int hop = rupdate_pkt->hop_count;
            	struct contentTableInfo* ct= new struct contentTableInfo();
            	struct hostTableInfo* ht= new struct hostTableInfo();
                ht->hopcount=hop;
                ct->hopcount=hop;
            	ht->portnum=port;
                ct->portnum=port;
                Vector<uint16_t> ids;
                Vector<uint16_t> new_ids;
                for (int i = 0;i < rupdate_pkt->length;i++){
                     ids.push_back(rupdate_pkt->addresses[i]);}
                int len1 = ids.size();
                for (int i = 0;i < len1; i++){
                    if(ids[i]<=100){
                       if(this->hostTable.get(ids[i]) == NULL){
                          this->hostTable.set(ids[i],ht);
                          new_ids.push_back(ids[i]);}
                       else if (this->hostTable.get(ids[i])->hopcount > hop){
                          this->hostTable.set(ids[i],ht); 
                          new_ids.push_back(ids[i]);}
                    }   
                    else{
                       if(this->contentTable.get(ids[i]) == NULL){
                          this->contentTable.set(ids[i],ct);
                          new_ids.push_back(ids[i]);}
                       else if (this->contentTable.get(ids[i])->hopcount > hop){
                          this->contentTable.set(ids[i],ct); 
                          new_ids.push_back(ids[i]);}       
                    }
                }
                int len2 = new_ids.size();
                struct routing_update_packet* update_p;       
                size_t size = sizeof (struct routing_update_packet) +len2 * sizeof (uint16_t);
                WritablePacket *pkt = Packet::make(size);
                struct header_common *header = (struct header_common *)pkt->data();
                update_p = reinterpret_cast<struct routing_update_packet *> (pkt->data());
                header->type = ROUTING_UPDATE_PACKET;
                header->src_address = cheader->src_address;
                update_p->length = len2;
                update_p->hop_count = rupdate_pkt->hop_count;
                for (int i = 0; i < len2; i++) {
                    update_p->addresses[i] = new_ids[i];
                }
                checked_output_push(0, pkt);
                packet->kill();}
        }
        else if (cheader->type == DATA_REQUEST_PACKET){
            click_chatter( "Data request packet received %d \n",cheader->src_address);  
        }
	else {
              click_chatter( "Router : Received unknown packet.\n");
              packet -> kill();
        }
   }
CLICK_ENDDECLS
EXPORT_ELEMENT(RoutingTable)
