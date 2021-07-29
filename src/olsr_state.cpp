/*                        Project title
 *
 *
 * Source Code Name   :   olsr_state.cpp
 *
 * Description        :   Implements OLSR state functionality
 *
 * Subsystem Name     :   OLSR
 *
 * Revision History
 * ---------------------------------------------------------------------------|
 * Version | Change Description               |    Date    |    Changed By    |
 * --------|----------------------------------|------------|------------------|
 * 1.0     |Initial Version                   | 14-06-2021 | Akash kumar P    |
 * --------|----------------------------------|------------|------------------|
 *
 *                              Copyright statement
 *
 *
 */
#include "olsr_state.hpp"

namespace ns_olsr2_0
{
  /********************************************************************
   * @function  C_OLSR_STATE
   * @brief     This function is the constructor of C_OLSR_STATE.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  C_OLSR_STATE::C_OLSR_STATE ()
  {

  }

  /********************************************************************
   * @function  C_OLSR_STATE
   * @brief     This function is the destructor of C_OLSR_STATE.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  C_OLSR_STATE::~C_OLSR_STATE ()
  {

  }

  /********************************************************************
   * @function  get_one_hop_neighbors
   * @brief     This function returns the neighbour set.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  const NeighbourSet &
  C_OLSR_STATE::get_one_hop_neighbors () const
  {
    return m_neighbour_set;
  }

  /********************************************************************
   * @function  find_neighbour_tuple
   * @brief     This function finds the neighbor tuple
   *            corresponding to the given neighbor address and returns
   * @param     nbr_addr.
   * @return    None.
   * @note      None.
   ********************************************************************/
   T_NEIGHBOUR_TUPLE* C_OLSR_STATE::find_neighbour_tuple (const T_NODE_ADDRESS & nbr_addr)
   {
     for (std::vector<T_NEIGHBOUR_TUPLE>::iterator nbr_iterator = m_neighbour_set.begin ();
         nbr_iterator != m_neighbour_set.end (); nbr_iterator++)
           {
             if (nbr_iterator->n_neighbor_addr == nbr_addr)
               {
                 return &(*nbr_iterator);
               }
           }
         return NULL;
   }

   /********************************************************************
    * @function  find_sym_neighbour_tuple
    * @brief     This function finds the neighbor tuple
    *            corresponding to the given neighbor address and returns
    * @param     nbr_addr.
    * @return    None.
    * @note      None.
    ********************************************************************/
   const T_NEIGHBOUR_TUPLE*
   C_OLSR_STATE::find_sym_neighbour_tuple (const T_NODE_ADDRESS &main_addr) const
   {
     for (std::vector<T_NEIGHBOUR_TUPLE>::const_iterator nbr_iterator = m_neighbour_set.begin ();
              nbr_iterator != m_neighbour_set.end (); nbr_iterator++)
      {
        if ((nbr_iterator->n_neighbor_addr == main_addr) and (nbr_iterator->n_symmetric == true))
          {
            return &(*nbr_iterator);
          }
      }
    return NULL;

   }

  /********************************************************************
   * @function  get_two_hop_neighbours
   * @brief     This function returns the two hop neighbour set.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  const TwoHopNeighborSet &
  C_OLSR_STATE::get_two_hop_neighbours () const
  {
    return m_two_hop_neighbour_set;
  }

  /********************************************************************
   * @function  get_links
   * @brief     This function returns the link set.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  const LinkSet &
  C_OLSR_STATE::get_links () const
  {
    return m_link_set;
  }

  /********************************************************************
   * @function  find_link_tuple
   * @brief     This function finds a link tuple from link set .
   * @param     send_iface_addr.
   * @return    None.
   * @note      None.
   ********************************************************************/
  T_LINK_TUPLE*
  C_OLSR_STATE::find_link_tuple (const T_NODE_ADDRESS & send_iface_addr)
  {
    for (std::vector<T_LINK_TUPLE>::iterator link_iterator = m_link_set.begin ();
        link_iterator != m_link_set.end (); link_iterator++)
      {
        if (link_iterator->l_neighbor_iface_addr == send_iface_addr)
          {
            return &(*link_iterator);
          }
      }
    return NULL;

  }

  /********************************************************************
   * @function  insert_link_tuple
   * @brief     This function inserts a link tuple to link set .
   * @param     send_iface_addr.
   * @return    None.
   * @note      None.
   ********************************************************************/
  T_LINK_TUPLE&
  C_OLSR_STATE::insert_link_tuple (const T_LINK_TUPLE &new_link_tuple)
  {
    m_link_set.push_back(new_link_tuple);
    m_link_set.back();

  }

  /********************************************************************
   * @function  check_is_routing_mpr_selector
   * @brief     This function checks if the node is a routing mpr selector.
   * @param     send_iface_addr.
   * @return    None.
   * @note      None.
   ********************************************************************/

