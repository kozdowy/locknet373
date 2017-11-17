/***************************************************************************//**
 * (c) Copyright 2015 Microsemi SoC Products Group.  All rights reserved.
 *
 * PHY MDIO access methods.
 *
 * SVN $Revision: 7444 $
 * SVN $Date: 2015-06-03 17:42:49 +0530 (Wed, 03 Jun 2015) $
 *
 ******************************************************************************/
#ifndef PHY_MDIO_H
#define PHY_MDIO_H      1

#ifdef __cplusplus
extern "C" {
#endif

uint16_t
MSS_MAC_mdio_read
(
    const uint8_t phy_addr,
    const uint8_t regad
);

void
MSS_MAC_mdio_write
(
    const uint8_t phy_addr,
    const uint8_t regad,
    const uint16_t data
);

#ifdef __cplusplus
}
#endif

#endif  /* PHY_MDIO_H */
