/*                            Project title
 *
 *
 *Source Code Name   :   olsr_types.hpp
 *       
 *Description        :   To store all the types used in OLSR.
 *
 *Subsystem Name     :   OLSR
 *       
 *Revision History               
 *---------------------------------------------------------------------------|
 *Version | Change Description               |    Date    |    Changed By    |
 *--------|----------------------------------|------------|------------------|
 *1.0     |Initial Version                   | 14-06-2021 |Shreehari H K     |
 *--------|----------------------------------|------------|------------------|
 *           
 *                              Copyright statement                            
 *
 *
 */


#ifndef OLSR_TYPES_HPP_
#define OLSR_TYPES_HPP_
#include <iostream>
#include <stdint.h>
#include <vector>
#include <array>
#include "time.h"	        /* Include file for Time functionality */
#include "olsr_utility.hpp"        /* Include file for basic utilities */

#define M_NS3_SIM	        /* To run in simulation mode */
#define M_TO_BE_DELETED
//#define COMMENT_SECTION


namespace ns_olsr2_0
{
#define M_HELLO_INTERVAL 1                              /* Periodic interval of Hello message */
#define M_TC_INTERVAL 2                                 /* Periodic interval of Tc message */
#define M_HELLO_MSG_VALID__TIME (3 * M_HELLO_INTERVAL)  /* Validity Time  of Hello message */
#define M_TC_MSG_VALID_TIME (3 * M_TC_INTERVAL)         /* Validity Time  of Tc message */
#define M_DUP_MSG_HOLD_TIME  30                         /* Validity time of duplicate message */
#define M_MAX_MSG_SEQ_NUM  65535                        /* Max sequence number of OLSR message */

#define M_LOST_LINK        0X1    /* Link is lost */
#define M_HEARD_LINK       0X2    /* Link is heard */
#define M_SYMMETRIC_LINK   0x4    /* Link is symmetric */

#define M_LOST             0X1    /* Link status is lost */
#define M_HEARD            0X2    /* Link status is heard */
#define M_SYMMETRIC        0x4    /* Link status is symmetric */

#define M_NOT_A_NEIGHBOR       0x0     /* Not a neighbor */
#define M_ROUTING_MPR          0x1     /* neighbor is a routing mpr */
#define M_FLOODING_MPR     	   0x2     /* neighbor is flooding mpr */
#define M_MPR_FLOOD_ROUTE  	   0x3    /* neighbor is both flooding and routing mpr */
#define M_SYMMETRIC_NEIGHBOR   0x4     /* Symmetric neighbor */

#define M_MAX_OLSR_BUF_SIZE    512     /* Maximum buffer size of a OLSR message */
#define M_UNKNOWN_VALUE  	   0xFF    /* link metric value which is unknown  */

#define M_ZERO 			   0x0	  /* value is Zero */
#define M_ONE			   0x1    /* value is One */
#define M_TWO			   0x2    /* value id Two */


typedef long long int Time;

/*
 * Status of the next hop route
 */
typedef enum
{
  ROUTE_NOT_FOUND = 0X0,
  ROUTE_FOUND = 0X1
}E_ROUTE_STATUS;

/*
 * Destination node address types
 */
typedef enum
{
  UNICAST_ADDR = 0X1,
  MULTICAST_ADDR = 0X2
}E_DEST_ADDR_TYPE;

/**
 * Address type of mpr selectors
 */
typedef enum
{
  ROUTABLE_ORIG = 0X1,
  ATTACHED = 0X2
}E_TC_ADDRESS_TYPE;

/**
 * types of olsr message
 *
 * HELLO_MESSAGE
 * TC_MESSAGE : Topology control message
 */
typedef enum
{
    HELLO_MESSAGE = 0x1,      /* Hello Message */
    TC_MESSAGE = 0x2          /* Topology control Message */
}E_OLSR_MSG_TYPE;

/**
 * this holds the flags with respect to
 * Address blocks in the message
 */
typedef enum
{
  NET_ID_COMMON = 0X1,
  NODE_ID_COMMON = 0X2
}E_ADDRESS_BLOCK_FLAGS;

/**
 * this holds the type of olsr instance
 */
typedef enum
{
  NORMAL_NODE_INSTANCE = 0X0,
  LEADER_NODE_INSTANCE = 0X1
}E_OLSR_INSTANCE;

/**
 * this holds the type of Willingness
 * that a node can have
 */
typedef enum
{
  WILL_NEVER = 0X0,
  WILL_LOW = 0X1,
  WILL_MEDIUM = 0X2,
  WILL_HIGH = 0X3,
  WILL_ALWAYS = 0X4
}E_WILLINGNESS;

/**
 * Holds the 2 types of node willingness
 */
  typedef struct
  {
    E_WILLINGNESS route_will:4;
    E_WILLINGNESS flood_will:4;
  }T_WILL_BASE;

