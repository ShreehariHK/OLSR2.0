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
  E_ROUTE_STATUS
  C_OLSR::get_routing_destination_address(const T_ADDR& Csrc, const T_ADDR& Dest, T_ADDR& RDest)
  {
    T_NODE_ADDRESS r_src_addr;
    E_ROUTE_STATUS route_status = ROUTE_NOT_FOUND;

    r_src_addr.net_id = Csrc.field.m_oid;
    r_src_addr.node_id = Csrc.field.m_nid;

    const T_LINK_TUPLE *link_tuple = m_state.find_sym_link_tuple(r_src_addr);

    if(link_tuple != NULL)
      {
        E_DEST_ADDR_TYPE dest_addr_type = check_address_type(Dest);
        switch(dest_addr_type)
        {
          case UNICAST_ADDR:
            route_status = m_state.find_next_routing_hop_addr(&Dest, &RDest);
            break;

          case MULTICAST_ADDR:
            if(link_tuple->l_mpr_selector == true)
              {
                route_status = ROUTE_FOUND;
                RDest = Dest;
              }
            break;

        }

      }

    else
      {
        route_status = ROUTE_NOT_FOUND;
      }



    return route_status;

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

   loc_addr.field.m_mux_ubm = M_ZERO;
   loc_addr.field.reserved = M_ZERO;
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

 T_UINT8 C_OLSR::get_node_willingness(void)
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
   return M_ONE;
 }

#endif

}
