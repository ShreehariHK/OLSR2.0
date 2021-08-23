/*               DEAL MANET Waveform Software Components
 *
 *
 * Source Code Name        : olsr.cpp
 *
 * Source Code Part Number : MNTWSC-321-RI-0004
 *
 * Description             : Implements OLSR protocol functionality .
 *
 * Subsystem Name          : OLSR
 *
 * Revision History
 * ---------------------------------------------------------------------------|
 * Version | Change Description               |    Date    |    Changed By    |
 * --------|----------------------------------|------------|------------------|
 * 1.0     |Initial Version                   | 14-06-2021 | Shreehari H K    |
 * --------|----------------------------------|------------|------------------|
 *
 * COPYRIGHT © Defence Electronics Applications Laboratory (DEAL), Raipur Road, Dehradun - 2480017.
 *
 * PROPRIETARY - This document and the information contained herein is the property of DEAL,
 * and must not be disclosed, copied, altered or used without written permission.
 *
 */

#include <iostream>

#include "olsr.hpp"
//#include <cmath>

using namespace std;

namespace ns_olsr2_0
{

  /********************************************************************
   * @function  C_OLSR_ROUTING_PROTOCOL
   * @brief     This is a  constructor, to set the operational state
   *            to Init state of current OLSR instance
   * @param     None.
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
   * @param     None.
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
   * @param     [1] instance_type - Normal/Leader node instance
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_OLSR::init(ns_olsr2_0::E_OLSR_INSTANCE instance_type)
  {
      cout << "OLSR instance : " << instance_type << endl;
      /* If operational state is INIT_STATE then calls init_state_tuples() to
       * initialize all tuples,  calls init_op() to initialize main data
       * members of this class and sets the operational state is RUN_STATE*/
      if(m_op_state == INIT_STATE)
        {

          this->m_state.init_state_tuples();

          cout << "OLSR Operational State = " << this->get_op_state() << endl;

         this->set_message_sequence_number(M_ZERO);

          this->init_op(instance_type);

          m_op_state = RUN_STATE;
          cout << "OLSR Operational State = " << this->get_op_state() << endl;
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
	   /* If the operational state is RUN_STATE then it does the following:
	    * i)  calls send_hello() if hello_timer is expired,
	    * ii) calls send_tc() if tc_timer is expired,
	    * iii) calls check_tables_timeout() to check tables' validity*/
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
   * @brief     This function resets the OLSR instance
   * @param     None.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_OLSR::reset(void)
  {
	 /* Sets the operational state to INIT_STATE */
     m_op_state = INIT_STATE;

     /*If this is a Leader node instance then, calls init() function
      * of leader node. Else calls the init() function of
      * Normal node.
      */
     if(this->get_is_leader() == true)
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
   * @brief     This function initializes the main data members
   * 			of C_OLSR class
   * @param     [1] inst_type - - Normal/Leader node instance
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_OLSR::init_op(ns_olsr2_0::E_OLSR_INSTANCE inst_type)
  {
    T_ADDR loc_node_addr;

    /* Calls get_node_address() of Node Config class to
     * get the Node Address*/
    loc_node_addr = this->get_node_address();

    /* Checks if the current OLSR instance is a Normal Node Instance */
    if(inst_type == NORMAL_NODE_INSTANCE)
    {
    	/* Assigns the m_node_address to Node address  */
        m_node_address.net_id = loc_node_addr.field.m_oid;
        m_node_address.node_id = loc_node_addr.field.m_nid;

        cout << "Normal Node address.netId = " << m_node_address.net_id<< endl;
        cout << "Normal Node address.nodeId = " << m_node_address.node_id<< endl;

        /* Calls set_is_leader() function to set the m_is_leader to false */
        this->set_is_leader(false);

        /*calls set_instance_type() function to set the m_instance to
         * Normal Node Instance */
        this->set_instance_type(NORMAL_NODE_INSTANCE);

        cout << "Leader status : " << m_is_leader << endl;

        /* Checks if the Node is a leader of the network */
        if(this->get_is_leader() == true)
          {
        	/* Creates a new leader tuple by adding net id equal to
        	 * current net id and node id equal to Zero*/
            T_LEADER_TUPLE leader_tuple;
            leader_tuple.leader_addr.net_id = m_node_address.net_id;
            leader_tuple.leader_addr.node_id = M_ZERO;

            leader_tuple.is_leader = true;

            /* Adds the new leader tuple to leader set */
            this->m_state.insert_leader_tuple(leader_tuple);
          }

    }
    /* Checks if the current OLSR instance is a Leader Node Instance*/
    else if(inst_type == LEADER_NODE_INSTANCE)
    {
    	/* Assigns the m_node_address to Node address by changing
    	 * node id to Zero */
        m_node_address.net_id = loc_node_addr.field.m_oid;
        m_node_address.node_id = M_ZERO;

        /* Checks if the current OLSR instance is a Leader Node instance
         * by calling get_is_leader() function of Node Configuration class.
         * If true then sets m_is_leader to true by calling set_is_leader() function,
         * else then sets m_is_leader to false by calling set_is_leader() function */
        //if((true == get_is_leader()) ? (this->set_is_leader(true)):(this->set_is_leader(false)))
        if(get_is_leader() == true)
          {
            this->set_is_leader(true);
          }
        else
          {
            this->set_is_leader(false);
          }

        cout << "Leader Node address.netId = " << m_node_address.net_id<< endl;
        cout << "Leader Node address.nodeId = " << m_node_address.node_id<< endl;


        cout << "Leader status : " << m_is_leader << endl;
        /* Sets the m_instance to Leader node instance by calling
         * set_instance_type() function */
        this->set_instance_type(LEADER_NODE_INSTANCE);
    }


    /* Calls get_node_willingness() function of Node Configuration class
     * to get node's willingness and assigns it to m_willingness */
    m_willingness.willingness = get_node_willingness();

    cout << "routing_willingness = " << m_willingness.fields.route_will << endl;
    cout << "flooding_willingness = " << m_willingness.fields.flood_will << endl;

    /* TBD  - Subscription to Gateway component */

  }

  /********************************************************************
      * @function  encode_metric_value
      * @brief     This function encodes the float data to 2 bytes
      * @param     [1] data - Floating value of link mettric
      * @return    Link metric value in two bytes.
      * @note      None.
  ********************************************************************/
  unsigned short
  C_OLSR::encode_metric_value(float data)
  {
    return(data * 100);
  }

  /********************************************************************
      * @function  decode_metric_value
      * @brief     This function encodes the float data to 2 bytes
      * @param     [1] data - Two bytes value of link mettric
      * @return    Link metric value in float.
      * @note      None.
  ********************************************************************/
  float
  C_OLSR::decode_metric_value(unsigned short data)
  {
    return(data / 100);
  }


  /********************************************************************
      * @function  check_address_type
      * @brief     This function checks the nodes' address type
      * @param     [1] p_dest_addr - Destination node address
      * @return    addr_type - Unicast/Multicast address.
      * @note      None.
  ********************************************************************/
  E_DEST_ADDR_TYPE
  C_OLSR::check_address_type(const T_ADDR& p_dest_addr)
  {
    E_DEST_ADDR_TYPE addr_type;

    /* If the multiplex bit is 0 then
     * sets the address type to Unicast address */
    if(p_dest_addr.field.m_mux_ubm == M_ZERO)
      {
        addr_type = UNICAST_ADDR;
      }
    /* If the multiplex bit is 1 then
     * sets the address type to Multicast address */
    else if(p_dest_addr.field.m_mux_ubm == M_ONE)
      {
        addr_type = MULTICAST_ADDR;
      }
    return addr_type;
  }


  /********************************************************************
      * @function  find_allowed_one_hop_address
      * @brief     This function finds the allowed 1-hop tuple for the
      *            given address
      * @param     [1] address - Address to be searched in allowed 1-Hop Tuple
      * 		   [2] tuple - Allowed 1-Hop set
      * 		   [3] idx - Tuple's index number
      * @return    addr_found_status.
      * @note      None.
  ********************************************************************/
template <typename T>
  T_BOOL C_OLSR::find_allowed_one_hop_address(T_NODE_ADDRESS address, const std::vector<T>& tuple, T_UINT8* idx)
  {
	T_BOOL addr_found_status = false;
	  for(typename std::vector<T>::const_iterator tuple_iter = tuple.begin(); tuple_iter != tuple.end(); tuple_iter++)
		{
		   /* Checks if the given address is equal to the
		    * allowed 1-hop tuple's one_hop_neighb_addr */
		   if(address == tuple_iter->one_hop_neighb_addr)
			  {
			    /* Assigns index number to found tuple's index number */
				*idx = tuple_iter - tuple.begin();

				/* Sets addr_found_status to true */
				addr_found_status = true;
				return addr_found_status;
			  }

		}
	  /* Sets addr_found_status to false */
	  addr_found_status = false;
	  return addr_found_status;
  }

  /********************************************************************
      * @function  get_next_msg_seq_num
      * @brief     This function increments the message sequence number
      * @param     None.
      * @return    m_message_sequence_number.
      * @note      None.
  ********************************************************************/

  T_UINT16 C_OLSR::get_next_msg_seq_num(void)
  {
    m_message_sequence_number = (m_message_sequence_number + M_ONE) % (M_MAX_MSG_SEQ_NUM + M_ONE);
    return m_message_sequence_number;
  }

  /********************************************************************
      * @function  set_leader_info
      * @brief     This function increments the message sequence
      *            number and sends
      * @param	   [1] hello_msg - Hello message to be prepared.
      * 		   [2] leader_tuple - Leader tuple
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::set_leader_info(C_MESSAGE_HEADER::T_HELLO& hello_msg, T_LEADER_TUPLE* leader_tuple)
  {
	/* Checks if the leader_tuple is not NULL and is a leader*/
    if((leader_tuple != NULL) and (leader_tuple->is_leader == true))
    {
      /* Adds leader_tuple to hello message*/
      hello_msg.leader_info.is_leader = true;
      hello_msg.leader_info = *leader_tuple;
      cout << "Leader node set in Hello message" << endl;
    }
    else
    {
      /* Sets the is_leader flag to false */
      hello_msg.leader_info.is_leader = false;
      hello_msg.leader_info.leader_addr.net_id = 0xff;
      hello_msg.leader_info.leader_addr.node_id = 0xff;
      cout << "Leader node not set in Hello message" << endl;
    }
  }

  /********************************************************************
      * @function  check_message_validity
      * @brief     This function checks the validity of the
      * 	       received OLSR message
      * @param     [1] msg_header - OLSR Message
      * @return    return_status.
      * @note      None.
  ********************************************************************/
  T_BOOL C_OLSR::check_message_validity(const C_MESSAGE_HEADER& msg_header)
  {
    T_BOOL return_status = true;

    /* Checks if the Time to live of received OLSR message is zero*/
    if (msg_header.get_time_to_live () == M_ZERO)
    {
        return_status = false;
        return return_status;
    }
    /* Checks if the originator of received OLSR message is the node itself*/
    if(msg_header.get_originator_address() == this->get_node_addr())
    {
        cout << "Same address error" << endl;
      return_status = false;
    }

    return return_status;
  }

  /********************************************************************
   * @function  check_is_routing_mpr_selector
   * @brief     This function checks if the given node
   *            is a routing mpr selector.
   * @param     [1] hello_msg - Hello message received.
   * @return    return_sts = true/false.
   * @note      None.
   ********************************************************************/
  T_BOOL
  C_OLSR::check_is_routing_mpr_selector(const C_MESSAGE_HEADER::T_HELLO& hello_msg) const
  {
    T_BOOL return_sts = false;
    for (std::vector<C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK>::const_iterator hello_msg_iter = hello_msg.neighbor_set.begin ();
            hello_msg_iter != hello_msg.neighbor_set.end (); hello_msg_iter++)
        {

          /* Checks the type of the instance and proceed if,
           * (the instance type is Normal and hello messages' neighbors' unique id is matching
           * with this node's Node Id) or
           * (the instance type is Leader and hello messages' neighbors' unique id is matching
           * with this node's Net Id)*/
          if (((this->get_instance_type () == NORMAL_NODE_INSTANCE) and (hello_msg_iter->unique_id == m_node_address.node_id))
              || ((this->get_instance_type () == LEADER_NODE_INSTANCE) and (hello_msg_iter->unique_id == m_node_address.net_id)))
            {

              /* Checks if the neighbor has selected this node as a Routing MPR,
               * If true then return true. Otherwise returns false */
              if(((hello_msg_iter->common_field.link_state.type_fields.nbr_type & M_ROUTING_MPR) != M_ZERO) ?
                  (return_sts = true):(return_sts = false))
              break;
                /* TBD - need to add n_advertised if it is necessary in future */
            }
        }
    return return_sts;
  }

  /********************************************************************
      * @function  add_one_hop_neighbor_tuple
      * @brief     This function creates a 1-hop neighbor tuple
      *             and returns it.
      * @param      [1] hello_msg - Received hello message
      *             [2] link_tuple - Link tuple
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::add_one_hop_neighbor_tuple(const C_MESSAGE_HEADER::T_HELLO& hello_msg, const T_LINK_TUPLE* link_tuple)
  {
    T_NEIGHBOUR_TUPLE new_neighbor_tuple;

    /* Creates a new 1-hop neighbor tuple with the data present in link tuple
     * for the particular neighbor*/
    new_neighbor_tuple.n_neighbor_addr = link_tuple->l_neighbor_iface_addr;
    new_neighbor_tuple.n_in_metric = link_tuple->l_in_metric;
    new_neighbor_tuple.n_out_metric = link_tuple->l_out_metric;

    /* Checks if the  neighbor is a symmetric one or not and update the field accordingly */
    if((link_tuple->l_status == M_SYMMETRIC) ? (new_neighbor_tuple.n_symmetric = true) : ( new_neighbor_tuple.n_symmetric = false))

    new_neighbor_tuple.n_willingness = hello_msg.node_willingness;

    /* Checks if the neighbor is a routing MPR selector or not and update the field accordingly */
    if((check_is_routing_mpr_selector(hello_msg) == true) ? (new_neighbor_tuple.n_mpr_selector = true) : (new_neighbor_tuple.n_mpr_selector = false))

    /* Calls insert_neighbour_tuple() to add the tuple to 1-hop neighbor set*/
    m_state.insert_neighbour_tuple(new_neighbor_tuple);

  }