  /**
   * Holds the overall willingness
   */
  typedef union
  {
    T_UINT8 willingness;
    T_WILL_BASE fields;
  }U_WILLINGNESS;

  /**
   * Contains information about neighbor type and its link.
   */
  typedef struct
  {
    T_UINT8 link_type:4;
    T_UINT8 nbr_type:4;
  }T_NEIGHBOR_LINK;

  /**
   * Contains information about neighbor type and its link.
   */
  typedef union
  {
    T_NEIGHBOR_LINK type_fields;
    T_UINT8 type;
  }U_NEIGHBOR_LINK;

#ifdef M_TO_BE_DELETED

  typedef struct
  {
    T_UINT8 m_mux_ubm:2;
    T_UINT8 reserved:2;
    T_UINT8 m_oid:6;
    T_UINT8 m_nid:6;
  }T_ADDR_BASE;

  typedef union
  {

    T_UINT16 addr;
    T_ADDR_BASE field;
  }T_ADDR;

  /* Checks if given 2 addresses are same*/
  static inline T_BOOL
  operator == (T_ADDR& addr_a, const T_ADDR& addr_b)
  {
    return(addr_a.field.m_nid == addr_b.field.m_nid);
  }

#endif

/**
 * Node address format
 */
typedef struct
{
  T_UINT8 net_id;    /*Orthogonal Network Id*/
  T_UINT8 node_id;   /*Node Id in an orthogonal net*/
}T_NODE_ADDRESS;

/* Checks if given 2 addresses are same*/
static inline T_BOOL
operator == (const T_NODE_ADDRESS& addr_a, const T_NODE_ADDRESS& addr_b)
{
  return((addr_a.net_id == addr_b.net_id) and (addr_a.node_id == addr_b.node_id));
}

/**
 * Holds Leader address tuple
 */
typedef struct
{
  T_NODE_ADDRESS leader_addr;
  T_BOOL is_leader;
}T_LEADER_TUPLE;

/*------------------------------------------------------------------
2) Interface Information Base
--------------------------------------------------------------------*/

/**
 * holds about the link information between the local and the neighbor node
 */
typedef struct
{
	T_FLOAT l_in_metric;		               /* Incoming data-link metric from 1-hop neighbor node to local node*/
	T_FLOAT l_out_metric;		               /* Outgoing data-link metric from local node to 1-hop neighbor node */
	T_BOOL l_mpr_selector;	                   /* True if the 1-hop neighbor selected this local node as flooding MPR */
	T_NODE_ADDRESS  l_neighbor_iface_addr;     /* The address of that interface of the 1-hop neighbor */
	Time l_heard_time;		                   /* the validity time of this incoming link */
	Time l_sym_time;		                   /* Validity time of this symmetric link */
	Time l_time;			                   /* Validity time of this link */
	T_UINT8 l_status;                          /* Status of this link */
}T_LINK_TUPLE;

/**
 * Holds about information between the local and the 2-hop neighbor node
 */
typedef struct
{
	T_FLOAT n2_in_metric;	                   /* Incoming neighbor metric from 1-hop neighbor node to local node*/
	T_FLOAT n2_out_metric;                     /* Outgoing neighbor metric from local node to 1-hop neighbor node */
	T_NODE_ADDRESS n2_neighbor_iface_addr;     /* The address of that interface of 1-hop neighbor which has a symmetric connection to this 2-hop neighbor */
	T_NODE_ADDRESS n2_2hop_addr;	           /* Ip address of the 2-hop neighbor */
	Time n2_time;		                       /* Validity time of this 2-hop connection */
}T_TWO_HOP_NEIGHBOUR_TUPLE;

/*------------------------------------------------------------------
3) Neighbor Information Base
--------------------------------------------------------------------*/

/**
 * holds the 1-hop neighbor information
 */
typedef struct
{
	T_NODE_ADDRESS n_neighbor_addr;	           /* Address of the 1-hop neighbor */
	T_FLOAT n_in_metric;	                                /* Neighbor In metric */
	T_FLOAT n_out_metric;	                                /* Neighbor out metric */
	U_WILLINGNESS n_willingness;                                  /* bits 0-3 - Flooding Will and bits 4-7 Routing Willingness */
	T_BOOL n_symmetric;		                                /* True if the connection is symmetric */
	T_BOOL n_flooding_mpr;	                                /* True if this neighbor is a flooding MPR */
	T_BOOL n_routing_mpr;		                            /* True if this neighbor is a routing MPR */
	T_BOOL n_mpr_selector;	                                /* True if this neighbor selected this local node as a Routing MPR */
	T_BOOL n_advertised;		                            /* True if n_mpr_selector is True */
}T_NEIGHBOUR_TUPLE;

/*------------------------------------------------------------------
3) Topology Information Base
--------------------------------------------------------------------*/

/**
 * holds the information about the Routing MPRs in network
 */
typedef struct
{
	T_NODE_ADDRESS ar_orig_addr;	                            /* Originator address of the TC message(Routing MPR) */
	T_UINT16 ar_seq_number;			                        /* Advertised neighbor sequence number */
	Time ar_time;				                            /* Validity time of this tuple */
}T_ADVERTISING_REMOTE_ROUTER_TUPLE;

/**
 * Holds the information network topology
 */
typedef struct
{
	T_NODE_ADDRESS tr_from_orig_addr;	                        /* Originator address of the TC message(Routing MPR) */
	T_NODE_ADDRESS tr_to_orig_addr;	                        /* Originator Address of the node who is a routing MPR selector of TC message originator*/
	T_UINT16 tr_seq_number;		                            /* Advertised neighbor sequence number */
	T_FLOAT tr_metric;		                                /* Out metric between the TC message originator and its routing MPR  selector*/
	Time tr_time;			                                /* Validity time of this tuple */
}T_ROUTER_TOPOLOGY_TUPLE;

/**
 * holds information about the attached network
 */
typedef struct
{
	T_NODE_ADDRESS an_orig_addr;	                            /* Originator address of the TC message */
	T_NODE_ADDRESS an_net_addr;	                            /* Attached network address connected to by this TC message generator*/
	T_UINT16 an_seq_number;		                            /* Advertised neighbor sequence number */
	T_UINT8 an_dist;		                                /* Distance to the attached network */
	Time an_time;			                                /* Validity time of this tuple */
	T_FLOAT an_metric;		                                /* Out metric between the TC message originator and the attached network*/
}T_ATTACHED_NETWORK_TUPLE;

/**
 * holds the information about the route to a destination in the network
 */
typedef struct
{
	T_NODE_ADDRESS r_dest_addr;	                            /* Destination Ip address of the node */
	T_NODE_ADDRESS r_next_iface_addr;                          /* Interface address of the Next node */
	T_NODE_ADDRESS r_local_iface_addr;                         /* Interface address of the local node */
	T_UINT8 r_dist; 		                                /* Distance to the destination node */
	T_FLOAT r_metric;		                                /* Out metric between the local node and the destination node*/
}T_ROUTING_TABLE_ENTRY;

/*------------------------------------------------------------------
3) Neighbor Graph Base
------------------------------------------------------------------*/

/**
 *holds the information of Symmetric 1-hop neighbors links
 */

typedef struct
{
    T_NODE_ADDRESS one_hop_neighb_addr;	/* Symmetric 1-Hop neighbor address */
    U_WILLINGNESS n_willingness;        /* 1-Hop neighbor's willingness */
    float in_out_metric;				/* Link Metric(In Link/Out Link) between the 1-Hop neighbor */
}T_ALLOWED_ONE_HOP_TUPLE; /* Used for both Flooding and Routing MPR calculation */

/**
 *holds the information of Symmetric 2-hop neighbors links
 */

typedef struct
{
    T_NODE_ADDRESS two_hop_neighb_addr; /* Symmetric 2-Hop neighbor address */
    T_NODE_ADDRESS one_hop_neighb_addr; /* Symmetric 1-Hop neighbor address */
    float in_out_metric;				/* Link Metric(In Link/Out Link) between the 2-Hop neighbor */
}T_ALLOWED_TWO_HOP_TUPLE; /* Used for both Flooding and Routing MPR calculation */

/**
 *holds the information that contains matrix between 1-hop and 2-hop  neighbors
 */
typedef struct
{
    T_NODE_ADDRESS two_hop_neighb_addr;     /* Symmetric two_hop neighbor address*/
    float d2;                              /* metric between one_hop and two_hop neighbors */
    float d;                               /* d1 + d2 */

}T_MATRIX;

/**
 *holds the information of symmetric 1-hop  neighbors,
 *their willingness , link metric and matrix set
 */

typedef struct
{
    T_NODE_ADDRESS one_hop_neighb_addr;         /* Symmetric one_hop neighbor address*/
    U_WILLINGNESS neighb_will;          /* Neighbor Willingness */
    float d1;                           /* metric between current node and one_hop neighbors */
    std::vector<T_MATRIX> matrix_set;   /* metrics between this one-hop and two-hop neighbors  connected by it*/
}T_N1;

/**
 *holds the information of symmetric 2-hop  neighbor,
 * and their willingness
 */
typedef struct
{
    T_NODE_ADDRESS two_hop_neighb_addr; /* Symmetric two_hop neighbor address*/
    float d1;   /* metric between current node and a node who is both one hop and two hop neighbor */
}T_N2;


/**
 * A Neighbor graph that holds N1 set and N2 set
 */

typedef struct
{
    std::vector<T_N1> n1_set;   /* Set holds the required info of symmetric 1-Hop neighbors*/
    std::vector<T_N2> n2_set;   /* Set holds the required info of symmetric 2-Hop neighbors*/
}T_NEIGHBOR_GRAPH;


/**
 * Network Link that
 * holds source and destination address with their Out link
 */

typedef struct
{
    T_NODE_ADDRESS src_addr;    /* Source node's address */
    T_NODE_ADDRESS dest_addr;   /* Destination node's address */
    float          out_metric;  /* Out Link metric between src and destination */
}T_NETWORK_LINK;

/**
 *Network topology graph holding
 *Network Link of 1-hop and
 * Topology set
 */

typedef struct
{
    std::vector<T_NETWORK_LINK>  one_hop_set;         /* Network link between current node and its 1-Hop neighbors*/
    std::vector<T_NETWORK_LINK>  router_topology_set; /* Network link other nodes in the network*/
    //std::vector<T_NETWORK_LINK>  two_hop_set;         /* Network link between current node and its 2-Hop neighbors */
}T_NETWORK_TOPOLOGY_GRAPH;

/*------------------------------------------------------------------
3) Received Message Information Base
------------------------------------------------------------------*/

/**
 * holds the received msg information
 */
typedef struct
{
	E_OLSR_MSG_TYPE rx_type;	                            /* Received message type */
	T_NODE_ADDRESS rx_orig_addr;	                            /* Originator address of the sender */
	T_UINT16 rx_seq_number;		                            /* Sequence number */
	Time rx_time;			                                /* Validity time of this tuple */
}T_RECEIVED_MSG_TUPLE;


/**
 * Holds the processed nsg information
 */
typedef struct
{
	E_OLSR_MSG_TYPE p_type;		                            /* Processed message type */
	T_NODE_ADDRESS p_orig_addr;	                            /* Originator address of the sender */
	T_UINT16 p_seq_number;		                            /* Sequence number */
	Time p_time;			                                /* Validity time of this tuple */
}T_PROCESSED_MSG_TUPLE;

/**
 * holds the forwarded msg information
 */
typedef struct
{
	E_OLSR_MSG_TYPE f_type;		                            /* Forwarded message type */
	T_NODE_ADDRESS f_orig_addr;	                            /* Originator address of the receiver */
	T_UINT16 f_seq_number;	                                /* Sequence number */
	Time f_time;                                            /* Validity time of this tuple */
}T_FORWARDED_MSG_TUPLE;


typedef std::vector<T_LINK_TUPLE> LinkSet;                                              /* Link Set */

typedef std::vector<T_NODE_ADDRESS> RoutingMprSet;                                         /* Routing MPR address Set */

typedef std::vector<T_NODE_ADDRESS> FloodingMprSet;                                        /* Flooding MPR address  Set */

typedef std::vector<T_TWO_HOP_NEIGHBOUR_TUPLE> TwoHopNeighborSet;                       /* 2-hop neighbor address  Set */

typedef std::vector<T_NEIGHBOUR_TUPLE> NeighbourSet;	                                /*  1-hop neighbor address Set */

typedef std::vector<T_ADVERTISING_REMOTE_ROUTER_TUPLE> AdvertisingRemoteRouterSet;	    /*  Advertising remote router address Set */

typedef std::vector<T_ATTACHED_NETWORK_TUPLE> AttachedNetworkSet;	    				/*  Attached Network address Set */

typedef std::vector<T_ROUTER_TOPOLOGY_TUPLE> RouterTopologySet;	                        /*  Router topology address Set */

typedef std::vector<T_PROCESSED_MSG_TUPLE> ProcessedMsgSet;	                            /*  Processed  message Set */

typedef std::vector<T_FORWARDED_MSG_TUPLE> ForwardedMsgSet;	                            /*  Forwarded  message Set */

typedef std::vector <T_ROUTING_TABLE_ENTRY> RoutingSet;                                 /*  Routing table Set */

T_BOOL get_is_leader(void);
Time get_ns();

/**
 * This function asserts the code
 */

static inline void OLSR_ASSERT(T_BOOL condition)
{

  if(condition != true)
    {
      std::cout << "Stuck in Assert" << std::endl;
      while(M_ONE)
        {
            ;
        }

    }

}

}

#endif  /* OLSR_TYPES_HPP_ */
