//============================================================================
// Name        : 0.cpp
// Author      : Accord
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Olsr_Repository.h"

using namespace std;

namespace olsr2_0
{



}


int main()
{
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	olsr2_0::LinkSet loc_linkset;
	olsr2_0::LinkTuple loc_link_tuple;

	loc_link_tuple.L_HEARD_time = 1700;
	loc_link_tuple.L_SYM_time = -1;
	loc_link_tuple.L_Status = olsr2_0::Link_States::HEARD;
	loc_link_tuple.L_lost = false;
	loc_link_tuple.L_mpr_selector = false;
	loc_link_tuple.L_out_metric = 1.0;
	loc_link_tuple.L_in_metric = 0.5;

	cout << "loc_link_tuple info : " << loc_link_tuple.L_Status << endl;
	cout << "loc_link_tuple info : " << loc_link_tuple.L_SYM_time << endl;
	cout << "loc_link_tuple info : " << loc_link_tuple.L_mpr_selector << endl;


	return 0;
}
