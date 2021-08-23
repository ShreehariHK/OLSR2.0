/*               DEAL MANET Waveform Software Components
 *
 *
 * Source Code Name        : olsr_header.cpp
 *
 * Source Code Part Number : MNTWSC-321-RI-0004
 *
 * Description             : To store OLSR message with header.
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

#include "olsr_header.hpp"

namespace ns_olsr2_0
{
  /********************************************************************
   * @function  write_u16
   * @brief     This function Adds the 2 bytes data to given buffer.
   * @param     [1] data = 2 bytes data.
   *            [2] ptr - pointer to buffer
   * @return    None.
   * @note      None.
  ********************************************************************/
  void
  write_u16 (T_UINT16 data, T_UINT8* ptr)
  {
    std::cout << "data = " << data << std::endl;

    *ptr++ = ((data >> 8) & 0xff);

    *ptr++ = (data & 0xff);
  }

  /********************************************************************
   * @function  read_u16
   * @brief     This function Adds the 2 bytes to data read from buffer.
   * @param     [1] ptr - pointer to buffer.
   *            [2] data = 2 bytes data.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void
  read_u16(T_UINT8* ptr, T_UINT16& data)
  {
    data = ((*ptr++ & 0xff) << 8);
    data |= (*ptr++ & 0xff);

    //std::cout << "data = " << data << std::endl;
  }
#ifdef COMMENT_SECTION
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
#endif

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
   if ((m_message_type == TC_MESSAGE) or (m_message_type == TC_FORWARDED))
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
        this->set_time_to_live(15);
        this->set_validity_time(M_HELLO_MSG_VALID__TIME);
        this->set_interval_time(M_HELLO_INTERVAL);

      }
    else
      {
        this->set_time_to_live(40);
        this->set_validity_time(M_TC_MSG_VALID_TIME);
        this->set_interval_time(M_TC_INTERVAL);
      }

    this->set_hop_count(25);
    set_message_sequence_number(msg_seq_num);

    std::cout << "Set m_message_sequence_number = " << msg_seq_num << std::endl;

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
      T_UINT16 size = 10; /*2bytes-Unused,  6bytes-From abf to common id, 2bytes-Size of neighbor set*/

      size += (this->neighbor_set.size() * 6);

      return size;

    }

  /********************************************************************
   * @function  serialise
   * @brief     Serialises the given Hello message
   * @param     [1] ptr - Pointer to the serialised buffer.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void
  C_MESSAGE_HEADER::T_HELLO::serialize(T_UINT8* buf, T_UINT16& index_value)
  {
      write_u16(M_ZERO, &buf[index_value]); //Unused Bits
      index_value += M_TWO;

      buf[index_value++] = this->node_willingness.willingness;

      buf[index_value++] = this->leader_info.is_leader;

      write_u16((T_UINT16&)this->leader_info.leader_addr, &buf[index_value]);
      index_value += M_TWO;

      buf[index_value++] = this->abf;

      buf[index_value++] = this->common_id;

      write_u16((M_SIX + (neighbor_set.size() * M_ROUTER_INFO_SIZE)), &buf[index_value]);
      index_value += M_TWO;

      std::cout << "neighbor info size= " << (M_SIX + (neighbor_set.size() * M_ROUTER_INFO_SIZE)) << std::endl;

      for(std::vector<T_GENERIC_ADDR_BLOCK>::const_iterator hello_iter = this->neighbor_set.begin();
          hello_iter != this->neighbor_set.end(); hello_iter++)
      {
          buf[index_value++] = hello_iter->unique_id;
          buf[index_value++] = hello_iter->common_field.link_state.type;
          write_u16(hello_iter->metric[M_ZERO], &buf[index_value]);
          index_value += M_TWO;
          write_u16(hello_iter->metric[M_ONE], &buf[index_value]);
          index_value += M_TWO;

      }

  }

  /********************************************************************
   * @function  deserialise
   * @brief     This function deserialises the given TC message.
   * @param     [1] ptr - Pointer to the given Serialised buffer.
   *            [2] hello_msg - Hello message
   * @return    None.
   * @note      None.
  ********************************************************************/
  void
  C_MESSAGE_HEADER::T_HELLO::deserialize(T_UINT8* buf, T_UINT16& index_value)
  {
      T_UINT16 reserved_bits = M_ZERO;
      T_UINT16 neighbor_info_size = M_ZERO;
      read_u16(&buf[index_value], reserved_bits); //Unused Bits
      index_value += M_TWO;


      this->node_willingness.willingness = (T_UINT8)buf[index_value++];
      std::cout << "Message flood willingness = " << this->node_willingness.fields.flood_will << std::endl;
      std::cout << "Message Route willingness = " << this->node_willingness.fields.route_will << std::endl;

      this->leader_info.is_leader = (T_BOOL)buf[index_value++];
      std::cout << "Is Leader  = " << this->leader_info.is_leader << std::endl;

      read_u16(&buf[index_value], (T_UINT16&)this->leader_info.leader_addr);
      index_value += M_TWO;

      this->abf = (E_ADDRESS_BLOCK_FLAGS)buf[index_value++];
      std::cout << "Message abf = " << this->abf << std::endl;

      this->common_id = (T_UINT8)buf[index_value++];
      std::cout << "Message Common id = " << this->common_id << std::endl;

      read_u16(&buf[index_value], neighbor_info_size);
      index_value += M_TWO;

      std::cout << "neighbor info size= " << neighbor_info_size << std::endl;

      while((neighbor_info_size - M_SIX) > M_ZERO)
      {
          for(T_UINT8 neighb_iter = ((neighbor_info_size-M_SIX)/M_ROUTER_INFO_SIZE); neighb_iter > M_ZERO; neighb_iter--)
          {
              T_GENERIC_ADDR_BLOCK hello_block;
              hello_block.unique_id = (T_UINT8)buf[index_value++];
              hello_block.common_field.link_state.type = (T_UINT8)buf[index_value++];
              read_u16(&buf[index_value], hello_block.metric[M_ZERO]);
              index_value += M_TWO;
              read_u16(&buf[index_value], hello_block.metric[M_ONE]);
              index_value += M_TWO;
              this->neighbor_set.push_back(hello_block);
              std::cout << "Neighbor= " << neighb_iter << std::endl;
              neighbor_info_size -= M_ROUTER_INFO_SIZE;
          }
      }

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
      T_UINT16 size = 9; /*2bytes-Unused,  6bytes-From ansn to common id, 2 bytes-Size of neighbor set*/

#ifdef COMMENT_SECTION
      for(std::vector<T_TC_ADDRESS_BLOCK>::const_iterator it = this->tc_addr_block.begin(); it != tc_addr_block.end(); it++)
        {
          size += ((it->network_info.size() * 6) + 3);
        }
#endif
      size += (tc_addr_block.network_info.size() * M_SIX);
      return size;

    }

  /********************************************************************
   * @function  serialise
   * @brief     This function serialises the given TC message.
   * @param     [1] ptr - Pointer to the given Serialised buffer.
   * @return    None.
   * @note      None.
  ********************************************************************/
  void
  C_MESSAGE_HEADER::T_TC::serialize(T_UINT8* buf, T_UINT16& index_value)
  {
      write_u16(M_ZERO, &buf[index_value]); //Unused Bits
      index_value += M_TWO;

      write_u16(this->ansn, &buf[index_value]);
      index_value += M_TWO;

      //WriteU16((tc_addr_set.size() * (5+(tc_addr_iter->network_info.size() * 6))), ptr);

      buf[index_value++] = (T_UINT8)this->tc_addr_block.tc_msg_type;
      buf[index_value++] = (T_UINT8)this->tc_addr_block.abf;
      buf[index_value++] = (T_UINT8)this->tc_addr_block.common_id;

      write_u16((M_SIX +(this->tc_addr_block.network_info.size() * M_ROUTER_INFO_SIZE)), &buf[index_value]);
      index_value += M_TWO;

      for(std::vector<T_GENERIC_ADDR_BLOCK>::const_iterator topo_iter = this->tc_addr_block.network_info.begin();
          topo_iter != this->tc_addr_block.network_info.end(); topo_iter++)
          {
              buf[index_value++] = (T_UINT8)topo_iter->unique_id;
              buf[index_value++] = (T_UINT8)topo_iter->common_field.hop_count;
              write_u16(topo_iter->metric[M_ZERO], &buf[index_value]);
              index_value += M_TWO;
              write_u16(topo_iter->metric[M_ONE], &buf[index_value]);
              index_value += M_TWO;

          }
  }

  /********************************************************************
   * @function  deserialise
   * @brief     This function deserialises the given TC message.
   * @param     [1] buf - Pointer to the given Serialised buffer.
   *            [2] index_value - Index to the buffer index
   * @return    None.
   * @note      None.
  ********************************************************************/
  void
  C_MESSAGE_HEADER::T_TC::deserialize(T_UINT8* buf, T_UINT16& index_value)
  {
      T_UINT16 reserved_bits = M_ZERO;
      T_UINT16 topology_info_size = M_ZERO;

      read_u16(&buf[index_value], reserved_bits); //Unused Bits
      index_value += M_TWO;

      read_u16(&buf[index_value], this->ansn);
      index_value += M_TWO;

      //WriteU16((tc_addr_set.size() * (5+(tc_addr_iter->network_info.size() * 6))), ptr);

      this->tc_addr_block.tc_msg_type = (E_TC_ADDRESS_TYPE)buf[index_value++];
      this->tc_addr_block.abf = (E_ADDRESS_BLOCK_FLAGS)buf[index_value++];
      this->tc_addr_block.common_id = (T_UINT8)buf[index_value++];

      read_u16(&buf[index_value],topology_info_size);
      std::cout << "topology_info_size = " << topology_info_size << std::endl;
      index_value += M_TWO;

      while((topology_info_size - M_SIX) > M_ZERO)
      {
          for(T_UINT8 topo_iter = ((topology_info_size-M_SIX)/M_ROUTER_INFO_SIZE); topo_iter > M_ZERO; topo_iter--)
          {
              T_GENERIC_ADDR_BLOCK tc_block;
              tc_block.unique_id = (T_UINT8)buf[index_value++];
              tc_block.common_field.hop_count = (T_UINT8)buf[index_value++];
              read_u16(&buf[index_value], tc_block.metric[M_ZERO]);
              index_value += M_TWO;
              read_u16(&buf[index_value], tc_block.metric[M_ONE]);
              index_value += M_TWO;
              this->tc_addr_block.network_info.push_back(tc_block);
              topology_info_size -= M_ROUTER_INFO_SIZE;
              std::cout << "Topo info added = "<< topo_iter << std::endl;
          }
      }
  }

  /********************************************************************
   * @function  serialise
   * @brief     This function serialises the given OLSR message.
   * @param     [index] = pointer to the serialised buffer.
   * @return    None.
   * @note      None.
  ********************************************************************/
  T_UINT32
  C_MESSAGE_HEADER::serialize(T_UINT8* buffer, T_UINT16 index)
  {
      T_UINT16 start_address = index;
      T_UINT16 end_address = 0;

      std::cout << "Start address = " << start_address << std::endl;

      //index++ = this->m_message_type;

      //write_u16(this->m_msg_len, index);

      write_u16((T_UINT16&)this->m_originator_address, &buffer[index]);
      index += M_TWO;

      buffer[index++] = (T_UINT8)this->m_time_to_live;

      buffer[index++] = (T_UINT8)this->m_hop_count;

      write_u16(this->m_message_sequence_number, &buffer[index]);
      index += M_TWO;

      std::cout << "Message m_message_sequence_number = " << this->m_message_sequence_number << std::endl;

      buffer[index++] = (T_UINT8)this->m_validity_time;

      buffer[index++] = (T_UINT8)this->m_interval_time;

      switch(this->m_message_type)
      {
          case HELLO_MESSAGE:
          this->m_message.hello.serialize(buffer, index);
          break;

          case TC_MESSAGE:
          this->m_message.tc.serialize(buffer, index);
          break;

          default:
          break;

      }
      end_address = index - start_address;
      std::cout << "End address = " << index << std::endl;
      //std::cout << "hello message size diff = " << end_address << std::endl;

      return end_address;
  }

  /********************************************************************
   * @function  deserialise
   * @brief     This function deserialises the given OLSR message.
   * @param     [1] msg_header = Olsr message.
   *            [2] msg_type - HELLO/TC/TC_FORWARDED
   *            [3] buf_iter - Pointer to buffer
   *            [4] msg_size - Size of the message
   * @return    None.
   * @note      None.
  ********************************************************************/
  void
  C_MESSAGE_HEADER::deserialize(E_OLSR_MSG_TYPE msg_type, T_UINT8* buffer, T_UINT16 index, T_UINT16 msg_size)
  {
      if(buffer != NULL)
      {
          this->m_message_type = msg_type;
          std::cout << "Message type = " << this->m_message_type << std::endl;
          this->m_msg_len = msg_size;

          read_u16(&buffer[index], (T_UINT16&)this->m_originator_address);
          index += M_TWO;

          std::cout << "Message Originator = " << (int)this->m_originator_address.net_id << "." <<

          (int)this->m_originator_address.node_id << std::endl;

          this->m_time_to_live = buffer[index++];
          std::cout << "Message TTL = " << this->m_time_to_live << std::endl;

          this->m_hop_count = buffer[index++];
          std::cout << "Message hop count = " << this->get_hop_count() << std::endl;

          read_u16(&buffer[index], this->m_message_sequence_number);
          index += M_TWO;
          std::cout << "Message m_message_sequence_number = " << this->m_message_sequence_number << std::endl;

          this->m_validity_time = buffer[index++];
          std::cout << "Message m_validity_time = " << this->m_validity_time << std::endl;

          this->m_interval_time = buffer[index++];
          std::cout << "Message Interval time = " << this->get_interval_time() << std::endl;


          switch(this->m_message_type)
          {
              case HELLO_MESSAGE:
              this->m_message.hello.deserialize(buffer, index);
              break;

              case TC_MESSAGE:
              case TC_FORWARDED:
              this->m_message.tc.deserialize(buffer, index);
              break;

              default:
              break;

          }
      }


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
        size += (this->m_message.hello.get_hello_msg_size() + M_MSG_COMMON_DATA_LEN);
        break;

       /* If its a Hello message then adds Tc message size with Tc message header length*/
      case TC_MESSAGE:
           TC_FORWARDED:
        size += (this->m_message.tc.get_tc_msg_size() + M_MSG_COMMON_DATA_LEN);
        break;

      default:
        break;
    }

    return size;
  }



}