  /********************************************************************
      * @function  update_one_hop_neighbor_tuple
      * @brief     This function updates 1-Hop neighbor tuple.
      * @param      [[1] hello_msg - Received hello message
      *             [2] link_tuple - Link tuple
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::update_one_hop_neighbor_tuple(const C_MESSAGE_HEADER::T_HELLO& hello_msg, const T_LINK_TUPLE* link_tuple)
  {
    /* Checks whether the neighbor table is already presentfor this neighbor
     * If not present, creates a new 1-Hop neighbor tuple
     * If present then updates it */
    T_NEIGHBOUR_TUPLE* neighbor_tuple = m_state.find_neighbour_tuple(link_tuple->l_neighbor_iface_addr);

    if(neighbor_tuple == NULL)
      {
        /* Calls add_one_hop_neighbor_tuple() to create and add the 1-hop neighbor tuple to
         * 1-hop neighbor set */
        add_one_hop_neighbor_tuple(hello_msg, link_tuple);

      }
    else if(neighbor_tuple!= NULL)
      {
        neighbor_tuple->n_in_metric = link_tuple->l_in_metric;
        neighbor_tuple->n_out_metric = link_tuple->l_out_metric;
        /* Checks if the  neighbor is a symmetric one or not and update the field accordingly */
        if((link_tuple->l_status == M_SYMMETRIC) ? (neighbor_tuple->n_symmetric = true) :
            ( neighbor_tuple->n_symmetric = false));

        /* Checks if the neighbor is a routing MPR selector or not and update the field accordingly */
        if((check_is_routing_mpr_selector(hello_msg) == true) ? (neighbor_tuple->n_mpr_selector = true) : (neighbor_tuple->n_mpr_selector = false))
        neighbor_tuple->n_willingness = hello_msg.node_willingness;;

      }
  }

  /********************************************************************
      * @function  create_two_hop_neighbor_tuple
      * @brief     This function creates a 2-Hop neighbor tuple
      *             and returns it.
      * @param      [1] msg - OLSR message
      *             [2] neighb_addr - 2-hop neighbor's info
      *             [3] neighb_addr - 2-hop neighbor's address
      * @return    new 2-Hop neighbor tuple.
      * @note      None.
  ********************************************************************/
  T_TWO_HOP_NEIGHBOUR_TUPLE
      C_OLSR::create_two_hop_neighbor_tuple(const C_MESSAGE_HEADER& msg,
                                    C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK neighb_info, const T_NODE_ADDRESS& neighb_addr)
  {
    T_TWO_HOP_NEIGHBOUR_TUPLE new_two_hop_tuple;

    new_two_hop_tuple.n2_2hop_addr = neighb_addr;
    new_two_hop_tuple.n2_neighbor_iface_addr = msg.get_originator_address();
    new_two_hop_tuple.n2_in_metric = decode_metric_value(neighb_info.metric[1]);
    new_two_hop_tuple.n2_out_metric = decode_metric_value(neighb_info.metric[0]);
    new_two_hop_tuple.n2_time = get_ns() + msg.get_validity_time();
    return new_two_hop_tuple;
  }
  /****
   * ****************************************************************
      * @function  add_two_hop_neighbor_tuple
      * @brief     This function adds a 2-Hop neighbor tuple
      *             and returns it.
      * @param      [1] new_two_hop_tuple - new 2-Hop tuple
      * @return    new 2-Hop neighbor tuple.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::add_two_hop_neighbor_tuple(const T_TWO_HOP_NEIGHBOUR_TUPLE& new_two_hop_tuple)
  {
    m_state.insert_two_hop_neighbour_tuple(new_two_hop_tuple);
  }

  /********************************************************************
      * @function  create_advertizing_router_tuple
      * @brief     This function creates a processed message tuple
      *             and returns it.
      * @param      [1] msg - OLSR message
      *             [2] p_ansn - Advertized neighbor sequence number
      * @return    new_ advertising router tuple.
      * @note      None.
  ********************************************************************/
  T_ADVERTISING_REMOTE_ROUTER_TUPLE
  C_OLSR::create_advertizing_router_tuple(const C_MESSAGE_HEADER& msg, const T_UINT16 p_ansn)
  {
    T_ADVERTISING_REMOTE_ROUTER_TUPLE new_tuple;

    new_tuple.ar_orig_addr = msg.get_originator_address();
    new_tuple.ar_seq_number = p_ansn;
    /* Adds the current time to message validity time and adds it to new advertizing router tuple*/
    new_tuple.ar_time = get_ns() + msg.get_validity_time();
    return new_tuple;
  }

  /********************************************************************
      * @function  create_router_topology_tuple
      * @brief     This function creates a processed message tuple
      *             and returns it.
      * @param      [1] msg_header - OLSR message
      * @return    new Router Topology tuple.
      * @note      None.
  ********************************************************************/
  T_ROUTER_TOPOLOGY_TUPLE
      C_OLSR::create_router_topology_tuple(const C_MESSAGE_HEADER& msg,
              const C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK remote_router,const T_NODE_ADDRESS& router_addr)
  {
    T_ROUTER_TOPOLOGY_TUPLE new_tuple;
    new_tuple.tr_from_orig_addr = msg.get_originator_address();
    new_tuple.tr_to_orig_addr = router_addr;
    new_tuple.tr_seq_number = msg.get_message_sequence_number();
    new_tuple.tr_metric = decode_metric_value(remote_router.metric[0]);
    /* Adds the current time to message validity time and adds it to new router topology tuple*/
    new_tuple.tr_time = get_ns() + msg.get_validity_time();
    return new_tuple;
  }

  /********************************************************************
      * @function  create_processed_msg_tuple
      * @brief     This function creates a processed message tuple
      * 			and returns it.
      * @param      [1] msg_header - OLSR message
      * @return    new_proc_msg_tuple.
      * @note      None.
  ********************************************************************/
  T_PROCESSED_MSG_TUPLE
  C_OLSR::create_processed_msg_tuple(const C_MESSAGE_HEADER& msg_header)
  {
    T_PROCESSED_MSG_TUPLE new_proc_msg_tuple;
    Time cur_time = get_ns();

    new_proc_msg_tuple.p_orig_addr = msg_header.get_originator_address();
    new_proc_msg_tuple.p_seq_number = msg_header.get_message_sequence_number();
    new_proc_msg_tuple.p_type = msg_header.get_message_type();
    /* Adds the current time to message validity time and adds it to new procssed msg tuple*/
    new_proc_msg_tuple.p_time = cur_time + msg_header.get_validity_time();
    return new_proc_msg_tuple;

  }
  /********************************************************************
       * @function  create_forwarded_msg_tuple
       * @brief     This function creates a forwarded message tuple
       * 			and returns it.
       * @param     [1] msg_header - OLSR message
       * @return    new_fwd_msg_tuple.
       * @note      None.
   ********************************************************************/

  T_FORWARDED_MSG_TUPLE
  C_OLSR::create_forwarded_msg_tuple(const C_MESSAGE_HEADER& msg_header)
  {
    T_FORWARDED_MSG_TUPLE new_fwd_msg_tuple;
    Time cur_time = get_ns();

    new_fwd_msg_tuple.f_orig_addr = msg_header.get_originator_address();
    new_fwd_msg_tuple.f_seq_number = msg_header.get_message_sequence_number();
    new_fwd_msg_tuple.f_type = msg_header.get_message_type();
    /* Adds the current time to message validity time and adds it to new forwarded msg tuple*/
    new_fwd_msg_tuple.f_time = cur_time + msg_header.get_validity_time();
    return new_fwd_msg_tuple;
  }

  /********************************************************************
  * @function  create_new_link_tuple
  * @brief     This function creates a link tuple and returns it.
  * @param 	   [1] msg_header - OLSR message
  * 		   [2] cur_time - Current time
  * 		   [3] in_metric - Inlink metric
  * @return    new_link_tuple.
  * @note      None.
  ********************************************************************/
  T_LINK_TUPLE
  C_OLSR::create_new_link_tuple(const C_MESSAGE_HEADER& msg_header, Time cur_time, float in_metric)
  {
    T_LINK_TUPLE new_link_tuple;
    new_link_tuple.l_neighbor_iface_addr = msg_header.get_originator_address ();
    new_link_tuple.l_sym_time = cur_time - M_ONE;
    /* Adds the current time to message validity time and adds it to new link tuple*/
    new_link_tuple.l_time = cur_time + msg_header.get_validity_time ();
    new_link_tuple.l_in_metric = in_metric;
    new_link_tuple.l_mpr_selector = false;
    new_link_tuple.l_out_metric = M_ZERO;
    new_link_tuple.l_status = M_HEARD_LINK;

    return new_link_tuple;
  }
  /********************************************************************
      * @function  validate_neighbor_link_type
      * @brief     This function increments the message sequence
      *            number and sends
      * @param	   [1] iter - Iterator of T_GENERIC_ADDR_BLOCK of OLSR msg
      * @return    return_value.
      * @note      None.
  ********************************************************************/
  T_BOOL C_OLSR::validate_neighbor_link_type(C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK iter)
  {
	T_BOOL  return_value = true;

	/* Checks if the following is true and returns true
	 * i) Link type is Lost
	 * ii) Link type is Heard and (neighbor type is Symmetric
	 * or Routing MPR or Flooding MPR or Both(Flooding and Routing MPR))
	 * iii) Link type is Symmetric and neighbor type is Not a neighbor*/

    if ((((iter.common_field.link_state.type_fields.link_type == M_LOST_LINK)
    or (iter.common_field.link_state.type_fields.link_type == M_HEARD_LINK))
    and ((iter.common_field.link_state.type_fields.nbr_type == M_SYMMETRIC_NEIGHBOR)
        or (iter.common_field.link_state.type_fields.nbr_type == M_ROUTING_MPR)
        or (iter.common_field.link_state.type_fields.nbr_type == M_FLOODING_MPR)
        or (iter.common_field.link_state.type_fields.nbr_type == M_MPR_FLOOD_ROUTE)))
    or ((iter.common_field.link_state.type_fields.link_type == M_SYMMETRIC_LINK)
        and (iter.common_field.link_state.type_fields.nbr_type == M_NOT_A_NEIGHBOR)))
    {
    	return_value = false;
    }

    return return_value;
  }

  /********************************************************************
      * @function  populate_two_hop_set
      * @brief     This function processes the received Hello message
      *            and updates the 2-hop neighbor set
      * param      [1] p_olsr_msg - Received olsr message
      *            [2] p_hello_msg - Hello message
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::populate_two_hop_set(const C_MESSAGE_HEADER& p_olsr_msg, const C_MESSAGE_HEADER::T_HELLO& p_hello_msg)
  {

    /* Checks whether the originator of the hello message is a
     * symmetric 1-Hop neibor.
     * If true then it processes hello message for 2-Hop set updations.
     * Otherwise skips */
    T_LINK_TUPLE* link_tuple = m_state.find_sym_link_tuple(p_olsr_msg.get_originator_address());
    if(link_tuple != NULL)
      {
        E_ADDRESS_BLOCK_FLAGS common_address_field;
        T_UINT8 common_address_id;
        T_NODE_ADDRESS remote_router_address;

        /* Gets the common address information
         * i.e whether the address contain common Net ID or Node ID*/
        get_common_address (p_hello_msg, &common_address_field, &common_address_id);

        for(std::vector<C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK>::const_iterator hello_neighb_iter = p_hello_msg.neighbor_set.begin();
            hello_neighb_iter != p_hello_msg.neighbor_set.end(); hello_neighb_iter++)
          {
            switch(common_address_field)
            {
              /* If Net ID is common then,
               * adds the common ID to net_id field
               * and adds router's unique id to node id */
              case NET_ID_COMMON:
                remote_router_address.net_id = common_address_id;
                remote_router_address.node_id = hello_neighb_iter->unique_id;
                break;

                /* If Node  ID is common then,
                 * adds the common ID to Ndde ID field
                 * and adds router's unique id to net id */
              case NODE_ID_COMMON:
                remote_router_address.node_id = common_address_id;
                remote_router_address.net_id = hello_neighb_iter->unique_id;
                break;

            }

            /* Checks if the neighbor mentioned in the hello message is either of the following
             * Symmetric or Flooding MPR or Routing MPR.
             * If true then pocessds further,
             * Else removes the 2-hop neighbor tuples connected through this
             * 1-Hop neighbor(Hello message originator) */
            if(((hello_neighb_iter->common_field.link_state.type_fields.nbr_type & M_SYMMETRIC_NEIGHBOR) != 0) or
              ((hello_neighb_iter->common_field.link_state.type_fields.nbr_type & M_FLOODING_MPR) != 0) or
              ((hello_neighb_iter->common_field.link_state.type_fields.nbr_type & M_ROUTING_MPR) != 0))
              {
                /* If the neighbor address mentioned in Hello message is this node's address only,
                 * then skip this iteration.
                 * (the instance type is Normal and hello messages' neighbors' unique id is matching
                 * with this node's Node Id) or
                 * (the instance type is Leader and hello messages' neighbors' unique id is matching
                 * with this node's Net Id) */
                if (((this->get_instance_type () == NORMAL_NODE_INSTANCE) and (hello_neighb_iter->unique_id == m_node_address.node_id))
                    || ((this->get_instance_type () == LEADER_NODE_INSTANCE) and (hello_neighb_iter->unique_id == m_node_address.net_id)))
                  {
                      continue;
                  }

                /* Finds whether the neighbor mentioned in the Hello message is already a
                 * 2-Hop neighbor of this node.
                 * If true then updates it.
                 * Otherwise creates a new 2-Hop neighbor tuple */
                T_TWO_HOP_NEIGHBOUR_TUPLE* two_hop_tuple = m_state.find_two_hop_neighbour_tuple(p_olsr_msg.get_originator_address(), remote_router_address);

                if(two_hop_tuple == NULL)
                  {
                    T_TWO_HOP_NEIGHBOUR_TUPLE new_two_hop_tuple;
                    new_two_hop_tuple = create_two_hop_neighbor_tuple(p_olsr_msg, *hello_neighb_iter, remote_router_address);
                    add_two_hop_neighbor_tuple(new_two_hop_tuple);
                  }
                else
                  {
                    two_hop_tuple->n2_time = get_ns() + p_olsr_msg.get_validity_time();
                    two_hop_tuple->n2_in_metric = decode_metric_value(hello_neighb_iter->metric[1]);
                    two_hop_tuple->n2_out_metric = decode_metric_value(hello_neighb_iter->metric[0]);
                  }
              }
            else
              {
                 m_state.erase_two_hop_neighbour_tuples(p_olsr_msg.get_originator_address(), remote_router_address);
              }

          }


      }
    else
      {
        return;
      }
  }

  /********************************************************************
      * @function  populate_router_topology_set
      * @brief     This function updates the Router topology set
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::populate_router_topology_set(const C_MESSAGE_HEADER& rcvd_msg,
    const C_MESSAGE_HEADER::T_TC_ADDRESS_BLOCK& tc_addr_tuple, E_ADDRESS_BLOCK_FLAGS common_flag, T_NODE_ADDRESS remote_router_address)
  {
    for (std::vector<C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK>::const_iterator topology_iter =
          tc_addr_tuple.network_info.begin (); topology_iter != tc_addr_tuple.network_info.end (); topology_iter++)
    {
        if(common_flag == NET_ID_COMMON)
          {
            remote_router_address.node_id = topology_iter->unique_id;
          }
        else
          {
            remote_router_address.net_id = topology_iter->unique_id;
          }

        /* If the Remote router's address mentioned in TC message is this node's address only,
         * then skip this iteration.
         * (the instance type is Normal and tc messages' neighbors' unique id is matching
         * with this node's Node Id) or
         * (the instance type is Leader and tc messages' neighbors' unique id is matching
         * with this node's Net Id) */
        if (((this->get_instance_type () == NORMAL_NODE_INSTANCE) and (topology_iter->unique_id == m_node_address.node_id))
            || ((this->get_instance_type () == LEADER_NODE_INSTANCE) and (topology_iter->unique_id == m_node_address.net_id)))
          {
              continue;
          }
        /* Checks if the Router topology tuple is already present for the
         * Advertized router*/
        T_ROUTER_TOPOLOGY_TUPLE* router_topology_tuple = m_state.find_router_topology_tuple (
            remote_router_address, rcvd_msg.get_originator_address ());

        /* If the tuple is present then updates the
         * tuple's validity time and Metric value*/
        if (router_topology_tuple != NULL)
          {
            router_topology_tuple->tr_time = rcvd_msg.get_validity_time ();
            router_topology_tuple->tr_metric = decode_metric_value(topology_iter->metric[0]);
          }
        /* If the tuple is nor present then creates a new
         * Router topology tuple and adds it to the Router topology set */
        else
          {
            T_ROUTER_TOPOLOGY_TUPLE new_tuple;
            new_tuple = create_router_topology_tuple (rcvd_msg, *topology_iter, remote_router_address);

            m_state.insert_router_topology_tuple (new_tuple);

          }
     }

  }

  /********************************************************************
      * @function  set_neighbor_info
      * @brief     This function adds the neighbor information into
      * 		   hello message
      * @param	   [1] hello_msg - Hello message
      * 		   [2] cur_time - Current time
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::set_neighbor_info(C_MESSAGE_HEADER::T_HELLO& hello_msg, Time cur_time)
  {
    C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK nbr_addr_block;

    /* Fethes the link set by calling get_links() function */
    const LinkSet &link_set = this->m_state.get_links();

    for(LinkSet::const_iterator link_set_iter = link_set.begin() ; link_set_iter != link_set.end() ; link_set_iter++)
    {
        T_LINK_TUPLE link_tuple = *link_set_iter;

        /* Checks if l_in_metric and l_out_metric of the link tuple is
         *  not equal to UNKNOWN_VALUE */
        if((link_tuple.l_in_metric != M_UNKNOWN_VALUE) and
        		(link_tuple.l_out_metric != M_UNKNOWN_VALUE))
          {
            nbr_addr_block.metric[M_ONE] = encode_metric_value(link_tuple.l_in_metric);
            nbr_addr_block.metric[M_ZERO] = encode_metric_value(link_tuple.l_out_metric);

            /* If the symmetric time of the link tuple is greater than
             * or equal to current time, then set link type to Symmetric */
            if(link_tuple.l_sym_time >= cur_time)
            {
                nbr_addr_block.common_field.link_state.type_fields.link_type = M_SYMMETRIC_LINK;
            }

            /* If the heard time of the link tuple is greater than
            * or equal to current time, then set link type to Heard */
            else if (link_tuple.l_heard_time >= cur_time)
            {
                nbr_addr_block.common_field.link_state.type_fields.link_type = M_HEARD_LINK;
            }
            /* sets link type to Lost */
            else
            {
                nbr_addr_block.common_field.link_state.type_fields.link_type = M_LOST_LINK;
            }

            /* Fetches the 1-Hop neighbor set by calling get_one_hop_neighbors() function*/
            const NeighbourSet &neighb_set = this->m_state.get_one_hop_neighbors();
            T_BOOL to_be_added = false;

            for(NeighbourSet::const_iterator neighb_set_iter = neighb_set.begin(); neighb_set_iter != neighb_set.end(); neighb_set_iter++)
            {
                T_NEIGHBOUR_TUPLE neighb_tuple = *neighb_set_iter;

                /* Checks if the l_neighbor_iface_addr of the link tuple is equal to
                 * n_neighbor_addr of 1-hop neighbor tuple
                 */
                if(link_tuple.l_neighbor_iface_addr == neighb_tuple.n_neighbor_addr)
                  {
                    /* If the neighbor is a symmetric one then
                     * process further */
                    if(neighb_tuple.n_symmetric == true)
                      {
                        /* If the neighbor is both Routing and Flooding MPR then
                         * set the neighbor type as both Flooding and Routing MPR*/
                        if((neighb_tuple.n_routing_mpr == true) and (neighb_tuple.n_flooding_mpr == true))
                        {
                            nbr_addr_block.common_field.link_state.type_fields.nbr_type = M_MPR_FLOOD_ROUTE;
                        }
                        /* If the neighbor is a Routing MPR then
                         * set the neighbor type as Routing MPR*/
                        else if(neighb_tuple.n_routing_mpr == true)
                        {
                            nbr_addr_block.common_field.link_state.type_fields.nbr_type = M_ROUTING_MPR;
                        }
                        /* If the neighbor is a Flooding MPR then
                         * set the neighbor type as Flooding MPR*/
                        else if(neighb_tuple.n_flooding_mpr == true)
                        {
                            nbr_addr_block.common_field.link_state.type_fields.nbr_type = M_FLOODING_MPR;
                        }
                        /* If the neighbor type is none of the above then
                         * set nbr_type as Symmetric */
                        else
                        {
                            nbr_addr_block.common_field.link_state.type_fields.nbr_type = M_SYMMETRIC_NEIGHBOR;
                        }
                      }
                    /* Sets nbr_type as Not a neighbor */
                    else
                      {
                        nbr_addr_block.common_field.link_state.type_fields.nbr_type = M_NOT_A_NEIGHBOR;
                      }

                    to_be_added = true;
                    break;
                  }
             }
            if(to_be_added)
            {
              /* Adds the neighbor address block to Neighbor set of hello message */
              hello_msg.neighbor_set.push_back(nbr_addr_block);
              cout << "Tuple added to hello message" << endl;
            }


          }

      }
    cout << "neighbor set size = " << hello_msg.neighbor_set.size() * 6 << endl;
  }

  /********************************************************************
      * @function  create_allowed_one_hop_set
      * @brief     This function creates the allowed 1-hop set
      * @param	   [1] p_allowed_one_hop_set - allowed 1-hop set
      * 		   [2] p_mpr_type - Flooding/Routing MPR
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::create_allowed_one_hop_set(std::vector<T_ALLOWED_ONE_HOP_TUPLE> &p_allowed_one_hop_set, T_UINT8 p_mpr_type)
  {
	/* Checks if the MPR type is Routing MPR*/
    if(p_mpr_type == M_ROUTING_MPR)
      {
        for(NeighbourSet::const_iterator nbr_itr = m_state.get_one_hop_neighbors().begin();
                    nbr_itr != m_state.get_one_hop_neighbors().end(); nbr_itr++)
        {
          /* Prepares and adds the allowed neighbor tuple to the allowed neighbor set if
           * the neighbor is symmetric one, his in_metric is not equal to UNKNOWN_VALUE
           * and his routing willingness is other than WILL_NEVER */
          if((nbr_itr->n_symmetric == true) and (nbr_itr->n_in_metric != M_UNKNOWN_VALUE) and
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
    /* Checks if the MPR type is Flooding MPR*/
    else if(p_mpr_type == M_FLOODING_MPR)
      {
        for(LinkSet::const_iterator link_itr = m_state.get_links().begin();
                link_itr != m_state.get_links().end(); link_itr++)
        {

		  /* Checks if the neighbor link status is symmetric,
		   * his out_metric is not equal to UNKNOWN_VALUE */
          if((link_itr->l_status == M_SYMMETRIC) and (link_itr->l_out_metric != M_UNKNOWN_VALUE))
            {
        	  /* Calls find_neighbour_tuple() to check whether this neighbor
        	   * is a symmetric 1-hop neighbor information*/
              T_NEIGHBOUR_TUPLE * nbr_tuple = m_state.find_neighbour_tuple(link_itr->l_neighbor_iface_addr);

              /* Prepares and adds the allowed link tuple to the allowed link set if
               * neighbor is Symmetric and his Flooding willingness is other than WILL_NEVER*/
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
      * @function  create_allowed_two_hop_set
      * @brief     This function creates the allowed 2-Hop set
      * @param	   [1] p_allowed_two_hop_set - allowed 2-Hop set
      * 		   [2] p_allowed_one_hop_set -allowed 1-Hop set
      * 		   [3] mpr_type - Flooding/Routing MPR
      * @return    None.
      * @note      None.
  ********************************************************************/

  void C_OLSR::create_allowed_two_hop_set(std::vector<T_ALLOWED_TWO_HOP_TUPLE>& p_allowed_two_hop_set, const std::vector<T_ALLOWED_ONE_HOP_TUPLE>& p_allowed_one_hop_set, float mpr_type)
  {

    for(TwoHopNeighborSet::const_iterator two_hop_iter = m_state.get_two_hop_neighbours().begin();
        two_hop_iter != m_state.get_two_hop_neighbours().end(); two_hop_iter++)
      {
        T_UINT8 index=M_ZERO;

        /* Checks if in_link and out_link cost of 2-Hop neighbor is
         * other than UNKNOWN_VALUE and this 2-Hop neighbor is connected
         * through any symmetric 1-Hop neighbor */
        if((two_hop_iter->n2_in_metric != M_UNKNOWN_VALUE) and
        		(two_hop_iter->n2_out_metric != M_UNKNOWN_VALUE) and
            (find_allowed_one_hop_address<T_ALLOWED_ONE_HOP_TUPLE>(two_hop_iter->n2_neighbor_iface_addr, p_allowed_one_hop_set, &index) == true))
          {
            T_ALLOWED_TWO_HOP_TUPLE new_allowed_two_hop_tuple;

            /* If MPR type is Routing then add 2-Hop neighbor's in_link metric to
             * new allowed 2-Hop tuple */
            if(mpr_type == M_ROUTING_MPR)
              {
                new_allowed_two_hop_tuple.in_out_metric = two_hop_iter->n2_in_metric;
              }

            /* If MPR type is Flooding then add 2-Hop neighbor's out_link metric to
             * new allowed 2-Hop tuple */
            else if(mpr_type == M_FLOODING_MPR)
              {
                new_allowed_two_hop_tuple.in_out_metric = two_hop_iter->n2_out_metric;
              }
            new_allowed_two_hop_tuple.two_hop_neighb_addr = two_hop_iter->n2_2hop_addr;
            new_allowed_two_hop_tuple.one_hop_neighb_addr = two_hop_iter->n2_neighbor_iface_addr;

            p_allowed_two_hop_set.push_back(new_allowed_two_hop_tuple);
          }
      }

  }
  /********************************************************************
      * @function  create_n1_neighbor_graph
      * @brief     This function creates the N1 neighbor Graph
      * @param     [1] p_allowed_one_hop_set - Allowed 1-Hop set
      * 	       [2] p_neighbor_graph - Neighbor graph
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::create_n1_neighbor_graph(const std::vector<T_ALLOWED_ONE_HOP_TUPLE> &p_allowed_one_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph)
  {
	/* Loop through the allowed 1-Hop set and add to N1 neighbor graph*/
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
      * @function  create_n2_neighbor_graph
      * @brief     This function creates the N2 Neighbor graph
      * @param 	   [1] p_allowed_two_hop_set - Allowed 2-Hop set
      * 		   [3] p_neighbor_graph - Neighbor Graph
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::create_n2_neighbor_graph(const std::vector<T_ALLOWED_TWO_HOP_TUPLE> &p_allowed_two_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph)
  {
	/* Loop through the allowed 2-Hop set and add to N1 neighbor graph*/
    for(std::vector<T_ALLOWED_TWO_HOP_TUPLE>::const_iterator allowed_two_hop_iter = p_allowed_two_hop_set.begin();
        allowed_two_hop_iter != p_allowed_two_hop_set.end(); allowed_two_hop_iter++)
      {
        T_N2 new_n2_tuple;

        new_n2_tuple.one_hop_neighb_addr = allowed_two_hop_iter->one_hop_neighb_addr;
        new_n2_tuple.two_hop_neighb_addr = allowed_two_hop_iter->two_hop_neighb_addr;
        p_neighbor_graph->n2_set.push_back(new_n2_tuple);
      }

  }

  /********************************************************************
      * @function  compute_total_metric
      * @brief     This function computes the Total link metric
      * 		   between current node and 2-Hop neighbor
      * @param	   [1] p_allowed_two_hop_set - Allowed 2-Hop set
      * 		   [2] p_neighbor_graph - Neighbor Graph
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::compute_total_metric(const std::vector<T_ALLOWED_TWO_HOP_TUPLE> &p_allowed_two_hop_set, T_NEIGHBOR_GRAPH *p_neighbor_graph)
  {
	/* Loops through Allowed 2-Hop set */
    for(std::vector<T_ALLOWED_TWO_HOP_TUPLE>::const_iterator allowed_two_hop_iter = p_allowed_two_hop_set.begin();
            allowed_two_hop_iter != p_allowed_two_hop_set.end(); allowed_two_hop_iter++)
    {
      /* Loops through neighbor Graph */
      for(std::vector<T_N1>::iterator n1_iter = p_neighbor_graph->n1_set.begin();
          n1_iter != p_neighbor_graph->n1_set.end(); n1_iter++)
        {
    	  /* If the 1-Hop neighbor address present in allowed 2-Hop set is
    	   * equal to the address present in N1 Neighbor graph then
    	   * i) assigns 2-Hop's address to new matrix,
    	   * ii) adds the link metric(Between 1-Hop and 2-Hop neighbor) value to matrix,
    	   * iii) adds total link metric and assign it to matrix
    	   */
          if(allowed_two_hop_iter->one_hop_neighb_addr == n1_iter->one_hop_neighb_addr)
          {
              T_MATRIX new_matrix;

              new_matrix.two_hop_neighb_addr = allowed_two_hop_iter->two_hop_neighb_addr;
              new_matrix.d2 = allowed_two_hop_iter->in_out_metric;
              new_matrix.d = n1_iter->d1 + new_matrix.d2;
#ifdef COMMENT_SECTION
              n1_iter->matrix_set.insert({allowed_two_hop_iter->two_hop_neighb_addr, new_matrix});
#endif
              n1_iter->matrix_set.push_back(new_matrix);

          }

        }

    }

  }
  /********************************************************************
      * @function  create_n_neighbor_graph
      * @brief     This function creates the N Neighbor graph
      * @param     [1] p_neighbor_graph - Neighbor grapgh
      *            [3] n_set - A subset of N2 neighbor grapgh
      *            which is to be filled.
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::create_n_neighbor_graph(const T_NEIGHBOR_GRAPH &p_neighbor_graph, std::vector<T_N2>& n_set)
  {
    /* Loops through the N2 Neighbor graph(2-Hop neighbors) and checks if d1(in/Out metric value) is undefined
     * If true then adds that 2_hop neighbor to N neighbor graph  */
    for(std::vector<T_N2>::const_iterator n2_iter = p_neighbor_graph.n2_set.begin();
            n2_iter != p_neighbor_graph.n2_set.end(); n2_iter++)
      {
        if(n2_iter->d1 == M_UNKNOWN_VALUE)
          {
            n_set.push_back(*n2_iter);
          }
        /* If d1(in/Out metric value) is defined then,
         * Loops through N1 Neighbor graph and finds atleast one 1-Hop neighbor is connected
         * to this 2-Hop neighbor and d(Total metric) is less than d1(in/Out metric value)
         * If true then adds that 2-Hop neighbor to N neighbor graph*/
        else
          {
            for(std::vector<T_N1>::const_iterator n1_iter = p_neighbor_graph.n1_set.begin();
                n1_iter != p_neighbor_graph.n1_set.end(); n1_iter++)
              {
                const T_N1 &n1_graph_tuple = *n1_iter;

                std::vector<T_MATRIX>::const_iterator matrix_iter = std::find(n1_graph_tuple.matrix_set.begin(),
                                                                              n1_graph_tuple.matrix_set.end(), n2_iter->two_hop_neighb_addr );
                if((matrix_iter!= n1_graph_tuple.matrix_set.end()) and (n2_iter->d1 > matrix_iter->d))
                {
                   n_set.push_back(*n2_iter);
                }

#ifdef COMMENT_SECTION
                std::map<T_NODE_ADDRESS, T_MATRIX>::const_iterator matrix_iter = n1_graph_tuple.matrix_set.find(n2_iter->two_hop_neighb_addr);
                if(matrix_iter != n1_graph_tuple.matrix_set.end())
                  {
                    if(n2_iter->d1 > matrix_iter->second.d)
                      {
                        n_set.push_back(*n2_iter);
                      }
                  }
#endif
              }
          }
      }
  }

  /********************************************************************
      * @function  find_reachability
      * @brief     This function finds the reachability of all 1-Hop neighbors
      * @param     [1] n1_neighbor_graph - 1-Hop neighbors set
      *            [2] remaining_two_hop_neighb - Uncovered 2-Hop neighbor set
      *            [3] rs - Reachability count
      *            [4] reachability - Reachabilty data
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::find_reachability(const std::vector<T_N1>& n1_neighbor_graph, const std::vector<T_N2>& remaining_two_hop_neighb,
                        std::set<T_UINT8>& rs, std::map<T_UINT8, std::vector<const T_N1*>>& reachability_map)
  {
    T_UINT8 reachability = 0;
    T_BOOL increment_reachability = false;

    /* Loops through the 1-Hop neighbor set*/
    for(std::vector<T_N1>::const_iterator n1_neighb_graph_iter = n1_neighbor_graph.begin();
        n1_neighb_graph_iter != n1_neighbor_graph.end(); n1_neighb_graph_iter++)
      {
        reachability= 0;
        increment_reachability = false;
        /* Loops through the 2-Hop neighbor set*/
        for(std::vector<T_N2>::const_iterator remaining_n_graph_iter = remaining_two_hop_neighb.begin();
            remaining_n_graph_iter != remaining_two_hop_neighb.end(); remaining_n_graph_iter++)
          {
            /* Checks if the 2-Hop neighbor is connected through the given 1-Hop neighbor */
            if(n1_neighb_graph_iter->one_hop_neighb_addr == remaining_n_graph_iter->one_hop_neighb_addr)
              {
                /* Checks if the metric value is present between this 2-Hop and 1-Hop neighbor */
#ifdef COMMENT_SECTION
                std::map<T_NODE_ADDRESS, T_MATRIX>::const_iterator one_hop_iter = n1_neighb_graph_iter->matrix_set.find(remaining_n_graph_iter->two_hop_neighb_addr);
#else
                std::vector<T_MATRIX>::const_iterator one_hop_iter = std::find(n1_neighb_graph_iter->matrix_set.begin(),
                                   n1_neighb_graph_iter->matrix_set.end(), remaining_n_graph_iter->two_hop_neighb_addr);
#endif
                if(one_hop_iter != n1_neighb_graph_iter->matrix_set.end())
                  {
                    /* Loops through the other 1-Hop neighbors */
                    for(std::vector<T_N1>::const_iterator other_n1_graph_iter = n1_neighbor_graph.begin();
                                        other_n1_graph_iter != n1_neighbor_graph.end(); other_n1_graph_iter++)
                    {
                      const T_N1& other_one_hop_neighbor = *other_n1_graph_iter;

                      /* Checks if the metric value is present between this 2-Hop and other 1-Hop neighbor */
#ifdef COMMENT_SECTION
                      std::map<T_NODE_ADDRESS, T_MATRIX>::const_iterator other_neighbor_iter = other_one_hop_neighbor.matrix_set.find(remaining_n_graph_iter->two_hop_neighb_addr);
#else
                      std::vector<T_MATRIX>::const_iterator other_neighbor_iter = std::find(other_one_hop_neighbor.matrix_set.begin(),
                                      other_one_hop_neighbor.matrix_set.end(), remaining_n_graph_iter->two_hop_neighb_addr);
#endif
                      if(other_neighbor_iter != n1_neighb_graph_iter->matrix_set.end())
                        {
                          /* Checks if the metric value of current 1-Hop neighbor is less than
                           * other 1-Hop neighbor. If true then increments its reachability */
                          if(one_hop_iter->d < other_neighbor_iter->d)
                            {
                              increment_reachability = true;
                            }
                          else
                            {
                              increment_reachability = false;
                            }
                         }

                     }
                    if(increment_reachability == true)
                      {
                        reachability++;
                        increment_reachability = false;
                      }
                  }
              }

          }
        /* Adds the reachability count of the current neighbor*/
        rs.insert(reachability);
        /* Adds the current neighbor to reachability map */
        reachability_map[reachability].push_back(&(*n1_neighb_graph_iter));
      }

  }

  /********************************************************************
      * @function  find_degree
      * @brief     This function finds the degree of all 1-Hop neighbors
      * @param     [1] one_hop_neighbor - 1-Hop neighbor
      *            [2] n1_neighbor_graph - 1-Hop neighbors set
      *            [3] n_neighbor_graph - 2-Hop neighbor set
      * @return    None.
      * @note      None.
  ********************************************************************/
  T_UINT8
  C_OLSR::find_degree(T_N1 one_hop_neighbor, const std::vector<T_N1>& n1_neighbor_graph,
                      const std::vector<T_N2>& n_neighbor_graph)
  {
    T_UINT8 degree = 0;
    T_BOOL increment_degree = false;

    /* Loops through the 2-hop set */
    for(std::vector<T_N2>::const_iterator n_graph_iter = n_neighbor_graph.begin();
        n_graph_iter != n_neighbor_graph.end(); n_graph_iter++)
      {
        /* Compares if the given 1-Hop neighbor connects the current 2-Hop neighbor */
        if(one_hop_neighbor.one_hop_neighb_addr == n_graph_iter->one_hop_neighb_addr)
          {
            /* Checks if the metric value is present between this 2-Hop and 1-Hop neighbor */
#ifdef COMMENT_SECTION
            std::map<T_NODE_ADDRESS, T_MATRIX>::const_iterator one_hop_iter = one_hop_neighbor.matrix_set.find(n_graph_iter->two_hop_neighb_addr);
#else
            std::vector<T_MATRIX>::const_iterator one_hop_iter = std::find(one_hop_neighbor.matrix_set.begin(),
                                                                           one_hop_neighbor.matrix_set.end(), n_graph_iter->two_hop_neighb_addr);
#endif
            if(one_hop_iter != one_hop_neighbor.matrix_set.end())
              {
                /* Loops through the other 1-Hop neighbors */
                for(std::vector<T_N1>::const_iterator other_n1_graph_iter = n1_neighbor_graph.begin();
                                    other_n1_graph_iter != n1_neighbor_graph.end(); other_n1_graph_iter++)
                {
                  const T_N1& other_one_hop_neighbor = *other_n1_graph_iter;

                  /* Checks if the metric value is present between this 2-Hop and other 1-Hop neighbor */
#ifdef COMMENT_SECTION
                  std::map<T_NODE_ADDRESS, T_MATRIX>::const_iterator other_neighbor_iter = other_one_hop_neighbor.matrix_set.find(n_graph_iter->two_hop_neighb_addr);
#else
                  std::vector<T_MATRIX>::const_iterator other_neighbor_iter = std::find(other_one_hop_neighbor.matrix_set.begin(),
                                                                                        other_one_hop_neighbor.matrix_set.end(), n_graph_iter->two_hop_neighb_addr);
#endif
                  if(other_neighbor_iter != one_hop_neighbor.matrix_set.end())
                    {
                      /* Checks if the metric value of current 1-Hop neighbor is less than
                      * other 1-Hop neighbor. If true then increments its degree */
                      if(one_hop_iter->d < other_neighbor_iter->d)
                        {
                          increment_degree = true;
                        }
                      else
                        {
                          increment_degree = false;
                        }
                     }

                 }
                if(increment_degree == true)
                  {
                    degree++;
                    increment_degree = false;
                  }
                }
          }

      }
    return degree;

  }

  /********************************************************************
      * @function  cover_always_willing_neighbors
      * @brief     This function finds the 1-hop neighbors whose
      *            willingness is Always and adds it to mpr set,
      *            2-hop neighbors which are connected through the
      *            given 1-Hop neighbor and removes them
      *            from the remaining 2-Hop set
      * @param     [1] mpr_set - (Routing/Flooding Mpr Set)
      *            [2] p_neighbor_graph - Neighbor graph
      *            [3] remaining_two_hop_neighb - 2-Hop neighbor set
      *            [4] mpr_type - Routing/Flooding MPR
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::cover_always_willing_neighbors(MprSet& mpr_set, const T_NEIGHBOR_GRAPH& p_neighbor_graph,
                                          std::vector<T_N2>& remaining_two_hop_neighb, T_UINT8 mpr_type)
  {
    switch(mpr_type)
    {
      case M_ROUTING_MPR:
        /* If the type is Routing MPR type, then finds if the 1-hop neighbor's
         * Routing willingness is Always, if true then add it mpr set.
         * Removes the 2-hop neighbors which are covered by this neighbor
         * from remaining 2-hop neighbor set */
        for(std::vector<T_N1>::const_iterator n1_graph_iter = p_neighbor_graph.n1_set.begin();
                n1_graph_iter != p_neighbor_graph.n1_set.end(); n1_graph_iter++)
        {
          if(n1_graph_iter->neighb_will.fields.route_will == WILL_ALWAYS)
            {
              mpr_set.insert(n1_graph_iter->one_hop_neighb_addr);
              cover_two_hop_neighbors(n1_graph_iter->one_hop_neighb_addr, remaining_two_hop_neighb);

            }
        }
        break;

      case M_FLOODING_MPR:
        /* If the type is Routing MPR type, then finds if the 1-hop neighbor's
       * Flooding willingness is Always, if true then add it mpr set.
       * Removes the 2-hop neighbors which are covered by this neighbor
       * from remaining 2-hop neighbor set */
        for(std::vector<T_N1>::const_iterator n1_graph_iter = p_neighbor_graph.n1_set.begin();
                        n1_graph_iter != p_neighbor_graph.n1_set.end(); n1_graph_iter++)
        {
          if(n1_graph_iter->neighb_will.fields.flood_will == WILL_ALWAYS)
            {
              mpr_set.insert(n1_graph_iter->one_hop_neighb_addr);
              cover_two_hop_neighbors(n1_graph_iter->one_hop_neighb_addr, remaining_two_hop_neighb);

            }
        }
        break;

      default:
        break;

    }

  }

  /********************************************************************
      * @function  cover_always_willing_neighbors
      * @brief     This function finds the 2-hop neighbors which
      *            are connected only through 1 one_hop neighbor and
      *            removes them from the remaining 2-Hop set and adds such
      *            one_hop neighbor to mpr set
      * @param     [1] mpr_set - (Routing/Flooding Mpr Set)
      *            [2] remaining_two_hop_neighb - 2-Hop neighbor set
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::cover_isolated_neighbors(MprSet& mpr_set, std::vector<T_N2>& remaining_two_hop_neighb)
  {
    std::set<T_NODE_ADDRESS> covered_two_hop_nodes;
    for(std::vector<T_N2>::const_iterator two_hop_neighb_iter = remaining_two_hop_neighb.begin();
        two_hop_neighb_iter != remaining_two_hop_neighb.end(); two_hop_neighb_iter++)
      {
        T_BOOL isolated_node = true;
        for(std::vector<T_N2>::const_iterator other_two_hop_neighb_iter = remaining_two_hop_neighb.begin();
            other_two_hop_neighb_iter != remaining_two_hop_neighb.end(); other_two_hop_neighb_iter++)
          {
            /* Checks whether 2-hop neighbors are connected through multiple 1-hop neighbors.
             * If False, then cover such 2-Hop neighbors, else skip */
            if((other_two_hop_neighb_iter->two_hop_neighb_addr == two_hop_neighb_iter->two_hop_neighb_addr) and
                (other_two_hop_neighb_iter->one_hop_neighb_addr != two_hop_neighb_iter->one_hop_neighb_addr))
              {
                isolated_node = false;
                break;
              }
          }
        /* If 2-hop neighbors are connected through a single 1-hop neighbor,
         * then add that 1-hop neighbor to mpr set and add 2-hop neighbor to covered set */
        if(isolated_node == true)
          {
            mpr_set.insert(two_hop_neighb_iter->one_hop_neighb_addr);
            covered_two_hop_nodes.insert(two_hop_neighb_iter->two_hop_neighb_addr);
          }
      }

    /* Finds the 2-hop neighbors which are covered above and
     * removes them from remaining 2-Hop set */
    for(std::vector<T_N2>::const_iterator two_hop_neighb_iter = remaining_two_hop_neighb.begin();
            two_hop_neighb_iter != remaining_two_hop_neighb.end();)
      {
        if(covered_two_hop_nodes.find(two_hop_neighb_iter->two_hop_neighb_addr) != covered_two_hop_nodes.end())
          {
            two_hop_neighb_iter = remaining_two_hop_neighb.erase(two_hop_neighb_iter);
          }
        else
          {
            two_hop_neighb_iter++;
          }
      }

  }

  /********************************************************************
      * @function  cover_two_hop_neghbors
      * @brief     This function finds the 2-hop neighbors which are
      *            connected through the given 1-Hop neighbor and
      *            removes them from the remaining 2-Hop set
      * @param     [1] one_hop_neighb - N1-Hop neighbor
      *            [2] remaining_two_hop_neighb - 2-Hop neighbor  set
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::cover_two_hop_neighbors(const T_NODE_ADDRESS& one_hop_neighb, std::vector<T_N2>& remaining_two_hop_neighb)
  {
    std::set<T_NODE_ADDRESS> two_hop_set_to_be_removed;
    for(std::vector<T_N2>::const_iterator two_hop_iter = remaining_two_hop_neighb.begin();
            two_hop_iter != remaining_two_hop_neighb.end(); two_hop_iter++)
      {
        if(two_hop_iter->one_hop_neighb_addr == one_hop_neighb)
          {
            two_hop_set_to_be_removed.insert(two_hop_iter->two_hop_neighb_addr);
          }
      }

    for(std::vector<T_N2>::iterator two_hop_iter = remaining_two_hop_neighb.begin();
        two_hop_iter != remaining_two_hop_neighb.end();)
      {
        if(two_hop_set_to_be_removed.find(two_hop_iter->two_hop_neighb_addr) != two_hop_set_to_be_removed.end())
          {
            two_hop_iter = remaining_two_hop_neighb.erase(two_hop_iter);
          }
        else
          {
            two_hop_iter++;
          }

      }

  }

  /********************************************************************
      * @function  calculate_mprs
      * @brief     This function calculates the MPRs of the node
      * @param	   [1] p_neighbor_graph - Neighbor graph
      *            [2] mpr_type - Routing/Flooding MPR
      * @return    None.
      * @note      None.
  ********************************************************************/

  void C_OLSR::calculate_mprs(T_NEIGHBOR_GRAPH &p_neighbor_graph, T_UINT8 mpr_type)
  {
    std::vector<T_N2> n_set;
    MprSet mpr_set;

    create_n_neighbor_graph(p_neighbor_graph, n_set);

    std::vector<T_N2> remaining_two_hop_neighb =  n_set;

    /* Adds the 1-hop neighbors to mpr set whose willingness is Always */
    cover_always_willing_neighbors(mpr_set, p_neighbor_graph, remaining_two_hop_neighb, mpr_type);

    /* Adds the 1-hop neighbors to mpr set which have the connectivity to
     * Isolated 2-Hop neighbors */
    cover_isolated_neighbors(mpr_set, remaining_two_hop_neighb);

    while(remaining_two_hop_neighb.begin() != remaining_two_hop_neighb.end())
      {
        std::set<T_UINT8> reachability_count;
        std::map<T_UINT8, std::vector<const T_N1*>> reachability_map;

        /* Finds the remaining number of 2-hop neighbors which are connected by each 1-hop neighbor*/
        find_reachability(p_neighbor_graph.n1_set, remaining_two_hop_neighb, reachability_count, reachability_map);

        T_N1 const *max_neighbor = NULL;
        T_UINT8 max_reachability = 0;
        T_UINT8 max_willingness = 0;

        for(std::set<T_UINT8>::iterator rs_iter = reachability_count.begin();
            rs_iter != reachability_count.end(); rs_iter++)
          {
            T_UINT8 r_count = *rs_iter;

            /* If the reachability count of neighbor is zero then skips,
             * otherwise proceeds*/
            if(r_count != 0)
              {
                for(std::vector<const T_N1*>::iterator rs_map_iter = reachability_map[r_count].begin();
                    rs_map_iter != reachability_map[r_count].end(); rs_map_iter++)
                  {
                    const T_N1* n1_graph_tuple = *rs_map_iter;
                    T_UINT8 n1_graph_willingness = 0;

                    if(mpr_type == M_ROUTING_MPR)
                      {
                        max_willingness = max_neighbor->neighb_will.fields.route_will;
                        n1_graph_willingness = n1_graph_tuple->neighb_will.fields.route_will;
                      }
                    else
                      {
                        max_willingness = max_neighbor->neighb_will.fields.flood_will;
                        n1_graph_willingness = n1_graph_tuple->neighb_will.fields.flood_will;
                      }
                    /* Comapares 2 1-hop neighbors as follows:
                     * Neighbor having highest willingness will be added to mpr set.
                     * If willingness is same then compares reachability of these two neighbors,
                     * Maximum reachable neighbor will be added to mpr set,
                     * If reachability is same then compares Degree(Total number of 2-hop
                     * neighbor can be connected) of these two neighbors,
                     * The neighbor with maximum Degree will be addded to mpr set*/
                    if((max_neighbor == NULL) || (n1_graph_willingness > max_willingness))
                      {
                        max_neighbor = n1_graph_tuple;
                        max_reachability = r_count;
                      }
                    else if(n1_graph_willingness == max_willingness)
                      {
                        if(r_count > max_reachability)
                          {
                            max_neighbor = n1_graph_tuple;
                            max_reachability = r_count;

                          }
                        else if(r_count == max_reachability)
                          {
                            if(find_degree(*n1_graph_tuple, p_neighbor_graph.n1_set, n_set) <
                                find_degree(*max_neighbor, p_neighbor_graph.n1_set, n_set))
                              {
                                max_neighbor = n1_graph_tuple;
                                max_reachability = r_count;
                              }
                          }

                      }

                  }

              }

          }
        /* If a neighbor with maximum coverage is obtained then,
         * adds it to mpr set and removes the covered 2-hop neighbors
         * from remaining 2-Hop set */
        if(max_neighbor != NULL)
          {
            mpr_set.insert(max_neighbor->one_hop_neighb_addr);
            cover_two_hop_neighbors(max_neighbor->one_hop_neighb_addr, remaining_two_hop_neighb);
          }


      }

    /* Calls the below function to update respective neighbors as MPRs*/
    m_state.populate_mpr_set(mpr_set, mpr_type);

  }

  /********************************************************************
      * @function  create_network_topology_graph
      * @brief     This function creates the Network Topology Graph
      * @param	   [1] p_network_topology_graph - Network Topology graph
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::create_network_topology_graph(T_NETWORK_TOPOLOGY_GRAPH* p_network_topology_graph)
  {
	/* Loops through 1-Hop neighbor set*/
    for(NeighbourSet::const_iterator neighb_set_iter = m_state.get_one_hop_neighbors().begin();
            neighb_set_iter != m_state.get_one_hop_neighbors().end(); neighb_set_iter++)
    {
      /* If the neighbor is symmetric one and his out_link metric is other than Unknown value then
       * adds this neighbor to Network Topology Graph*/
      if((neighb_set_iter->n_symmetric == true) and (neighb_set_iter->n_out_metric != M_UNKNOWN_VALUE))
        {
          T_NETWORK_LINK neighb_network_link;

          neighb_network_link.src_addr = m_node_address;
          neighb_network_link.dest_addr = neighb_set_iter->n_neighbor_addr;
          neighb_network_link.out_metric = neighb_set_iter->n_out_metric;

          p_network_topology_graph->one_hop_set.push_back(neighb_network_link);

          //cout << "Added neighbor graph" << endl;
          //std::cout << (int)neighb_network_link.dest_addr.net_id << "." << (int)neighb_network_link.dest_addr.node_id << std::endl;
        }

    }
  /* Loops through Router Topology set and add to the Network Topology graph*/
  for(RouterTopologySet::const_iterator topology_set_iter = m_state.get_router_topology_set().begin();
      topology_set_iter != m_state.get_router_topology_set().end(); topology_set_iter++)
    {
      T_NETWORK_LINK topology_network_link;

      topology_network_link.src_addr = topology_set_iter->tr_from_orig_addr;
      topology_network_link.dest_addr = topology_set_iter->tr_to_orig_addr;
      topology_network_link.out_metric = topology_set_iter->tr_metric;

      p_network_topology_graph->router_topology_set.push_back(topology_network_link);

      //cout << "Added Router Topology graph" << endl;
      //std::cout << (int)topology_network_link.dest_addr.net_id << "." << (int)topology_network_link.dest_addr.node_id << std::endl;

    }

  }

