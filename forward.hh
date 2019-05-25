/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   forward.hh
 * Author: Prasad & Mareesh
 *
 * Created on May 4, 2019, 2:48 PM
 */
#ifndef CLICK_FORWARD_HH
#define CLICK_FORWARD_HH

#include <click/hashmap.hh>
#include <click/element.hh>
#include <click/timer.hh>
#include "routingtable.hh"

CLICK_DECLS

class Forward: public Element {

    public:

        Forward();
        ~Forward();
        const char *class_name() const { return "Forward";}
        const char *port_count() const { return "1-/1-";}
        const char *processing() const { return PUSH; }

        void push(int port, Packet *packet);
        int initialize(ErrorHandler*);
	int configure(Vector<String>&, ErrorHandler*);

    private:

        int myaddress;
        RoutingTable* routingElement;

};

CLICK_ENDDECLS
#endif

