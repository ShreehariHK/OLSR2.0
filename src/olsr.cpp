/*                        Project title
 *
 *
 * Source Code Name   :   olsr.cpp
 *
 * Description        :   Implements OLSR protocol functionality .
 *
 * Subsystem Name     :   OLSR
 *
 * Revision History
 * ---------------------------------------------------------------------------|
 * Version | Change Description               |    Date    |    Changed By    |
 * --------|----------------------------------|------------|------------------|
 * 1.0     |Initial Version                   | 14-06-2021 | Shreehari H K    |
 * --------|----------------------------------|------------|------------------|
 *
 *                              Copyright statement
 *
 *
 */
#include <iostream>

#include "olsr.hpp"

using namespace std;

namespace ns_olsr2_0
{

  /********************************************************************
   * @function  C_OLSR_ROUTING_PROTOCOL
   * @brief     This is a  constructor,
   *            to set the interface address for current OLSR instance
   * @param     interface_ip.
   * @return    None.
   * @note      None.
  ********************************************************************/

  C_OLSR::C_OLSR()
  {

    m_op_state = INIT_STATE;

  }



  /********************************************************************
   * @function  ~C_OLSR_ROUTING_PROTOCOL
   * @brief     This is a destructor of C_OLSR_ROUTING_PROTOCOL class
   * @param     interface_ip.
   * @return    None.
   * @note      None.
  ********************************************************************/
  C_OLSR::~C_OLSR()
  {

  }

  /********************************************************************
   * @function  init
   * @brief     This is an entry point function for initializing OLSR
   *            instance in INIT_STATE.
   * @param     None.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_OLSR::init(ns_olsr2_0::E_OLSR_INSTANCE instance_type)
  {
      cout << "OLSR instance : " << instance_type << endl;
      if(m_op_state == INIT_STATE)
        {
          this->m_state.init_state_tuples();

          cout << "OLSR Operational State = " << this->getOperationState() << endl;

          this->init_op(instance_type);

          m_op_state = RUN_STATE;
          cout << "OLSR Operational State = " << this->getOperationState() << endl;
        }

      else
      {
          ;
      }
  }

  /********************************************************************
    * @function  run
    * @brief     This is an entry point function for running OLSR
    *            instance in RUN_STATE.
    * @param     None.
    * @return    None.
    * @note      None.
   ********************************************************************/
   void C_OLSR::run(void)
   {
       if(m_op_state == RUN_STATE)
         {
           send_hello();
           /* TBD - send_tc(); */

           this->m_state.check_tables_timeout();

           cout << "OLSR is inside run state "<< endl;
         }

       else
       {
           ;
       }
   }


