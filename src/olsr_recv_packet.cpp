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
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR::recv_olsr (const C_PACKET_HEADER olsr_packet, T_NODE_ADDRESS& sender_address,
                     T_NODE_ADDRESS& receiver_address, float l_in_metric)
  {
    /* TBD Deserializarion */

    if (receiver_address == m_node_address)
      {
        const C_MESSAGE_HEADER::OlsrMsgList olsr_msg_list;
        for (C_MESSAGE_HEADER::OlsrMsgList::const_iterator list_iter = olsr_msg_list.begin ();
            list_iter != olsr_msg_list.end (); list_iter++)
          {
            const C_MESSAGE_HEADER &message_header = *list_iter;

            if(check_message_validity(message_header) == true)
              {
                continue;
              }

            T_PROCESSED_MSG_TUPLE* processed_tuple = this->m_state.find_processed_msg_tuple (
                message_header.get_originator_address (), message_header.get_message_sequence_number ());

            //T_FORWARDED_MSG_TUPLE *forwarded_tuple = this->m_state.find_forwarded_msg_tuple (
            //message_header.get_originator_address (), message_header.get_message_sequence_number ());

            if (processed_tuple == NULL)
              {
                switch (message_header.get_message_type ())
                  {
                  case HELLO_MESSAGE:
                    this->process_hello (message_header, sender_address, l_in_metric);
                    break;

                  case TC_MESSAGE:
                    this->process_tc (message_header, sender_address);
                    forward_default (message_header);
                    break;

                  default:
                    break;

                  }
                T_PROCESSED_MSG_TUPLE new_processed_msg_tuple = create_processed_msg_tuple(message_header);

                m_state.insert_processed_msg_tuple(new_processed_msg_tuple);

              }
            else
            {
                processed_tuple->p_time = get_cur_time() + message_header.get_validity_time();

            }

          }

        routing_table_computation ();

      }
  }

  /********************************************************************
   * @function  process_hello
   * @brief     This function processes OLSR hello message
   *            received from neighbor nodes.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR::process_hello (const C_MESSAGE_HEADER& msg_header, const T_NODE_ADDRESS& sender_iface, float l_in_metric)
  {
    if (sender_iface == msg_header.get_originator_address ())
      {
        const C_MESSAGE_HEADER::T_HELLO &hello_msg = msg_header.get_hello ();

        link_sensing (msg_header, hello_msg, sender_iface, l_in_metric);

        populate_two_hop_set();

        mpr_computation();

      }

  }

  /********************************************************************
   * @function  link_sensing
   * @brief     This function senses link from neighbor and processes it
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR::link_sensing (const C_MESSAGE_HEADER& message_header, const C_MESSAGE_HEADER::T_HELLO& hello_message,
                        const T_NODE_ADDRESS& sender_iface_addr, float l_in_metric)
  {

    Time cur_time = get_cur_time ();
    T_BOOL link_created = false, link_updated = false;

    OLSR_ASSERT (message_header.get_validity_time () > cur_time);

    T_LINK_TUPLE* link_tuple = m_state.find_link_tuple (sender_iface_addr);

    if (link_tuple == NULL)
      {
        T_LINK_TUPLE new_link_tuple = create_new_link_tuple(message_header, cur_time, l_in_metric);

        link_tuple = &m_state.insert_link_tuple (new_link_tuple);

        link_created = true;

      }
    else
      {
        link_updated = true;
      }
    link_tuple->l_heard_time = cur_time + message_header.get_validity_time ();

    for (std::vector<C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK>::const_iterator iter = hello_message.neighbor_set.begin ();
        iter != hello_message.neighbor_set.end (); iter++)
      {
        if (((m_instance == NORMAL_NODE_INSTANCE) and (iter->unique_id == m_node_address.node_id))
            || ((m_instance == LEADER_NODE_INSTANCE) and (iter->unique_id == m_node_address.net_id)))
          {
            if(validate_neighbor_link_type(*iter) == true)
              {
                continue;
              }

            if (iter->common_field.link_state.type_fields.link_type == LOST_LINK)
              {
                link_tuple->l_sym_time = cur_time - 1;
                link_tuple->l_heard_time = cur_time - 1;
                link_tuple->l_status = LOST_LINK;
                link_updated = true;
              }
            else if ((iter->common_field.link_state.type_fields.link_type == HEARD_LINK)
                or (iter->common_field.link_state.type_fields.link_type == SYMMETRIC_LINK))
              {
                link_tuple->l_sym_time = cur_time + message_header.get_validity_time ();
                link_tuple->l_time = link_tuple->l_sym_time + message_header.get_validity_time ();
                link_tuple->l_status = SYMMETRIC_LINK;
                link_updated = true;
              }
            break;

          }

      }

    if(link_updated == true)
        {
           ;/* TBD - Update neighbor tuple */
        }
    if(link_created == true)
      {
        ; /* TBD - Create neighbor tuple */
      }

  }

  /********************************************************************
   * @function  process_tc
   *            received from neighbor nodes.
   * @param     None.
   * @return    None.
   * @note      None.
   ********************************************************************/
  void
  C_OLSR::process_tc (const C_MESSAGE_HEADER& msg_header, const T_NODE_ADDRESS& sender_iface)
  {
    ;
  }

}
