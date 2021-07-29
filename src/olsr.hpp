/*                            Project title
 *
 *
 * Source Code Name   :   olsr.hpp
 *        
 * Description        :   To store main data of olsr instance .
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
#ifndef OLSR_HPP_
#define OLSR_HPP_

#include "olsr_header.hpp"           /* Include file for hearder functionalities */
#include "olsr_state.hpp"            /* Include file for olsr state functionalities */
#include "array"

namespace ns_olsr2_0
{


  class C_OLSR
  {

  public:
    C_OLSR();

    ~C_OLSR();

    /**
     * OLSR operational states
     */
    typedef enum
    {
      INIT_STATE = 0X0,
      RUN_STATE = 0X1
    }E_OPERATION_STATE;


    void init(ns_olsr2_0::E_OLSR_INSTANCE);    /* OLSR entry point function for INIT State */

    void run(void);     /* OLSR entry point function for RUN State */

    void reset(void);    /* Resets the OLSR instance */

#ifdef M_TO_BE_DELETED
    T_ADDR get_node_address(void);  /* Returns the Node address */

    T_UINT8 get_willingness(void);  /* Returns the Node willingness */

    float get_in_link_metric(void); /* Returns in link cost between the neighbor node */

#endif

    void send_olsr_msg(void);   /* Sends OLSR packet to Gateway */

    void recv_olsr(void* olsr_packet);  /* Receives OLSR packet from Gateway */

    T_ADDR* get_routing_destination_address(T_ADDR* Csrc, T_ADDR* Dest, T_ADDR* RDest);

    template <typename T>
    T_BOOL find_allowed_one_hop_address(T_NODE_ADDRESS address, const std::vector<T>& tuple, T_UINT8* idx);  /* Finds the one_hop_address */

    E_OPERATION_STATE
    getOperationState () const
    {
      return m_op_state;
    }

    T_UINT16
    getAnsn () const
    {
      return m_ansn;
    }

    void
    setAnsn (T_UINT16 ansn)
    {
      m_ansn = ansn;
    }

    Time
    getHelloInterval () const
    {
      return m_hello_interval;
    }

    void
    setHelloInterval (Time helloInterval)
    {
      m_hello_interval = helloInterval;
    }

    T_NODE_ADDRESS
    getInterfaceIpAddress () const
    {
      return m_interface_ip_address;
    }

    void
    setInterfaceIpAddress (T_NODE_ADDRESS interfaceIpAddress)
    {
      m_interface_ip_address = interfaceIpAddress;
    }

    T_BOOL
    isIsLeader () const
    {
      return m_is_leader;
    }

    void
    setIsLeader (T_BOOL isLeader)
    {
      m_is_leader = isLeader;
    }

    T_UINT16
    getMessageSequenceNumber () const
    {
      return m_message_sequence_number;
    }

    void
    setMessageSequenceNumber (T_UINT16 messageSequenceNumber)
    {
      m_message_sequence_number = messageSequenceNumber;
    }

    T_NODE_ADDRESS
    get_node_addr () const
    {
      return m_node_address;
    }

    void
    set_node_addr (T_NODE_ADDRESS nodeAddress)
    {
      m_node_address = nodeAddress;
    }

    E_OPERATION_STATE
    getOpState () const
    {
      return m_op_state;
    }

    void
    setOpState (E_OPERATION_STATE opState)
    {
      m_op_state = opState;
    }

    const C_OLSR_STATE&
    getState () const
    {
      return m_state;
    }

    void
    setState (const C_OLSR_STATE& state)
    {
      m_state = state;
    }

    Time
    getTcInterval () const
    {
      return m_tc_interval;
    }

    void
    setTcInterval (Time tcInterval)
    {
      m_tc_interval = tcInterval;
    }

    const U_WILLINGNESS&
    getWillingness () const
    {
      return m_willingness;
    }

    void
    setWillingness (const U_WILLINGNESS& willingness)
    {
      m_willingness = willingness;
    }

    void recv_olsr(const C_PACKET_HEADER, T_NODE_ADDRESS&, T_NODE_ADDRESS&, float);    /* Segregates OLSR messages and calls for processing */

    void send_hello(void);  /* Prepares hello message and sends to the tx buffer */

    void process_hello(const C_MESSAGE_HEADER&, const T_NODE_ADDRESS&, float);   /* Processes  hello message received from neighbor node */