#ifdef COMMENT_SECTION
  /********************************************************************
      * @function  calculate_routing_table
      * @brief     This function calculates the routing table
      * @param	   [1] p_network_topology_graph - Network Topology Graph
      * @return    None.
      * @note      Its a map based implementation.
  ********************************************************************/
  void C_OLSR::calculate_routing_table(const T_NETWORK_TOPOLOGY_GRAPH& p_network_topology_graph)
  {
    /* Erases the complete Routing Table */
    m_state.erase_routing_table();
    T_UINT8 hop_count = M_ONE;

    /* Adds all the symmetric 1-hop neighbors to Routing Table */
    for(std::vector<T_NETWORK_LINK>::const_iterator one_hop_neigh_iter = p_network_topology_graph.one_hop_set.begin();
        one_hop_neigh_iter != p_network_topology_graph.one_hop_set.end(); one_hop_neigh_iter++)
      {
        T_NETWORK_LINK const & one_hop_graph_tuple = *one_hop_neigh_iter;
        m_state.insert_routing_tuple(one_hop_graph_tuple, one_hop_neigh_iter->dest_addr, hop_count);

      }

    T_BOOL new_entry_added = false;
    /* Loops through Routing table and finds the routes to all the destinations present in Router Topology table */
    for(std::map<T_NODE_ADDRESS, T_ROUTING_TABLE_ENTRY>::iterator routing_table_iter = m_state.get_routing_table().begin();
        routing_table_iter != m_state.get_routing_table().end();)
      {
        cout << "Next tuple for comparing is " << (int)routing_table_iter->first.net_id << "." << (int)routing_table_iter->first.node_id << endl;
        if(routing_table_iter->second.r_used == false)
          {
            if(routing_table_iter->first == routing_table_iter->second.r_dest_addr)
              {
            cout << "Inside main for loop" << endl;

            //set_routing_tables_used_field();
            routing_table_iter->second.r_used = true;


            for(std::vector<T_NETWORK_LINK>::const_iterator router_topo_iter = p_network_topology_graph.router_topology_set.begin();
                router_topo_iter != p_network_topology_graph.router_topology_set.end(); router_topo_iter++)
              {
                /* Checks whether Routing tuple's destination address is matching
                 * with Router topology tuple's source address */
                if(routing_table_iter->second.r_dest_addr == router_topo_iter->src_addr)
                  {
                    cout << "Comparing address =" << (int)routing_table_iter->second.r_dest_addr.net_id << "." << (int)routing_table_iter->second.r_dest_addr.node_id <<
                        " to the destination - " << (int)router_topo_iter->dest_addr.net_id << "." << (int)router_topo_iter->dest_addr.node_id<< endl;
                    /* Creates a new Network link and adds the following
                     * i) Set out link metric of the new tuple by adding the out link metric value of current Routing tuple and
                     * Out link  metric value of Router topology tuple
                     * ii) Set the new distance by adding 1 to distance value of current Routing tuple */

                    cout << " Router Topology metric + table metric = " << router_topo_iter->out_metric <<  "+" << routing_table_iter->second.r_metric <<endl;
                    T_NETWORK_LINK new_tuple;

                    new_tuple.out_metric = routing_table_iter->second.r_metric + router_topo_iter->out_metric;
                    hop_count = routing_table_iter->second.r_dist + M_ONE;

                    cout << "Initial metric  =" << (float)new_tuple.out_metric << "and Hop count" << (int)hop_count << endl;

                    /* Finds whether a Routing tuple for the destination is already present
                     * and its Out link metric is greater or equal to the new metric.
                     * If true then updates the found Routing tuple with
                     * next hop address = Next Hop address address of the current Routing Tuple,
                     * r_metric = New metric,
                     * r_dist = New Hop count */
                    T_ROUTING_TABLE_ENTRY * routing_tuple = m_state.find_routing_tuple(router_topo_iter->dest_addr, new_tuple.out_metric);

                    /* If the Routing tuple is not found then,
                     * Creates a new routing tuple with the following data,
                     * i) Destination address = Router Topology tuple's destination address,
                     * ii) next hop address = Next Hop address address of the current Routing Tuple,
                     * iii) r_metric = New metric,
                     * iv) r_dist = New Hop count */
                    if(routing_tuple == NULL)
                      {
                        cout << "Added new entry - Dest = " << (int)new_tuple.dest_addr.net_id << "." << (int)new_tuple.dest_addr.net_id << endl;
                        cout << "Next hop ="  << (int)new_tuple.src_addr.net_id << "." << (int)new_tuple.src_addr.net_id << endl;
                        new_tuple.dest_addr = router_topo_iter->dest_addr;
                        new_tuple.src_addr = m_node_address;
                        m_state.insert_routing_tuple(new_tuple, routing_table_iter->second.r_next_iface_addr, hop_count);
                        new_entry_added = true;
                        cout<< "New tuple added" << endl;
                      }
                    else
                      {
                        routing_tuple->r_next_iface_addr = routing_table_iter->second.r_next_iface_addr;
                        routing_tuple->r_metric =new_tuple.out_metric;
                        routing_tuple->r_dist = hop_count;
                        cout<< "Current tuple updated" << endl;

                      }

                  }
              }
              }
          }
        if(new_entry_added == true)
          {
            routing_table_iter = m_state.get_routing_table().begin();
            new_entry_added = false;
          }
        else
          {
            routing_table_iter++;
          }

      }

  }
