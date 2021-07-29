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

     Time cur_time = get_cur_time();

     this->m_state.fill_tuples();

     C_MESSAGE_HEADER msg;

     msg.set_message_attributes(get_node_addr(), E_OLSR_MSG_TYPE::HELLO_MESSAGE, get_next_msg_seq_num());

     C_MESSAGE_HEADER::T_HELLO &hello_msg = msg.get_hello();

     hello_msg.node_willingness = m_willingness;

     if(!m_is_leader)
     {
        T_LEADER_TUPLE* leader_tuple = this->m_state.get_leader_tuple();

        set_leader_info(hello_msg, leader_tuple);

        set_common_address(hello_msg, E_ADDRESS_BLOCK_FLAGS::NET_ID_COMMON, m_node_address.net_id);

     }
     else
     {
       set_common_address(hello_msg,E_ADDRESS_BLOCK_FLAGS::NODE_ID_COMMON, m_node_address.node_id);
     }

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
    * @function  forward_default
    * @brief     This function finds a forwarded message tuple and returns
    * @param     msg_header.
    * @return    leader_tuple.
    * @note      None.
    ********************************************************************/
   void C_OLSR::forward_default(const C_MESSAGE_HEADER& msg_header)
   {

     T_FORWARDED_MSG_TUPLE * forwrded_msg_tuple = m_state.find_forwarded_msg_tuple(msg_header.get_originator_address(), msg_header.get_message_sequence_number());

     if(forwrded_msg_tuple == NULL)
       {
         /* The function shall create a new forwarded message tuple if not present*/
         T_FORWARDED_MSG_TUPLE new_fwd_msg_tuple = create_forwarded_msg_tuple(msg_header);

         m_state.insert_forwarded_msg_tuple(new_fwd_msg_tuple);

         /* The function shall push the message to the queue to be retransmitted*/
         olsr_msg_list.push_back(msg_header);
       }
     else
       {
         /* The function shall update the validity time of forwarded message tuple if present */
         forwrded_msg_tuple->f_time == get_cur_time() + msg_header.get_validity_time();
       }
   }


}