    void link_sensing(const C_MESSAGE_HEADER&, const C_MESSAGE_HEADER::T_HELLO&, const T_NODE_ADDRESS&, float); /* Sense the link and processes it*/

    void process_tc(const C_MESSAGE_HEADER&, const T_NODE_ADDRESS&);  /* Processes  tc message received from neighbor node */

    void forward_default(const C_MESSAGE_HEADER&);  /* Forwards the TC messages received from Flooding mpr selectors*/

    void mpr_computation();   /* wrapper function for MPR Computation */
    void routing_mpr_computation();   /* Finds the routing mprs of this node*/
    void flooding_mpr_computation();  /* Finds the flooding mprs of this node*/

    void routing_table_computation();   /* Computes the routing table */

  private:


    E_OPERATION_STATE m_op_state;               /* Operation state of OLSR instance. */

    C_OLSR_STATE m_state;                       /* OLSR_STATE object to store State Information of OLSR instance. */

    T_NODE_ADDRESS m_interface_ip_address;

    T_UINT16 m_message_sequence_number;         /* Messages sequence number counter. */
    T_UINT16 m_ansn;                            /* Advertised Neighbor Set sequence number. */

    Time m_hello_interval;                      /* HELLO messages' transmission interval. */
    Time m_tc_interval;                         /* TC messages' transmission interval.  */
    U_WILLINGNESS m_willingness;                      /* Willingness for forwarding packets on behalf of other nodes. */

    T_NODE_ADDRESS m_node_address;              /* Node address */

    T_BOOL m_is_leader;                         /* Flag to check whether the current node is a leader or not */

    E_OLSR_INSTANCE m_instance;                 /* Instance type(Normal node instance / Leader node instance) */

    C_MESSAGE_HEADER::OlsrMsgList olsr_msg_list;

    void init_op(ns_olsr2_0::E_OLSR_INSTANCE);  /* Initializes the OLSR main data members  */

    T_UINT16 get_next_msg_seq_num(void);    /* Prepares the next OLSR message sequence number and sends */

    void set_leader_info(C_MESSAGE_HEADER::T_HELLO& hello_msg, T_LEADER_TUPLE* leader_tuple);

    void set_common_address(C_MESSAGE_HEADER::T_HELLO& hello_msg, E_ADDRESS_BLOCK_FLAGS common_field, T_UINT8 common_id);

    void set_neighbor_info(C_MESSAGE_HEADER::T_HELLO& hello_msg, Time cur_time);

    T_BOOL check_message_validity(const C_MESSAGE_HEADER& message_header);

    T_PROCESSED_MSG_TUPLE create_processed_msg_tuple(const C_MESSAGE_HEADER& msg_header);

    T_FORWARDED_MSG_TUPLE create_forwarded_msg_tuple(const C_MESSAGE_HEADER& msg_header);

    T_LINK_TUPLE
    create_new_link_tuple(const C_MESSAGE_HEADER& msg_header, Time cur_time, float in_metric);

    T_BOOL validate_neighbor_link_type(C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK iter);

    void populate_two_hop_set();

    void create_allowed_one_hop_set(std::vector<T_ALLOWED_ONE_HOP_TUPLE> &p_allowed_one_hop_set, T_UINT8 p_mpr_type);

    void create_allowed_two_hop_set(std::vector<T_ALLOWED_TWO_HOP_TUPLE> &p_allowed_two_hop_set, const std::vector<T_ALLOWED_ONE_HOP_TUPLE>& p_allowed_one_hop_set, float mpr_type);

    void
      create_n1_neighbor_graph(const std::vector<T_ALLOWED_ONE_HOP_TUPLE> &p_allowed_one_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph);

    void
      create_n2_neighbor_graph(const std::vector<T_ALLOWED_TWO_HOP_TUPLE> &p_allowed_two_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph);

    void compute_total_metric(const std::vector<T_ALLOWED_TWO_HOP_TUPLE> &p_allowed_two_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph);

    void calculate_mprs(const T_NEIGHBOR_GRAPH &p_neighbor_graph);

    void create_network_topology_graph(T_NETWORK_TOPOLOGY_GRAPH* p_network_topology_graph);

    void calculate_routing_table(const T_NETWORK_TOPOLOGY_GRAPH& p_network_topology_graph);
  };

}

#endif /* OLSR_HPP_ */
