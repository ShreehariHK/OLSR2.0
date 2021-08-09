/*
 * olsr_recv_packet.cpp
 *
 *  Created on: 21-Jul-2021
 *      Author: accord
 */

#include <iostream>

#include "olsr.hpp"

using namespace std;

namespace ns_olsr2_0
{

  /********************************************************************
   * @function  recv_olsr
   * @brief     This function segregates received OLSR messages and
   *            calls for processing.
   * @param     [1] olsr_packet - Received OLSR packet
   * 		    [2] sender_address - Sender of this message
   * 		    [3] receiver_address - Receiver of this message
   * 		    [4] l_in_metric - Link In metric
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR::recv_olsr (const C_PACKET_HEADER olsr_packet, T_NODE_ADDRESS& sender_address,
                     T_NODE_ADDRESS& receiver_address, float l_in_metric)
  {
    /* TBD Deserializarion */

    /* If the receiver_address is equal to the current node's address then proceed */
    if (receiver_address == m_node_address)
      {
        C_MESSAGE_HEADER::OlsrMsgList olsr_msg_list;
        for (C_MESSAGE_HEADER::OlsrMsgList::const_iterator list_iter = olsr_msg_list.begin ();
            list_iter != olsr_msg_list.end (); list_iter++)
          {
            const C_MESSAGE_HEADER &message_header = *list_iter;

            /* Checks the validity of the received OLSR message
             * If not valid then skips */
            if (check_message_validity (message_header) == false)
              {
                continue;
              }

            /* Checks if the received OLSR message is already processed or not*/
            T_PROCESSED_MSG_TUPLE* processed_tuple = this->m_state.find_processed_msg_tuple (
                message_header.get_originator_address (), message_header.get_message_sequence_number ());

            /* If the received message is not yet processed then checks for message type */
            if (processed_tuple == NULL)
              {
                switch (message_header.get_message_type ())
                  {
                  /* If the message type is Hello then calls process_hello() function
                   * to process Hello message*/
                  case HELLO_MESSAGE:
                    this->process_hello (message_header, sender_address, l_in_metric);
                    break;
                    /* If the message type is TC then
                     * i) calls process_tc() function to process TC message
                     * ii) calls forward_default() to handle the message forwarding */
                  case TC_MESSAGE:
                    this->process_tc (message_header, sender_address);
                    forward_default (message_header, sender_address);
                    break;

                  default:
                    break;

                  }
                /* Creates a new processed message tuple if its not present */
                T_PROCESSED_MSG_TUPLE new_processed_msg_tuple = create_processed_msg_tuple (message_header);

                m_state.insert_processed_msg_tuple (new_processed_msg_tuple);

              }
            else
              {
                /* Updates the validity time of processed message tuple if its present already*/
                processed_tuple->p_time = get_ns () + message_header.get_validity_time ();

              }

          }

        /* Calls routing_table_computation() for computing the Routes to all destinations*/
        routing_table_computation ();

      }
  }

  /********************************************************************
   * @function  process_hello
   * @brief     This function processes OLSR hello message
   *            received from neighbor nodes.
   * @param     [1] msg_header - Received OLSR message
   * 			[2] sender_iface - 1-Hop sender of the node
   * 			[3] l_in_metric - Link in metric
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR::process_hello (const C_MESSAGE_HEADER& olsr_msg, const T_NODE_ADDRESS& sender_iface, float l_in_metric)
  {
    /* If the sender of this message is the same as the originator of this message then
     * proceed further. Return otherwise. */
    if (sender_iface == olsr_msg.get_originator_address ())
      {
        const C_MESSAGE_HEADER::T_HELLO &hello_msg = olsr_msg.get_hello ();

        /* Calls link_sensing() function to sense the link */
        link_sensing(olsr_msg, hello_msg, sender_iface, l_in_metric);

        /* Updates the 2-Hop set by calling populate_two_hop_set() function*/
        populate_two_hop_set (olsr_msg, hello_msg);

        /* Calls mpr_computation() function to compute Routing and Flooding MPRs of the node*/
        mpr_computation ();

      }

  }

  /********************************************************************
   * @function  link_sensing
   * @brief     This function senses link from neighbor and processes it
   * @param     [1] message_header - Received OLSR message
   * 			[2] hello_message - Hello message
   * 			[3] sender_iface_addr - Sender address of this message
   * 			[4] l_in_metric - Link in metric.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR::link_sensing (const C_MESSAGE_HEADER& message_header, const C_MESSAGE_HEADER::T_HELLO& hello_message,
                        const T_NODE_ADDRESS& sender_iface_addr, float l_in_metric)
  {

    /* Gets current time by calling get_cur_time() */
    Time cur_time = get_ns ();
    T_BOOL link_created = false, link_updated = false;

    /* Asserts the validity time of the message */
    OLSR_ASSERT (message_header.get_validity_time () > cur_time);

    /* Finds the link tuple for the given sender address*/
    T_LINK_TUPLE* link_tuple = m_state.find_link_tuple (sender_iface_addr);

    /* If link tuple is not present then creates a new link tuple for that sender */
    if (link_tuple == NULL)
      {
        T_LINK_TUPLE new_link_tuple = create_new_link_tuple (message_header, cur_time, l_in_metric);

        link_tuple = &m_state.insert_link_tuple (new_link_tuple);

        link_created = true;

      }
    else
      {
        link_updated = true;
      }
    link_tuple->l_heard_time = cur_time + message_header.get_validity_time ();

    for (std::vector<C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK>::const_iterator hello_msg_iter = hello_message.neighbor_set.begin ();
        hello_msg_iter != hello_message.neighbor_set.end (); hello_msg_iter++)
      {
        /* Checks the type of the instance and proceed if,
         * (the instance type is Normal and hello messages' neighbors' unique id is matching
         * with this node's Node Id) or
         * (the instance type is Leader and hello messages' neighbors' unique id is matching
         * with this node's Net Id)*/
        if (((this->get_instance_type () == NORMAL_NODE_INSTANCE) and (hello_msg_iter->unique_id == m_node_address.node_id))
            || ((this->get_instance_type () == LEADER_NODE_INSTANCE) and (hello_msg_iter->unique_id == m_node_address.net_id)))
          {

            /* Checks the validity of the neighbor link
             * If not valid then skips*/
            if (validate_neighbor_link_type (*hello_msg_iter) == false)
              {
                continue;
              }

            /* If the link between neighbor is Lost then sets the neighbor link to Lost*/
            if (hello_msg_iter->common_field.link_state.type_fields.link_type == M_LOST_LINK)
              {
                link_tuple->l_sym_time = cur_time - M_ONE;
                link_tuple->l_heard_time = cur_time - M_ONE;
                link_tuple->l_status = M_LOST;
                link_tuple->l_out_metric = M_UNKNOWN_VALUE;
                link_tuple->l_mpr_selector = false;
                link_updated = true;
              }
            /* If the link between neighbor is either Heard or Symmetric then
             * sets the neighbor link to Symmetric*/
            else if ((hello_msg_iter->common_field.link_state.type_fields.link_type == M_HEARD_LINK)
                or (hello_msg_iter->common_field.link_state.type_fields.link_type == M_SYMMETRIC_LINK))
              {
                /* Adds the current time to message validity time and adds it to link tuple*/
                link_tuple->l_sym_time = cur_time + message_header.get_validity_time ();
                /* Adds the link tuple's symmetric time to message validity time and adds it to the tuple*/
                link_tuple->l_time = link_tuple->l_sym_time + message_header.get_validity_time ();
                link_tuple->l_status = M_SYMMETRIC;
                link_tuple->l_out_metric = hello_msg_iter->metric[0];
                /* checks if the neighbor has selected this node as a Flooding MPR and
                 * update the field accordingly */
                if(((hello_msg_iter->common_field.link_state.type_fields.nbr_type & M_FLOODING_MPR) != M_ZERO) ?
                    (link_tuple->l_mpr_selector = true):(link_tuple->l_mpr_selector = false))

                link_updated = true;
              }
            break;

          }

      }

    /* If the link is updated then calls update_one_hop_neighbor_tuple() to
     * update the existing 1-hop neighbor corresponding to this link*/
    if (link_updated == true)
      {
        update_one_hop_neighbor_tuple(hello_message, link_tuple);
      }

    /* If the link is created then calls add_one_hop_neighbor_tuple() to
    * create and add a new 1-hop neighbor corresponding to this link*/
    if (link_created == true)
      {
        add_one_hop_neighbor_tuple (hello_message, link_tuple);
      }

  }

  /********************************************************************
   * @function  process_tc
   * @brief     Processes the TC message received from neighbor nodes.
   * @param     [1] msg_header - Received OLSR Message
   * 			[2] sender_iface - Sender address of this message
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR::process_tc (const C_MESSAGE_HEADER &msg, const T_NODE_ADDRESS &sender_interface)
  {
    const C_MESSAGE_HEADER::T_TC &tc_msg = msg.get_tc ();

    /* Checks whether the sender of this message is a symmetric one hop neighbor or not.
     * If not then it discards the message as this message is not received by 1-Hop neighbor*/
    T_LINK_TUPLE* link_tuple = m_state.find_sym_link_tuple (sender_interface);

    if (link_tuple == NULL)
      {
        return;
      }

    /* Checks whether the received TC messsage is an older one
     * If true, then discards the message
     * else, creates the new entry in advertising remote router set*/
    T_ADVERTISING_REMOTE_ROUTER_TUPLE *advtz_router_tuple = m_state.find_newer_advertising_router_tuple (
        msg.get_originator_address (), tc_msg.ansn);

    if (advtz_router_tuple != NULL)
      {
        return;
      }
    else
      {
        T_ADVERTISING_REMOTE_ROUTER_TUPLE new_advt_router_tuple;
        new_advt_router_tuple = create_advertizing_router_tuple (msg, tc_msg.ansn);
        m_state.insert_advertising_router_tuple (new_advt_router_tuple);

      }

    /* Removes all older entries in advertising remote router set w.r.t this
     * TC message originator*/
    m_state.erase_older_advertising_router_tuple (msg.get_originator_address (), tc_msg.ansn);

    for (std::vector<C_MESSAGE_HEADER::T_TC_ADDRESS_BLOCK>::const_iterator tc_msg_iter = tc_msg.tc_addr_set.begin ();
        tc_msg_iter != tc_msg.tc_addr_set.end (); tc_msg_iter++)
      {
        const C_MESSAGE_HEADER::T_TC_ADDRESS_BLOCK &tc_addr_tuple = *tc_msg_iter;
        E_ADDRESS_BLOCK_FLAGS common_address_field;
        T_UINT8 common_address_id;
        T_NODE_ADDRESS remote_router_address;

        /* Gets the common address information
         * i.e whether the address contain common Net ID or Node ID*/
        get_common_address (tc_addr_tuple, &common_address_field, &common_address_id);

        /* Checks if the TC message type */
        switch (tc_addr_tuple.tc_msg_type)
          {
          /* If the message type is ROUTABLE_ORIG
           * i.e originated from a Routing MPR to share local network info,
           * then process the following*/
          case ROUTABLE_ORIG:
                /* Checks common ID field in the address */
                switch (common_address_field)
                  {
                  /* If Net ID is common then,
                   * adds the common ID to net_id field
                   * and adds router's unique id to node id */
                  case NET_ID_COMMON:
                    remote_router_address.net_id = common_address_id;
                    populate_router_topology_set(msg, tc_addr_tuple, common_address_field, remote_router_address);
                    break;

                  /* If Node  ID is common then,
                   * adds the common ID to Ndde ID field
                   * and adds router's unique id to net id */
                  case NODE_ID_COMMON:
                    remote_router_address.node_id = common_address_id;
                    populate_router_topology_set(msg, tc_addr_tuple, common_address_field, remote_router_address);
                    break;

                  }

            break;

          /* If the TC message type is a ATTACHED
           * i.e originated from the leader nodes to share attached network info
           * then process the following */
          case ATTACHED:
            /* TBD - Left for future implementation */
            break;
          }
      }

  }

}