/*                            Project title
 *
 *
 * Source Code Name   :   olsr_state.hpp
 *        
 * Description        :   To store state information of olsr instance .
 * 
 * Subsystem Name     :   OLSR
 *        
 * Revision History               
 * ---------------------------------------------------------------------------|
 * Version | Change Description               |    Date    |    Changed By    |
 * --------|----------------------------------|------------|------------------|
 * 1.0     |Initial Version                   | 14-06-2021 |Shreehari H L     | 
 * --------|----------------------------------|------------|------------------|
 *           
 *                              Copyright statement                            
 *
 *
 */
#ifndef OLSR_STATE_HPP_
#define OLSR_STATE_HPP_

#include <map>
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

    AdvertisingRemoteRouterSet m_advertising_remote_router_set;     /* advertising remote router set of current OLSR instance */

    RouterTopologySet m_router_topology_set;                        /* router topology set of current OLSR instance */

    ProcessedMsgSet m_processed_msg_set;                            /* router topology set of current OLSR instance */
    ForwardedMsgSet m_forwarded_msg_set;                            /* router topology set of current OLSR instance */

    std::array<T_LEADER_TUPLE, 1> m_leader_node;

    std::map<T_NODE_ADDRESS, T_ROUTING_TABLE_ENTRY> m_routing_table;

  public:


    /* -------------------------------  [  Link set  ] ------------------------------------ */

    /* Gets the Link set.                                              */
    const LinkSet & get_links () const;

    /* Finds a link tuple.                                 */
    T_LINK_TUPLE*
    find_link_tuple (const T_NODE_ADDRESS &);

    /* Finds a symmetrical link tuple.                                 */
    T_LINK_TUPLE*
    find_sym_link_tuple (const T_NODE_ADDRESS &iface_addr, Time time);

    /* Erases a link tuple.                                            */
    void erase_link_tuple (const T_LINK_TUPLE &tuple);

    /* Inserts a link tuple.                                           */
    T_LINK_TUPLE& insert_link_tuple (const T_LINK_TUPLE &new_link_tuple);

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
    void erase_neighbour_tuple (const T_NEIGHBOUR_TUPLE &neighbour_tuple);

    /* Inserts a neighbor tuple.                                       */
    void insert_neighbour_tuple (const T_NEIGHBOUR_TUPLE &tuple);

    void check_one_hop_neigh_set_timeout();

    /* ----------------------------  [ Two hop neighbour ] -------------------------------- */

    /* Get the 2-hop neighbor set.                                     */
    const TwoHopNeighborSet & get_two_hop_neighbours () const;

    /* Finds a 2-hop neighbor tuple.                                   */
    T_TWO_HOP_NEIGHBOUR_TUPLE*
    find_two_hop_neighbour_tuple (const T_NODE_ADDRESS &neighbour, const T_NODE_ADDRESS &two_hop_neighbour);

    /* Erases a 2-hop neighbor tuple.                                  */
    void erase_two_hop_neighbour_tuple (const T_TWO_HOP_NEIGHBOUR_TUPLE &tuple);
	
    /* Erases the 2-hop neighbor tuples with the same 1-hop neighbor.  */
    void erase_two_hop_neighbour_tuples (const T_NODE_ADDRESS &neighbor);
	
    /*  Erases the 2-hop neighbor tuples with matching predicates.     */
    void
    erase_two_hop_neighbour_tuples (const T_NODE_ADDRESS &neighbour, const T_NODE_ADDRESS &two_hop_neighbour);
	
    /* Inserts a 2-hop neighbor tuple.                                 */
    void insert_two_hop_neighbour_tuple (const T_TWO_HOP_NEIGHBOUR_TUPLE &tuple);

    void check_two_hop_neigh_set_timeout();

    /* ---------------------------------  [   MPR Selector Set   ] --------------------------------- */

    /* Checks if the address is a routing mpr selector*/
    T_BOOL check_is_routing_mpr_selector(const T_NODE_ADDRESS &address) const ;

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

    void check_processed_msg_set_timeout();

    /* --------------------------------  [ Forwarded message ] ---------------------------- */

    /* Finds a Forwarded message tuple.                                */
    T_FORWARDED_MSG_TUPLE*
    find_forwarded_msg_tuple (const T_NODE_ADDRESS &address, T_UINT16 sequence_number);

    /* Erases a Forwarded message.                                     */
    void erase_forwarded_msg_tuple (const T_FORWARDED_MSG_TUPLE &tuple);
	
    /* Inserts a Forwarded message.                                    */

    /* Inserts a Forwarded message.                                    */
    void insert_forwarded_msg_tuple (const T_FORWARDED_MSG_TUPLE &tuple);

    void check_forwarded_msg_set_timeout();

    /* --------------------------------  [ Toplogy set ] ---------------------------------- */

    /* Gets the Advertising Remote Router set.                         */
    const AdvertisingRemoteRouterSet & get_advertising_remote_router_set () const;
	
    /* Finds a Advertising Remote Router tuple.                        */
    T_ADVERTISING_REMOTE_ROUTER_TUPLE*
    find_advertising_remote_router_tuple (const T_NODE_ADDRESS &dest_addr, const T_NODE_ADDRESS &last_addr);
	
    /* Finds a Advertising Remote Router tuple.                        */
    T_ADVERTISING_REMOTE_ROUTER_TUPLE*
    find_newer_advertising_remote_router_tuple (const T_NODE_ADDRESS &last_addr, uint16_t ansn);
	
    /* Erases a Advertising Remote Router tuple.                       */
    void erase_advertising_remote_router_tuple (const T_ADVERTISING_REMOTE_ROUTER_TUPLE &tuple);
	
    /* Erases a Advertising Remote Router tuple.                       */
    void erase_older_advertising_remote_router_tuple (const T_NODE_ADDRESS &last_addr, uint16_t ansn);
	
    /* Inserts a Advertising Remote Router tuple.                      */
    void insert_advertising_remote_router_tuple (const T_ADVERTISING_REMOTE_ROUTER_TUPLE &tuple);

    void check_advt_router_set_timeout();

    /* Gets the Advertising Remote Router set.                         */
    const RouterTopologySet & get_router_topology_set () const;
	
    /* Finds a Router Topology tuple.                                  */
    RouterTopologySet*
    find_router_topology_tuple (const T_NODE_ADDRESS &dest_addr, const T_NODE_ADDRESS &last_addr);
	
    /* Finds a Router Topology tuple.                                  */
    RouterTopologySet*
    find_newer_router_topology_tuple (const T_NODE_ADDRESS &last_addr, uint16_t ansn);
	
    /* Erases a Router Topology tuple.                                 */
    void erase_router_topology_tuple (const RouterTopologySet &tuple);
	
    /* Erases a Router Topology tuple tuple.                           */
    void erase_older_router_topology_tuple (const T_NODE_ADDRESS &last_addr, uint16_t ansn);
	
    /* Inserts a Router Topology tuple tuple.                          */
    void insert_router_topology_tuple (const RouterTopologySet &tuple);

    void check_router_topo_set_timeout();


    /**************** Additional functions *********************/

    void init_state_tuples();      /* Initialize all tuples*/

    void insert_leader_tuple(T_LEADER_TUPLE leader_tuple);  /*Insert a leader tuple */

    T_LEADER_TUPLE* get_leader_tuple();      /* Returns the leader tuple */

    void fill_tuples();

    void check_tables_timeout();

  };

}
#endif /* OLSR_STATE_HPP_ */
