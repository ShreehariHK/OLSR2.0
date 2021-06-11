/*
 * Olsr_Header.h
 *
 *  Created on: 10-Jun-2021
 *      Author: accord
 */

#ifndef OLSR_HEADER_H_
#define OLSR_HEADER_H_

namespace olsr2_0 {
class PacketHeader
{
public:
  PacketHeader ();
	#ifdef NS3_SIM
  	  virtual ~PacketHeader ();
	#else
  	  ~PacketHeader ();
	#endif


  /**
   * Set the packet total length.
   * \param length The packet length.
   */
  void SetPacketLength (uint16_t length)
  {
    m_packetLength = length;
  }

  /**
   * Get the packet total length.
   * \return The packet length.
   */
  uint16_t GetPacketLength () const
  {
    return m_packetLength;
  }

  /**
   * Set the packet sequence number.
   * \param seqnum The packet sequence number.
   */
  void SetPacketSequenceNumber (uint16_t seqnum)
  {
    m_packetSequenceNumber = seqnum;
  }

  /**
   * Get the packet sequence number.
   * \returns The packet sequence number.
   */
  uint16_t GetPacketSequenceNumber () const
  {
    return m_packetSequenceNumber;
  }

private:
  uint16_t m_packetLength;          //!< The packet length.
  uint16_t m_packetSequenceNumber;  //!< The packet sequence number.

};

class MessageHeader
{
	public:
	MessageHeader ();
	#ifdef NS3_SIM
		virtual ~MessageHeader ();
	#else
		~MessageHeader ();
	#endif

	/**
	   * Set the message type.
	   * \param messageType The message type.
	   */
	  void SetMessageType (OlsrMsgType messageType)
	  {
		m_messageType = messageType;
	  }
	  /**
	   * Get the message type.
	   * \return The message type.
	   */
	  OlsrMsgType GetMessageType () const
	  {
		return m_messageType;
	  }

	  /**
	   * Set the validity time.
	   * \param time The validity time.
	   */
	  void SetVTime (Time time)
	  {
		  m_validity_time = SecondsToEmf (time.GetSeconds ());
	  }
	  /**
	   * Get the validity time.
	   * \return The validity time.
	   */
	  Time GetVTime () const
	  {
		return Seconds (EmfToSeconds (m_vTime));
	  }

	  /**
	   * Set the originator address.
	   * \param originatorAddress The originator address.
	   */
	  void SetOriginatorAddress (Ipv4Address originatorAddress)
	  {
		m_originatorAddress = originatorAddress;
	  }
	  /**
	   * Get the originator address.
	   * \return The originator address.
	   */
	  Ipv4Address GetOriginatorAddress () const
	  {
		return m_originatorAddress;
	  }

	  /**
	   * Set the time to live.
	   * \param timeToLive The time to live.
	   */
	  void SetTimeToLive (uint8_t timeToLive)
	  {
		m_timeToLive = timeToLive;
	  }
	  /**
	   * Get the time to live.
	   * \return The time to live.
	   */
	  uint8_t GetTimeToLive () const
	  {
		return m_timeToLive;
	  }

	  /**
	   * Set the hop count.
	   * \param hopCount The hop count.
	   */
	  void SetHopCount (uint8_t hopCount)
	  {
		m_hopCount = hopCount;
	  }
	  /**
	   * Get the hop count.
	   * \return The hop count.
	   */
	  uint8_t GetHopCount () const
	  {
		return m_hopCount;
	  }

	  /**
	   * Set the message sequence number.
	   * \param messageSequenceNumber The message sequence number.
	   */
	  void SetMessageSequenceNumber (uint16_t messageSequenceNumber)
	  {
		m_messageSequenceNumber = messageSequenceNumber;
	  }
	  /**
	   * Get the message sequence number.
	   * \return The message sequence number.
	   */
	  uint16_t GetMessageSequenceNumber () const
	  {
		return m_messageSequenceNumber;
	  }

	  /**
	   * Set the message flag.
	   * \param messageFlag The message flag.
	   */
	  void SetMessageFlag (uint8_t messageFlag)
	  {
		  m_msgflag = messageFlag;
	  }
	  /**
	   * Get the message flag.
	   * \return The Message flag.
	   */
	  uint8_t GetMessageFlag () const
	  {
		return m_msgflag;
	  }

	  /**
	   * Set the message address length.
	   * \param message_addr_length The message address length.
	   */
	  void SetMessageAddressLength (uint8_t message_addr_length)
	  {
		  m_msg_addr_len = message_addr_length;
	  }
	  /**
	   * Get the message address length.
	   * \return The message address length.
	   */
	  uint8_t GetMessageAddressLength () const
	  {
		return m_msg_addr_len;
	  }

