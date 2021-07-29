/*
 * Ipv4_address.h
 *
 *  Created on: 10-Jun-2021
 *      Author: accord
 */

#ifndef IPV4_ADDRESS_HPP_
#define IPV4_ADDRESS_HPP_

#include "olsr_utility.hpp"   /* Include file for basic utilities */

namespace ns_olsr2_0
{

#ifdef COMMENT_SECTION
  typedef union
{
  struct
  {
    T_UINT8    netid0;
    T_UINT8    netid1;
    T_UINT8    hostid0;
    T_UINT8    hostid1;
    T_UINT8    subnet_prefix;
    T_UINT8    reserved_bytes[3];

  }Ipv4_format;

  T_UINT64 Ip;

}Ipv4Address;

#else

typedef struct
{
  T_UINT8 netid;
  T_UINT8 hostid;
}Ipv4Address;

#endif
}

#endif /* IPV4_ADDRESS_HPP_ */
