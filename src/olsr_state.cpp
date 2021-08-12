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
   * @brief     This function returns the 1-Hop neighbour set.
   * @param     None.
   * @return    m_neighbour_set.
   * @note      None.
   ********************************************************************/
  const NeighbourSet &
  C_OLSR_STATE::get_one_hop_neighbors () const
  {
    return m_neighbour_set;
  }

  /********************************************************************
   * @function  insert_neighbour_tuple
   * @brief     This function inserts the tuple to 1-Hop neighbor set
   * @param     [1] new_neighb_tuple - 1-Hop neighbor tuple.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR_STATE::insert_neighbour_tuple (const T_NEIGHBOUR_TUPLE &new_neighb_tuple)
  {
    m_neighbour_set.push_back(new_neighb_tuple);
  }

  /********************************************************************
   * @function  find_neighbour_tuple
   * @brief     This function finds the neighbor tuple
   *            corresponding to the given neighbor address and returns.
    * @param    [1] nbr_addr - 1-Hop neighbor address.
    * @return   Reference of 1-Hop neighbor tuple.
   * @note      None.
   ********************************************************************/
   T_NEIGHBOUR_TUPLE* C_OLSR_STATE::find_neighbour_tuple (const T_NODE_ADDRESS & nbr_addr)
   {
	 /* Loops through the 1-Hop neighbor set and finds the given address is matching with
	  * any of the 1-Hop neighbors' address */
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
    * @brief     This function finds the symmetric neighbor tuple
    *            corresponding to the given neighbor address and returns.
    * @param     [1] nbr_addr - 1-Hop neighbor address.
    * @return    Reference of Symmetric 1-Hop neighbor tuple.
    * @note      None.
    ********************************************************************/
   const T_NEIGHBOUR_TUPLE*
   C_OLSR_STATE::find_sym_neighbour_tuple (const T_NODE_ADDRESS &main_addr) const
   {
	 /* Loops through the 1-Hop neighbor set and finds the given address is matching with
	 * any of the symmetric 1-Hop neighbors' address */
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
     * @function  update_neighbour_tuple
     * @brief     This function finds the symmetric neighbor tuple
     *            corresponding to the given neighbor address and returns.
     * @param     [1] nbr_addr - 1-Hop neighbor address.
     * @return    None.
     * @note      None.
     ********************************************************************/
   void
   C_OLSR_STATE::update_neighbour_tuple (const T_NODE_ADDRESS &neighb_main_addr)
   {
     /* Finds the given neighbor in the neighbor set and
      * make that neighbor as not asymmetric*/
     for (std::vector<T_NEIGHBOUR_TUPLE>::iterator nbr_iterator = m_neighbour_set.begin ();
                   nbr_iterator != m_neighbour_set.end (); nbr_iterator++)
           {
             if (nbr_iterator->n_neighbor_addr == neighb_main_addr)
               {

                 nbr_iterator->n_symmetric = false;
               }
           }

   }
   /********************************************************************
     * @function  erase_neighbour_tuple
     * @brief     This function finds the symmetric neighbor tuple
     *            corresponding to the given neighbor address and erases it.
     * @param     [1] nbr_addr - 1-Hop neighbor address.
     * @return    None.
     * @note      None.
     ********************************************************************/
   void
   C_OLSR_STATE::erase_neighbour_tuple (const T_NODE_ADDRESS &neighb_main_addr)
   {
     /* Finds the given neighbor in the neighbor set and
      * removes it*/
     for (std::vector<T_NEIGHBOUR_TUPLE>::iterator nbr_iterator = m_neighbour_set.begin ();
                   nbr_iterator != m_neighbour_set.end ();)
           {
             if (nbr_iterator->n_neighbor_addr == neighb_main_addr)
               {

                 nbr_iterator = m_neighbour_set.erase(nbr_iterator);
               }
             else
               {
                 nbr_iterator++;
               }
           }

   }
  /********************************************************************
    * @function  populate_mpr_set
    * @brief     This function finds the neighbor address in the mpr set.
    *            If found then make him as MPR .
    * @param     [1] mpr_set - 1-Hop neighbor address.
    *            [2] mpr_type - Routing/Flooding type
    * @return    None.
    * @note      None.
    ********************************************************************/
   void
   C_OLSR_STATE::populate_mpr_set(const MprSet& mpr_set, T_UINT8 mpr_type)
   {
     if(mpr_type == M_ROUTING_MPR)
       {
         /* Finds the 1-hop neighbor in the mpr set, if found then,
          * makes him as Routing MPR */
         for (std::vector<T_NEIGHBOUR_TUPLE>::iterator nbr_iterator = m_neighbour_set.begin ();
                           nbr_iterator != m_neighbour_set.end (); nbr_iterator++)
           {
             if (mpr_set.find(nbr_iterator->n_neighbor_addr) != mpr_set.end())
               {

                 nbr_iterator->n_routing_mpr = true;
               }
           }

       }
     else
       {
         /* Finds the 1-hop neighbor in the mpr set, if found then,
         * makes him as Flooding MPR */
         for (std::vector<T_NEIGHBOUR_TUPLE>::iterator nbr_iterator = m_neighbour_set.begin ();
                            nbr_iterator != m_neighbour_set.end (); nbr_iterator++)
          {
            if (mpr_set.find(nbr_iterator->n_neighbor_addr) != mpr_set.end())
              {

                nbr_iterator->n_flooding_mpr = true;
              }
          }

        }


   }


  /********************************************************************
   * @function  get_two_hop_neighbours
   * @brief     This function returns the 2-hop neighbor set.
   * @param     None.
   * @return    m_two_hop_neighbour_set.
   * @note      None.
   ********************************************************************/
  const TwoHopNeighborSet &
  C_OLSR_STATE::get_two_hop_neighbours () const
  {
    return m_two_hop_neighbour_set;
  }

  /********************************************************************
    * @function  insert_two_hop_neighbour_tuple
    * @brief     This function inserts 2-Hop tuple to
    *                2-Hop set.
    * @param     [1] two_hop_tuple - New 2-Hop neighbor
    * @return    None.
    * @note      None.
    ********************************************************************/
  void
  C_OLSR_STATE::insert_two_hop_neighbour_tuple (const T_TWO_HOP_NEIGHBOUR_TUPLE &two_hop_tuple)
  {
    m_two_hop_neighbour_set.push_back(two_hop_tuple);
  }

  /********************************************************************
    * @function  find_two_hop_neighbour_tuple
    * @brief     This function finds the two_hop neighbor connected
    *            through the given 1-hop neighbor.
    * @param     [1] one_hop_neighbor - 1-Hop neighbor.
    *            [2] two_hop_neighbor - 2-Hop neighbor
    * @return    ADdress 2-Hop neighbor tuple.
    * @note      None.
    ********************************************************************/
  T_TWO_HOP_NEIGHBOUR_TUPLE*
  C_OLSR_STATE::find_two_hop_neighbour_tuple (const T_NODE_ADDRESS &one_hop_neighbor, const T_NODE_ADDRESS &two_hop_neighbor)
  {

    /* Loops through the 2-Hop neighbor set and finds whether the 2-Hop neighbor is
         * connected through this given 1-Hop neighbor,
         * If found then remove that tuple*/

    for (TwoHopNeighborSet::iterator two_hop_iter = m_two_hop_neighbour_set.begin ();
             two_hop_iter != m_two_hop_neighbour_set.end ();)
    {
       if((two_hop_iter->n2_2hop_addr == two_hop_neighbor) and
           (two_hop_iter->n2_neighbor_iface_addr == one_hop_neighbor))
         {
           return &(*two_hop_iter);
         }
    }
    return NULL;

  }

  /********************************************************************
    * @function  erase_two_hop_neighbour_tuples
    * @brief     This function removes the 2-Hop tuples
    *            which are connected from the given 1-hop neigbor.
    * @param     [1] one_hop_neighbor - 1-Hop neighbor.
    *            [2] two_hop_neighbor - 2-Hop neighbor
    * @return    None.
    * @note      None.
    ********************************************************************/
  void
  C_OLSR_STATE::erase_two_hop_neighbour_tuples (const T_NODE_ADDRESS &one_hop_neighbor, const T_NODE_ADDRESS &two_hop_neighbor)
  {
    /* Loops through the 2-Hop neighbor set and finds whether the 2-Hop neighbor is
     * connected through this given 1-Hop neighbor,
     * If found then remove that tuple*/
     for (TwoHopNeighborSet::const_iterator two_hop_iter = m_two_hop_neighbour_set.begin ();
         two_hop_iter != m_two_hop_neighbour_set.end ();)
      {
         if((two_hop_iter->n2_2hop_addr == two_hop_neighbor) and
             (two_hop_iter->n2_neighbor_iface_addr == one_hop_neighbor))
           {
             two_hop_iter = m_two_hop_neighbour_set.erase(two_hop_iter);
           }
         else
           {
             two_hop_iter++;
           }
      }

  }

  /********************************************************************
    * @function  get_links
    * @brief     This function returns the link set.
    * @param     None.
    * @return    m_link_set.
    * @note      None.
    ********************************************************************/
  const LinkSet &
  C_OLSR_STATE::get_links () const
  {
    return m_link_set;
  }

  /********************************************************************
   * @function  find_link_tuple
   * @brief     This function finds a link tuple from link set
   * 			corresponding to the given sender address and returns .
   * @param     send_iface_addr.
   * @return    Reference of link tuple.
   * @note      None.
   ********************************************************************/
  T_LINK_TUPLE*
  C_OLSR_STATE::find_link_tuple (const T_NODE_ADDRESS & send_iface_addr)
  {
	  /* Loops through the link set and finds the given address is matching with
	   * neighbor address of any of the link tuples  */
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
   * @function  find_sym_link_tuple
   * @brief     This function finds a symmetric link tuple from link set
   * 			corresponding to the given sender address and returns .
   * @param     [1] iface_addr - Sender address.
   * @return    Reference of Symmetric link tuple.
   * @note      None.
   ********************************************************************/
  T_LINK_TUPLE*
  C_OLSR_STATE::find_sym_link_tuple (const T_NODE_ADDRESS &iface_addr)
  {
	/* Loops through the link set and finds the given address is matching with
	 * neighbor address of any of the symmetric link tuples  */
    for (std::vector<T_LINK_TUPLE>::iterator link_iterator = m_link_set.begin ();
            link_iterator != m_link_set.end (); link_iterator++)
    {
      if ((link_iterator->l_neighbor_iface_addr == iface_addr) and (link_iterator->l_status == M_SYMMETRIC))
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
   * @return    Added link_tuple.
   * @note      None.
   ********************************************************************/
  T_LINK_TUPLE&
  C_OLSR_STATE::insert_link_tuple (const T_LINK_TUPLE &new_link_tuple)
  {
	/* Adds the new link tuple to the link set and sends it's reference */
    m_link_set.push_back(new_link_tuple);
    return m_link_set.back();

  }

  /********************************************************************
   * @function  check_is_routing_mpr_selector
   * @brief     This function checks if the given node
   * 			is a routing mpr selector.
   * @param     [1] address - Address of the 1-Hop neighbor.
   * @return    return_status = true/false.
   * @note      None.
   ********************************************************************/

  T_BOOL C_OLSR_STATE::check_is_routing_mpr_selector(const T_NODE_ADDRESS &address) const
  {
	 T_BOOL return_status = false;

	 /* Loops through the 1-Hop neighbor set and checks if the requested node is
	  * a Routing MPR selector of this node or not*/
     for(NeighbourSet::const_iterator nbr_tuple_iter = m_neighbour_set.begin();
         nbr_tuple_iter != m_neighbour_set.end(); nbr_tuple_iter++)
       {
         if((nbr_tuple_iter->n_neighbor_addr == address) and (nbr_tuple_iter->n_mpr_selector == true))
           {
        	 return_status = true;
        	 return return_status;
           }
       }
     return return_status;
  }

  /********************************************************************
   * @function  get_routing_mpr_selector_set
   * @brief     This function returns the Routing MPR selector set
   * @param     None
   * @return    NeighbourSet
   * @note      None.
   ********************************************************************/
  const NeighbourSet&
  C_OLSR_STATE::get_routing_mpr_selector_set () const
  {
    NeighbourSet routing_mpr_sel_set;

     /* Loops through the 1-Hop neighbor set and fetches the
      * Routing MPRs of thi node*/
     for(NeighbourSet::const_iterator nbr_tuple_iter = m_neighbour_set.begin();
         nbr_tuple_iter != m_neighbour_set.end(); nbr_tuple_iter++)
       {
         if((nbr_tuple_iter->n_symmetric == true) and (nbr_tuple_iter->n_mpr_selector == true))
           {
             routing_mpr_sel_set.push_back(*nbr_tuple_iter);
           }
       }
     return routing_mpr_sel_set;

  }

  /********************************************************************
   * @function  check_is_flooding_mpr_selector
   * @brief     This function checks if the given node
   * 			is a flooding mpr selector.
   * @param     [1] address - Address of the 1-Hop neighbor.
   * @return    return_sts = true/false.
   * @note      None.
   ********************************************************************/

  T_BOOL C_OLSR_STATE::check_is_flooding_mpr_selector(const T_NODE_ADDRESS &address) const
  {
	 T_BOOL return_sts = false;
	 /* Loops through the 1-Hop neighbor set and checks if the requested node is
	  * a Routing MPR selector of this node or not*/
     for(LinkSet::const_iterator link_tuple_iter = m_link_set.begin();
         link_tuple_iter != m_link_set.end(); link_tuple_iter++)
       {
         if((link_tuple_iter->l_neighbor_iface_addr == address) and (link_tuple_iter->l_mpr_selector == true))
           {
        	 return_sts = true;
             return return_sts;
           }
       }
     return return_sts;
  }


  /********************************************************************
   * @function  get_advertising_remote_router_set
   * @brief     This function returns the advertising remote router set.
   * @param     None.
   * @return    m_advertising_remote_router_set.
   * @note      None.
   ********************************************************************/
  const AdvertisingRemoteRouterSet &
  C_OLSR_STATE::get_advertising_router_set () const
  {
    return m_advertising_remote_router_set;
  }

  /********************************************************************
   * @function  insert_advertising_router_tuple
   * @brief     This function inserts the new tuple to
   *            advertising router set.
   * @param     [1] new_tuple - advertising router tuple
   * @return    m_advertising_remote_router_tuple.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR_STATE::insert_advertising_router_tuple (const T_ADVERTISING_REMOTE_ROUTER_TUPLE &new_tuple)
  {
    m_advertising_remote_router_set.push_back(new_tuple);
  }


  /********************************************************************
    * @function  find_newer_advertising_router_tuple
    * @brief     This function returns the latest
    *            advertising remote router tuple w.r.t originator address.
    * @param     [1] tc_sender - Originator of the TC message,
    *            [2] ansn - Advertised neighbor sequence number
    * @return    m_advertising_remote_router_tuple.
    * @note      None.
    ********************************************************************/
  T_ADVERTISING_REMOTE_ROUTER_TUPLE*
  C_OLSR_STATE::find_newer_advertising_router_tuple (const T_NODE_ADDRESS& tc_sender, T_UINT16 ansn)
  {
    for(AdvertisingRemoteRouterSet::iterator advtz_router_iter = m_advertising_remote_router_set.begin();
        advtz_router_iter !=  m_advertising_remote_router_set.end(); advtz_router_iter++)
      {
        if((advtz_router_iter->ar_orig_addr == tc_sender) and (advtz_router_iter->ar_seq_number > ansn))
          {
            return &(*advtz_router_iter);

          }
      }
    return NULL;
  }

  /********************************************************************
    * @function  erase_older_advertising_router_tuple
    * @brief     This function erases the older
    *            advertising remote router tuple.
    * @param     [1] tc_sender - Originator of the TC message,
    *            [2] ansn - Advertised neighbor sequence number.
    * @return    m_advertising_remote_router_set.
    * @note      None.
    ********************************************************************/
  void
  C_OLSR_STATE::erase_older_advertising_router_tuple (const T_NODE_ADDRESS &tc_sender, T_UINT16 ansn)
  {
    /* Checks whether older advertising_remote_router tuples are present
     * when compared to the new TC message.
     * If present remove those tuples and calls erase_older_router_topology_tuple()
     * to remove old entries*/
    for(AdvertisingRemoteRouterSet::const_iterator advt_router_iter = m_advertising_remote_router_set.begin();
        advt_router_iter !=  m_advertising_remote_router_set.end();)
      {
        if((advt_router_iter->ar_orig_addr == tc_sender) and (advt_router_iter->ar_seq_number < ansn))
          {
            advt_router_iter = m_advertising_remote_router_set.erase(advt_router_iter);
            erase_older_router_topology_tuple(tc_sender, ansn);
          }
        else
          {
            advt_router_iter++;
          }

      }
  }

  /********************************************************************
   * @function  get_router_topology_set
   * @brief     This function returns the router toplogy set.
   * @param     None.
   * @return    m_router_topology_set.
   * @note      None.
   ********************************************************************/
  const RouterTopologySet &
  C_OLSR_STATE::get_router_topology_set () const
  {
    return m_router_topology_set;
  }

  /********************************************************************
   * @function  insert_router_topology_tuple
   * @brief     This function inserts the new tuple to the
   *            router toplogy set.
   * @param     [1] new_tuple  - router toplogy tuple
   * @return    Remote Router tuple.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR_STATE::insert_router_topology_tuple (const T_ROUTER_TOPOLOGY_TUPLE &new_tuple)
  {
    m_router_topology_set.push_back(new_tuple);
  }
  /********************************************************************
    * @function  find_router_topology_tuple
    * @brief     This function finds the router topology tuple in the
    *            router toplogy set.
    * @param     [1] dest_addr - Destination node,
    *            [2] dest_advertizer - The TC message originator.
    * @return    Remote Router tuple.
    * @note      None.
    ********************************************************************/
  T_ROUTER_TOPOLOGY_TUPLE*
  C_OLSR_STATE::find_router_topology_tuple (const T_NODE_ADDRESS &dest_addr, const T_NODE_ADDRESS &dest_advertizer)
  {
    /* Removes all the Router Topology Tuples whose tr_from_orig_addr
     * match with tc_sender and  and tr_seq_number is less than ansn */
    for(RouterTopologySet::iterator router_topo_iter = m_router_topology_set.begin();
        router_topo_iter != m_router_topology_set.end(); router_topo_iter++)
      {
        if((router_topo_iter->tr_from_orig_addr == dest_advertizer) and (router_topo_iter->tr_to_orig_addr == dest_advertizer))
          {
            return &(*router_topo_iter);
          }
      }
    return NULL;

  }

  /********************************************************************
   * @function  erase_older_router_topology_tuple
   * @brief     This function removes the older topology tuples
   * @param     [1] tc_sender - Originator of TC message.
   *            [2] ansn - Advertized sequence number.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR_STATE::erase_older_router_topology_tuple(const T_NODE_ADDRESS &tc_sender, T_UINT16 ansn)
  {
    /* Removes all the Router Topology Tuples whose tr_from_orig_addr
     * match with tc_sender and  and tr_seq_number is less than ansn */
    for(RouterTopologySet::const_iterator router_topo_iter = m_router_topology_set.begin();
        router_topo_iter != m_router_topology_set.end(); router_topo_iter++)
      {
        if((router_topo_iter->tr_from_orig_addr == tc_sender) and (router_topo_iter->tr_seq_number < ansn))
          {
            router_topo_iter = m_router_topology_set.erase(router_topo_iter);
          }

        else
          {
            router_topo_iter++;
          }

      }


  }

  /********************************************************************
   * @function  erase_router_topology_tuple
   * @brief     This function removes the router topology tuples
   *            when timeout happens.
   * @param     [1] tc_sender - Originator of TC message.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR_STATE::erase_router_topology_tuple(const T_NODE_ADDRESS &tc_sender)
  {
    /* Removes all the Router Topology Tuples whose tr_from_orig_addr and tr_seq_number
     * match with tc_sender and ansn */
    for(RouterTopologySet::const_iterator router_topo_iter = m_router_topology_set.begin();
        router_topo_iter != m_router_topology_set.end(); router_topo_iter++)
      {
        if(router_topo_iter->tr_from_orig_addr == tc_sender)
          {
            router_topo_iter = m_router_topology_set.erase(router_topo_iter);
          }

        else
          {
            router_topo_iter++;
          }

      }


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

    m_leader_node[M_ZERO] = leader_tuple;

    if ((m_leader_node.at (M_ZERO).is_leader == true) and (m_leader_node.at (M_ZERO).leader_addr.net_id == 10)
        and (m_leader_node.at (M_ZERO).leader_addr.node_id == M_ZERO))
      {
        std::cout << "successsfully insertted leader node info " << std::endl;
      }

  }

  /********************************************************************
   * @function  get_leader_tuple
   * @brief     This function returns the leader address of the network.
   * @param     None.
   * @return    Reference of leader_tuple.
   * @note      None.
   ********************************************************************/
  T_LEADER_TUPLE*
  C_OLSR_STATE::get_leader_tuple (void)
  {
	  /* Checks if the leader is present for the network or not
	   * If present then returns his address */
    if (m_leader_node.size () != M_ZERO)
      {
        return &(m_leader_node.at (M_ZERO));
      }
    else
      {
        //return (T_LEADER_TUPLE*)M_ZERO;
        return NULL;
      }

  }

  /********************************************************************
   * @function  find_processed_msg_tuple
   * @brief     This function finds a processed message tuple and returns
   * @param     [1] address - Address of the processed msg originator.
   * 			[2] sequence_number - Message sequence number of processed msg
   * @return    Reference of processed msg tuple.
   * @note      None.
   ********************************************************************/
  T_PROCESSED_MSG_TUPLE*
  C_OLSR_STATE::find_processed_msg_tuple (const T_NODE_ADDRESS &address, uint16_t sequence_number)
  {
	/* Loops through processed message set and finds if the processed message tuple entry
	 * is present for the given originator address and message sequence number*/
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
   * @brief     This function inserts a processed message tuple to the set.
   * @param     [1] tuple - Newly created processed message tuple.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void C_OLSR_STATE::insert_processed_msg_tuple (const T_PROCESSED_MSG_TUPLE &tuple)
  {
    m_processed_msg_set.push_back(tuple);
  }

  /********************************************************************
   * @function  find_forwarded_msg_tuple
   * @brief     This function finds a forwarded message tuple and returns.
   * @param     [1] address - Address of the forwarded msg originator.
   * 			[2] sequence_number - Message sequence number of forwarded msg
   * @return    Reference of forwarded msg tuple.
   * @note      None.
   ********************************************************************/
  T_FORWARDED_MSG_TUPLE*
  C_OLSR_STATE::find_forwarded_msg_tuple (const T_NODE_ADDRESS &address, T_UINT16 sequence_number)
  {
	/* Loops through forwarded message set and finds if the forwarded message tuple entry
	 * is present for the given originator address and message sequence number*/
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
   * @brief     This function inserts a forwarded message tuple to the set.
   * @param     [1] tuple - Newly created forwarded message tuple..
   * @return    None.
   * @note      None.
   ********************************************************************/
  void C_OLSR_STATE::insert_forwarded_msg_tuple (const T_FORWARDED_MSG_TUPLE &tuple)
  {
    m_forwarded_msg_set.push_back(tuple);
  }

  /********************************************************************
   * @function  find_next_routing_hop_addr
   * @brief     This function finds the next router address for
   * 			the message to be transmitted/forwarded.
   * @param     [1] p_dest_addr - Destination address of the message,
   * 			[2] p_rdest_addr - Next 1-Hop router address to be found
   * @return    route_state - ROUTE_FOUND/ROUTE_NOT_FOUND.
   * @note      None.
   ********************************************************************/
  E_ROUTE_STATUS
  C_OLSR_STATE::find_next_routing_hop_addr(const T_ADDR* p_dest_addr, T_ADDR* p_rdest_addr)
  {
    T_NODE_ADDRESS dest_address;
    E_ROUTE_STATUS route_state = ROUTE_NOT_FOUND;

    dest_address.net_id = p_dest_addr->field.m_oid;
    dest_address.node_id = p_dest_addr->field.m_nid;

    /* Loops through the Routing table and finds if the given destination address'
     * entry is present or not. If present then returns the next 1-Hop router's
     * address to reach that destination */
    std::map<T_NODE_ADDRESS, T_ROUTING_TABLE_ENTRY>::const_iterator router_iter = m_routing_table.find(dest_address);

    if(router_iter != m_routing_table.end())
      {
        p_rdest_addr->field.m_nid = router_iter->second.r_next_iface_addr.node_id;
        p_rdest_addr->field.m_oid = router_iter->second.r_next_iface_addr.net_id;
        route_state = ROUTE_FOUND;
        return route_state;
      }

    return route_state;

  }


 /********************************************************************
  * @function  insert_routing_tuple
  * @brief     This function Creates and inserts routing tuple to
  *            Routing Table.
  * @param     [1] new_route_data - New Routing table data.
  *            [2] next_address - Next hop address.
  *            [3] hop_count - Distance from this node to destination.
  * @return    None.
  * @note      None.
  ********************************************************************/
  void
  C_OLSR_STATE::insert_routing_tuple(T_NETWORK_LINK new_route_data, const T_NODE_ADDRESS& next_address, T_UINT8 hop_count)
  {

    T_ROUTING_TABLE_ENTRY new_entry;

    new_entry.r_dest_addr = new_route_data.dest_addr;
    new_entry.r_next_iface_addr = next_address;
    new_entry.r_local_iface_addr = new_route_data.src_addr;
    new_entry.r_metric = new_route_data.out_metric;
    new_entry.r_dist = hop_count;
    new_entry.r_used = false;

    m_routing_table.insert({new_entry.r_dest_addr, new_entry});
  }

  /********************************************************************
   * @function  get_routing_table
   * @brief     This function erases complete Routing Table.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  std::map<T_NODE_ADDRESS, T_ROUTING_TABLE_ENTRY>
  C_OLSR_STATE::get_routing_table(void) const
  {
    return m_routing_table;
  }

  /********************************************************************
   * @function  find_routing_tuple
   * @brief     This function finds the Routing tuple with the
   *            given destination address where the metric value
   *            is greater than the given new_metric value.
   * @param     [1] dest_addr - Destination address.
   *            [2] new_metric - New out link metric
   * @return    Address of the matched routing tuple.
   * @note      None.
   ********************************************************************/
  T_ROUTING_TABLE_ENTRY*
  C_OLSR_STATE::find_routing_tuple(const T_NODE_ADDRESS& dest_addr, float new_metric)
  {
    T_ROUTING_TABLE_ENTRY* existing_tuple = NULL;

    /* Finds whether a routing tuple is already present for the given destination and
     * that tuple is not used for finding further routes.
     * Then checks whether the link out metric value of that tuple is greater than or equal to
     * the given new_metric, if true then this tuple must be updated for new metric
     * So it returns that tuple*/
    std::map<T_NODE_ADDRESS, T_ROUTING_TABLE_ENTRY>::iterator routing_table_iter = m_routing_table.find(dest_addr);

    if(routing_table_iter != m_routing_table.end())
      {
        if((routing_table_iter->second.r_used == false) and (routing_table_iter->second.r_metric >= new_metric))
          {
            existing_tuple = &(routing_table_iter->second);

          }
      }
    return existing_tuple;

  }

  /********************************************************************
   * @function  erase_routing_tuple
   * @brief     This function erases complete Routing Table.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR_STATE::erase_routing_table()
  {
    m_routing_table.clear();
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
    m_leader_node[M_ZERO].is_leader = false;
    m_leader_node[M_ZERO].leader_addr.net_id = M_UNKNOWN_VALUE;
    m_leader_node[M_ZERO].leader_addr.node_id == M_UNKNOWN_VALUE;

    m_routing_table.clear ();
  }

  /********************************************************************
      * @function  check_link_set_timeout
      * @brief     This function checks the timeout of all tuples
      * 		   in the link set.
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_link_set_timeout()
  {
    Time now = get_ns();

    for(LinkSet::iterator link_set_iter = m_link_set.begin(); link_set_iter != m_link_set.end();)
      {
        /* Checks if the link tuple is not symmetric but assymetric
         * If true then set the link status as Heard  */
        if((now > link_set_iter->l_sym_time) and (now < link_set_iter->l_heard_time))
        {
            link_set_iter->l_status = M_HEARD;
            /* Calls the below function to set neighbor as asymmetric in neihbor tuple */
            update_neighbour_tuple(link_set_iter->l_neighbor_iface_addr);
            link_set_iter++;
        }
        /* If the link is neither symmetric nor assymetric then,
         * checks whether the validity time of the tuple is not over.
         * If true then sets the link status as Lost
         * otherwise removes the tuple from link set*/
        else
          {
            if (now < link_set_iter->l_time)
              {
                link_set_iter->l_status = M_LOST;
                /* Calls the below function to set neighbor as asymmetric in neihbor tuple */
                update_neighbour_tuple(link_set_iter->l_neighbor_iface_addr);
                link_set_iter++;
              }
            else
              {
                /* Calls the below function to remove the neighbor from neighbor set */
                erase_neighbour_tuple(link_set_iter->l_neighbor_iface_addr);
                link_set_iter = m_link_set.erase(link_set_iter);
              }
          }
      }

  }

#ifdef COMMENT_SECTION
  /********************************************************************
      * @function  check_one_hop_neigh_set_timeout
      * @brief     This function checks the timeout of all tuples
      * 		   in the 1-hop neighbor set.
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_one_hop_neigh_set_timeout()
  {
    ;
  }
#endif
  /********************************************************************
      * @function  check_two_hop_neigh_set_timeout
      * @brief     This function checks the timeout of all tuples
      * 		   in the 2-hop neighbor set.
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_two_hop_neigh_set_timeout()
  {
    Time now = get_ns();

    /* Checks validity of each tuple in  2-hop neighbor set
     * If not valid then removes that tuple */
    for(TwoHopNeighborSet::iterator two_hop_set_iter = m_two_hop_neighbour_set.begin(); two_hop_set_iter != m_two_hop_neighbour_set.end();)
      {
        if(now > two_hop_set_iter->n2_time)
        {
            two_hop_set_iter = m_two_hop_neighbour_set.erase(two_hop_set_iter);
        }
        else
        {
            two_hop_set_iter++;
        }
      }

  }

  /********************************************************************
      * @function  check_advt_router_set_timeout
      * @brief     This function checks the timeout of all tuples
      * 		   in the advertised router set.
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_advt_router_set_timeout()
  {
    Time now = get_ns();

    /* Checks validity of each tuple in  Advertised router set
     * If not valid then removes that tuple and also
     * removes Router topology tuples depending on this tuple*/
    for(AdvertisingRemoteRouterSet::iterator advt_router_set_iter = m_advertising_remote_router_set.begin(); advt_router_set_iter != m_advertising_remote_router_set.end();)
      {
        if(now > advt_router_set_iter->ar_time)
        {
            erase_router_topology_tuple(advt_router_set_iter->ar_orig_addr);
            advt_router_set_iter = m_advertising_remote_router_set.erase(advt_router_set_iter);

        }
        else
        {
            advt_router_set_iter++;
        }
      }
  }

  /********************************************************************
      * @function  check_router_topo_set_timeout
      * @brief     This function checks the timeout of all tuples
      * 		   in the Router topology set.
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_router_topo_set_timeout()
  {
    Time now = get_ns();
    /* Checks validity of each tuple in  Router topology set
     * If not valid then removes that tuple */
    for(RouterTopologySet::iterator router_topo_iter = m_router_topology_set.begin(); router_topo_iter != m_router_topology_set.end();)
      {
        if(now > router_topo_iter->tr_time)
        {
            router_topo_iter = m_router_topology_set.erase(router_topo_iter);
        }
        else
        {
            router_topo_iter++;
        }
      }
  }

  /********************************************************************
      * @function  check_processed_msg_set_timeout
      * @brief     This function checks the timeout of all tuples
      * 		   in the processed msg set.
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_processed_msg_set_timeout()
  {
    Time now = get_ns();
    /* Checks validity of each tuple in  processed msg set
     * If not valid then removes that tuple */
    for(ProcessedMsgSet::iterator processed_msg_iter = m_processed_msg_set.begin(); processed_msg_iter != m_processed_msg_set.end();)
      {
        if(now > processed_msg_iter->p_time)
        {
            processed_msg_iter = m_processed_msg_set.erase(processed_msg_iter);
        }
        else
        {
            processed_msg_iter++;
        }
      }
  }

  /********************************************************************
      * @function  check_forwarded_msg_set_timeout
      * @brief     This function checks the timeout of all tuples
      * 		   in the forwarded msg set.
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_forwarded_msg_set_timeout()
  {
    Time now = get_ns();
    /* Checks validity of each tuple in  forwarded msg set
     * If not valid then removes that tuple */
    for(ForwardedMsgSet::iterator forwarded_msg_iter = m_forwarded_msg_set.begin(); forwarded_msg_iter != m_forwarded_msg_set.end();)
      {
        if(now > forwarded_msg_iter->f_time)
        {
            forwarded_msg_iter = m_forwarded_msg_set.erase(forwarded_msg_iter);
        }
        else
        {
            forwarded_msg_iter++;
        }
      }
  }

  /********************************************************************
      * @function  check_tables_timeout
      * @brief     This function checks the timeout of all tables.
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR_STATE::check_tables_timeout()
  {
    check_link_set_timeout();
    check_two_hop_neigh_set_timeout();
    check_advt_router_set_timeout();
    check_router_topo_set_timeout();
    check_processed_msg_set_timeout();
    check_forwarded_msg_set_timeout();

  }

  /********************************************************************
      * @function  fill_tuples
      * @brief     This function fills some Link and 1-hop neighbor tuples.
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR_STATE::fill_tuples ()
  {
    for (int i = M_ZERO; i < 5; i++)
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

        if ((i == M_ONE) or (i == 3))
          {

            loc_link_tup.l_in_metric = 0.9;
            loc_link_tup.l_out_metric = 0.8;
            loc_link_tup.l_status = M_SYMMETRIC_LINK;

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
            loc_link_tup.l_status = M_SYMMETRIC_LINK;

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
