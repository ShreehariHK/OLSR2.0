/*               DEAL MANET Waveform Software Components
 *
 *
 * Source Code Name        : olsr_send_packet.cpp
 *
 * Source Code Part Number : MNTWSC-321-RI-0004
 *
 * Description             : Implements OLSR send message functionality
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
//#include <cstdint>
#include <cstring>
#include "olsr.hpp"

using namespace std;

namespace ns_olsr2_0
{

  /********************************************************************
      * @function  send_Queued_messages
      * @brief     This function sends the serialised message to Gateway.
      * @param
      * @return    None.
      * @note      None.
  ********************************************************************/
  void
  C_OLSR::send_queued_messages()
  {
    T_NODE_ADDRESS recv_neighb_addr = {10,10};
    T_NODE_ADDRESS send_neighb_addr = {10,15};
      T_OLSR_PACKET olsr_packet = {0};

      T_UINT16 size_occupied = 0;
      for(C_MESSAGE_HEADER::OlsrMsgList::iterator itr= olsr_msg_list.begin(); itr != olsr_msg_list.end();)
      {
          if(itr->get_message_type() == HELLO_MESSAGE)
          {
              cout << "Hello message serialisation begin" << endl;
              olsr_packet.packet_header.msg_type |= HELLO_MESSAGE;
              size_occupied += itr->serialize(olsr_packet.olsr_msg_buffer, size_occupied);
              olsr_packet.packet_header.hello_msg_size = size_occupied;

              itr = olsr_msg_list.erase(itr);
              cout << "Serialised Hello message size =" << olsr_packet.packet_header.hello_msg_size << endl;
              cout << "Hello message serialisation end" << endl;
              break;
          }
          else
          {
              itr++;
          }

      }

      for(C_MESSAGE_HEADER::OlsrMsgList::iterator itr= olsr_msg_list.begin(); itr != olsr_msg_list.end();)
      {
          if((itr->get_message_type() == TC_MESSAGE) and ((itr->get_msg_size() + 3) < (512 - size_occupied)))
          {
              cout << "Tc message serialisation begin" << endl;
              olsr_packet.packet_header.msg_type |= TC_MESSAGE;
              size_occupied += itr->serialize(olsr_packet.olsr_msg_buffer, size_occupied);
              olsr_packet.packet_header.tc_msg_size = size_occupied - olsr_packet.packet_header.hello_msg_size;

              itr = olsr_msg_list.erase(itr);
              cout << "Serialised TC message size =" << olsr_packet.packet_header.tc_msg_size << endl;
              cout << "TC message serialisation end" << endl;
              break;
          }
          else
          {
              itr++;
          }

      }

      for(C_MESSAGE_HEADER::OlsrMsgList::iterator itr= olsr_msg_list.begin(); itr != olsr_msg_list.end();)
      {
          if((itr->get_message_type() == TC_FORWARDED) and ((itr->get_msg_size() + 3) < (512 - size_occupied)))
          {
              cout << "Tc forwarded message serialisation begin" << endl;
              olsr_packet.packet_header.msg_type |= TC_FORWARDED;
              size_occupied += itr->serialize(olsr_packet.olsr_msg_buffer, size_occupied);
              olsr_packet.packet_header.tcf_msg_size = olsr_packet.packet_header.hello_msg_size + olsr_packet.packet_header.tc_msg_size;

              itr = olsr_msg_list.erase(itr);
              cout << "TC forwarded message serialisation end" << endl;
              break;
          }
          else
          {
              itr++;
          }

      }

      cout << "Size of buffer =" << size_occupied << endl;

      //transmit(olsr_packet, OLSR_MSG);
      float met = 0.9;
      recv_olsr((T_UINT8*)&olsr_packet, send_neighb_addr, recv_neighb_addr, met);
  }

