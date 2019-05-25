#ifndef CLICK_ROUTING_TABLE_HH
#define CLICK_ROUTING_TABLE_HH

#include <click/element.hh>
#include <click/timer.hh>
#include <click/hashtable.hh>

CLICK_DECLS

class RoutingTable: public Element {

    public:

        RoutingTable();
        ~RoutingTable();

        const char *class_name() const { return "RoutingTable";}
        const char *port_count() const { return "1-/1-";}
        const char *processing() const { return PUSH; }

        void push(int port, Packet *packet);
        int initialize(ErrorHandler*);
	    int configure(Vector<String>&, ErrorHandler*);
        void run_timer(Timer*); 
 
        struct contentTableInfo{
            uint8_t hopcount;
			uint8_t portnum;}; 			
 		
	struct hostTableInfo{
           	 uint8_t hopcount;
		uint8_t portnum;}; 

        HashTable<int,struct contentTableInfo*> contentTable;
        HashTable<int,struct hostTableInfo*> hostTable;

        void print_table();
        void send_routing_update();
        int get_forward_port(int destination);
	
    private:

        int myaddress;
        Timer print_timer;
        int print_period;
};

CLICK_ENDDECLS
#endif
