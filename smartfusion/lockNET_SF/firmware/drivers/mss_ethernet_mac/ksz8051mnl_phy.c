/***************************************************************************//**
 * (c) Copyright 2012-2015 Microsemi SoC Products Group.  All rights reserved.
 *
 * PHY access methods for Micrel KSZ8051MNL.
 *
 * SVN $Revision: 7506 $
 * SVN $Date: 2015-07-01 10:29:36 +0530 (Wed, 01 Jul 2015) $
 *
 ******************************************************************************/

#include "phy.h"
#include "phy_mdio.h"
#include "mss_ethernet_mac.h"

/*
 * PHY registers.
 */
#define PHYREG_MIIMCR                       ((uint8_t)0x00u)    /**< MII Management Control Register */

#define MIIMCR_RESET                        (1u << 15u)
#define MIIMCR_LOOPBACK                     (1u << 14u)
#define MIIMCR_SPEED_SELECT                 (1u << 13u)
#define MIIMCR_ENABLE_AUTONEGOTIATION       (1u << 12u)
#define MIIMCR_RESTART_AUTONEGOTIATION      (1u << 9u)
#define MIIMCR_DUPLEX_MODE                  (1u << 8u)
#define MIIMCR_COLLISION_TEST               (1u << 7u)

#define PHYREG_MIIMSR                       0x01u    /**< MII Management Status Register */
#define MIIMSR_ANC                          (1u << 5u)    /**< Auto-Negotiation Completed. */

#define PHYREG_ANAR                         0x04u    /**< Auto-Negotiation Advertisement Register */
#define ANAR_100FD                          (1u << 8u)
#define ANAR_100HD                          (1u << 7u)
#define ANAR_10FD                           (1u << 6u)
#define ANAR_10HD                           (1u << 5u)

#define PHYREG_MPCTRL1                      0x1Eu    /**< Micrel PHYCTRL1 */

/*
 * Mask of the "Operation Mode Indication" bits in the PHY Control 1 register.
 * The value of these bits is used retrieve the current link status.
 */
#define OPERATION_MODE_MASK                 0x07u

/*
 * Data structure used to build a look-up table of the link status indexed on
 * the value of the PHY's "Operation Mode Indication" register field.
 */
typedef struct phy_link_status
{
    mss_mac_speed_t speed;
    uint8_t duplex;
    uint8_t link_up;
} phy_link_status_t;

/***************************************************************************//**
 * Initialize and configure the PHY. See "phy.h" for details.
 */
void
MSS_MAC_phy_init
(
    const uint8_t phy_addr,
    const uint8_t allowed_link_speeds
)
{
    uint16_t phy_ctlr_reg;
    uint16_t phy_anar_reg;
    
    /*
     * Reset the PHY
     */
    MSS_MAC_mdio_write(phy_addr, PHYREG_MIIMCR, MIIMCR_RESET);
    while(MIIMCR_RESET & MSS_MAC_mdio_read(phy_addr, PHYREG_MIIMCR))
    {
        ;
    }    
    phy_anar_reg = MSS_MAC_mdio_read(phy_addr, PHYREG_ANAR);
    phy_anar_reg &= ~(ANAR_100FD | ANAR_100HD | ANAR_10FD | ANAR_10HD);
    
    /*
     * Configure duplex mode and link speed.
     */
    phy_ctlr_reg = MIIMCR_COLLISION_TEST;
    
    if ((allowed_link_speeds & MSS_MAC_ANEG_10M_FD) != 0u)
    {
        phy_anar_reg |= ANAR_10FD;
        phy_ctlr_reg |= MIIMCR_ENABLE_AUTONEGOTIATION | MIIMCR_RESTART_AUTONEGOTIATION;
    }
    
    if ((allowed_link_speeds & MSS_MAC_ANEG_10M_HD) != 0u)
    {
        phy_anar_reg |= ANAR_10HD;
        phy_ctlr_reg |= MIIMCR_ENABLE_AUTONEGOTIATION | MIIMCR_RESTART_AUTONEGOTIATION;
    }
    
    if ((allowed_link_speeds & MSS_MAC_ANEG_100M_FD) != 0u)
    {
        phy_anar_reg |= ANAR_100FD;
        phy_ctlr_reg |= MIIMCR_ENABLE_AUTONEGOTIATION | MIIMCR_RESTART_AUTONEGOTIATION;
    }
    
    if ((allowed_link_speeds & MSS_MAC_ANEG_100M_HD) != 0u)
    {
        phy_anar_reg |= ANAR_100HD;
        phy_ctlr_reg |= MIIMCR_ENABLE_AUTONEGOTIATION | MIIMCR_RESTART_AUTONEGOTIATION;
    }
    
    MSS_MAC_MSS_MAC_mdio_write(phy_addr, PHYREG_ANAR, phy_anar_reg);
    MSS_MAC_MSS_MAC_mdio_write(phy_addr, PHYREG_MIIMCR, phy_ctlr_reg);
    
    /*
     * Wait for auto-negotiation to complete.
     */
    {
        int32_t cnt;
        int32_t exit = 1;
        uint16_t reg;
        
        for (cnt = 0; (cnt < 10000) && (exit != 0); cnt++)
        {
            reg = MSS_MAC_mdio_read(phy_addr, PHYREG_MIIMSR);

            if ( (reg & MIIMSR_ANC) != 0u )
            {
                exit = 0;
            }
        }
    }
}

