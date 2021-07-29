/*                        Project title
 *
 *
 * Source Code Name   :   gateway_interface.cpp
 *
 * Description        :   Implements Gateway interface for OLSR.
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

#include <iostream>
#include "olsr.hpp"


namespace ns_olsr2_0
{

  /********************************************************************
   * @function  get_routing_destination_address
   * @brief     This function returns the list if IP addresses of local node
   * @param     None.
   * @return    None.
   * @note      None.
  ********************************************************************/
  T_ADDR* get_routing_destination_address(T_ADDR* Csrc, T_ADDR* Dest, T_ADDR* RDest)
  {
    return RDest;

  }

 /********************************************************************
  * @function  recv_olsr
  * @brief     This function stores the attached network information
  *            received from Gateway
  * @param     network_table.
  * @return    None.
  * @note      None.
 ********************************************************************/
 void C_OLSR::recv_olsr(void * olsr_packet)
 {
   ;
 }

 /********************************************************************
   * @function  send_olsr_msg
   * @brief     This function returns the attached network information
   *            to the Gateway
   * @param     None.
   * @return    None.
   * @note      None.
  ********************************************************************/
 void C_OLSR::send_olsr_msg()
 {
   ;
 }

#ifdef M_TO_BE_DELETED

 /********************************************************************
   * @function  get_node_address
   * @brief     This function returns the node address
   * @param     None.
   * @return    None.
   * @note      None.
  ********************************************************************/
 T_ADDR C_OLSR::get_node_address(void)
 {
   T_ADDR loc_addr;

   loc_addr.field.m_mux_ubm = 0;
   loc_addr.field.reserved = 0;
   loc_addr.field.m_oid = 10;
   loc_addr.field.m_nid = 10;

   return loc_addr;

 }

 /********************************************************************
   * @function  get_willingness
   * @brief     This function returns the willingess
   * @param     None.
   * @return    None.
   * @note      None.
  ********************************************************************/

 T_UINT8 C_OLSR::get_willingness(void)
 {
   return 0x33;  /* 0011 0011 = Willingness HIGH is set to both Routing and Flooding */
 }

 /********************************************************************
   * @function  get_in_link_metric
   * @brief     This function returns in link cost between the neighbor node
   * @param     None.
   * @return    None.
   * @note      None.
  ********************************************************************/
 float C_OLSR::get_in_link_metric(void)
 {
   return 1.0;
 }

#endif

}
