#ifndef OLSR_REPOSITORY_H
#define OLSR_REPOSITORY_H
#include <iostream>
#include <stdint.h>
#include <vector>
#include "Ipv4_address.h"
#include "time.h"

namespace olsr2_0 {

typedef long int Time;
typedef long int Ipv4Address;
enum Link_States
{
	LOST = 0x00,
	PENDING = 0x01,
	HEARD = 0x02,
	SYMMETRIC = 0x03
};

enum OlsrMsgType
{
	HELLO_MESSAGE,
	TC_MESSAGE
};

/*1) Local Information Base
 ------------------------------------------------------------*/

struct LocalInterfaceTuple {
	Ipv4Address I_local_iface_addr;
	bool I_manet;
};

struct OriginatorTuple {
	Ipv4Address O_orig_addr;
	Time O_time;
};

struct LocalAttachedNetworkTuple {
	Ipv4Address AL_net_addr;
	uint8_t AL_dist;
	float AL_metric;

};

struct RemovedInterfaceAddressTuple {
	Ipv4Address IR_local_iface_addr;
	Time IR_time;
};

/*------------------------------------------------------------------
2) Interface Information Base
--------------------------------------------------------------------*/

struct LinkTuple
{
	float L_in_metric;
	float L_out_metric;
	bool L_mpr_selector;
	std::vector<Ipv4Address> L_neighbor_iface_addr_list;
	Time L_HEARD_time;
	Time L_SYM_time;
	float L_quality;
	bool L_pending;
	bool L_lost;
	Time L_time;
	enum Link_States L_Status;
};

struct TwoHopNeighborTuple
{
	float N2_in_metric;
	float N2_out_metric;
	std::vector<Ipv4Address> N2_neighbor_iface_addr_list;
	Ipv4Address N2_2hop_addr;
	Time N2_time;
};

/*------------------------------------------------------------------
3) Neighbour Information Base
--------------------------------------------------------------------*/

struct NeighbourTuple
{
	Ipv4Address N_orig_addr;
	std::vector<Ipv4Address> N_neighbor_addr_list;
	float N_in_metric;
	float N_out_metric;
	uint8_t N_willingness; // bits 0-3 - Flooding Will and bits 4-7 Routing Willingness
	bool N_symmetric;
	bool N_flooding_mpr;
	bool N_routing_mpr;
	bool N_mpr_selector;
	bool N_advertised;
};

/*------------------------------------------------------------------
3) Topology Information Base
--------------------------------------------------------------------*/

struct AdvertisingRemoteRouterTuple
{
	Ipv4Address AR_orig_addr;
	int AR_seq_number;
	Time AR_time;
};

struct RouterTopologyTuple
{
	Ipv4Address TR_from_orig_addr;
	Ipv4Address TR_to_orig_addr;
	int TR_seq_number;
	float TR_metric;
	Time TR_time;
};

struct RoutableAddressTopologyTuple
{
	Ipv4Address TA_from_orig_addr;
	Ipv4Address TA_to_orig_addr;
	int TA_seq_number;
	float TA_metric;
	Time TA_time;
};

struct AttachedNetworkTuple
{
	Ipv4Address AN_orig_addr;
	Ipv4Address AN_net_addr;
	int AN_seq_number;
	uint8_t AN_dist;
	Time AN_time;
	float AN_metric;
};

struct RoutingTuple
{
	Ipv4Address R_dest_addr;
	Ipv4Address R_next_iface_addr;
	Ipv4Address R_local_iface_addr;
	uint8_t R_dist;
	float R_metric;
};

/*------------------------------------------------------------------
3) Received Message Information Base
------------------------------------------------------------------*/

struct ReceivedMsgTuple
{
	OlsrMsgType RX_type;
	Ipv4Address RX_orig_addr;
	int RX_seq_number;
	Time RX_time;
};



struct ProcessedMsgTuple
{
	OlsrMsgType P_type;
	Ipv4Address P_orig_addr;
	int P_seq_number;
	Time P_time;
};



struct ForwardedMsgTuple
{
	OlsrMsgType F_type;
	Ipv4Address F_orig_addr;
	int F_seq_number;
	Time F_time;
};
typedef std::vector<LocalInterfaceTuple> LocalInterfaceSet;
typedef std::vector<OriginatorTuple> OriginatorSet;
typedef std::vector<LocalAttachedNetworkTuple> LocalAttachedNetworkSet;
typedef std::vector<RemovedInterfaceAddressTuple> RemovedInterfaceAddressSet;
typedef std::vector<LinkTuple> LinkSet;
typedef std::vector<TwoHopNeighborTuple> TwoHopNeighborSet;
typedef std::vector<NeighbourTuple> NeighbourSet;
typedef std::vector<AdvertisingRemoteRouterTuple> AdvertisingRemoteRouterSet;
typedef std::vector<RouterTopologyTuple> RouterTopologySet;
typedef std::vector<RoutableAddressTopologyTuple> RoutableAddressTopologySet;
typedef std::vector<AttachedNetworkTuple> AttachedNetworkSet;
typedef std::vector<RoutingTuple> RoutingSet;
typedef std::vector<ReceivedMsgTuple> ReceivedMessageSet;
typedef std::vector<ProcessedMsgTuple> ProcessedMsgSet;
typedef std::vector<ForwardedMsgTuple> ForwardedMsgSet;

}

#endif  //OLSR_REPOSITORY_H