/********************************************************************
    * @function  send_hello
    * @brief     This function prepares OLSR hello message
    *            and sends to the OLSR Tx buffer
    * @param     None.
    * @return    None.
    * @note      None.
   ********************************************************************/
   void C_OLSR::send_hello(void)
   {

	 /* Calls get_cur_time() function to get the current time */
     Time cur_time = get_ns();


     this->m_state.fill_tuples();

     C_MESSAGE_HEADER msg;

     /* Calls set_message_attributes() function to set generic attributes of Hello message */
     msg.set_message_attributes(get_node_addr(), E_OLSR_MSG_TYPE::HELLO_MESSAGE, get_next_msg_seq_num());

     /* Fetches Hello message structure by calling get_hello() */
     C_MESSAGE_HEADER::T_HELLO &hello_msg = msg.get_hello();

     hello_msg.node_willingness = m_willingness;

     /* Checks if the instance is not a leader instance
      * If true then adds the Network leader information to hello message */
     if(this->get_is_leader() != true)
     {
        T_LEADER_TUPLE* leader_tuple = this->m_state.get_leader_tuple();

        set_leader_info(hello_msg, leader_tuple);
        /* Calls set_common_address() and sets Net ID as common in Hello message*/
        this->set_common_address<C_MESSAGE_HEADER::T_HELLO>(hello_msg, E_ADDRESS_BLOCK_FLAGS::NET_ID_COMMON, m_node_address.net_id);

     }
     else
     {
    	 /* Calls set_common_address() and sets Node ID as common in Hello message*/
       this->set_common_address<C_MESSAGE_HEADER::T_HELLO>(hello_msg, E_ADDRESS_BLOCK_FLAGS::NODE_ID_COMMON, m_node_address.node_id);
     }

     /* Calls set_neighbor_info() function to add neighbor information to Hello message*/
     set_neighbor_info(hello_msg, cur_time);

     msg.set_message_length(msg.get_msg_size());

     cout << "size of total hello message = " << msg.get_message_length() << endl;

   olsr_msg_list.push_back(msg);

   send_queued_messages();



   }

   /********************************************************************
     * @function  send_tc
     * @brief     This function prepares OLSR tc message
     *            and sends to the OLSR Tx buffer
     * @param     None.
     * @return    None.
     * @note      None.
    ********************************************************************/
   void C_OLSR::send_tc(void)
   {
       //this->m_state.fill_tuples();

       C_MESSAGE_HEADER msg;

        /* Calls set_message_attributes() function to set generic attributes of Hello message */
       msg.set_message_attributes(this->get_node_addr(), E_OLSR_MSG_TYPE::TC_MESSAGE, this->get_next_msg_seq_num());

       C_MESSAGE_HEADER::T_TC &tc_msg = msg.get_tc();

       tc_msg.ansn = this->get_ansn();
       C_MESSAGE_HEADER::T_TC_ADDRESS_BLOCK tc_addr_block;

       if(this->get_is_leader() != true)
       {
           /* If the olsr instance is not a leader then,
            * sets the Tc message type as ROUTABLE_ORIG(Routing MPR sharing network topology info)
            * calls set_common_address() and sets Net ID as common in Tc message*/
          tc_addr_block.tc_msg_type = ROUTABLE_ORIG;

          this->set_common_address<C_MESSAGE_HEADER::T_TC_ADDRESS_BLOCK>(tc_addr_block, E_ADDRESS_BLOCK_FLAGS::NET_ID_COMMON, m_node_address.net_id);

       }
       else
       {
           /* If the olsr instance is a leader then,
            * sets the Tc message type as ROUTABLE_ORIG(Routing MPR sharing network topology info)
            * calls set_common_address() and sets Node ID as common in Tc message*/
           tc_addr_block.tc_msg_type = ROUTABLE_ORIG;

           this->set_common_address<C_MESSAGE_HEADER::T_TC_ADDRESS_BLOCK>(tc_addr_block, E_ADDRESS_BLOCK_FLAGS::NODE_ID_COMMON, m_node_address.node_id);

       }


       for (NeighbourSet::const_iterator mprsel_tuple = m_state.get_routing_mpr_selector_set().begin();
             mprsel_tuple != m_state.get_routing_mpr_selector_set().end(); mprsel_tuple++)
          {
           cout << "Router MPR finding begin" << endl;
           C_MESSAGE_HEADER::T_GENERIC_ADDR_BLOCK tc_net_info;

             /* If the olsr instance is not a leader then,
              * adds Routing MPR selector's Node ID as unique id.
              * otherwise adds Routing MPR selector's Net ID as unique Id*/
             if(this->get_is_leader() == false)
             {
                 tc_net_info.unique_id = mprsel_tuple->n_neighbor_addr.node_id;
             }
             else
             {
                 tc_net_info.unique_id = mprsel_tuple->n_neighbor_addr.net_id;
             }

             /* Adds the out metric to Tc net info */
             tc_net_info.metric[0] = encode_metric_value(mprsel_tuple->n_out_metric);

             tc_addr_block.network_info.push_back(tc_net_info);

           }
       tc_msg.tc_addr_block = tc_addr_block;

       msg.set_message_length(msg.get_msg_size());

       if(msg.get_message_length() > M_INSUFFICIENT_TC_MSG)
         {
           olsr_msg_list.push_back(msg);
           cout << "TC message is complete" << endl;
         }
       else
         {
           cout << "TC message is not complete" << endl;
         }



       send_queued_messages();


   }

   /********************************************************************
    * @function  forward_default
    * @brief     This function forwards the message if its not
    * 			 forwarded already
    * @param     [1] msg_header - OLSR TC message
    * 			 [2] p_sender_iface - 1-Hop sender of the node
    * @return    None.
    * @note      None.
    ********************************************************************/
   void C_OLSR::forward_default(C_MESSAGE_HEADER message, const T_NODE_ADDRESS& p_sender_iface)
   {

	 /* Checks if the Time to Live field of the OLSR TC message is greater than 1 */
     if(message.get_time_to_live() > M_ONE)
       {
    	 /* Checks if the 1-Hop sender is a flooding MPR selector of the node */
         if(m_state.check_is_flooding_mpr_selector(p_sender_iface) == true)
           {
             T_FORWARDED_MSG_TUPLE * forwrded_msg_tuple = m_state.find_forwarded_msg_tuple(message.get_originator_address(), message.get_message_sequence_number());

             /* Checks if the OSLR TC Message is not forwarded yet
              * If true then create a new forwarded message tuple and add it
              * to the forwarded message set*/
             if(forwrded_msg_tuple == NULL)
               {
                 /* The function shall create a new forwarded message tuple if not present*/
                 T_FORWARDED_MSG_TUPLE new_fwd_msg_tuple = create_forwarded_msg_tuple(message);

                 m_state.insert_forwarded_msg_tuple(new_fwd_msg_tuple);

                 message.set_time_to_live(message.get_time_to_live() - M_ONE);
                 message.set_hop_count(message.get_hop_count() + M_ONE);

                 /* The function shall push the message to the queue to be retransmitted*/
                 olsr_msg_list.push_back(message);

               }
             else
              {
                /* The function shall update the validity time of forwarded message tuple if present */
                forwrded_msg_tuple->f_time = get_ns() + message.get_validity_time();
              }
           }

       }
   }

}