  /********************************************************************
   * @function  reset
   * @brief     This is an entry point function for resetting OLSR
   *            instance.
   * @param     None.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_OLSR::reset(void)
  {
     m_op_state = INIT_STATE;
     if(m_is_leader == true)
       {
         this->init(ns_olsr2_0::E_OLSR_INSTANCE::LEADER_NODE_INSTANCE);
       }
     else
     {
         this->init(ns_olsr2_0::E_OLSR_INSTANCE::NORMAL_NODE_INSTANCE);
     }

  }
  /********************************************************************
   * @function  init_op
   * @brief     This is an entry point function for initializing OLSR
   *            instance.
   * @param     None.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_OLSR::init_op(ns_olsr2_0::E_OLSR_INSTANCE inst_type)
  {
    T_ADDR loc_node_addr;

    loc_node_addr = this->get_node_address();
    if(inst_type == NORMAL_NODE_INSTANCE)
    {
        m_node_address.net_id = loc_node_addr.field.m_oid;
        m_node_address.node_id = loc_node_addr.field.m_nid;

        cout << "Normal Node address.netId = " << m_node_address.net_id<< endl;
        cout << "Normal Node address.nodeId = " << m_node_address.node_id<< endl;
        m_is_leader = false;
        m_instance = NORMAL_NODE_INSTANCE;

        cout << "Leader status : " << m_is_leader << endl;

        if(get_is_leader() == true)
          {
            T_LEADER_TUPLE leader_tuple;
            leader_tuple.leader_addr.net_id = m_node_address.net_id;
            leader_tuple.leader_addr.node_id = 0x0;

            leader_tuple.is_leader = true;

            this->m_state.insert_leader_tuple(leader_tuple);
          }

    }
    else if(inst_type == LEADER_NODE_INSTANCE)
    {
        m_node_address.net_id = loc_node_addr.field.m_oid;
        m_node_address.node_id = 0x0;

        if((get_is_leader() == true) ? (m_is_leader = true):(m_is_leader = false))

        cout << "Leader Node address.netId = " << m_node_address.net_id<< endl;
        cout << "Leader Node address.nodeId = " << m_node_address.node_id<< endl;


        cout << "Leader status : " << m_is_leader << endl;
        m_instance = LEADER_NODE_INSTANCE;
    }


    m_willingness.willingness = this->get_willingness();

    cout << "routing_willingness = " << m_willingness.fields.route_will << endl;
    cout << "flooding_willingness = " << m_willingness.fields.flood_will << endl;

    /* TBD  - Subscription to Gateway component */

  }

  /********************************************************************
      * @function  find_allowed_one_hop_address
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
template <typename T>
  T_BOOL C_OLSR::find_allowed_one_hop_address(T_NODE_ADDRESS address, const std::vector<T>& tuple, T_UINT8* idx)
  {

      for(typename std::vector<T>::const_iterator tuple_iter = tuple.begin(); tuple_iter != tuple.end(); tuple_iter++)
        {
           if(address == tuple_iter->one_hop_neighb_addr)
              {
                *idx = tuple_iter - tuple.begin();
                return true;
              }

        }
      return false;
  }

  /********************************************************************
      * @function  get_next_msg_seq_num
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/

  T_UINT16 C_OLSR::get_next_msg_seq_num(void)
  {
    m_message_sequence_number += (m_message_sequence_number + 1) % (M_MAX_MSG_SEQ_NUM + 1);
    return m_message_sequence_number;
  }

  /********************************************************************
      * @function  set_leader_info
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::set_leader_info(C_MESSAGE_HEADER::T_HELLO& hello_msg, T_LEADER_TUPLE* leader_tuple)
  {
    if((leader_tuple != NULL) and (leader_tuple->is_leader == true))
    {
      hello_msg.leader_info = *leader_tuple;
      cout << "Leader node set in Hello message" << endl;
    }
    else
    {
      hello_msg.leader_info.is_leader = false;
      cout << "Leader node not set in Hello message" << endl;
    }
  }

  /********************************************************************
      * @function  set_common_address
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::set_common_address(C_MESSAGE_HEADER::T_HELLO& hello_msg, E_ADDRESS_BLOCK_FLAGS common_field, T_UINT8 common_id)
  {
    hello_msg.abf = common_field;
    hello_msg.common_id = common_id;
  }

  /********************************************************************
      * @function  set_common_address
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  T_BOOL C_OLSR::check_message_validity(const C_MESSAGE_HEADER& msg_header)
  {
    T_BOOL return_status = false;

    if (msg_header.get_time_to_live () == 0)
    {
        return_status = true;
    }
    if(msg_header.get_originator_address() == this->get_node_addr())
    {
      return_status = true;
    }

    return return_status;
  }

  /********************************************************************
      * @function  create_processed_msg_tuple
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  T_PROCESSED_MSG_TUPLE
  C_OLSR::create_processed_msg_tuple(const C_MESSAGE_HEADER& msg_header)
  {
    T_PROCESSED_MSG_TUPLE new_proc_msg_tuple;
    Time cur_time = get_cur_time();

    new_proc_msg_tuple.p_orig_addr = msg_header.get_originator_address();
    new_proc_msg_tuple.p_seq_number = msg_header.get_message_sequence_number();
    new_proc_msg_tuple.p_type = msg_header.get_message_type();
    new_proc_msg_tuple.p_time = cur_time + msg_header.get_validity_time();
    return new_proc_msg_tuple;

  }
  /********************************************************************
       * @function  create_forwarded_msg_tuple
       * @brief     This function increments the message sequence
       *            number and sends
       * @return    None.
       * @note      None.
   ********************************************************************/

  T_FORWARDED_MSG_TUPLE
  C_OLSR::create_forwarded_msg_tuple(const C_MESSAGE_HEADER& msg_header)
  {
    T_FORWARDED_MSG_TUPLE new_fwd_msg_tuple;
    Time cur_time = get_cur_time();

    new_fwd_msg_tuple.f_orig_addr = msg_header.get_originator_address();
    new_fwd_msg_tuple.f_seq_number = msg_header.get_message_sequence_number();
    new_fwd_msg_tuple.f_type = msg_header.get_message_type();
    new_fwd_msg_tuple.f_time = cur_time + msg_header.get_validity_time();
    return new_fwd_msg_tuple;
  }

  /********************************************************************
      * @function  populate_two_hop_set
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  T_LINK_TUPLE
  C_OLSR::create_new_link_tuple(const C_MESSAGE_HEADER& msg_header, Time cur_time, float in_metric)
  {
    T_LINK_TUPLE new_link_tuple;
    new_link_tuple.l_neighbor_iface_addr = msg_header.get_originator_address ();
    new_link_tuple.l_sym_time = cur_time - 1;
    new_link_tuple.l_time = cur_time + msg_header.get_validity_time ();
    new_link_tuple.l_in_metric = in_metric;
    new_link_tuple.l_mpr_selector = false;
    new_link_tuple.l_out_metric = 0;
    new_link_tuple.l_status = HEARD_LINK;

    return new_link_tuple;
  }
  /********************************************************************
      * @function  validate_neighbor_link_type
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  T_BOOL C_OLSR::validate_neighbor_link_type(C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK iter)
  {
    if ((((iter.common_field.link_state.type_fields.link_type == LOST_LINK)
    or (iter.common_field.link_state.type_fields.link_type == HEARD_LINK))
    and ((iter.common_field.link_state.type_fields.nbr_type == SYMMETRIC_NEIGHBOR)
        or (iter.common_field.link_state.type_fields.nbr_type == ROUTING_MPR)
        or (iter.common_field.link_state.type_fields.nbr_type == FLOODING_MPR)
        or (iter.common_field.link_state.type_fields.nbr_type == MPR_FLOOD_ROUTE)))
    or ((iter.common_field.link_state.type_fields.link_type == SYMMETRIC_LINK)
        and (iter.common_field.link_state.type_fields.nbr_type == NOT_A_NEIGHBOR)))
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  /********************************************************************
      * @function  populate_two_hop_set
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::populate_two_hop_set()
  {
    ;
  }

  /********************************************************************
      * @function  set_neighbor_info
      * @brief     This function increments the message sequence
      *            number and sends
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::set_neighbor_info(C_MESSAGE_HEADER::T_HELLO& hello_msg, Time cur_time)
  {
    C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK nbr_addr_block;

    const LinkSet &link_set = this->m_state.get_links();

    for(LinkSet::const_iterator link_set_iter = link_set.begin() ; link_set_iter != link_set.end() ; link_set_iter++)
    {
        T_LINK_TUPLE link_tuple = *link_set_iter;

        if((link_tuple.l_in_metric > 0) and (link_tuple.l_out_metric > 0))
          {
            nbr_addr_block.metric[1] = link_tuple.l_in_metric;
            nbr_addr_block.metric[0] = link_tuple.l_out_metric;

            if(link_tuple.l_sym_time >= cur_time)
            {
                nbr_addr_block.common_field.link_state.type_fields.link_type = SYMMETRIC_LINK;
            }

            else if (link_tuple.l_heard_time >= cur_time)
            {
                nbr_addr_block.common_field.link_state.type_fields.link_type = HEARD_LINK;
            }
            else
            {
                nbr_addr_block.common_field.link_state.type_fields.link_type = LOST_LINK;
            }

            const NeighbourSet &neighb_set = this->m_state.get_one_hop_neighbors();
            T_BOOL to_be_added = false;
            for(NeighbourSet::const_iterator neighb_set_iter = neighb_set.begin(); neighb_set_iter != neighb_set.end(); neighb_set_iter++)
            {
                T_NEIGHBOUR_TUPLE neighb_tuple = *neighb_set_iter;

                if(link_tuple.l_neighbor_iface_addr == neighb_tuple.n_neighbor_addr)
                  {
                    if((neighb_tuple.n_routing_mpr == true) and (neighb_tuple.n_flooding_mpr == true))
                    {
                        nbr_addr_block.common_field.link_state.type_fields.nbr_type = MPR_FLOOD_ROUTE;
                    }
                    else if(neighb_tuple.n_routing_mpr == true)
                    {
                        nbr_addr_block.common_field.link_state.type_fields.nbr_type = ROUTING_MPR;
                    }
                    else if(neighb_tuple.n_flooding_mpr == true)
                    {
                        nbr_addr_block.common_field.link_state.type_fields.nbr_type = FLOODING_MPR;
                    }
                    else if(neighb_tuple.n_symmetric == true)
                    {
                        nbr_addr_block.common_field.link_state.type_fields.nbr_type = SYMMETRIC_NEIGHBOR;
                    }
                    else
                    {
                        nbr_addr_block.common_field.link_state.type_fields.nbr_type = NOT_A_NEIGHBOR;
                    }

                    to_be_added = true;
                    break;
                  }
             }
            if(to_be_added)
            {
              hello_msg.neighbor_set.push_back(nbr_addr_block);
              cout << "Tuple added to hello message" << endl;
            }


          }

      }
  }

  /********************************************************************
      * @function  routing_table_computation
      * @brief     This function computes the routing table
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::create_allowed_one_hop_set(std::vector<T_ALLOWED_ONE_HOP_TUPLE> &p_allowed_one_hop_set, T_UINT8 p_mpr_type)
  {
    if(p_mpr_type == ROUTING_MPR)
      {

        for(NeighbourSet::const_iterator nbr_itr = m_state.get_one_hop_neighbors().begin();
                    nbr_itr != m_state.get_one_hop_neighbors().end(); nbr_itr++)
        {
          if((nbr_itr->n_symmetric == true) and (nbr_itr->n_in_metric >= 0) and
              (nbr_itr->n_willingness.fields.route_will > E_WILLINGNESS::WILL_NEVER))
            {
              T_ALLOWED_ONE_HOP_TUPLE new_allowed_nbr_tuple;
              new_allowed_nbr_tuple.one_hop_neighb_addr = nbr_itr->n_neighbor_addr;
              new_allowed_nbr_tuple.n_willingness = nbr_itr->n_willingness;
              new_allowed_nbr_tuple.in_out_metric = nbr_itr->n_in_metric;
              p_allowed_one_hop_set.push_back(new_allowed_nbr_tuple);
            }

        }

      }
    else if(p_mpr_type == FLOODING_MPR)
      {
        for(LinkSet::const_iterator link_itr = m_state.get_links().begin();
                link_itr != m_state.get_links().end(); link_itr++)
        {

          if((link_itr->l_status == SYMMETRIC) and (link_itr->l_out_metric >= 0))
            {
              T_NEIGHBOUR_TUPLE * nbr_tuple = m_state.find_neighbour_tuple(link_itr->l_neighbor_iface_addr);
              if((nbr_tuple != NULL) and (nbr_tuple->n_willingness.fields.flood_will > E_WILLINGNESS::WILL_NEVER))
                {
                  T_ALLOWED_ONE_HOP_TUPLE new_allowed_link_tuple;
                  new_allowed_link_tuple.one_hop_neighb_addr = link_itr->l_neighbor_iface_addr;
                  new_allowed_link_tuple.n_willingness = nbr_tuple->n_willingness;
                  new_allowed_link_tuple.in_out_metric = link_itr->l_out_metric;
                  p_allowed_one_hop_set.push_back(new_allowed_link_tuple);
                }
            }
        }

      }

  }
  /********************************************************************
      * @function  routing_table_computation
      * @brief     This function computes the routing table
      * @return    None.
      * @note      None.
  ********************************************************************/

  void C_OLSR::create_allowed_two_hop_set(std::vector<T_ALLOWED_TWO_HOP_TUPLE>& p_allowed_two_hop_set, const std::vector<T_ALLOWED_ONE_HOP_TUPLE>& p_allowed_one_hop_set, float mpr_type)
  {

    for(TwoHopNeighborSet::const_iterator two_hop_iter = m_state.get_two_hop_neighbours().begin();
        two_hop_iter != m_state.get_two_hop_neighbours().end(); two_hop_iter++)
      {
        T_UINT8 index=0;
        if((two_hop_iter->n2_in_metric >= 0) and
            (find_allowed_one_hop_address<T_ALLOWED_ONE_HOP_TUPLE>(two_hop_iter->n2_neighbor_iface_addr, p_allowed_one_hop_set, &index) == true))
          {
            T_ALLOWED_TWO_HOP_TUPLE new_allowed_two_hop_tuple;

            new_allowed_two_hop_tuple.two_hop_neighb_addr = two_hop_iter->n2_2hop_addr;
            new_allowed_two_hop_tuple.one_hop_neighb_addr = two_hop_iter->n2_neighbor_iface_addr;
            if(mpr_type == ROUTING_MPR)
              {
                new_allowed_two_hop_tuple.in_out_metric = two_hop_iter->n2_in_metric;
              }

            else if(mpr_type == FLOODING_MPR)
              {
                new_allowed_two_hop_tuple.in_out_metric = two_hop_iter->n2_out_metric;
              }

            p_allowed_two_hop_set.push_back(new_allowed_two_hop_tuple);
          }
      }

  }
  /********************************************************************
      * @function  routing_table_computation
      * @brief     This function computes the routing table
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::create_n1_neighbor_graph(const std::vector<T_ALLOWED_ONE_HOP_TUPLE> &p_allowed_one_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph)
  {
    for(std::vector<T_ALLOWED_ONE_HOP_TUPLE>::const_iterator allowed_one_hop_iter = p_allowed_one_hop_set.begin();
        allowed_one_hop_iter != p_allowed_one_hop_set.end(); allowed_one_hop_iter++)
      {
        T_N1 new_n1_tuple;
        new_n1_tuple.one_hop_neighb_addr = allowed_one_hop_iter->one_hop_neighb_addr;
        new_n1_tuple.neighb_will = allowed_one_hop_iter->n_willingness;
        new_n1_tuple.d1 = allowed_one_hop_iter->in_out_metric;

        p_neighbor_graph->n1_set.push_back(new_n1_tuple);
      }

  }
  /********************************************************************
      * @function  routing_table_computation
      * @brief     This function computes the routing table
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::create_n2_neighbor_graph(const std::vector<T_ALLOWED_TWO_HOP_TUPLE> &p_allowed_two_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph)
  {

    for(std::vector<T_ALLOWED_TWO_HOP_TUPLE>::const_iterator allowed_two_hop_iter = p_allowed_two_hop_set.begin();
        allowed_two_hop_iter != p_allowed_two_hop_set.end(); allowed_two_hop_iter++)
      {
        T_N2 new_n2_tuple;

        new_n2_tuple.two_hop_neighb_addr = allowed_two_hop_iter->two_hop_neighb_addr;
        p_neighbor_graph->n2_set.push_back(new_n2_tuple);
      }

  }

  /********************************************************************
      * @function  routing_table_computation
      * @brief     This function computes the routing table
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::compute_total_metric(const std::vector<T_ALLOWED_TWO_HOP_TUPLE> &p_allowed_two_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph)
  {
    for(std::vector<T_ALLOWED_TWO_HOP_TUPLE>::const_iterator allowed_two_hop_iter = p_allowed_two_hop_set.begin();
            allowed_two_hop_iter != p_allowed_two_hop_set.end(); allowed_two_hop_iter++)
    {
      for(std::vector<T_N1>::iterator n1_iter = p_neighbor_graph->n1_set.begin();
          n1_iter != p_neighbor_graph->n1_set.end(); n1_iter++)
        {
          if(allowed_two_hop_iter->one_hop_neighb_addr == n1_iter->one_hop_neighb_addr)
          {
              T_MATRIX new_matrix;

              new_matrix.two_hop_neighb_addr = allowed_two_hop_iter->two_hop_neighb_addr;
              new_matrix.d2 = allowed_two_hop_iter->in_out_metric;
              new_matrix.d = n1_iter->d1 + new_matrix.d2;

              n1_iter->matrix_set.push_back(new_matrix);

          }

        }

    }

  }

  /********************************************************************
      * @function  routing_table_computation
      * @brief     This function computes the routing table
      * @return    None.
      * @note      None.
  ********************************************************************/

  void C_OLSR::calculate_mprs(const T_NEIGHBOR_GRAPH &p_neighbor_graph)
  {
    ;
  }

  /********************************************************************
      * @function  create_network_topology_graph
      * @brief     This function computes the routing table
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::create_network_topology_graph(T_NETWORK_TOPOLOGY_GRAPH* p_network_topology_graph)
  {
    for(NeighbourSet::const_iterator neighb_set_iter = m_state.get_one_hop_neighbors().begin();
            neighb_set_iter != m_state.get_one_hop_neighbors().end(); neighb_set_iter++)
    {
      if((neighb_set_iter->n_symmetric == true) and (neighb_set_iter->n_out_metric != UNKNOWN_METRIC))
        {
          T_NETWORK_LINK neighb_network_link;

          neighb_network_link.src_addr = m_node_address;
          neighb_network_link.dest_addr = neighb_set_iter->n_neighbor_addr;
          neighb_network_link.out_metric = neighb_set_iter->n_out_metric;

          p_network_topology_graph->one_hop_set.push_back(neighb_network_link);
        }

    }

  for(RouterTopologySet::const_iterator topology_set_iter = m_state.get_router_topology_set().begin();
      topology_set_iter != m_state.get_router_topology_set().end(); topology_set_iter++)
    {
      T_NETWORK_LINK topology_network_link;

      topology_network_link.src_addr = topology_set_iter->tr_from_orig_addr;
      topology_network_link.dest_addr = topology_set_iter->tr_to_orig_addr;
      topology_network_link.out_metric = topology_set_iter->tr_metric;

      p_network_topology_graph->router_topology_set.push_back(topology_network_link);

    }

  }

  /********************************************************************
      * @function  calculate_routing_table
      * @brief     This function computes the routing table
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::calculate_routing_table(const T_NETWORK_TOPOLOGY_GRAPH& p_network_topology_graph)
  {
    ;
  }

  /********************************************************************
      * @function  routing_table_computation
      * @brief     This function computes the routing table
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::routing_table_computation(void)
  {
    T_NETWORK_TOPOLOGY_GRAPH network_topology_graph;

    create_network_topology_graph(&network_topology_graph);

    /* TBD - calculate_routing_table();*/
    cout << "Completed Routing Tale preparation" << endl;

  }

  /********************************************************************
      * @function  mpr_selection
      * @brief     This function calls the routing and flooding
      *             selection functions
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::mpr_computation(void)
  {
    routing_mpr_computation();
    flooding_mpr_computation();
  }

  /********************************************************************
      * @function  routing_mpr_selection
      * @brief     This function finds the routing mprs of this node
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::routing_mpr_computation(void)
  {

    T_NEIGHBOR_GRAPH routing_neighbor_graph;
    std::vector<T_ALLOWED_ONE_HOP_TUPLE> allowed_one_hop_set;

    std::vector<T_ALLOWED_TWO_HOP_TUPLE> allowed_two_hop_set;

    create_allowed_one_hop_set(allowed_one_hop_set, ROUTING_MPR);

    create_allowed_two_hop_set(allowed_two_hop_set, allowed_one_hop_set, ROUTING_MPR);

    create_n1_neighbor_graph(allowed_one_hop_set, &routing_neighbor_graph);

    create_n2_neighbor_graph(allowed_two_hop_set, &routing_neighbor_graph);

    for(std::vector<T_N2>::iterator n2_iter = routing_neighbor_graph.n2_set.begin(); n2_iter != routing_neighbor_graph.n2_set.end(); n2_iter++)
      {
        T_UINT8 index;
        if(find_allowed_one_hop_address<T_ALLOWED_ONE_HOP_TUPLE>(n2_iter->two_hop_neighb_addr, allowed_one_hop_set, &index) == true)
        {
          n2_iter->d1 = allowed_one_hop_set.at(index).in_out_metric;
        }
        else
        {
          n2_iter->d1 = UNKNOWN_METRIC;
        }

      }

    compute_total_metric(allowed_two_hop_set, &routing_neighbor_graph);

    /* TBD - calculate_mprs(routing_neighbor_graph);*/

  }

  /********************************************************************
      * @function  flooding_mpr_selection
      * @brief     This function finds the flooding mprs of this node
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::flooding_mpr_computation(void)
  {

    T_NEIGHBOR_GRAPH flooding_neighbor_graph;
    std::vector<T_ALLOWED_ONE_HOP_TUPLE> allowed_link_set;

    std::vector<T_ALLOWED_TWO_HOP_TUPLE> allowed_two_hop_link_set;

    create_allowed_one_hop_set(allowed_link_set, FLOODING_MPR);

    create_allowed_two_hop_set(allowed_two_hop_link_set, allowed_link_set, FLOODING_MPR);

    create_n1_neighbor_graph(allowed_link_set, &flooding_neighbor_graph);

    create_n2_neighbor_graph(allowed_two_hop_link_set, &flooding_neighbor_graph);

    for(std::vector<T_N2>::iterator n2_iter = flooding_neighbor_graph.n2_set.begin(); n2_iter != flooding_neighbor_graph.n2_set.end(); n2_iter++)
      {
        T_UINT8 index;
        if(find_allowed_one_hop_address<T_ALLOWED_ONE_HOP_TUPLE>(n2_iter->two_hop_neighb_addr, allowed_link_set, &index) == true)
        {
          n2_iter->d1 = allowed_two_hop_link_set.at(index).in_out_metric;
        }
        else
        {
          n2_iter->d1 = UNKNOWN_METRIC;
        }

      }


    /* TBD - calculate_mprs(routing_neighbor_graph);*/

  }

   /********************************************************************
     * @function  get_is_leader
     * @brief     This function returns the Is_leader status
     * @param     None.
     * @return    None.
     * @note      None.
   ********************************************************************/
   T_BOOL get_is_leader(void)
   {
     return true;
   }

   /********************************************************************
     * @function  get_cur_time
     * @brief     This function returns the current time
     * @param     None.
     * @return    None.
     * @note      None.
   ********************************************************************/
   Time get_cur_time()
   {
     Time now = 1;
     return now;
   }
}




int main()
{

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	ns_olsr2_0::C_OLSR Normal_Node_Olsr;
	ns_olsr2_0::C_OLSR Leader_Node_Olsr;

	Normal_Node_Olsr.init(ns_olsr2_0::E_OLSR_INSTANCE::NORMAL_NODE_INSTANCE);

	Leader_Node_Olsr.init(ns_olsr2_0::E_OLSR_INSTANCE::LEADER_NODE_INSTANCE);

	cout << "Size of olsr msg header class " << sizeof(ns_olsr2_0::C_MESSAGE_HEADER) << endl;

    Normal_Node_Olsr.mpr_computation();
    Normal_Node_Olsr.routing_table_computation();

	Normal_Node_Olsr.send_hello();


	//Leader_Node_Olsr.send_hello();

	return 0;
}
