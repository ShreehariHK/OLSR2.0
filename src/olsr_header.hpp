/*                            Project title
 *
 *
 * Source Code Name   :   olsr_header.hpp
 *
 * Description        :   To store message header information .
 *
 * Subsystem Name     :   OLSR
 *
 * Revision History
 * ---------------------------------------------------------------------------|
 * Version | Change Description               |    Date    |    Changed By    |
 * --------|----------------------------------|------------|------------------|
 * 1.0     |Initial Version                   | 14-06-2021 |Shreehari H K     |
 * --------|----------------------------------|------------|------------------|
 *
 *                              Copyright statement
 *
 *
 */


#ifndef OLSR_HEADER_HPP_
#define OLSR_HEADER_HPP_

#include "olsr_types.hpp"

namespace ns_olsr2_0
{
#define M_HELLO_MSG_HEADER_LEN    14
#define M_TC_MSG_HEADER_LEN       10

class C_PACKET_HEADER
{
public:

	C_PACKET_HEADER ();
	#ifdef NS3_SIM
  	  virtual ~C_PACKET_HEADER ();
	#else
  	  ~C_PACKET_HEADER ();
	#endif

/* Sets the packet length */
  void set_packet_length (T_UINT16 length);

  /* Gets the packet total length.*/
  T_UINT16 get_packet_length () const;

  /* Set the packet type. */
  void set_packet_type (T_UINT16 seqnum);

  /* Get the packet type. */
  T_UINT16 get_packet_type () const;

private:
  T_UINT16 m_packet_length; /* The packet length */
  T_UINT16 m_packet_type;   /* The packet type */

  /* A buffer to store serialised OLSR message */
  std::array<T_UINT8, 512> serialised_buffer;

};

class C_MESSAGE_HEADER
{
	public:

#ifndef COMMENT_SECTION
	C_MESSAGE_HEADER ();              /* Constructor */
	#ifdef M_NS3_SIM
		virtual ~C_MESSAGE_HEADER (); /* Destructor */
	#else
		~C_MESSAGE_HEADER ();         /* Destructor */
	#endif
#endif

	/* Sets the message type. */
    void set_message_type (E_OLSR_MSG_TYPE message_type);

    /* Get the message type */
    E_OLSR_MSG_TYPE get_message_type () const;

    /* Set the validity time */
    void set_validity_time (Time time);

    /* Get the validity time */
    Time get_validity_time () const;

    /* Set the interval time */
    void set_interval_time (Time time);

    /* Get the interval time */
    Time get_interval_time () const;

    /* Set the originator address */
    void set_originator_address (T_NODE_ADDRESS originator_address);

    /* Get the originator address */
    T_NODE_ADDRESS get_originator_address () const;

    /* Set the time to live */
    void set_time_to_live (T_UINT8 time_to_live);

    /* Get the time to live */
    T_UINT8 get_time_to_live () const;

    /* Set the hop count */
    void set_hop_count (T_UINT8 hop_count);

    /* Get the hop count */
    T_UINT8 get_hop_count () const;

    /* Set the message sequence number. */
    void set_message_sequence_number (T_UINT16 message_sequence_number);

    /* Get the message sequence number */
    T_UINT16 get_message_sequence_number () const;

    /* Set the message length */
    void set_message_length (T_UINT16 message_length);

    /* Get the message length */
    T_UINT16 get_message_length () const;


	private:

	  	E_OLSR_MSG_TYPE m_message_type;         /* Message type of current OLSR instance */
	  	T_UINT16 m_msg_len;                     /* Message address length of current OLSR instance */
		T_NODE_ADDRESS m_originator_address;    /* Message originator address of current OLSR instance */
		T_UINT8 m_time_to_live;            	    /* Time to live of a message of current OLSR instance */
		T_UINT8 m_hop_count;                    /* Hop count of a message of current OLSR instance */
		T_UINT16 m_message_sequence_number;     /* Sequence number of a message of current OLSR instance */
		T_UINT8 m_validity_time;                /* Validity time of a message of current OLSR instance */
		T_UINT8 m_interval_time;                /* Interval time of a message of current OLSR instance */

	public:

		/* A queue to hold OLSR messages */
		typedef std::vector<C_MESSAGE_HEADER> OlsrMsgList;

