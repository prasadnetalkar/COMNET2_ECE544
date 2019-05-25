#ifndef CLICK_HOST_HH
#define CLICK_HOST_HH

#include <click/packet.hh>
#include <click/element.hh>

CLICK_DECLS

class Host : public Element {

public:
     Host();
	~Host();
	
	const char *class_name() const { return "Host";}
    const char *port_count() const { return "1-/1-"; }
	const char *processing() const { return PUSH; }
	
	int configure(Vector<String>&, ErrorHandler*);
    int initialize(ErrorHandler*);

	void push(int, Packet *);
	void run_timer(Timer*);
    void send_update_packet();
    void send_data_packet();
        
private:

    uint8_t update_period; // how frequent hello messages will be sent
    uint8_t send_period; //50s
    uint16_t myaddress; // my address
    uint16_t get=0; // get
    uint8_t length; //1
    uint16_t content_id;//currently just 1 content
    String filename;//content filename
    Vector<String> data; 
    Vector<String> datafile;
    Timer update_timer; // timer required for periodic generation of packets
    Timer send_timer;
       

};

CLICK_ENDDECLS
#endif