/***************************************************************************//**
 * Returns link status. See "phy.h" for details.
 */
uint8_t
MSS_MAC_phy_get_link_status
(
    const uint8_t phy_addr,
    mss_mac_speed_t * speed,
    uint8_t * fullduplex
)
{
    uint16_t op_mode;
    /*
     * Look-up table translating 3-bit "Operation Mode Indication" found in
     * bits [2:0] of PHY Control 1 register into link status.
     */
    const phy_link_status_t operating_mode_lut[8] =
    {
        {MSS_MAC_INVALID_SPEED, MSS_MAC_HALF_DUPLEX, MSS_MAC_LINK_DOWN},    /* Auto negoatiating. */
        {MSS_MAC_10MBPS, MSS_MAC_HALF_DUPLEX, MSS_MAC_LINK_UP},             /* 10Mbps, half-duplex. */
        {MSS_MAC_100MBPS, MSS_MAC_HALF_DUPLEX, MSS_MAC_LINK_UP},            /* 100Mbps, half-duplex. */
        {MSS_MAC_INVALID_SPEED, MSS_MAC_HALF_DUPLEX, MSS_MAC_LINK_DOWN},    /* Reserved. */
        {MSS_MAC_INVALID_SPEED, MSS_MAC_HALF_DUPLEX, MSS_MAC_LINK_DOWN},    /* Reserved. */
        {MSS_MAC_10MBPS, MSS_MAC_FULL_DUPLEX, MSS_MAC_LINK_UP},             /* 10Mbps, full-duplex. */
        {MSS_MAC_100MBPS, MSS_MAC_FULL_DUPLEX, MSS_MAC_LINK_UP},            /* 100Mbps, full-duplex. */
        {MSS_MAC_INVALID_SPEED, MSS_MAC_HALF_DUPLEX, MSS_MAC_LINK_DOWN}     /* Reserved. */
    };

    op_mode = MSS_MAC_mdio_read(phy_addr, PHYREG_MPCTRL1) & OPERATION_MODE_MASK;
    *speed = operating_mode_lut[op_mode].speed;
    *fullduplex = operating_mode_lut[op_mode].duplex;

    return operating_mode_lut[op_mode].link_up;
}

/***************************************************************************//**
 * Puts the PHY in Loopback mode. See "phy.h" for details.
 */
void
MSS_MAC_phy_set_loopback
(
    const uint8_t phy_addr
)
{
    uint16_t reg;

    reg = MIIMCR_LOOPBACK | MIIMCR_SPEED_SELECT | MIIMCR_DUPLEX_MODE;
    MSS_MAC_mdio_write(phy_addr, PHYREG_MIIMCR, reg);
}
