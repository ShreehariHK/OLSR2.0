/*
 * olsr_send_packet.cpp
 *
 *  Created on: 21-Jul-2021
 *      Author: accord
 */
#include <iostream>
//#include <cstdint>
#include <cstring>
#include "olsr.hpp"

using namespace std;

namespace ns_olsr2_0
{


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

   olsr_msg_list.push_back(msg);

   unsigned char list[255];


   std::memcpy(list, &msg, sizeof(C_MESSAGE_HEADER));

   C_MESSAGE_HEADER recv_msg;

   std::memcpy(&recv_msg, list, sizeof(C_MESSAGE_HEADER));

   cout << "received message type " << recv_msg.get_message_type() << endl;

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



       for (NeighbourSet::const_iterator mprsel_tuple = m_state.get_routing_mpr_selector_set().begin ();
             mprsel_tuple != m_state.get_routing_mpr_selector_set().end (); mprsel_tuple++)
          {
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
             tc_net_info.metric[0] = mprsel_tuple->n_out_metric;

             tc_addr_block.network_info.push_back(tc_net_info);

           }
       tc_msg.tc_addr_set.push_back(tc_addr_block);

       msg.set_message_length(msg.get_msg_size());

       olsr_msg_list.push_back(msg);


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
   void C_OLSR::forward_default(C_MESSAGE_HEADER message, T_NODE_ADDRESS& p_sender_iface)
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
