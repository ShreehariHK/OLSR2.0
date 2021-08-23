/*               DEAL MANET Waveform Software Components
 *
 *
 * Source Code Name        : olsr_state.hpp
 *
 * Source Code Part Number : MNTWSC-321-DI-0004
 *
 * Description             : To store state information of olsr instance .
 * 
 * Subsystem Name          : OLSR
 *        
 * Revision History               
 * ---------------------------------------------------------------------------|
 * Version | Change Description               |    Date    |    Changed By    |
 * --------|----------------------------------|------------|------------------|
 * 1.0     |Initial Version                   | 14-06-2021 |Shreehari H L     | 
 * --------|----------------------------------|------------|------------------|
 *           
 * COPYRIGHT © Defence Electronics Applications Laboratory (DEAL), Raipur Road, Dehradun - 2480017.
 *
 * PROPRIETARY - This document and the information contained herein is the property of DEAL,
 * and must not be disclosed, copied, altered or used without written permission.
 *
 */
#ifndef OLSR_STATE_HPP_
#define OLSR_STATE_HPP_

#include "olsr_types.hpp"    /* Include file for OLSR types functionalities */

namespace ns_olsr2_0
{

  class C_OLSR_STATE
  {

  public:
    C_OLSR_STATE();
    virtual ~C_OLSR_STATE();


  private:
    LinkSet m_link_set;                                             /* link set of current OLSR instance */
    TwoHopNeighborSet m_two_hop_neighbour_set;                      /* two hop neighbor set of current OLSR instance */
    NeighbourSet m_neighbour_set;                                   /* one hop neighbor set of current OLSR instance */
    NeighbourSet m_routing_mpr_sel_set;
    AdvertisingRemoteRouterSet m_advertising_remote_router_set;     /* advertising remote router set of current OLSR instance */

    RouterTopologySet m_router_topology_set;                        /* router topology set of current OLSR instance */

    ProcessedMsgSet m_processed_msg_set;                            /* router topology set of current OLSR instance */
    ForwardedMsgSet m_forwarded_msg_set;                            /* router topology set of current OLSR instance */

    std::array<T_LEADER_TUPLE, M_ONE> m_leader_node;		        /* Leader node information of the network */

#ifdef COMMENT_SECTION
    std::map<T_NODE_ADDRESS, T_ROUTING_TABLE_ENTRY> m_routing_table; /* Routing table of the current OLSR instance */
#endif
    std::vector<T_ROUTING_TABLE_ENTRY> m_routing_table;

  public:


    /* -------------------------------  [  Link set  ] ------------------------------------ */

    /* Gets the Link set.                                              */
    const LinkSet & get_links () const;

    /* Finds a link tuple.                                 */
    T_LINK_TUPLE*
    find_link_tuple (const T_NODE_ADDRESS &);

    /* Finds a symmetrical link tuple.                                 */
    T_LINK_TUPLE*
    find_sym_link_tuple (const T_NODE_ADDRESS &iface_addr);

    /* Erases a link tuple.                                            */
    void erase_link_tuple (const T_LINK_TUPLE &tuple);

    /* Inserts a link tuple.                                           */
    T_LINK_TUPLE& insert_link_tuple (const T_LINK_TUPLE &new_link_tuple);

    /* Checks the timeout of the tuples in Linkset*/
    void check_link_set_timeout();
  
    /* ----------------------------  [ Neighbour Set ] ------------------------------------ */
    /* Get neighbour set                      */
    const NeighbourSet &get_one_hop_neighbors () const;


    /* Find a neighbor tuple.                                          */
    T_NEIGHBOUR_TUPLE* find_neighbour_tuple (const T_NODE_ADDRESS &);

    /* Find a symmetrical neighbor tuple.                              */
    const T_NEIGHBOUR_TUPLE*
    find_sym_neighbour_tuple (const T_NODE_ADDRESS &main_addr) const;

    /* Erases a neighbor tuple.                                        */
    void update_neighbour_tuple (const T_NODE_ADDRESS &neighb_main_addr);

    /* Erases a neighbor tuple.                                        */
    void erase_neighbour_tuple (const T_NODE_ADDRESS &neighb_main_addr);

    /* Inserts a neighbor tuple.                                       */
    void insert_neighbour_tuple (const T_NEIGHBOUR_TUPLE &);

