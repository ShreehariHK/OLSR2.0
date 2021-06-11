/*
 * Olsr_State.h
 *
 *  Created on: 10-Jun-2021
 *      Author: accord
 */

#ifndef OLSR_STATE_H_
#define OLSR_STATE_H_

#include "Olsr_Repository.h"


namespace olsr2_0 {

class OlsrState
{

public:
	OlsrState();
	virtual ~OlsrState();

private:
	LocalInterfaceSet m_LocalInterfaceSet;
	OriginatorSet m_OriginatorSet;
	LocalAttachedNetworkSet m_LocalAttachedNetworkSet;
	RemovedInterfaceAddressSet m_RemovedInterfaceAddressSet;
	LinkSet m_LinkSet;
	TwoHopNeighborSet m_TwoHopNeighborSet;
	NeighbourSet m_NeighbourSet;
	Mpr_Set m_Mpr_Set;
	AdvertisingRemoteRouterSet m_AdvertisingRemoteRouterSet;
	RouterTopologySet m_RouterTopologySet;
	RoutableAddressTopologySet m_RoutableAddressTopologySet;
	AttachedNetworkSet m_AttachedNetworkSet;
	ReceivedMessageSet m_ReceivedMessageSet;
	ProcessedMsgSet m_ProcessedMsgSet;
	ForwardedMsgSet m_ForwardedMsgSet;

};



}
#endif /* OLSR_STATE_H_ */
