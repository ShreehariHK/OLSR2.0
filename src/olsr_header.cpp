/*                        Project title
 *
 *
 * Source Code Name   :   olsr_header.cpp
 *
 * Description        :   Implements OLSR Message and its header formation functionality .
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

#include "olsr_header.hpp"

namespace ns_olsr2_0
{

  /********************************************************************
   * @function  C_PACKET_HEADER
   * @brief     This function is the constructor of C_PACKET_HEADER.
   * @param     length.
   * @return    None.
   * @note       None.
  ********************************************************************/
  C_PACKET_HEADER::C_PACKET_HEADER ()
  {

  }

  /********************************************************************
   * @function  ~C_PACKET_HEADER
   * @brief     This function is the destructor of C_PACKET_HEADER.
   * @param     None.
   * @return    None.
   * @note      None.
  ********************************************************************/
  C_PACKET_HEADER::~C_PACKET_HEADER ()
  {

  }

  /********************************************************************
   * @function  set_packet_length
   * @brief     This function sets the packet length.
   * @param     length.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_PACKET_HEADER::set_packet_length (uint16_t length)
  {
        m_packet_length = length;
  }

  /********************************************************************
   * @function  get_packet_length
   * @brief     This function returns the packet length.
   * @param     None.
   * @return    m_packet_length.
   * @note      None.
  ********************************************************************/
  T_UINT16 C_PACKET_HEADER::get_packet_length () const
  {
      return m_packet_length;
  }

  /********************************************************************
   * @function  set_packet_type
   * @brief     This function sets the packet type.
   * @param     seqnum.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_PACKET_HEADER::set_packet_type (T_UINT16 seqnum)
  {
        m_packet_type = seqnum;
  }

  /********************************************************************
   * @function  get_packet_type
   * @brief     This function returns the packet type.
   * @param     None.
   * @return    m_packet_type.
   * @note      None.
  ********************************************************************/
  T_UINT16 C_PACKET_HEADER::get_packet_type () const
  {
      return m_packet_type;
  }

  /********************************************************************
   * @function  C_MESSAGE_HEADER
   * @brief     This function is the constructor of C_MESSAGE_HEADER.
   * @param     length.
   * @return    None.
   * @note      None.
  ********************************************************************/
  C_MESSAGE_HEADER::C_MESSAGE_HEADER ()
  {

  }

  /********************************************************************
   * @function  ~C_MESSAGE_HEADER
   * @brief     This function is the destructor of C_MESSAGE_HEADER.
   * @param     None.
   * @return    None.
   * @note      None.
  ********************************************************************/
  C_MESSAGE_HEADER::~C_MESSAGE_HEADER ()
  {

  }

  /********************************************************************
   * @function  set_message_type
   * @brief     This function sets the message type.
   * @param     message_type.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_MESSAGE_HEADER::set_message_type (E_OLSR_MSG_TYPE message_type)
  {
    m_message_type = message_type;
  }

  /********************************************************************
   * @function  get_message_type
   * @brief     This function returns the message type.
   * @param     None.
   * @return    m_message_type.
   * @note      None.
  ********************************************************************/
  E_OLSR_MSG_TYPE C_MESSAGE_HEADER::get_message_type () const
  {
    return m_message_type;
  }

  /********************************************************************
   * @function  set_validity_time
   * @brief     This function sets the validity time.
   * @param     time.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_MESSAGE_HEADER::set_validity_time(Time time)
  {
    #ifndef M_NS3_SIM
    m_validity_time = SecondsToEmf (time.GetSeconds ());
    #endif

    m_validity_time = time;
  }

  /********************************************************************
   * @function  get_validity_time
   * @brief     This function returns the validity time.
   * @param     None.
   * @return    m_validity_time.
   * @note      None.
  ********************************************************************/
  Time C_MESSAGE_HEADER::get_validity_time () const
  {
    #ifndef M_NS3_SIM
    return Seconds (EmfToSeconds (m_validity_time));
    #endif

    return m_validity_time;
  }

  /********************************************************************
   * @function  set_interval_time
   * @brief     This function sets the interval time.
   * @param     time.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_MESSAGE_HEADER::set_interval_time (Time time)
  {
    m_interval_time = time;
  }

  /********************************************************************
   * @function  get_interval_time
   * @brief     This function sets the interval time.
   * @param     None.
   * @return    m_interval_time.
   * @note      None.
  ********************************************************************/
  Time C_MESSAGE_HEADER::get_interval_time () const
  {
    return m_interval_time;
  }

  /********************************************************************
   * @function  set_originator_address
   * @brief     This function sets the originator address.
   * @param     originator_address.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_MESSAGE_HEADER::set_originator_address (T_NODE_ADDRESS originator_address)
  {
    m_originator_address = originator_address;
  }

  /********************************************************************
   * @function  get_originator_address
   * @brief     This function returns the originator address.
   * @param     None.
   * @return    m_originator_address.
   * @note      None.
  ********************************************************************/
  T_NODE_ADDRESS C_MESSAGE_HEADER::get_originator_address () const
  {
    return m_originator_address;
  }

  /********************************************************************
   * @function  set_time_to_live
   * @brief     This function sets the Time to Live.
   * @param     time_to_live.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_MESSAGE_HEADER::set_time_to_live (uint8_t time_to_live)
  {
    m_time_to_live = time_to_live;
  }

  /********************************************************************
   * @function  get_time_to_live
   * @brief     This function returns the Time to Live.
   * @param     None.
   * @return    m_time_to_live.
   * @note      None.
  ********************************************************************/
  uint8_t C_MESSAGE_HEADER::get_time_to_live () const
  {
    return m_time_to_live;
  }

  /********************************************************************
   * @function  set_hop_count
   * @brief     This function sets the hop count.
   * @param     hop_count.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_MESSAGE_HEADER::set_hop_count (uint8_t hop_count)
  {
    m_hop_count = hop_count;
  }

  /********************************************************************
   * @function  get_hop_count
   * @brief     This function returns the hop count.
   * @param     None.
   * @return    m_hop_count.
   * @note      None.
  ********************************************************************/
  uint8_t C_MESSAGE_HEADER::get_hop_count () const
  {
    return m_hop_count;
  }

  /********************************************************************
   * @function  set_message_sequence_number
   * @brief     This function sets the message sequence number.
   * @param     message_sequence_number.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_MESSAGE_HEADER::set_message_sequence_number (uint16_t message_sequence_number)
  {
    m_message_sequence_number = message_sequence_number;
  }

  /********************************************************************
   * @function  get_message_sequence_number
   * @brief     This function returns the message sequence number.
   * @param     None.
   * @return    m_message_sequence_number.
   * @note      None.
  ********************************************************************/
  T_UINT16 C_MESSAGE_HEADER::get_message_sequence_number () const
  {
    return m_message_sequence_number;
  }

  /********************************************************************
   * @function  set_message_length
   * @brief     This function sets the message length.
   * @param     message_length.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void C_MESSAGE_HEADER::set_message_length (uint16_t message_length)
  {
      m_msg_len = message_length;
  }

  /********************************************************************
   * @function  get_message_length
   * @brief     This function returns the message length.
   * @param     None.
   * @return    m_msg_len.
   * @note      None.
  ********************************************************************/
  uint16_t C_MESSAGE_HEADER::get_message_length () const
  {
    return m_msg_len;
  }

  /********************************************************************
   * @function  get_hello
   * @brief     Checks if the received message type is HELLO and
   *            returns the message content.
   * @param     None.
   * @return    hello message.
   * @note      None.
  ********************************************************************/
  const C_MESSAGE_HEADER::T_HELLO& C_MESSAGE_HEADER::get_hello (void) const
  {
	  /* Checks if the received message is Hello.
	   * If true then returns the hello message*/
    if(m_message_type == HELLO_MESSAGE)
      {
        return m_message.hello;
      }
  }

  /********************************************************************
   * @function  get_tc
   * @brief     Checks if the message type is TC and
   *            returns the message content.
   * @param     None.
   * @return    tc message.
   * @note      None.
  ********************************************************************/
  const C_MESSAGE_HEADER::T_TC& C_MESSAGE_HEADER::get_tc() const
  {
   /* Checks if the received message is Tc.
    * If true then returns the Tc message*/
   if (m_message_type == TC_MESSAGE)
   {
     return m_message.tc;
   }

  }

  /********************************************************************
   * @function  get_hello
   * @brief     Returns the reference of Hello message.
   * @param     None.
   * @return    Reference of hello message.
   * @note      None.
  ********************************************************************/
  C_MESSAGE_HEADER::T_HELLO& C_MESSAGE_HEADER::get_hello (void)
  {
	/*Checks if the message type is Hello.
	 * If true then returns the hello message*/
    if (m_message_type == M_ZERO)
    {
        m_message_type = HELLO_MESSAGE;
    }
    else
    {
        OLSR_ASSERT(m_message_type == HELLO_MESSAGE);
    }
    return m_message.hello;
  }

  /********************************************************************
   * @function  get_tc
   * @brief     Returns the reference of Tc message.
   * @param     None.
   * @return    Reference of hello message.
   * @note      None.
  ********************************************************************/
  C_MESSAGE_HEADER::T_TC& C_MESSAGE_HEADER::get_tc()
  {
   /*Checks if the message type is Tc.
	* If true then returns the Tc message*/
   if (m_message_type == M_ZERO)
   {
       m_message_type = TC_MESSAGE;
   }
   else
   {
       OLSR_ASSERT(m_message_type == TC_MESSAGE);
   }
   return m_message.tc;

  }

  /********************************************************************
   * @function  set_message_attributes
   * @brief     Set the following common message attributes
   * 			i) Msg type, ii) Msg originator address, iii) Msg TTL,
   * 			iv) Msg Hop-count, v) Msg sequence number,
   * 			vi) Msg validity time, vii) Msg interval time,\.
   * @param     [1] node_address - Address of the node.
   * 			[2] msg_type - Type of the message.
   * 			[3] msg_seq_num - Message sequence number.
   * @return    None.
   * @note      None.
  ********************************************************************/

  void C_MESSAGE_HEADER::set_message_attributes(T_NODE_ADDRESS node_address, E_OLSR_MSG_TYPE msg_type, T_UINT16 msg_seq_num)
  {
    this->set_message_type(msg_type);

    this->set_originator_address(node_address);

    /* If the message type is Hello, then
     * Set Time to Live as 1,
     * 40 otherwise */
    if(msg_type == E_OLSR_MSG_TYPE::HELLO_MESSAGE)
      {
        this->set_time_to_live(M_ONE);
        this->set_validity_time(M_HELLO_MSG_VALID__TIME);
        this->set_interval_time(M_HELLO_INTERVAL);

      }
    else
      {
        this->set_time_to_live(40);
        this->set_validity_time(M_TC_MSG_VALID_TIME);
        this->set_interval_time(M_TC_INTERVAL);
      }

    this->set_hop_count(M_ZERO);
    set_message_sequence_number(msg_seq_num);





  }

  /********************************************************************
   * @function  get_hello_msg_size
   * @brief     Finds the message hello size and returns.
   * @param     None.
   * @return    Hello message size.
   * @note      None.
  ********************************************************************/
  T_UINT16 C_MESSAGE_HEADER::T_HELLO::get_hello_msg_size()
    {

      T_UINT16 size = this->neighbor_set.size();
      return (size * 6);

    }

  /********************************************************************
   * @function  get_tc_msg_size
   * @brief     Finds the tc message size and returns.
   * @param     None.
   * @return    Tc message size.
   * @note      None.
  ********************************************************************/
  T_UINT16 C_MESSAGE_HEADER::T_TC::get_tc_msg_size()
    {
      T_UINT16 size = M_ZERO;

      for(std::vector<T_TC_ADDRESS_BLOCK>::const_iterator it = this->tc_addr_set.begin(); it != tc_addr_set.end(); it++)
        {
          size += ((it->network_info.size() * 6) + 3);
        }

      return size;

    }

  /********************************************************************
   * @function  get_msg_size
   * @brief     Finds the message OLSR message size and returns.
   * @param     None.
   * @return    size.
   * @note      None.
  ********************************************************************/
  T_UINT16 C_MESSAGE_HEADER::get_msg_size()
  {
    T_UINT16 size = M_ZERO;

    /* Checks the type of the message */
    switch(m_message_type)
    {
      /* If its a Hello message then adds hello message size with Hello message header length*/
      case HELLO_MESSAGE:
        size += (this->m_message.hello.get_hello_msg_size() + M_HELLO_MSG_HEADER_LEN);
        break;

       /* If its a Hello message then adds Tc message size with Tc message header length*/
      case TC_MESSAGE:
        size += (this->m_message.tc.get_tc_msg_size() + M_TC_MSG_HEADER_LEN);
        break;

      default:
        break;
    }

    return size;
  }



}