    #ifdef COMMENT_SECTION
    /* Checks the timeout of the tuples in 1-Hop neighbor set*/
    void check_one_hop_neigh_set_timeout();
    #endif

    void populate_mpr_set(const MprSet& mpr_set, T_UINT8 mpr_type);

    /* ----------------------------  [ Two hop neighbour ] -------------------------------- */

    /* Get the 2-hop neighbor set.                                     */
    const TwoHopNeighborSet & get_two_hop_neighbours () const;

    /* Finds a 2-hop neighbor tuple.                                   */
    T_TWO_HOP_NEIGHBOUR_TUPLE*
    find_two_hop_neighbour_tuple (const T_NODE_ADDRESS &one_hop_neighbor, const T_NODE_ADDRESS &two_hop_neighbor);

    /* Erases a 2-hop neighbor tuple.                                  */
    void erase_two_hop_neighbour_tuple (const T_TWO_HOP_NEIGHBOUR_TUPLE &tuple);
	
    /* Erases the 2-hop neighbor tuples with the same 1-hop neighbor.  */
    void erase_two_hop_neighbour_tuples (const T_NODE_ADDRESS &neighbor);
	
    /*  Erases the 2-hop neighbor tuples with matching predicates.     */
    void
    erase_two_hop_neighbour_tuples (const T_NODE_ADDRESS &one_hop_neighbor, const T_NODE_ADDRESS &two_hop_neighbor);
	
    /* Inserts a 2-hop neighbor tuple.                                 */
    void insert_two_hop_neighbour_tuple (const T_TWO_HOP_NEIGHBOUR_TUPLE &two_hop_tuple);

    /* Checks the timeout of the tuples in 2-Hop neighbor set */
    void check_two_hop_neigh_set_timeout();

    /* ---------------------------------  [   MPR Selector Set   ] --------------------------------- */

    /* Checks if the address is a routing mpr selector*/
    T_BOOL check_is_routing_mpr_selector(const T_NODE_ADDRESS &address) const ;

    /* Gets Routing MPR selector set */
    const NeighbourSet& get_routing_mpr_selector_set ();

    /* Checks if the address is a routing mpr selector*/
    T_BOOL check_is_flooding_mpr_selector(const T_NODE_ADDRESS &address) const ;


    /* --------------------------------  [ Processed message ] ---------------------------- */

    /* Finds a Processed message tuple.                                */
    T_PROCESSED_MSG_TUPLE*
    find_processed_msg_tuple (const T_NODE_ADDRESS &address, uint16_t sequence_number);

    /* Erases a Processed message.                                     */
    void erase_processed_msg_tuple (const T_PROCESSED_MSG_TUPLE &tuple);

    /* Inserts a Processed message.                                    */
    void insert_processed_msg_tuple (const T_PROCESSED_MSG_TUPLE &tuple);

    /* Checks the timeout of the tuples in processed message set */
    void check_processed_msg_set_timeout();

    /* --------------------------------  [ Forwarded message ] ---------------------------- */

    /* Finds a Forwarded message tuple.                                */
    T_FORWARDED_MSG_TUPLE*
    find_forwarded_msg_tuple (const T_NODE_ADDRESS &address, T_UINT16 sequence_number);

    /* Erases a Forwarded message.                                     */
    void erase_forwarded_msg_tuple (const T_FORWARDED_MSG_TUPLE &tuple);
	
    /* Inserts a Forwarded message.                                    */
    void insert_forwarded_msg_tuple (const T_FORWARDED_MSG_TUPLE &tuple);

    /* Checks the timeout of the tuples in forwarded message set */
    void check_forwarded_msg_set_timeout();

    /* --------------------------------  [ Toplogy set ] ---------------------------------- */

    /* Gets the Advertising Remote Router set.                         */
    const AdvertisingRemoteRouterSet & get_advertising_router_set () const;

    /* Finds a Advertising Remote Router tuple.                        */
    T_ADVERTISING_REMOTE_ROUTER_TUPLE*
    find_advertising_router_tuple (const T_NODE_ADDRESS &dest_addr, const T_NODE_ADDRESS &last_addr);
	
