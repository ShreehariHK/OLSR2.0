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
#include "array"					 /* Include file for C++ array functionalities */

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

    /* OLSR entry point function for INIT State */
    void init(ns_olsr2_0::E_OLSR_INSTANCE);

    /* OLSR entry point function for RUN State */
    void run(void);

    /* Resets the OLSR instance */
    void reset(void);

#ifdef M_TO_BE_DELETED

    /* Returns the Node address */
    T_ADDR get_node_address(void);

    /* Returns the Node willingness */
    T_UINT8 get_node_willingness(void);

    /* Returns in link cost between the neighbor node */
    float get_in_link_metric(void);

#endif

    /* Sends OLSR packet to Gateway */
    void send_olsr_msg(void);

    /* Receives OLSR packet from Gateway */
    void recv_olsr(void* olsr_packet);

    /* Gets the next hop router to the destination(Unicast/Multicast) */
    E_ROUTE_STATUS get_routing_destination_address(const T_ADDR& Csrc, const T_ADDR& Dest, T_ADDR& RDest);

    /* Checks the address type of the node address(Unicast/Multicast) */
    E_DEST_ADDR_TYPE check_address_type(const T_ADDR& Dest);

    /* Finds the allowed 1-hop neighbor tuple for the given address */
    template <typename T>
    T_BOOL find_allowed_one_hop_address(T_NODE_ADDRESS address, const std::vector<T>& tuple, T_UINT8* idx);  /* Finds the one_hop_address */

    /* Gets the Advertised neighbor sequence number of the node */
    T_UINT16
    get_ansn () const
    {
      return m_ansn;
    }

    /* Sets the Advertised neighbor sequence number of the node */
    void
    set_ansn (T_UINT16 ansn)
    {
      m_ansn = ansn;
    }

    /* Gets the Hello message interval of the node */
    Time
    get_hello_interval () const
    {
      return m_hello_interval;
    }

    /* Sets the Hello message interval of the node */
    void
    set_hello_interval (Time hello_interval)
    {
      m_hello_interval = hello_interval;
    }

    /* Checks whether the olsr instance is a leader */
    T_BOOL
    get_is_leader () const
    {
      return m_is_leader;
    }

    /* Sets olsr instance as a leader */
    void
    set_is_leader (T_BOOL is_leader)
    {
      m_is_leader = is_leader;
    }

    /* Gets the message sequence number of the node */
    T_UINT16
    get_message_sequence_number () const
    {
      return m_message_sequence_number;
    }

    /* Sets the message sequence number of the node */
    void
    set_message_sequence_number (T_UINT16 msg_seq_num)
    {
      m_message_sequence_number = msg_seq_num;
    }

    /* Gets the node address of the node */
    T_NODE_ADDRESS
    get_node_addr () const
    {
      return m_node_address;
    }

    /* Sets the node address of the node */
    void
    set_node_addr (T_NODE_ADDRESS node_addr)
    {
      m_node_address = node_addr;
    }

    /* Gets the operational state of the node */
    E_OPERATION_STATE
    get_op_state () const
    {
      return m_op_state;
    }

    /* Sets the operational state of the node */
    void
    set_op_state (E_OPERATION_STATE op_state)
    {
      m_op_state = op_state;
    }

    /* Gets the state data of the node */
    const C_OLSR_STATE&
    get_state () const
    {
      return m_state;
    }

    /* Sets the state data of the node */
    void
    set_state (const C_OLSR_STATE& state)
    {
      m_state = state;
    }

    /* Gets TC message interval of the node */
    Time
    get_tc_interval () const
    {
      return m_tc_interval;
    }

    /* Sets TC message interval of the node */
    void
    set_tc_interval (Time tc_interval)
    {
      m_tc_interval = tc_interval;
    }

    /* Gets willingness of the node */
    const U_WILLINGNESS&
    get_willingness () const
    {
      return m_willingness;
    }

    /* Sets willingness of the node */
    void
    set_willingness (const U_WILLINGNESS& willingness)
    {
      m_willingness = willingness;
    }

    /* Gets instance type of the node */
    const E_OLSR_INSTANCE&
    get_instance_type () const
    {
      return m_instance;
    }

    /* Sets instance type of the node */
    void
    set_instance_type (const E_OLSR_INSTANCE& inst_type)
    {
        m_instance = inst_type;
    }

    /* Segregates OLSR messages and calls for processing */
    void recv_olsr(const C_PACKET_HEADER, T_NODE_ADDRESS&, T_NODE_ADDRESS&, float);

    /* Prepares hello message and sends to the tx buffer */
    void send_hello(void);

    /* Processes  hello message received from neighbor node */
    void process_hello(const C_MESSAGE_HEADER&, const T_NODE_ADDRESS&, float);

    /* Senses the link and processes it*/
    void link_sensing(const C_MESSAGE_HEADER&, const C_MESSAGE_HEADER::T_HELLO&, const T_NODE_ADDRESS&, float);

    /* Creates a 1-hop tuple */
    void add_one_hop_neighbor_tuple(const C_MESSAGE_HEADER::T_HELLO&, const T_LINK_TUPLE*);

    /* Creates a 1-hop tuple */
    void update_one_hop_neighbor_tuple(const C_MESSAGE_HEADER::T_HELLO&, const T_LINK_TUPLE*);

    /* Checks if the address is a routing mpr selector*/
    T_BOOL check_is_routing_mpr_selector(const C_MESSAGE_HEADER::T_HELLO&) const ;

    /* Prepares Tc message and sends to the tx buffer */
    void send_tc(void);

    /* Processes  tc message received from neighbor node */
    void process_tc(const C_MESSAGE_HEADER&, const T_NODE_ADDRESS&);

    /* Forwards the TC messages received from Flooding mpr selectors*/
    void forward_default(C_MESSAGE_HEADER, T_NODE_ADDRESS&);

    /* wrapper function for MPR Computation */
    void mpr_computation();

    /* Finds the routing mprs of this node*/
    void routing_mpr_computation();

    /* Finds the flooding mprs of this node*/
    void flooding_mpr_computation();

    /* Computes the routing table */
    void routing_table_computation();

    /* Sets the common address to be present in OLSR message */
    //template <typename T>
    //void set_common_address(T& msg, E_ADDRESS_BLOCK_FLAGS common_field, T_UINT8 common_id);

    /********************************************************************
        * @function  set_common_address
        * @brief     This function sets the common address to OLSR message
        * @param     [1] msg - OLSR message
        *            [2] common_field - (Net ID Common/Node ID Common)
        *            [3] common_id - (Net ID/Node ID)
        * @return    None.
        * @note      None.
    ********************************************************************/
    template <typename T>
    void set_common_address(T& msg, E_ADDRESS_BLOCK_FLAGS common_field, T_UINT8 common_id)
    {
      /* Adds common field and common id to hello/tc message */
      msg.abf = common_field;
      msg.common_id = common_id;
    }

    /********************************************************************
        * @function  get_common_address
        * @brief     This function gets the common address from received
        *            OLSR message
        * @param     [1] msg - Received OLSR message
        *            [2] common_field - (Net ID Common/Node ID Common)
        *            [3] common_id - (Net ID/Node ID)
        * @return    None.
        * @note      None.
    ********************************************************************/
    template <typename T>
    void get_common_address(T& msg, E_ADDRESS_BLOCK_FLAGS* common_field, T_UINT8* common_id)
    {
      /* Adds common field and common id to hello/tc message */
      *common_field = msg.abf ;
      *common_id = msg.common_id;
    }

  private:


    E_OPERATION_STATE m_op_state;               /* Operation state of OLSR instance. */
    C_OLSR_STATE m_state;                       /* OLSR_STATE object to store State Information of OLSR instance. */

    T_UINT16 m_message_sequence_number;         /* Messages sequence number counter. */
    T_UINT16 m_ansn;                            /* Advertised Neighbor Set sequence number. */

    Time m_hello_interval;                      /* HELLO messages' transmission interval. */
    Time m_tc_interval;                         /* TC messages' transmission interval.  */

    U_WILLINGNESS m_willingness;                /* Willingness for forwarding packets on behalf of other nodes. */
    T_NODE_ADDRESS m_node_address;              /* Node address */

    T_BOOL m_is_leader;                         /* Flag to check whether the current olsr instance is a leader or not */
    E_OLSR_INSTANCE m_instance;                 /* Instance type(Normal node instance / Leader node instance) */

    C_MESSAGE_HEADER::OlsrMsgList olsr_msg_list; /* OLSR messages list */

    /* Initializes the OLSR main data members  */
    void init_op(ns_olsr2_0::E_OLSR_INSTANCE);

    /* Gets the next OLSR message sequence number */
    T_UINT16 get_next_msg_seq_num(void);

    /* Sets the leader information of the network */
    void set_leader_info(C_MESSAGE_HEADER::T_HELLO& hello_msg, T_LEADER_TUPLE* leader_tuple);



    /* Sets the neighbor information to be added in Hello message */
    void set_neighbor_info(C_MESSAGE_HEADER::T_HELLO& hello_msg, Time cur_time);

    /* Checks the validity of the received OLSR message */
    T_BOOL check_message_validity(const C_MESSAGE_HEADER& message_header);

    /* Creates a new advertising router tuple */
    T_ADVERTISING_REMOTE_ROUTER_TUPLE
    create_advertizing_router_tuple(const C_MESSAGE_HEADER&, const T_UINT16);

    T_ROUTER_TOPOLOGY_TUPLE
    create_router_topology_tuple(const C_MESSAGE_HEADER&, const C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK,const T_NODE_ADDRESS&);

    /* Creates a new processed message tuple */
    T_PROCESSED_MSG_TUPLE create_processed_msg_tuple(const C_MESSAGE_HEADER& msg_header);

    /* Creates a new forwarded message tuple */
    T_FORWARDED_MSG_TUPLE create_forwarded_msg_tuple(const C_MESSAGE_HEADER& msg_header);

    /* Creates a new link tuple */
    T_LINK_TUPLE
    create_new_link_tuple(const C_MESSAGE_HEADER& msg_header, Time cur_time, float in_metric);

    /* Creates a new 2-Hop tuple */
    T_TWO_HOP_NEIGHBOUR_TUPLE
    create_two_hop_neighbor_tuple(const C_MESSAGE_HEADER& msg, C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK neighb_info, const T_NODE_ADDRESS& neighb_addr);

    /* Adds a new 2-Hop tuple to 2-Hop set*/
    void add_two_hop_neighbor_tuple(const T_TWO_HOP_NEIGHBOUR_TUPLE& new_two_hop_tuple);

    /* Validates the neighbor type and link type  of a neighbor*/
    T_BOOL validate_neighbor_link_type(C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK iter);

    /* Updates the 2-hop set based on recieved OLSR message */
    void populate_two_hop_set(const C_MESSAGE_HEADER&, const C_MESSAGE_HEADER::T_HELLO&);

    /* Updates Routetr_topology set based on recieved Tc message */
    void populate_router_topology_set(const C_MESSAGE_HEADER&, const C_MESSAGE_HEADER::T_TC_ADDRESS_BLOCK &, E_ADDRESS_BLOCK_FLAGS, T_NODE_ADDRESS);

    /* Creates the allowed 1-hop set*/
    void create_allowed_one_hop_set(std::vector<T_ALLOWED_ONE_HOP_TUPLE> &p_allowed_one_hop_set, T_UINT8 p_mpr_type);

    /* Creates the allowed 2-hop set*/
    void create_allowed_two_hop_set(std::vector<T_ALLOWED_TWO_HOP_TUPLE> &p_allowed_two_hop_set, const std::vector<T_ALLOWED_ONE_HOP_TUPLE>& p_allowed_one_hop_set, float mpr_type);

    /* Creates N1-Neighbor graph */
    void
      create_n1_neighbor_graph(const std::vector<T_ALLOWED_ONE_HOP_TUPLE> &p_allowed_one_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph);

    /* Creates N2-Neighbor graph */
    void
      create_n2_neighbor_graph(const std::vector<T_ALLOWED_TWO_HOP_TUPLE> &p_allowed_two_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph);

    /* Computes total link metric(n1_metric + n2_metric) */
    void compute_total_metric(const std::vector<T_ALLOWED_TWO_HOP_TUPLE> &p_allowed_two_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph);

    /* Creates N-Neighbor graph */
    void create_n_neighbor_graph(const T_NEIGHBOR_GRAPH &p_neighbor_graph, std::vector<T_N2>& n_set);

    /* Calculate the MPRs of the node */
    void calculate_mprs(T_NEIGHBOR_GRAPH &p_neighbor_graph, T_UINT8 mpr_type);

    /* Adds the 1-hop neighbors to mpr set whose willingness is Always */
    void cover_always_willing_neighbors(MprSet& mpr_set, const T_NEIGHBOR_GRAPH& p_neighbor_graph,
                                        std::vector<T_N2>& remaining_two_hop_neighb, T_UINT8 mpr_type);

    void cover_isolated_neighbors(MprSet& mpr_set, std::vector<T_N2>& remaining_two_hop_neighb);

    /* Finds the 2-Hop neighbors covered from this given 1-Hop neighbor */
    void cover_two_hop_neighbors(const T_NODE_ADDRESS& one_hop_neighb, std::vector<T_N2>& remaining_two_hop_neighb);

    /* Finds the Reachability of all 1-Hop neighbor */
    void find_reachability(const std::vector<T_N1>& n1_neighbor_graph, const std::vector<T_N2>& remaining_two_hop_neighb,
                      std::set<T_UINT8>& rs, std::map<T_UINT8, std::vector<const T_N1*>>& reachability_map);

    /* Finds the degree of all 1-Hop neighbor */
    T_UINT8 find_degree(T_N1 one_hop_neighbor, const std::vector<T_N1>& n1_neighbor_graph, const std::vector<T_N2>& n_neighbor_graph);

    /*Finds maximum connectivity neighbor between the given 2 ngihbors */
    void find_max_neighbor(T_N1 const *max_neighbor, T_UINT8 max_neighbor_will, const T_N1* n1_graph_tuple,
                           T_UINT8 n1_graph_will, T_UINT8* r_count, T_UINT8* max_reachability);

    /* Creates Network Topology graph */
    void create_network_topology_graph(T_NETWORK_TOPOLOGY_GRAPH* p_network_topology_graph);

    /* Calculates the Routing Table */
    void calculate_routing_table(const T_NETWORK_TOPOLOGY_GRAPH& p_network_topology_graph);
  };

}

#endif /* OLSR_HPP_ */