		/**
		 * It holds Common fields required
		 * for Hello and TC messages
		 */
		typedef union
		{
		  U_NEIGHBOR_LINK       link_state;    /* Link state */
		  T_UINT8               hop_count;     /* Hop count - ATTACHED  */
		}U_COMMON_FIELD;

		/**
		 * It holds Hello address block
		 */
		typedef struct
		{
		    T_UINT8 unique_id;                 /* net id or Node id*/
		    U_COMMON_FIELD common_field;       /* Link state or neighbor address type */
			std::array<float,M_TWO> metric;    /* metric[M_ONE] - In link metric and metric[M_ZERO] - out link metric */
		}T_GENERIC_ADDR_BLOCK;

		/*------------------------------------------------------------------
		 Hello Message
		------------------------------------------------------------------*/

		/**
		 * It stores complete Hello message
		 */
		typedef struct
		{
		    U_WILLINGNESS node_willingness;                  /* Node's willingness for Routing and Flooding*/
		    T_LEADER_TUPLE leader_info;                      /* Info about Leader of the network */
		    E_ADDRESS_BLOCK_FLAGS abf;                       /* Address block Flags */
            T_UINT8 common_id;                               /* Orthogonal network address/ Node address */
            std::vector<T_GENERIC_ADDR_BLOCK> neighbor_set;  /* Address TLVs of Hello message */

            /* Finds  the size of the hello message */
            T_UINT16 get_hello_msg_size();
		}T_HELLO;


		/*------------------------------------------------------------------
		 Tc Message
		------------------------------------------------------------------*/
		/**
		 * It stores Common address block
		 * of TC message
		 */

		typedef struct
		{
		  E_TC_ADDRESS_TYPE tc_msg_type;					 /* Info about type of data carried out by TC message*/
		  E_ADDRESS_BLOCK_FLAGS abf;                         /* Address block Flags */
          T_UINT8 common_id;                                 /* Orthogonal network address/ Node address */
          std::vector<T_GENERIC_ADDR_BLOCK> network_info;    /* Address TLVs of TC message */
		}T_TC_ADDRESS_BLOCK;


		/**
		 * It stores complete TC message
		 */
		typedef struct
		{
		    T_UINT16 ansn;                                   /* Advertised neighbor sequence number*/
		    std::vector<T_TC_ADDRESS_BLOCK> tc_addr_set;	 /* Stores a set of address blocks*/

		    /* Finds  the size of the tc message */
		    T_UINT16 get_tc_msg_size();
		}T_TC;

		/**
		 * Holds Hello and TC messages
		 */
		typedef struct
		{
		    E_OLSR_MSG_TYPE m_message_type;         /* Message type of current OLSR instance */
		    T_UINT16 m_msg_len;                     /* Message address length of current OLSR instance */
		    T_NODE_ADDRESS m_originator_address;    /* Message originator address of current OLSR instance */
            T_UINT8 m_time_to_live;                 /* Time to live of a message of current OLSR instance */
            T_UINT8 m_hop_count;                    /* Hop count of a message of current OLSR instance */
            T_UINT16 m_message_sequence_number;     /* Sequence number of a message of current OLSR instance */
            T_UINT8 m_validity_time;                /* Validity time of a message of current OLSR instance */
            T_UINT8 m_interval_time;                /* Interval time of a message of current OLSR instance */
			T_HELLO hello;     					    /* Hello message */
			T_TC tc;            					/* Topology control message */
		}T_MESSAGE;

	private:

		T_MESSAGE m_message;	/* Variable containing Hello and Tc messages*/


	public:


      /* Gets the Hello message received from neighbor and return the message content */
      const T_HELLO& get_hello() const;

      /* Gets the TC message received from neighbor and return the message content */
      const T_TC& get_tc() const;

      /* Set the message type to HELLO and return the message content */
      T_HELLO& get_hello();

      /* Set the message type to TC and return the message content */
      T_TC& get_tc();

      /* Finds  the size of the OLSR message */
      T_UINT16 get_msg_size();

      /* sets the common attributes like message type, hop count etc of OLSR message */
      void set_message_attributes(T_NODE_ADDRESS node_address, E_OLSR_MSG_TYPE msg_type, T_UINT16 msg_seq_num);
};

}

#endif /* OLSR_HEADER_HPP_ */
