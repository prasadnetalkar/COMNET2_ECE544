/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <clicknet/ether.h>
#include <clicknet/ip.h>
#include <click/packet.hh>
#include "forward.hh"
#include "packet_new.hh"

#define UPDATE_PACKET 0
#define DELETE_PACKET 1
#define DATA_REQUEST_PACKET 2
#define DATA_RESPONSE_PACKET 3
#define ROUTING_UPDATE_PACKET 4
#define ACK_PACKET 5
#define HELLO_PACKET 6

CLICK_DECLS
Forward::Forward() {
}

Forward::~Forward(){
	
}

int Forward::initialize(ErrorHandler *errh){
    return 0;
}


int Forward::configure(Vector<String> &conf, ErrorHandler *errh) {

        if (cp_va_kparse(conf, this, errh,
            "ROUTING_ELEMENT", cpkP+cpkM, cpElement, &routingElement,
            "MY_ADDRESS", cpkM, cpInteger, &myaddress,
            cpEnd) < 0) {

                  return -1;
        }
        return 0;
}
        
void Forward::push(int port, Packet *packet) {

	assert(packet);
	struct header_common *header = (struct header_common *)packet->data();
         if(header->type == DATA_REQUEST_PACKET){
             struct data_request_packet *request_pkt = (struct data_request_packet*) packet->data();
             int dst = request_pkt->dst_address;
             click_chatter("@@@@@destination lookup@@@@,%d",dst);
             int nextport = routingElement -> get_forward_port(dst);
             if (nextport == -1){
                 click_chatter("id not present in table\n");
                 packet->kill();}
             else{
                 click_chatter("data request packet forwarded %d\n",nextport);
                 output(nextport).push(packet);
                 //checked_output_push(nextport, packet); 
               }
            }
        else if(header->type == DATA_RESPONSE_PACKET){
             struct data_response_packet *response_pkt = (struct data_response_packet*) packet->data();
             int dst = response_pkt->dst_address;
             click_chatter("@@@@@destination lookup@@@@,%d",dst);
             int nextport = routingElement -> get_forward_port(dst);
             if (nextport == -1){
                 click_chatter("id not present in table\n");
                 packet->kill();}
             else{
                 click_chatter("data response packet forwarded %d\n",nextport);
                 output(nextport).push(packet);
                 //checked_output_push(nextport, packet); 
             }
         }
        else if(header->type == ACK_PACKET){
             struct ack_packet *ack_pkt = (struct ack_packet*) packet->data();
             int dst = ack_pkt->dst_address;
             click_chatter("@@@@@destination lookup@@@@,%d",dst);
             int nextport = routingElement -> get_forward_port(dst);
             if (nextport == -1){
                 click_chatter("id not present in table\n");
                 packet->kill();}
             else{
                 click_chatter("ack response packet forwarded %d\n",nextport);
                 output(nextport).push(packet);
                 //checked_output_push(nextport, packet); 
             }
             }
         else{
             click_chatter("unknown packet");
         }
}
CLICK_ENDDECLS
EXPORT_ELEMENT(Forward)