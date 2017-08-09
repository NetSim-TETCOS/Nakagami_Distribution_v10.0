/************************************************************************************
 * Copyright (C) 2016                                                               *
 * TETCOS, Bangalore. India                                                         *
 *                                                                                  *
 * Tetcos owns the intellectual property rights in the Product and its content.     *
 * The copying, redistribution, reselling or publication of any or all of the       *
 * Product or its content without express prior written consent of Tetcos is        *
 * prohibited. Ownership and / or any other right relating to the software and all  *
 * intellectual property rights therein shall remain at all times with Tetcos.      *
 *                                                                                  *
 * Author:    Shashi Kant Suman                                                     *
 *                                                                                  *
 * ---------------------------------------------------------------------------------*/
#include "main.h"
#include "IEEE802_11_Phy.h"

int fn_NetSim_IEEE802_11_PropagationModelCallback(NETSIM_ID nodeId)
{
	return fn_NetSim_IEEE802_11_PropagationModel(nodeId, pstruEventDetails->dEventTime);
}
/**
This function used to calculate the received from any wireless node other wireless
nodes in the network. It check is there any interference between the adjacent channel.
If any interference then it consider that effect to calculate the received power.
*/
int fn_NetSim_IEEE802_11_PropagationModel(NETSIM_ID nodeId,double time)
{
	NETSIM_ID* ifids;
	NETSIM_ID txif,rxif;
	unsigned int c;
	NETSIM_ID i;

	ifids = get_IEEE802_11_Interface(nodeId,&c);
	if(ifids)
	{
		txif = ifids[0];
		free(ifids);
	}
	else
		return 1;

	for(i=1;i<=NETWORK->nDeviceCount;i++)
	{		
		ifids= get_IEEE802_11_Interface(i,&c);
		if(!ifids)
		{
			free(ifids);
			continue;
		}
		else
		{
			rxif=ifids[0];
			free(ifids);
		}

		//Distance will change between Tx and Rx due to mobility.
		// So update the power from both side
		PTX_INFO info = get_tx_info(propagationHandle, nodeId, txif, i, rxif);
		info->dTx_Rx_Distance = DEVICE_DISTANCE(nodeId, i);
		propagation_calculate_received_power(propagationHandle,
											 nodeId,txif,i,rxif,time);

		info = get_tx_info(propagationHandle, i, rxif, nodeId, txif);
		info->dTx_Rx_Distance = DEVICE_DISTANCE(i, nodeId);
		propagation_calculate_received_power(propagationHandle,
											 i, rxif, nodeId, txif, time);
	}
	return 0;
}