    /* Finds a Advertising Remote Router tuple.                        */
    T_ADVERTISING_REMOTE_ROUTER_TUPLE*
    find_newer_advertising_router_tuple (const T_NODE_ADDRESS&, T_UINT16);
	
    /* Erases a Advertising Remote Router tuple.                       */
    void erase_advertising_router_tuple (const T_ADVERTISING_REMOTE_ROUTER_TUPLE &tuple);
	
    /* Erases a Advertising Remote Router tuple.                       */
    void erase_older_advertising_router_tuple (const T_NODE_ADDRESS &last_addr, T_UINT16 ansn);
	
    /* Inserts a Advertising Remote Router tuple.                      */
    void insert_advertising_router_tuple (const T_ADVERTISING_REMOTE_ROUTER_TUPLE &tuple);

    /* Checks the timeout of the tuples in advertised routers set */
    void check_advt_router_set_timeout();

    /* Gets the Advertising Remote Router set.                         */
    const RouterTopologySet & get_router_topology_set () const;
	
    /* Finds a Router Topology tuple.                                  */
    T_ROUTER_TOPOLOGY_TUPLE*
    find_router_topology_tuple (const T_NODE_ADDRESS &dest_addr, const T_NODE_ADDRESS &last_addr);

    /* Erases a Older Router Topology tuple.                                 */
    void erase_older_router_topology_tuple (const T_NODE_ADDRESS &, T_UINT16);
	
    /* Erases a Router Topology tuple.                                 */
    void erase_router_topology_tuple (const T_NODE_ADDRESS&);
	
    /* Inserts a Router Topology tuple tuple.                          */
    void insert_router_topology_tuple (const T_ROUTER_TOPOLOGY_TUPLE &tuple);

    /* Checks the timeout of the tuples in Router Topology set */
    void check_router_topo_set_timeout();


    /**************** Additional functions *********************/

    /* Initialize all tuples*/
    void init_state_tuples();

    /*Insert a leader tuple */
    void insert_leader_tuple(T_LEADER_TUPLE leader_tuple);

    /* Returns the leader tuple */
    T_LEADER_TUPLE* get_leader_tuple();

    /* Fills Link and Neighbor tuples*/
    void fill_tuples();

    /* Checks the timeout of all table entries */
    void check_tables_timeout();

    /* Creates and adds the new routing tuple to Routing Table*/
    void insert_routing_tuple(T_NETWORK_LINK const &new_route_data, const T_NODE_ADDRESS& next_address, T_UINT8 hop_count);

    /* Erases the complete Routing Table */
    void erase_routing_table();

    /* Prints the complete routing table*/

    void print_routing_table();

#ifdef COMMENT_SECTION
    /* Gets the complete Routing Table */
    std::map<T_NODE_ADDRESS, T_ROUTING_TABLE_ENTRY>& get_routing_table(void);
#endif

    /* Gets the complete Routing Table */
    std::vector<T_ROUTING_TABLE_ENTRY>& get_routing_table(void);

    /* Finds the Routing Table with the given distination address */
    T_ROUTING_TABLE_ENTRY* find_routing_tuple(const T_NODE_ADDRESS& dest_addr, float new_metric);

    /* Finds the next routing node's address */
    E_ROUTE_STATUS find_next_routing_hop_addr(const T_ADDR* p_dest_addr, T_ADDR* p_rdest_addr);

    /* Gets the complete routing table */
    void get_network_topology_info(std::vector<T_NETWORK_TOPOLOGY_TUPLE>& topology_table);

    /* Dummy functions
     * Fill the tables which are necessary for Routing Table Computation */

    void fill_tables_for_routing_comp();

    /* Back up able */

#ifdef COMMENT_SECTION
    std::vector<T_ROUTING_TABLE_ENTRY>& get_back_up_routing_table(void);

    T_ROUTING_TABLE_ENTRY* find_back_up_routing_tuple(const T_NODE_ADDRESS& dest_addr, float new_metric);

    void erase_back_up_routing_table();


    void
    insert_table_specifically(T_NETWORK_LINK const &new_route_data, const T_NODE_ADDRESS& next_address, T_UINT8 hop_count,
                                            std::vector<T_ROUTING_TABLE_ENTRY>::iterator  &position);
#endif
  };

}
#endif /* OLSR_STATE_HPP_ */