#endif

  /********************************************************************
      * @function  calculate_routing_table
      * @brief     This function calculates the routing table
      * @param     [1] p_network_topology_graph - Network Topology Graph
      * @return    None.
      * @note      Its a vector based implementation.
  ********************************************************************/
#ifdef COMMENT_SECTION
  void C_OLSR::calculate_back_up_routing_table(const T_NETWORK_TOPOLOGY_GRAPH& p_network_topology_graph)
#else
  void C_OLSR::calculate_routing_table(const T_NETWORK_TOPOLOGY_GRAPH& p_network_topology_graph)
  {
#endif

    /* Erases the complete Routing Table */
#ifdef COMMENT_SECTION
    m_state.erase_back_up_routing_table();
#else
    m_state.erase_routing_table();
#endif

    T_UINT8 hop_count = M_ONE;

    /* Adds all the symmetric 1-hop neighbors to Routing Table */
    for(std::vector<T_NETWORK_LINK>::const_iterator one_hop_neigh_iter = p_network_topology_graph.one_hop_set.begin();
        one_hop_neigh_iter != p_network_topology_graph.one_hop_set.end(); one_hop_neigh_iter++)
      {
        T_NETWORK_LINK const & one_hop_graph_tuple = *one_hop_neigh_iter;
        m_state.insert_routing_tuple(one_hop_graph_tuple, one_hop_neigh_iter->dest_addr, hop_count);

      }


    /* Loops through Routing table and finds the routes to all the destinations present in Router Topology table */
#ifdef COMMENT_SECTION
    for(std::vector<T_ROUTING_TABLE_ENTRY>::iterator routing_table_iter = m_state.get_back_up_routing_table().begin();
        routing_table_iter != m_state.get_back_up_routing_table().end();)
#else
    for(std::vector<T_ROUTING_TABLE_ENTRY>::iterator routing_table_iter = m_state.get_routing_table().begin();
            routing_table_iter != m_state.get_routing_table().end();)
#endif

      {
        T_BOOL new_entry_added = false;
        //cout << "Next tuple for comparing is " << (int)routing_table_iter->first.net_id << "." << (int)routing_table_iter->first.node_id << endl;
        if(routing_table_iter->r_used == false)
          {
            cout << "Inside main for loop" << endl;

            //set_routing_tables_used_field();
            routing_table_iter->r_used = true;


            for(std::vector<T_NETWORK_LINK>::const_iterator router_topo_iter = p_network_topology_graph.router_topology_set.begin();
                router_topo_iter != p_network_topology_graph.router_topology_set.end(); router_topo_iter++)
              {
                /* Checks whether Routing tuple's destination address is matching
                 * with Router topology tuple's source address */
                if(routing_table_iter->r_dest_addr == router_topo_iter->src_addr)
                  {
                    cout << "Comparing address =" << (int)routing_table_iter->r_dest_addr.net_id << "." << (int)routing_table_iter->r_dest_addr.node_id <<
                        " to the destination - " << (int)router_topo_iter->dest_addr.net_id << "." << (int)router_topo_iter->dest_addr.node_id<< endl;
                    /* Creates a new Network link and adds the following
                     * i) Set out link metric of the new tuple by adding the out link metric value of current Routing tuple and
                     * Out link  metric value of Router topology tuple
                     * ii) Set the new distance by adding 1 to distance value of current Routing tuple */

                    cout << " Router Topology metric + table metric = " << router_topo_iter->out_metric <<  "+" << routing_table_iter->r_metric <<endl;
                    T_NETWORK_LINK new_tuple;

                    new_tuple.out_metric = routing_table_iter->r_metric + router_topo_iter->out_metric;
                    hop_count = routing_table_iter->r_dist + M_ONE;

                    cout << "Initial metric  =" << (float)new_tuple.out_metric << "and Hop count" << (int)hop_count << endl;

                    /* Finds whether a Routing tuple for the destination is already present
                     * and its Out link metric is greater or equal to the new metric.
                     * If true then updates the found Routing tuple with
                     * next hop address = Next Hop address address of the current Routing Tuple,
                     * r_metric = New metric,
                     * r_dist = New Hop count */
#ifdef COMMENT_SECTION
                    T_ROUTING_TABLE_ENTRY* routing_tuple = m_state.find_back_up_routing_tuple(router_topo_iter->dest_addr, new_tuple.out_metric);
#else
                    T_ROUTING_TABLE_ENTRY* routing_tuple = m_state.find_routing_tuple(router_topo_iter->dest_addr, new_tuple.out_metric);
#endif
                    /* If the Routing tuple is not found then,
                     * Creates a new routing tuple with the following data,
                     * i) Destination address = Router Topology tuple's destination address,
                     * ii) next hop address = Next Hop address address of the current Routing Tuple,
                     * iii) r_metric = New metric,
                     * iv) r_dist = New Hop count */
                    if(routing_tuple == NULL)
                      {
                        cout << "Added new entry - Dest = " << (int)new_tuple.dest_addr.net_id << "." << (int)new_tuple.dest_addr.net_id << endl;
                        cout << "Next hop ="  << (int)new_tuple.src_addr.net_id << "." << (int)new_tuple.src_addr.net_id << endl;
                        new_tuple.dest_addr = router_topo_iter->dest_addr;
                        new_tuple.src_addr = m_node_address;
                        m_state.insert_routing_tuple(new_tuple, routing_table_iter->r_next_iface_addr, hop_count);
#ifdef COMMENT_SECTION
                        m_state.insert_table_specifically(new_tuple, routing_table_iter->r_next_iface_addr, hop_count,routing_table_iter);
#endif
                        new_entry_added = true;
                        cout<< "New tuple added" << endl;
                      }
                    else
                      {
                        routing_tuple->r_next_iface_addr = routing_table_iter->r_next_iface_addr;
                        routing_tuple->r_metric = new_tuple.out_metric;
                        routing_tuple->r_dist = hop_count;
                        cout<< "Current tuple updated" << endl;

                      }

                  }

              }
          }
        if(new_entry_added != true)
          {
            routing_table_iter++;
          }
        else
          {
#ifdef COMMENT_SECTION
        	routing_table_iter = m_state.get_back_up_routing_table().begin() + M_ONE;
#else
        	routing_table_iter = m_state.get_routing_table().begin() + M_ONE;
            new_entry_added = false;
#endif
          }

      }

    m_state.print_routing_table();

  }

  /********************************************************************
      * @function  routing_table_computation
      * @brief     This function computes the routing table
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::routing_table_computation(void)
  {
    m_state.fill_tables_for_routing_comp();


    T_NETWORK_TOPOLOGY_GRAPH network_topology_graph;

    /* Calls create_network_topology_graph() function to
     * create the Network Topology Graph*/
    create_network_topology_graph(&network_topology_graph);

    /* Calls calculate_routing_table() function to calculate
     * the Routing Table*/
    calculate_routing_table(network_topology_graph);