  T_BOOL C_OLSR_STATE::check_is_routing_mpr_selector(const T_NODE_ADDRESS &address) const
  {
     for(NeighbourSet::const_iterator nbr_tuple_iter = m_neighbour_set.begin();
         nbr_tuple_iter != m_neighbour_set.end(); nbr_tuple_iter++)
       {
         if((nbr_tuple_iter->n_neighbor_addr == address) and (nbr_tuple_iter->n_mpr_selector == true))
           {
             return true;
           }
       }
     return false;
  }

  /********************************************************************
   * @function  check_is_flooding_mpr_selector
   * @brief     This function checks if the node is a routing mpr selector.
   * @param     send_iface_addr.
   * @return    None.
   * @note      None.
   ********************************************************************/

  T_BOOL C_OLSR_STATE::check_is_flooding_mpr_selector(const T_NODE_ADDRESS &address) const
  {
     for(LinkSet::const_iterator link_tuple_iter = m_link_set.begin();
         link_tuple_iter != m_link_set.end(); link_tuple_iter++)
       {
         if((link_tuple_iter->l_neighbor_iface_addr == address) and (link_tuple_iter->l_mpr_selector == true))
           {
             return true;
           }
       }
     return false;
  }


  /********************************************************************
   * @function  get_advertising_remote_router_set
   * @brief     This function returns the advertising remote router
   *            set.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  const AdvertisingRemoteRouterSet &
  C_OLSR_STATE::get_advertising_remote_router_set () const
  {
    return m_advertising_remote_router_set;
  }

  /********************************************************************
   * @function  get_router_topology_set
   * @brief     This function returns the router toplogy set.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  const RouterTopologySet &
  C_OLSR_STATE::get_router_topology_set () const
  {
    return m_router_topology_set;
  }

  /********************************************************************
   * @function  insert_leader_tuple
   * @brief     This function stores the leader tuple information.
   * @param     leader_tuple.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR_STATE::insert_leader_tuple (T_LEADER_TUPLE leader_tuple)
  {

    m_leader_node[0] = leader_tuple;

    if ((m_leader_node.at (0).is_leader == true) and (m_leader_node.at (0).leader_addr.net_id == 10)
        and (m_leader_node.at (0).leader_addr.node_id == 0x0))
      {
        std::cout << "successsfully insertted leader node info " << std::endl;
      }

  }

  /********************************************************************
   * @function  get_leader_tuple
   * @brief     This function returns the leader address of the network.
   * @param     leader_tuple.
   * @return    leader_tuple.
   * @note      None.
   ********************************************************************/
  T_LEADER_TUPLE*
  C_OLSR_STATE::get_leader_tuple (void)
  {
    if (m_leader_node.size () != 0)
      {
        return &(m_leader_node.at (0));
      }
    else
      {
        //return (T_LEADER_TUPLE*)0;
        return NULL;
      }

  }

  /********************************************************************
   * @function  find_processed_msg_tuple
   * @brief     This function finds a processed message tuple and returns
   * @param     address.
   * @return    sequence_number.
   * @note      None.
   ********************************************************************/
  T_PROCESSED_MSG_TUPLE*
  C_OLSR_STATE::find_processed_msg_tuple (const T_NODE_ADDRESS &address, uint16_t sequence_number)
  {
    for (std::vector<T_PROCESSED_MSG_TUPLE>::iterator iterater = m_processed_msg_set.begin ();
        iterater != m_processed_msg_set.end (); iterater++)
      {
        if ((iterater->p_orig_addr == address) and (iterater->p_seq_number == sequence_number))
          {
            return &(*iterater);
          }

      }
    return NULL;
  }

  /********************************************************************
   * @function  insert_processed_msg_tuple
   * @brief     This function finds a forwarded message tuple and returns
   * @param     address.
   * @return    sequence_number.
   * @note      None.
   ********************************************************************/
  void C_OLSR_STATE::insert_processed_msg_tuple (const T_PROCESSED_MSG_TUPLE &tuple)
  {
    m_processed_msg_set.push_back(tuple);
  }

  /********************************************************************
   * @function  find_forwarded_msg_tuple
   * @brief     This function finds a forwarded message tuple and returns
   * @param     address.
   * @return    sequence_number.
   * @note      None.
   ********************************************************************/
  T_FORWARDED_MSG_TUPLE*
  C_OLSR_STATE::find_forwarded_msg_tuple (const T_NODE_ADDRESS &address, T_UINT16 sequence_number)
  {
    for (std::vector<T_FORWARDED_MSG_TUPLE>::iterator iterater = m_forwarded_msg_set.begin ();
        iterater != m_forwarded_msg_set.end (); iterater++)
      {
        if ((iterater->f_orig_addr == address) and (iterater->f_seq_number == sequence_number))
          {
            return &(*iterater);
          }

      }
    return NULL;

  }

