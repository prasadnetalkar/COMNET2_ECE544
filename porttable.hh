#ifndef CLICK_PORT_TABLE_HH
#define CLICK_PORT_TABLE_HH

#include <click/hashtable.hh>
#include <click/element.hh>
#include <click/timer.hh>

CLICK_DECLS

class PortTable: public Element {

    public:

        PortTable();
        ~PortTable();

        HashTable<int, int> portmap;

        const char *class_name() const { return "PortTable";}
        const char *port_count() const { return "1-/1-";}
        const char *processing() const { return PUSH; }

        void push(int port, Packet *packet);
        int initialize(ErrorHandler*);
        int configure(Vector<String>&, ErrorHandler*);
        void print_port_table();
        void send_hello();
        void run_timer(Timer*); 
    private:

        int myaddress;
        int hello_period;
        Timer hello_timer;
	
        
};

CLICK_ENDDECLS
#endif