	  /**
	   * Set the message length.
	   * \param messageLength The message length.
	   */
	  void SetMessageLength (uint16_t messageLength)
	  {
		  m_msg_len = messageLength;
	  }
	  /**
	   * Get the message length.
	   * \return The message length.
	   */
	  uint16_t GetMessageLength () const
	  {
		return m_msg_len;
	  }


	private:

		OlsrMsgType m_messageType;
		uint8_t m_msgflag;
		uint8_t m_msg_addr_len;
		uint16_t m_msg_len;
		Ipv4Address m_originatorAddress;
		uint8_t m_timeToLive;            	  //!< The time to live.
		uint8_t m_hopCount;               //!< The hop count.
		uint16_t m_messageSequenceNumber; //!< The message sequence number.
		Time m_validity_time;

	public:
		struct GenericMsgTLV
		{
			OlsrMsgType Type;	//1 byte
			uint8_t MTLVF;	//1 byte
			uint8_t Len;	//1 byte
			uint8_t Value;	//1 byte
		};

		struct MsgBlockTLV
		{
			uint8_t Msg_TLV_Block_Len;  // value = 12
			std::vector<GenericMsgTLV> Msg_TLV_Set; //i) Validity time ii) Interval Time iii) MPR Willingness iv) ANSN number
		};


		struct AddrBlock
		{
			uint8_t Num_Addrs;				//1 byte
			uint16_t ABF;					//1 byte
			uint8_t Head_Len;				//1 byte
			std::vector<int> Head;			// Head_Len*1
			uint8_t Tail_Len;				//1 byte
			std::vector<int> Tail;			// Tail_Len*1
			uint8_t Mid_Len;				//1 byte
			std::vector<int> Mid;			//  (Num Addrs*4) - (Head_Len*Num Addrs) - (Tail_Len*Num Addrs)
			uint16_t pre_fix_length;
			//pre-fix_value;
		};

		struct MetricTLV
		{
			OlsrMsgType Type; //
			uint8_t ATLVF;
			uint8_t Len;
			std::vector<float> Metric;
		};

		union OptionalFields
		{
			enum Link_States link_state;
			uint8_t Hop_count;
		};

		struct GenericAddrTLV
		{

			OlsrMsgType Type;	//1 byte	// LOCAL_IF, LINK_STATUS, LINK_METRIC, NBR_ADDR_TYPE, GATEWAY
			uint8_t ATLVF;	//1 byte
			uint8_t Start_Indx;	//1 byte
			uint8_t Stop_Indx;	//1 byte
			uint8_t Len;		//1 byte
			std::vector<union OptionalFields> OptionalData;
			struct MetricTLV metric;
		};

		/*------------------------------------------------------------------
		 Hello Message
		------------------------------------------------------------------*/
		struct HelloAddrBlockTLV
		{
			struct AddrBlock addrblock;
			uint8_t Addr_TLV_Block_Len;  // value = 12
			std::vector<GenericAddrTLV> Local_Nbr_Information;
		};

		struct Hello
		{
			struct MsgBlockTLV Msg_TLVs;
			struct HelloAddrBlockTLV Hello_Addr_TLVs;
		};

		/*------------------------------------------------------------------
		 Tc Message
		------------------------------------------------------------------*/
		struct TcAddrBlockTLV
		{
			struct AddrBlock addrblock;
			uint8_t Addr_TLV_Block_Len;  // value = 12
			std::vector<GenericAddrTLV> Information_List;
		};

		struct Tc
		{
			struct MsgBlockTLV Msg_TLVs;
			std::vector<TcAddrBlockTLV> TC_Addr_TLVs;
		};

	private:

		struct
		{
			struct Hello hello;
			struct Tc tc;
		}m_message;

	public:


		/**
		   * Set the message type to HELLO and return the message content.
		   * \returns The HELLO message.
		   */
		  Hello& GetHello ()
		  {
		    if (m_messageType == 0)
		      {
		        m_messageType = HELLO_MESSAGE;
		      }
		    else
		      {
				#ifdef NS3_SIM
		        	NS_ASSERT (m_messageType == HELLO_MESSAGE);
				#endif
		      }
		    return m_message.hello;
		  }

		  /**
		   * Set the message type to TC and return the message content.
		   * \returns The TC message.
		   */
		  Tc& GetTc ()
		  {
		    if (m_messageType == 0)
		      {
		        m_messageType = TC_MESSAGE;
		      }
		    else
		      {
				#ifdef NS3_SIM
		        	NS_ASSERT (m_messageType == TC_MESSAGE);
				#endif
		      }
		    return m_message.tc;
		  }
};

}

#endif /* OLSR_HEADER_H_ */