  /********************************************************************
   * @function  insert_forwarded_msg_tuple
   * @brief     This function finds a forwarded message tuple and returns
   * @param     address.
   * @return    sequence_number.
   * @note      None.
   ********************************************************************/
  void C_OLSR_STATE::insert_forwarded_msg_tuple (const T_FORWARDED_MSG_TUPLE &tuple)
  {
    m_forwarded_msg_set.push_back(tuple);
  }

  /********************************************************************
   * @function  init_state_tuples
   * @brief     This function initializes all the OLSR state tuples.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR_STATE::init_state_tuples ()
  {
    m_link_set.clear ();
    m_neighbour_set.clear ();
    m_two_hop_neighbour_set.clear ();
    m_advertising_remote_router_set.clear ();
    m_router_topology_set.clear ();
    m_processed_msg_set.clear ();
    m_forwarded_msg_set.clear ();
    m_leader_node[0].is_leader = false;
    m_leader_node[0].leader_addr.net_id = 0xff;
    m_leader_node[0].leader_addr.node_id == 0xff;

    m_routing_table.clear ();
  }

  /********************************************************************
      * @function  check_link_set_timeout
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_link_set_timeout()
  {
    ;
  }

  /********************************************************************
      * @function  check_one_hop_neigh_set_timeout
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_one_hop_neigh_set_timeout()
  {
    ;
  }

  /********************************************************************
      * @function  check_two_hop_neigh_set_timeout
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_two_hop_neigh_set_timeout()
  {
    ;
  }

  /********************************************************************
      * @function  check_advt_router_set_timeout
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_advt_router_set_timeout()
  {
    ;
  }

  /********************************************************************
      * @function  check_router_topo_set_timeout
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_router_topo_set_timeout()
  {
    ;
  }

  /********************************************************************
      * @function  check_processed_msg_set_timeout
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_processed_msg_set_timeout()
  {
    ;
  }

  /********************************************************************
      * @function  check_forwarded_msg_set_timeout
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_forwarded_msg_set_timeout()
  {
    ;
  }

  /********************************************************************
      * @function  check_tables_timeout
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_tables_timeout()
  {
    check_link_set_timeout();
    check_one_hop_neigh_set_timeout();
    check_two_hop_neigh_set_timeout();
    check_advt_router_set_timeout();
    check_router_topo_set_timeout();
    check_processed_msg_set_timeout();
    check_forwarded_msg_set_timeout();

  }

  void
  C_OLSR_STATE::fill_tuples ()
  {
    for (int i = 0; i < 5; i++)
      {
        T_LINK_TUPLE loc_link_tup;
        T_NEIGHBOUR_TUPLE loc_nbr_tup;

        loc_link_tup.l_mpr_selector = true;
        loc_link_tup.l_neighbor_iface_addr.net_id = 10;
        loc_link_tup.l_neighbor_iface_addr.node_id = i + 5;
        loc_link_tup.l_heard_time = i + 9;
        loc_link_tup.l_time = i + 9;
        loc_link_tup.l_sym_time = i + 9;

        loc_nbr_tup.n_neighbor_addr = loc_link_tup.l_neighbor_iface_addr;

        if ((i == 1) or (i == 3))
          {

            loc_link_tup.l_in_metric = 0.9;
            loc_link_tup.l_out_metric = 0.8;
            loc_link_tup.l_status = SYMMETRIC_LINK;

            loc_nbr_tup.n_flooding_mpr = false;
            loc_nbr_tup.n_routing_mpr = true;
            loc_nbr_tup.n_advertised = true;
            loc_nbr_tup.n_in_metric = loc_link_tup.l_in_metric;
            loc_nbr_tup.n_out_metric = loc_link_tup.l_out_metric;
            loc_nbr_tup.n_mpr_selector = true;
            loc_nbr_tup.n_symmetric = true;
            loc_nbr_tup.n_willingness.willingness = 0x33;

          }
        else
          {
            loc_link_tup.l_in_metric = 0.6;
            loc_link_tup.l_out_metric = 0.9;
            loc_link_tup.l_status = SYMMETRIC_LINK;

            loc_nbr_tup.n_flooding_mpr = true;
            loc_nbr_tup.n_routing_mpr = false;
            loc_nbr_tup.n_advertised = false;
            loc_nbr_tup.n_in_metric = loc_link_tup.l_in_metric;
            loc_nbr_tup.n_out_metric = loc_link_tup.l_out_metric;
            loc_nbr_tup.n_mpr_selector = false;
            loc_nbr_tup.n_symmetric = true;
            loc_nbr_tup.n_willingness.willingness = 0x32;

          }

        m_link_set.push_back (loc_link_tup);
        m_neighbour_set.push_back (loc_nbr_tup);

      }

  }

}