#ifdef COMMENT_SECTION
    calculate_back_up_routing_table(network_topology_graph);
#endif
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
      * @brief     This function finds the routing MPRs of the node
      * @return    None.
      * @note      None.
  ********************************************************************/
  void C_OLSR::routing_mpr_computation(void)
  {

    T_NEIGHBOR_GRAPH routing_neighbor_graph;
    std::vector<T_ALLOWED_ONE_HOP_TUPLE> allowed_one_hop_set;

    std::vector<T_ALLOWED_TWO_HOP_TUPLE> allowed_two_hop_set;

    /* Calls create_allowed_one_hop_set() function to create allowed 1-Hop neighbor set*/
    create_allowed_one_hop_set(allowed_one_hop_set, M_ROUTING_MPR);
    /* Calls create_allowed_two_hop_set() function to create allowed 2-Hop neighbor set*/
    create_allowed_two_hop_set(allowed_two_hop_set, allowed_one_hop_set, M_ROUTING_MPR);
    /* Calls create_n1_neighbor_graph() function to create N1 neighbor graph*/
    create_n1_neighbor_graph(allowed_one_hop_set, &routing_neighbor_graph);
    /* Calls create_n2_neighbor_graph() function to create N2 neighbor graph*/
    create_n2_neighbor_graph(allowed_two_hop_set, &routing_neighbor_graph);

    /* Loops through N2 neighbor graph*/
    for(std::vector<T_N2>::iterator n2_iter = routing_neighbor_graph.n2_set.begin(); n2_iter != routing_neighbor_graph.n2_set.end(); n2_iter++)
      {
        T_UINT8 index;
        /* Checks if the allowed 2-Hop neighbor is also a 1-Hop neighbor of the node
         * If true then adds the In link_metric to N2 neighbor graph
         * otherwise, add Unknown value*/
        if(find_allowed_one_hop_address<T_ALLOWED_ONE_HOP_TUPLE>(n2_iter->two_hop_neighb_addr, allowed_one_hop_set, &index) == true)
        {
          n2_iter->d1 = allowed_one_hop_set.at(index).in_out_metric;
        }
        else
        {
          n2_iter->d1 = M_UNKNOWN_VALUE;
        }

      }

    /* Calls compute_total_metric() to compute total link_metric
     * between the node and its 2-Hop neighbor */
    compute_total_metric(allowed_two_hop_set, &routing_neighbor_graph);

    calculate_mprs(routing_neighbor_graph, M_ROUTING_MPR);

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

    /* Calls create_allowed_one_hop_set() function to create allowed link set*/
    create_allowed_one_hop_set(allowed_link_set, M_FLOODING_MPR);
    /* Calls create_allowed_two_hop_set() function to create allowed 2-Hop link set*/
    create_allowed_two_hop_set(allowed_two_hop_link_set, allowed_link_set, M_FLOODING_MPR);
    /* Calls create_n1_neighbor_graph() function to create N1 neighbor graph*/
    create_n1_neighbor_graph(allowed_link_set, &flooding_neighbor_graph);
    /* Calls create_n2_neighbor_graph() function to create N2 neighbor graph*/
    create_n2_neighbor_graph(allowed_two_hop_link_set, &flooding_neighbor_graph);

    /* Loops through N2 neighbor graph */
    for(std::vector<T_N2>::iterator n2_iter = flooding_neighbor_graph.n2_set.begin(); n2_iter != flooding_neighbor_graph.n2_set.end(); n2_iter++)
      {
        T_UINT8 index;
        /* Checks if the allowed 2-Hop neighbor is also a 1-Hop neighbor of the node
		 * If true then add the Out link_metric to N2 neighbor graph
		 * otherwise, add Unknown value*/
        if(find_allowed_one_hop_address<T_ALLOWED_ONE_HOP_TUPLE>(n2_iter->two_hop_neighb_addr, allowed_link_set, &index) == true)
        {
          n2_iter->d1 = allowed_two_hop_link_set.at(index).in_out_metric;
        }
        else
        {
          n2_iter->d1 = M_UNKNOWN_VALUE;
        }

      }

    /* Calls compute_total_metric() to compute total link_metric
     * between the node and its 2-Hop neighbor */
    compute_total_metric(allowed_two_hop_link_set, &flooding_neighbor_graph);


    calculate_mprs(flooding_neighbor_graph, M_FLOODING_MPR);

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
   Time get_ns()
   {
     Time now = M_ONE;
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

	//cout << "Size of olsr msg header class " << sizeof(ns_olsr2_0::C_MESSAGE_HEADER) << endl;

    //Normal_Node_Olsr.mpr_computation();
    //Normal_Node_Olsr.routing_table_computation();

	//Normal_Node_Olsr.send_hello();
	//Normal_Node_Olsr.send_tc();

	Normal_Node_Olsr.routing_table_computation();

	return M_ZERO;
}
