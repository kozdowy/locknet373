/***************************************************************************//**
 * (c) Copyright 2012-2015 Microsemi SoC Products Group.  All rights reserved.
 *
 * PHY access methods.
 *
 * SVN $Revision: 7444 $
 * SVN $Date: 2015-06-03 17:42:49 +0530 (Wed, 03 Jun 2015) $
 *
 ******************************************************************************/

#ifndef PHY_H
#define PHY_H    1

#include "mss_ethernet_mac.h"

#ifdef __cplusplus
extern "C" {
#endif 

/***************************************************************************//**
 * Initialize the PHY.
 */
void
MSS_MAC_phy_init
(
    const uint8_t phy_addr,
    const uint8_t allowed_link_speeds
);

/***************************************************************************//**
 * Returns link status.
 *
 * @return #MSS_MAC_LINK_UP if link is up.
 */
uint8_t
MSS_MAC_phy_get_link_status
(
    const uint8_t phy_addr,
    mss_mac_speed_t * speed,
    uint8_t * fullduplex
);

/***************************************************************************//**
 * Sets/Clears the phy loop back mode, based on the enable value
 */
void
MSS_MAC_phy_set_loopback
(
    const uint8_t phy_addr
);

#ifdef __cplusplus
}
#endif

#endif /* PHY_H */
