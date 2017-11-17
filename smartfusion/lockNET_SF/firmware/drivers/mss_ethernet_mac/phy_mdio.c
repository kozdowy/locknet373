/***************************************************************************//**
 * (c) Copyright 2014-2015 Microsemi SoC Products Group.  All rights reserved.
 *
 * PHY MDIO access methods.
 *
 * SVN $Revision: 7444 $
 * SVN $Date: 2015-06-03 17:42:49 +0530 (Wed, 03 Jun 2015) $
 *
 ******************************************************************************/

#include "../../CMSIS/a2fxxxm3.h"
#include "../../CMSIS/mss_assert.h"

#include "mss_ethernet_mac_regs.h"
#include "mss_ethernet_mac.h"
#include "phy.h"
#include "phy_mdio.h"

#ifdef __cplusplus
extern "C" {
#endif 

/***************************** MDIO FUNCTIONS *********************************/

/********************************* Defines ************************************/
#define MDIO_START_BITS           0x00004000u
#define MDIO_READ_BITS            0x00002000u
#define MDIO_WRITE_BITS           0x00001002u
#define MDIO_ADDR_OFFSET          7u
#define MDIO_ADDR_MASK            0x00000f80u
#define MDIO_REG_ADDR_OFFSET      2u
#define MDIO_REG_ADDR_MASK        0x0000007cu
#define PREAMBLECOUNT             32u
#define ONEMICROSECOND            20u

typedef enum {
    MDIO_CMD_READ,
    MDIO_CMD_WRITE
} mdio_cmd_t;


static void
management_clock
(
    const int32_t clock
);

static void
send_cmd
(
    const uint8_t phy_addr,
    const uint8_t regad,
    const mdio_cmd_t mdio_cmd
);

/***************************************************************************//**
 * Set clock high or low.
 */
static void
management_clock
(
    const int32_t clock
)
{
    volatile int32_t delay_idx = 0;
    
    if(clock != 0u)
    {
        MAC->CSR9 |= CSR9_MDC_MASK;
    }
    else
    {
        MAC->CSR9 &= ~CSR9_MDC_MASK;
    
    }

    /* Delay for 1us */
    while (delay_idx < ONEMICROSECOND)
    {
        /*
         * Increment delay_idx via a non-volatile variable to avoid delay++
         * construct resulting in a compliance violation (volatile variable in 
         * complex expression).
         */
        int32_t temp_idx = delay_idx;
        temp_idx++;
        delay_idx = temp_idx;
    }
}


/****************************************************************************//**
 * Send read or write command to PHY.
 */
static void
send_cmd
(
    const uint8_t phy_addr,
    const uint8_t regad,
    const mdio_cmd_t mdio_cmd
)
{
    int32_t idx;
    uint16_t mask;
    uint16_t data;

    /* Enable MII output */
    MAC->CSR9 |= CSR9_MII_MASK;

    /* Send 32 1's preamble */
    MAC->CSR9 |= CSR9_MDO_MASK;

    for (idx = 0u; idx < PREAMBLECOUNT; idx++)
    {
        management_clock(0u);
        management_clock(1u);
    }

    /* Calculate data bits */
    data = MDIO_START_BITS |
        (( mdio_cmd == MDIO_CMD_READ ) ? MDIO_READ_BITS : MDIO_WRITE_BITS ) |
        (((uint16_t)phy_addr << MDIO_ADDR_OFFSET) & MDIO_ADDR_MASK) |
        (((uint16_t)regad << MDIO_REG_ADDR_OFFSET) & MDIO_REG_ADDR_MASK);

    /* Send out */
    mask = 0x8000u;
    while (mask > 0u)
    {
        if ((mask == 0x2u) && (mdio_cmd == MDIO_CMD_READ))
        {
            /* Enable MII input */
            MAC->CSR9 &= ~CSR9_MII_MASK;              
        }
        
        management_clock(0u);
        
        /* Prepare MDO */
        if((uint32_t)(mask & data) != 0u)
        {
            MAC->CSR9 |= CSR9_MDO_MASK;
        }
        else
        {
            MAC->CSR9 &= ~CSR9_MDO_MASK;      
        }
        management_clock(1u);
        mask = mask >> 1u;
    }
}

/****************************************************************************//**
 * Reads a PHY register.
 */
uint16_t
MSS_MAC_mdio_read
(
    const uint8_t phy_addr,
    const uint8_t regad
)
{
    uint16_t mask = 0x8000u;
    uint16_t data = 0u;

    send_cmd(phy_addr, regad, MDIO_CMD_READ);

    /* Read data */
    while (mask > 0u)
    {
        management_clock(0u);

        /* Read MDI */
        if (((MAC->CSR9 & CSR9_MDI_MASK) >> CSR9_MDI) != 0u)
        {
            data |= mask;
        }
        management_clock(1u);
        mask = mask >> 1u;
    }

    management_clock(0u);

    return data;
}

/***************************************************************************//**
 * Writes to a PHY register.
 */
void
MSS_MAC_mdio_write
(
    const uint8_t phy_addr,
    const uint8_t regad,
    const uint16_t data
)
{
    uint16_t mask = 0x8000u;

    send_cmd(phy_addr, regad, MDIO_CMD_WRITE);

    /* Write data */
    while (mask > 0u)
    {
        management_clock(0u);

        /* Prepare MDO */
        if((uint32_t)(mask & data) != 0u)
        {
            MAC->CSR9 |= CSR9_MDO_MASK;
        }
        else
        {
            MAC->CSR9 &= ~CSR9_MDO_MASK;       
        }
        
        management_clock(1U);
        mask = mask >> 1U;
    }

    management_clock(0U);
}

#ifdef __cplusplus
}
#endif

/******************************** END OF FILE *********************************/

