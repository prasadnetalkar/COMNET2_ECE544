#ifndef CLICK_PACKETS_HH
#define CLICK_PACKETS_HH

/* common header*/
struct header_common{
	uint8_t type;
	uint16_t src_address;
	
};

/* host update packet */
struct update_packet{
        struct header_common header;
	uint8_t length;
        uint16_t addresses[];
}CLICK_SIZE_PACKED_ATTRIBUTE;

/* host delete packet */
struct delete_packet{
	struct header_common header;
	uint8_t length;
};

/* data request packet */
struct data_request_packet{
	struct header_common header;
	uint16_t dst_address;	
	
};

/* data response packet */
struct data_response_packet{
	struct header_common header;
	uint16_t dst_address;	
	uint8_t seq_num;
	uint8_t end_of_file;
};

/* routing update packet */
struct routing_update_packet{
        struct header_common header;
	uint8_t hop_count;	
	uint8_t length;
        uint16_t addresses[];
}CLICK_SIZE_PACKED_ATTRIBUTE;	


/* ack packet */
struct ack_packet {
	struct header_common header;
	uint16_t dst_address;
	uint8_t seq_num;
	
};

/* hello packet */
struct hello_packet {
	struct header_common header;
};

static bool is_content(uint16_t id) {
    return id & 0x8000;
}

#endif
