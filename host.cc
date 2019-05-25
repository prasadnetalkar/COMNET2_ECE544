#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/timer.hh>
#include <clicknet/ether.h>
#include <clicknet/ip.h>
#include <click/glue.hh>
#include <click/args.hh>
#include "host.hh"
#include "packet_new.hh"
#include <click/fromfile.hh>
#include <unistd.h>

#define UPDATE_PACKET 0
#define DELETE_PACKET 1
#define DATA_REQUEST_PACKET 2
#define DATA_RESPONSE_PACKET 3
#define ROUTING_UPDATE_PACKET 4
#define ACK_PACKET 5
#define HELLO_PACKET 6

using namespace std;
CLICK_DECLS

Host::Host():update_timer(this),send_timer(this){
        update_period = 5;
        send_period = 50;
}


Host::~Host(){}


int Host::initialize(ErrorHandler *errh) {

	update_timer.initialize(this);
	update_timer.schedule_after_sec(1);
        send_timer.initialize(this);
	send_timer.schedule_after_sec(send_period);
        return 0;
}


int Host::configure(Vector<String> &conf, ErrorHandler *errh) {
      
        int res = Args(conf, this, errh)
            .read_p("FILENAME", FilenameArg(), filename)
            .read_mp("MY_ADDRESS", myaddress)      
            .read_p("GET",get)
            .read_mp("LENGTH", length) 
            .read_mp("CONTENT_ID", content_id)
            .complete();
        if(filename)
        {
        if(res >= 0){
             FILE *fp = fopen(filename.c_str(), "r");
        if(fp == 0)
            return(errh->error("cannot open file %s", filename.c_str()));
        
        char buf[2];
        while(fgets(buf,2, fp)){
            datafile.push_back(buf);
            }
        fclose(fp);
        if(datafile.size() < 1)
            return(errh->error("no locations in file %s", filename.c_str()));
    
        click_chatter("read %d deltas from %s",datafile.size(),filename.c_str());
        }
   }
  return res;
}


/* Timer configurations */
void Host::run_timer(Timer* timer){
    if(timer == &update_timer){
        send_update_packet();  
        update_timer.schedule_after_sec(update_period); 
    }else if (timer == &send_timer){
        if(get){
          send_data_packet();
          send_timer.schedule_after_sec(update_period+1);	
		}
    }
}

/* Function to send update messages */
void Host::send_update_packet() {
    struct update_packet* update_p;       
    size_t size = sizeof (struct update_packet) +length * sizeof (uint16_t);
    WritablePacket *pkt = Packet::make(size);
    struct header_common *cheader = (struct header_common *)pkt->data();
    update_p = reinterpret_cast<struct update_packet *> (pkt->data());
    cheader->type = UPDATE_PACKET;
    cheader->src_address = uint16_t(myaddress);
    update_p->length = length;
    for (int i = 0; i < length; i++) {
        update_p->addresses[i] = content_id;
    }
    click_chatter("Sending update packet from SRC %d having content id %d ",myaddress,content_id);
    checked_output_push(0, pkt);
}

void Host::send_data_packet() {
    struct data_request_packet* update_data_p;       
    size_t size = sizeof (struct data_request_packet);
    WritablePacket *pkt = Packet::make(size);
    struct header_common *dheader = (struct header_common *)pkt->data();
    update_data_p = reinterpret_cast<struct data_request_packet *> (pkt->data());
    dheader->type = DATA_REQUEST_PACKET;
    dheader->src_address = uint16_t(myaddress);
    update_data_p->dst_address = get;
    click_chatter("Sending data request packet from SRC %d for %d at time %lu ",myaddress,update_data_p->dst_address,Timestamp::now_steady());
    output(0).push(pkt);
}

void Host::push(int port, Packet *packet) {

	assert(packet);
        struct header_common *header = (struct header_common *)packet->data();
        if (header->type == DATA_REQUEST_PACKET){
            struct data_request_packet *request_pkt = (struct data_request_packet*) packet->data();
            int cid_request = request_pkt->dst_address;
            
            click_chatter("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
            click_chatter("Received data packet from %d through port %d ",header->src_address, request_pkt->dst_address);
            click_chatter("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
            
            if(cid_request==content_id){
                int count= datafile.size()/1475;
                for(int seq_n = 0;seq_n<count+1;seq_n++){
                        struct data_response_packet* data_p; 
                        size_t size = sizeof (struct data_response_packet) + 1475;
                        //size_t size = sizeof (struct data_response_packet);
                        //String my_data="hello";
                        WritablePacket *pkt = Packet::make(size);
                        struct header_common *header_new = (struct header_common *)pkt->data();
                        data_p = reinterpret_cast<struct data_response_packet *> (pkt->data());
                        header_new->type = DATA_RESPONSE_PACKET;
                        header_new->src_address = uint16_t(myaddress);
                        data_p->dst_address = header->src_address;
                        data_p->seq_num = uint8_t(seq_n);
                        data_p->end_of_file=0;
                        if (seq_n==count){
                            data_p->end_of_file=1;}//end of file marker
                        char *payload= (char*)(pkt->data()+sizeof(struct data_response_packet));
                        memcpy(payload,&datafile.at(seq_n*1475),1475);
                        click_chatter("Send response packet to %d with sequence number %d and packet length %d, payload %s at time %lu",data_p->dst_address,seq_n,pkt->length(),payload,Timestamp::now_steady());
                        output(0).push(pkt);
                        //pkt->kill();
                        sleep(2);
                    }
                packet->kill();
                }
        }else if (header->type == DATA_RESPONSE_PACKET){
                        struct data_response_packet *response_pkt = (struct data_response_packet*) packet->data();
                        if(response_pkt->seq_num>=0){
			    send_timer.unschedule();							
		         }
                        if(response_pkt->end_of_file==0){
                          click_chatter("Received data response packet from %d having seq num %d ",header->src_address, response_pkt->seq_num);  
                         }
		        if(response_pkt->end_of_file==1){
		        struct ack_packet* ack_p; 
                        size_t size = sizeof (struct ack_packet);
                        WritablePacket *pkt = Packet::make(size);
                        struct header_common *header_new = (struct header_common *)pkt->data();
                        ack_p = reinterpret_cast<struct ack_packet *> (pkt->data());
                        header_new->type = ACK_PACKET;
                        header_new->src_address = uint16_t(myaddress);
                        ack_p->dst_address = header->src_address;
                        ack_p->seq_num = response_pkt->seq_num+1;
                        click_chatter("Sending ACK Packet with seq_number",ack_p->seq_num);
                        checked_output_push(0, pkt);
						}
        }
        else if (header->type == ACK_PACKET){
            click_chatter("Received ACK packet from %d",header->src_address);
            packet->kill();
        }
        else{
            click_chatter("--------------------------------------------------------");
            click_chatter("Received Packet from %d through port %d ",header->src_address, port);
            click_chatter("------------------------------------------------------------");
            packet->kill();
        }        
}

CLICK_ENDDECLS
EXPORT_ELEMENT(Host)