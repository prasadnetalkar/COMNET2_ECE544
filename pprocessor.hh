#ifndef CLICK_PACKET_PROCESSOR_HH
#define CLICK_PACKET_PROCESSOR_HH

#include <click/element.hh>
#include <click/timer.hh>

CLICK_DECLS

class Processor: public Element {

    public:
        Processor();
        ~Processor();
        const char *class_name() const { return "Processor";}
        const char *port_count() const { return "1-/1-";}
        const char *processing() const { return PUSH; }

        void push(int port, Packet *packet);
        int initialize(ErrorHandler*);
        int configure(Vector<String>&, ErrorHandler*);

};

CLICK_ENDDECLS
#endif
