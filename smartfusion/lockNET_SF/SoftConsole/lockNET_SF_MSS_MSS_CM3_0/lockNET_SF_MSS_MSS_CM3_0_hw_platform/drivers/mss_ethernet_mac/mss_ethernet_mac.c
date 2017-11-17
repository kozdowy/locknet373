/*******************************************************************************
 * (c) Copyright 2012-2015 Microsemi SoC Products Group.  All rights reserved.
 *
 * SmartFusion Microcontroller Subsystem 10/100 Mbps Ethernet MAC bare
 * metal software driver implementation.
 *
 *
 * SVN $Revision: 7582 $
 * SVN $Date: 2015-07-23 12:53:17 +0530 (Thu, 23 Jul 2015) $
 *
*******************************************************************************/
#include <string.h>

#include "../../CMSIS/a2fxxxm3.h"
#include "../../CMSIS/mss_assert.h"
#include "mss_ethernet_mac.h"
#include "mss_ethernet_mac_regs.h"
#include "mss_ethernet_mac_desc.h"
#include "crc32.h"
#include "phy.h"
#include "phy_mdio.h"

#ifdef __cplusplus
extern "C" {
#endif
/**************************** INTERNAL DEFINES ********************************/
#define NULL_POINTER                0

#define INVALID_INDEX               (-1)

#define DESC_OWNED_BY_DRIVER        ((uint32_t)0u)
#define TX_DESC_AVAILABLE           ((uint32_t)0xFFFFFFFFu)

#define MAC_EVENT_NONE              ((uint32_t)0u)
#define MAC_EVENT_PACKET_SEND       ((uint32_t)1u)
#define MAC_EVENT_PACKET_RECEIVED   ((uint32_t)2u)

#define PHY_ADDRESS_MIN             0u
#define PHY_ADDRESS_MAX             31u

/**
 * Descriptor byte ordering mode.
 * 1 - Big-endian mode used for data descriptors <br>
 * 0 - Little-endian mode used for data descriptors <br>
 */
#define MAC_DESCRIPTOR_BYTE_ORDERING_MODE   LITTLEENDIAN

/**
 * Big/little endian.
 * Selects the byte-ordering mode used by the data buffers.
 * 1 - Big-endian mode used for the data buffers
 * 0 - Little-endian mode used for the data buffers
 */
#define MAC_BUFFER_BYTE_ORDERING_MODE       LITTLEENDIAN

/**************************************************************************/
/* Global variables                                                       */
/**************************************************************************/

static mss_mac_instance_t g_mac;


/**************************************************************************/
/* Private Functions                                                      */
/**************************************************************************/

static void mac_reset(void);
static void config_mac_hw(const mss_mac_cfg_t * cfg);
static void tx_desc_ring_init(void);
static void rx_desc_ring_init(void);

static void stop_transmission( void );
static void start_transmission( void );
static void stop_receiving( void );
static void start_receiving( void );

static void enable_tx_interrupt( void );
static void disable_tx_interrupt( void );
static void enable_rx_interrupt( void );
static void disable_rx_interrupt( void );
static void txpkt_handler( void );
static void rxpkt_handler( void );

static void update_tx_statistics( int16_t tx_desc_index );
static void update_rx_statistics ( int16_t rx_desc_index );

static int16_t increment_tx_index(int16_t index);
static int16_t increment_rx_index(int16_t index);

static uint32_t adjust_tx_pkt_length(uint32_t tx_length);
static uint32_t get_rx_frame_errors(uint32_t descriptor0);
static uint32_t get_rx_packet_length(uint32_t descriptor0);

static uint8_t probe_phy( void );
static void send_setup_frame(const uint8_t * setup_frame, uint32_t option);

/**************************************************************************/
/* Public Functions                                                       */
/**************************************************************************/
void EthernetMAC_IRQHandler(void);

 
/*------------------------------------------------------------------------------
 * Test harness global variables.
 */
#ifdef MSCC_ETHERNET_MAC_TEST_HARNESS_FORCE_RX_ERRORS
volatile uint32_t g_mac_test_harness_force_rx_error = 0u;
#endif

/***************************************************************************//**
 See mss_ethernet_mac.h for details of how to use this function
*/
void
MSS_MAC_cfg_struct_def_init
(
    mss_mac_cfg_t * cfg
)
{
    const uint8_t default_mac_addr[6] = { 0xc0u, 0xb1u, 0x3cu, 0x88u, 0x88u, 0x82u };

    ASSERT(NULL_POINTER != cfg);

    if (NULL_POINTER != cfg)
    {
        cfg->link_speed         = MSS_MAC_ANEG_ALL_SPEEDS;
        cfg->loopback           = MSS_MAC_DISABLE;
        cfg->receive_all        = MSS_MAC_DISABLE;
        cfg->tx_threshold_mode  = MSS_MAC_CFG_TX_THRESHOLD_100MB_MODE;
        cfg->store_and_forward  = MSS_MAC_ENABLE;
        cfg->threshold_control  = MSS_MAC_CFG_THRESHOLD_CONTROL_00;
        cfg->pass_all_multicast = MSS_MAC_ENABLE;
        cfg->promiscous_mode    = MSS_MAC_DISABLE;
        cfg->pass_badframe      = MSS_MAC_DISABLE;
        cfg->phy_addr           = 0u;

        /* Set default MAC address */
        (void)memcpy(cfg->mac_addr, default_mac_addr, 6u);
    }
}

/***************************************************************************//**
 See mss_ethernet_mac.h for details of how to use this function
*/
void
MSS_MAC_init
(
    mss_mac_cfg_t * cfg
)
{

    ASSERT(cfg != NULL_POINTER);

    if (cfg != NULL_POINTER)
    {
        mss_mac_speed_t speed = MSS_MAC_INVALID_SPEED;
        uint8_t fullduplex = MSS_MAC_FULL_DUPLEX;
        
        NVIC_DisableIRQ(EthernetMAC_IRQn);
        /* Reset MAC */
        SYSREG->SOFT_RST_CR |= SYSREG_MAC_SOFTRESET_MASK;
        /* Clear any previously pended MAC interrupt */
        NVIC_ClearPendingIRQ(EthernetMAC_IRQn);
        /* Take MAC out of reset. */
        SYSREG->SOFT_RST_CR &= ~SYSREG_MAC_SOFTRESET_MASK;
        
        mac_reset();  
        
        MAC->CSR5 |= CSR5_UNF_MASK;
        /* Clear interrupts */
        MAC->CSR5 |= CSR5_INT_BITS;

        /* Instance setup */
        (void)memset(&g_mac, 0u, sizeof(g_mac));

        /* Initialize Tx & Rx descriptor rings */
        tx_desc_ring_init();
        rx_desc_ring_init();


        /* Initialize Tx descriptors related variables. */
        g_mac.first_tx_index = INVALID_INDEX;
        g_mac.last_tx_index = INVALID_INDEX;
        g_mac.next_free_tx_index = 0u;
        
        /* Initialize Rx descriptors related variables. */
        g_mac.next_free_rx_desc_index = 0u;
        g_mac.first_rx_desc_index = 0u;

        /* Initialize link status monitoring variables. */
        g_mac.previous_speed = MSS_MAC_INVALID_SPEED;
        g_mac.previous_duplex_mode = MSS_MAC_HALF_DUPLEX;

        /* initialize default interrupt handlers */
        g_mac.tx_complete_handler = NULL_POINTER;
        g_mac.pckt_rx_callback = NULL_POINTER;
        
        /* Configurable settings */

        MAC->CSR0 &= ~(CSR0_DBO_MASK | CSR0_BLE_MASK | CSR0_BAR_MASK);
        MAC->CSR0 |=  (uint32_t)((MSS_MAC_PROGRAMMABLE_BURST_LENGTH << CSR0_PBL) | \
                                  (MSS_MAC_BUS_ARBITRATION_SCHEME << CSR0_BAR));
        /* Fixed settings */
        /* No automatic polling */        
        MAC->CSR0 &= ~(CSR0_TAP_MASK);

        /* No space between descriptors */
        MAC->CSR0 &= ~(CSR0_DSL_MASK);

        /* General-purpose timer works in one-shot mode */
        MAC->CSR11 &= ~(CSR11_CON_MASK);
        /* Start general-purpose timer*/
        MAC->CSR11 |= CSR11_TIM_MASK;

        /* Disable promiscuous mode */
        MAC->CSR6 &= ~(CSR6_PR_MASK);

        /* Enable store and forward */
        MAC->CSR6 |= CSR6_SF_MASK;

        config_mac_hw(cfg);
        
        /* Set descriptors */
        MAC->CSR3 = (uint32_t)&(g_mac.rx_descriptors[0].descriptor_0);
        MAC->CSR4 = (uint32_t)&(g_mac.tx_descriptors[0].descriptor_0);
        

        (void)memcpy(g_mac.local_mac_addr, cfg->mac_addr, 6u);

        MSS_MAC_set_address_filter(g_mac.local_mac_addr, 0u);
        
        /* Detect PHY */
        if (MSS_MAC_AUTO_DETECT_PHY_ADDRESS == cfg->phy_addr)
        {
            g_mac.phy_address = probe_phy();
            ASSERT( g_mac.phy_address <= PHY_ADDRESS_MAX );
        }
        else
        {
            g_mac.phy_address = cfg->phy_addr;
        }

        /* Reset PHY */
        MSS_MAC_phy_init(g_mac.phy_address, cfg->link_speed);
        
        if(MSS_MAC_ENABLE == cfg->loopback)
        {
            MSS_MAC_phy_set_loopback(g_mac.phy_address);
        }

        /* Configure MAC according to PHY link status */
        (void)MSS_MAC_get_link_status(&speed, &fullduplex);

        /* enable normal interrupts */
        MAC->CSR7 |= CSR7_NIE_MASK; 
    }
}

/***************************************************************************//**
 See mss_ethernet_mac.h for details of how to use this function
*/
uint8_t
MSS_MAC_send_pkt
(
    uint8_t const * tx_buffer,
    uint32_t tx_length,
    void * p_user_data
)
{
    uint8_t error;
    int16_t first_tx_idx;
    int16_t next_free_tx_idx;
    uint32_t ownership;

    ASSERT( tx_buffer != NULL_POINTER );
    ASSERT( tx_length >= 12u );
    ASSERT( tx_length <= MSS_MAC_MAX_PACKET_SIZE );

    /*--------------------------------------------------------------------------
     * Treat this function as a critical section with regard to the tx packet
     * handler interrupt service routine.
     */
    disable_tx_interrupt();
    
    first_tx_idx = g_mac.first_tx_index;
    next_free_tx_idx = g_mac.next_free_tx_index;
    
    /*--------------------------------------------------------------------------
     * Reclaim a transmit descriptor for packets that have already been sent but
     * for which a transmit interrupt has not occured yet.
     */
    if (next_free_tx_idx == first_tx_idx)
    {
        ownership = g_mac.tx_descriptors[first_tx_idx].descriptor_0 & TDES0_OWN;
        if (DESC_OWNED_BY_DRIVER == ownership)
        {
            update_tx_statistics(first_tx_idx);
            if (g_mac.tx_complete_handler != NULL_POINTER)
            {
                g_mac.tx_complete_handler(g_mac.tx_descriptors[first_tx_idx].caller_info);
            }
            first_tx_idx = increment_tx_index(first_tx_idx);
        }
    }
    
    /*--------------------------------------------------------------------------
     * Send frame if there is an available transmit descriptor.
     */
    if (next_free_tx_idx != first_tx_idx)
    {
        uint32_t tx_pkt_length;
        volatile mss_mac_tx_descriptor_t * p_tx_desc = &(g_mac.tx_descriptors[next_free_tx_idx]);
        
        if (INVALID_INDEX == first_tx_idx)
        {
            first_tx_idx = next_free_tx_idx;
        }
        g_mac.last_tx_index = next_free_tx_idx;

        tx_pkt_length = adjust_tx_pkt_length(tx_length);
        
        /* Every buffer can hold a full frame so they are always first and last
           descriptor
        */
        p_tx_desc->descriptor_1 = TDES1_TCH | TDES1_LS | TDES1_FS
                                  | (tx_pkt_length << TDES1_TBS1_OFFSET);
        p_tx_desc->buffer_1 = tx_buffer;
        p_tx_desc->caller_info = p_user_data;
        next_free_tx_idx = increment_tx_index(next_free_tx_idx);
        
        /* Give ownership of descriptor to Ethernet MAC */
        p_tx_desc->descriptor_0 = TDES0_OWN;

        start_transmission();

        /* transmit poll demand */
        MAC->CSR1 = 1u;
        
        error = MSS_MAC_SUCCESS;
    }
    else
    {
        error = MSS_MAC_FAILED;
    }
    
    /*--------------------------------------------------------------------------
     * Update instance transmit variables and quit critical section.
     */
    g_mac.first_tx_index = first_tx_idx;
    g_mac.next_free_tx_index = next_free_tx_idx;
    
    enable_tx_interrupt();
    
    return error;
}

/***************************************************************************//**
 See mss_ethernet_mac.h for details of how to use this function
*/
uint8_t
MSS_MAC_receive_pkt
(
    uint8_t * rx_buffer,
    void * p_user_data
)
{
    uint8_t error;
    int16_t next_rx_desc_index;
    uint32_t ownership;
    
    ASSERT( rx_buffer != NULL_POINTER );
    
    disable_rx_interrupt();
    
    next_rx_desc_index = g_mac.next_free_rx_desc_index;
    ownership = g_mac.rx_descriptors[next_rx_desc_index].descriptor_0 & RDES0_OWN;
    
    if ((DESC_OWNED_BY_DRIVER == ownership) && (rx_buffer != NULL_POINTER))
    {
        g_mac.rx_descriptors[next_rx_desc_index].buffer_1 = rx_buffer;
        g_mac.rx_descriptors[next_rx_desc_index].caller_info = p_user_data;
        g_mac.rx_descriptors[next_rx_desc_index].descriptor_1 = RDES1_RCH | (uint32_t)MSS_MAC_MAX_PACKET_SIZE;
        
        /* Give ownership of descriptor to Ethernet MAC */
        g_mac.rx_descriptors[next_rx_desc_index].descriptor_0 = RDES0_OWN;
        
        /* Point the next_rx_desc to next free descriptor in the ring */
        next_rx_desc_index = increment_rx_index(next_rx_desc_index);
        
        g_mac.next_free_rx_desc_index = next_rx_desc_index;
        
        enable_rx_interrupt();
        
        start_receiving();
        
        error = MSS_MAC_SUCCESS;
    }
    else
    {
        enable_rx_interrupt();
        error = MSS_MAC_FAILED;
    }
    
    return error;
}

/***************************************************************************//**
 See mss_ethernet_mac.h for details of how to use this function
*/
void MSS_MAC_set_rx_callback
(
    mss_mac_receive_callback_t rx_callback
)
{
    g_mac.pckt_rx_callback = rx_callback;
}

/***************************************************************************//**
 See mss_ethernet_mac.h for details of how to use this function
*/
void MSS_MAC_set_tx_callback
(
    mss_mac_transmit_callback_t tx_complete_handler
)
{
    /* disable tx interrupts */    
    g_mac.tx_complete_handler = tx_complete_handler;
}

/***************************************************************************//**
 See mss_ethernet_mac.h for details of how to use this function
*/
uint8_t
MSS_MAC_get_link_status
(
    mss_mac_speed_t * speed,
    uint8_t * fullduplex
)
{
    uint32_t link;
    
    ASSERT( speed != NULL_POINTER );
    ASSERT( fullduplex != NULL_POINTER );
    
    link = MSS_MAC_phy_get_link_status(g_mac.phy_address,
                                   speed,
                                   fullduplex);

    /*
     * Update the MAC's configuration if the link's speed of duplex mode changed
     * since the last time MAC_get_link_status() was called.
     */
    if ((MSS_MAC_LINK_UP == link) &&
        ((*speed != g_mac.previous_speed) ||
         (*fullduplex != g_mac.previous_duplex_mode)))
    {
        g_mac.previous_speed = *speed;
        g_mac.previous_duplex_mode = *fullduplex;
        
        stop_transmission();

        stop_receiving();

        if(MSS_MAC_100MBPS == *speed)
        {
            MAC->CSR6 |= CSR6_TTM_MASK; 
            MAC->CSR0 |= CSR0_SPD_MASK; 
        }
        else
        {
            MAC->CSR6 &= ~(CSR6_TTM_MASK); 
            MAC->CSR0 &= ~(CSR0_SPD_MASK); 
        }
        
        if(MSS_MAC_FULL_DUPLEX == *fullduplex)
        {
            MAC->CSR6 |= CSR6_FD_MASK; 
        }
        else
        {
            MAC->CSR6 &= ~(CSR6_FD_MASK);        
        }
        
        start_transmission();
        
        start_receiving();

    }
    return (link);
}
/**************************************************************************//**
 * 
 */
static void 
mac_reset
(
    void
)
{
    uint32_t soft_reset_bit;
    
    /* Try to reset Ethernet MAC */
    MAC->CSR0 |= CSR0_SWR_MASK;    
    /*Wait for the reset to complete. Indicated by self clearing of CSRO_SWR*/
    do
    {
        soft_reset_bit = ((MAC->CSR0 & CSR0_SWR_MASK) >> CSR0_SWR) ;
    }
    while (soft_reset_bit!= 0u);
}

/***************************************************************************//**
 See mss_ethernet_mac.h for details of how to use this function
*/
#define SETUP_FRAME_LENGTH      192u
#define MAC_ADDR_LENGTH         6u

#define PERFECT_FILTERING       1u
#define HASH_FILTERING          2u

static void
send_setup_frame
(
    const uint8_t * setup_frame,
    uint32_t option
)
{
    mss_mac_tx_descriptor_t descriptor;
    uint32_t tx_process_state;
    uint32_t initial_rx_state;

    /* prepare descriptor */
    descriptor.descriptor_0 = TDES0_OWN;
    descriptor.descriptor_1 = TDES1_SET | TDES1_TER |(SETUP_FRAME_LENGTH << TDES1_TBS1_OFFSET);

    if (HASH_FILTERING == option)
    {
        descriptor.descriptor_1 |= TDES1_FT0;
    }

    descriptor.buffer_1 = setup_frame;
    descriptor.buffer_2 = 0u;

    /* Stop transmission */
    stop_transmission();

    initial_rx_state = ((MAC->CSR5 & CSR5_RS_MASK) >> CSR5_RS);
    stop_receiving();

    /* Set descriptor */
    MAC->CSR4 = (uint32_t)&descriptor;
    /* Start transmission */
    start_transmission();

    do
    {
        /* transmit poll demand */
        MAC->CSR1 = 1u;
        tx_process_state = ((MAC->CSR5 & CSR5_TS_MASK) >> CSR5_TS);
    }
    while (tx_process_state != CSR5_TS_SUSPENDED);

     stop_transmission();

    /* Set tx descriptor */
    MAC->CSR4 = (uint32_t)g_mac.tx_descriptors;
    /* Start receiving and transmission */
    if (initial_rx_state != CSR5_RS_STOPPED)
    {
        start_receiving();
    }
}

/***************************************************************************//**
 See mss_ethernet_mac.h for details of how to use this function
*/
void MSS_MAC_set_address_filter
(
    const uint8_t * mac_addresses,
    uint32_t nb_addresses
)
{
    uint8_t setup_frame[SETUP_FRAME_LENGTH];
    
    (void)memset(setup_frame, 0u, SETUP_FRAME_LENGTH);
    
    if (nb_addresses <= 15u)
    {
        /*----------------------------------------------------------------------
         * Use perfect filtering.
         */
        uint32_t inc;
        uint32_t in_mac_addr_offset;
        uint32_t frame_addr_offset;

        /* Copy local MAC address into filter setup frame. */
        setup_frame[0] = g_mac.local_mac_addr[0];
        setup_frame[1] = g_mac.local_mac_addr[1];
        setup_frame[4] = g_mac.local_mac_addr[2];
        setup_frame[5] = g_mac.local_mac_addr[3];
        setup_frame[8] = g_mac.local_mac_addr[4];
        setup_frame[9] = g_mac.local_mac_addr[5];
        
        in_mac_addr_offset = 0u;
        frame_addr_offset = 12u;
        for (inc = 0u; inc < nb_addresses; ++inc)
        {
            setup_frame[frame_addr_offset] = mac_addresses[in_mac_addr_offset];
            setup_frame[frame_addr_offset + 1] = mac_addresses[in_mac_addr_offset + 1];
            setup_frame[frame_addr_offset + 4] = mac_addresses[in_mac_addr_offset + 2];
            setup_frame[frame_addr_offset + 5] = mac_addresses[in_mac_addr_offset + 3];
            setup_frame[frame_addr_offset + 8] = mac_addresses[in_mac_addr_offset + 4];
            setup_frame[frame_addr_offset + 9] = mac_addresses[in_mac_addr_offset + 5];
            
            in_mac_addr_offset += MAC_ADDR_LENGTH;
            frame_addr_offset += 12u;
        }
        
        /* Fill remaining filter address locations with local MAC address. */
        for (inc = nb_addresses; inc < 15u; ++inc)
        {
            setup_frame[frame_addr_offset] = g_mac.local_mac_addr[0];
            setup_frame[frame_addr_offset + 1] = g_mac.local_mac_addr[1];
            setup_frame[frame_addr_offset + 4] = g_mac.local_mac_addr[2];
            setup_frame[frame_addr_offset + 5] = g_mac.local_mac_addr[3];
            setup_frame[frame_addr_offset + 8] = g_mac.local_mac_addr[4];
            setup_frame[frame_addr_offset + 9] = g_mac.local_mac_addr[5];
            
            in_mac_addr_offset += MAC_ADDR_LENGTH;
            frame_addr_offset += 12u;
        }
        
        send_setup_frame(setup_frame, PERFECT_FILTERING);
    }
    else
    {
        /*----------------------------------------------------------------------
         * Use hash filtering.
         */
        uint32_t hash;
        uint32_t target_idx = 0u;
        uint32_t src_idx = 0u;
        uint32_t inc;
        
        uint32_t hash_table[64];

        /* Copy local MAC address into filter setup frame. */
        setup_frame[156] = g_mac.local_mac_addr[0];
        setup_frame[157] = g_mac.local_mac_addr[1];
        setup_frame[160] = g_mac.local_mac_addr[2];
        setup_frame[161] = g_mac.local_mac_addr[3];
        setup_frame[164] = g_mac.local_mac_addr[4];
        setup_frame[165] = g_mac.local_mac_addr[5];
        
        /* reset hash table */
        (void)memset(hash_table, 0, 256);

        for (inc = 0u; inc < nb_addresses; ++inc)
        {
            hash = mss_mac_ethernet_crc(&mac_addresses[inc * 6], 6u) & 0x1FFu;
            hash_table[ hash / 8 ] |= 1u << (hash & 0x07u);
        }
        
        for (inc = 0u; inc < 32u; ++inc)
        {
            setup_frame[target_idx] = (uint8_t)hash_table[src_idx];
            ++src_idx;
            ++target_idx;
            setup_frame[target_idx] = (uint8_t)hash_table[src_idx];
            ++src_idx;
            target_idx += 3u;
        }
        
        send_setup_frame(setup_frame, HASH_FILTERING);
    }
}

/***************************************************************************//**
 See mss_ethernet_mac.h for details of how to use this function
*/
uint32_t
MSS_MAC_read_stat
(
    mss_mac_stat_t stat_id
)
{
    uint32_t returnval;

    switch (stat_id)
    {
        case MSS_MAC_RX_INTERRUPTS:
            returnval = g_mac.statistics.rx_interrupts;
        break;

        case MSS_MAC_RX_FILTERING_FAIL:
            returnval = g_mac.statistics.rx_filtering_fail;
        break;

        case MSS_MAC_RX_DESCRIPTOR_ERROR:
            returnval = g_mac.statistics.rx_descriptor_error;
        break;

        case MSS_MAC_RX_RUNT_FRAME:
            returnval = g_mac.statistics.rx_runt_frame;
        break;

        case MSS_MAC_RX_NOT_FIRST:
            returnval = g_mac.statistics.rx_not_first;
        break;

        case MSS_MAC_RX_NOT_LAST:
            returnval = g_mac.statistics.rx_not_last;
        break;

        case MSS_MAC_RX_FRAME_TOO_LONG:
            returnval = g_mac.statistics.rx_frame_too_long;
        break;

        case MSS_MAC_RX_COLLISION_SEEN:
            returnval = g_mac.statistics.rx_collision_seen;
        break;

        case MSS_MAC_RX_CRC_ERROR:
            returnval = g_mac.statistics.rx_crc_error;
        break;

        case MSS_MAC_RX_FIFO_OVERFLOW:
            returnval = g_mac.statistics.rx_fifo_overflow;
        break;

        case MSS_MAC_RX_MISSED_FRAME:
            returnval = g_mac.statistics.rx_missed_frame;
        break;

        case MSS_MAC_TX_INTERRUPTS:
            returnval = g_mac.statistics.tx_interrupts;
        break;

        case MSS_MAC_TX_LOSS_OF_CARRIER:
            returnval = g_mac.statistics.tx_loss_of_carrier;
        break;

        case MSS_MAC_TX_NO_CARRIER:
            returnval = g_mac.statistics.tx_no_carrier;
        break;

        case MSS_MAC_TX_LATE_COLLISION:
            returnval = g_mac.statistics.tx_late_collision;
        break;

        case MSS_MAC_TX_EXCESSIVE_COLLISION:
            returnval = g_mac.statistics.tx_excessive_collision;
        break;

        case MSS_MAC_TX_COLLISION_COUNT:
            returnval = g_mac.statistics.tx_collision_count;
        break;

        case MSS_MAC_TX_UNDERFLOW_ERROR:
            returnval = g_mac.statistics.tx_underflow_error;
        break;

        default:
            returnval = 0u;
        break;
    }

    return returnval;
}

/***************************************************************************//**
 See mss_ethernet_mac.h for details of how to use this function
*/
void MSS_MAC_clear_statistics
(
    void
)
{
    (void)memset(&g_mac.statistics, 0, sizeof(mss_mac_statistics_t));
}

/***************************************************************************//**
 * MAC interrupt service routine.
 *
 */
void
EthernetMAC_IRQHandler
(
    void
)
{
    uint32_t events;
    uint32_t intr_status;
    uint32_t irq_clear = 0u;

    events = 0u;

    intr_status = MAC->CSR5;

    if ((intr_status & CSR5_NIS_MASK) != 0u)
    {
        if ((intr_status & CSR5_TI_MASK) != 0u) /* Transmit */
        {
            g_mac.statistics.tx_interrupts++;
            events |= MAC_EVENT_PACKET_SEND;
            irq_clear |= CSR5_TI_MASK;
        }
        
        if ((intr_status & CSR5_TU_MASK) != 0u) /* Transmit */
        {
            irq_clear |= CSR5_TU_MASK;
        }

        if ((intr_status & CSR5_RI_MASK) != 0u) /* Receive */
        {
            g_mac.statistics.rx_interrupts++;
            events |= MAC_EVENT_PACKET_RECEIVED;
            irq_clear |= CSR5_RI_MASK;
        }
        
        if ((intr_status & CSR5_RU_MASK) != 0u) /* Receive buffer unavailable */
        {
            irq_clear |= CSR5_RU_MASK;
        }
    }

    /* Clear interrupts */
    MAC->CSR5 |= irq_clear;

    if (events != MAC_EVENT_NONE)
    {
        if ((events & MAC_EVENT_PACKET_RECEIVED) == MAC_EVENT_PACKET_RECEIVED)
        {
            rxpkt_handler();
        }
        
        if ((events & MAC_EVENT_PACKET_SEND) == MAC_EVENT_PACKET_SEND)
        {
            txpkt_handler();
        }
    }
}

/**************************** INTERNAL FUNCTIONS ******************************/

/***************************************************************************//**
 * increment_tx_index().
 */
static int16_t increment_tx_index
(
    int16_t index
)
{
    int16_t incremented_idx = index;
    
    if ((MSS_MAC_TX_RING_SIZE - 1) == incremented_idx)
    {
        incremented_idx = 0;
    }
    else
    {
        incremented_idx++;
    }
    
    return incremented_idx;
}

/***************************************************************************//**
 * increment_rx_index().
 */
static int16_t increment_rx_index
(
    int16_t index
)
{
    int16_t incremented_idx = index;
    
    if ((MSS_MAC_RX_RING_SIZE - 1) == incremented_idx)
    {
        incremented_idx = 0;
    }
    else
    {
        incremented_idx++;
    }
    
    return incremented_idx;
}

/***************************************************************************//**
 * adjust_tx_pkt_length().
 */
static uint32_t adjust_tx_pkt_length
(
    uint32_t tx_length
)
{
    uint32_t adjusted_tx_pkt_length;
    
    if (tx_length > MSS_MAC_MAX_PACKET_SIZE)
    {
        adjusted_tx_pkt_length = MSS_MAC_MAX_PACKET_SIZE;
    }
    else
    {
        adjusted_tx_pkt_length = tx_length;
    }
    
    return adjusted_tx_pkt_length;
}

/***************************************************************************//**
 * txpkt_handler().
 */
static void
txpkt_handler
(
    void
)
{
    int16_t first_tx_idx;
    int16_t last_tx_idx;
    uint32_t ownership;
    
    ASSERT(g_mac.first_tx_index != INVALID_INDEX);
    ASSERT(g_mac.last_tx_index != INVALID_INDEX);
    
    last_tx_idx = g_mac.last_tx_index;
    first_tx_idx = g_mac.first_tx_index;
    do
    {
        update_tx_statistics(first_tx_idx);
        if (g_mac.tx_complete_handler != NULL_POINTER)
        {
            g_mac.tx_complete_handler(g_mac.tx_descriptors[first_tx_idx].caller_info);
        }
    
        if (first_tx_idx == last_tx_idx)
        {
            /* all pending tx packets sent. */
            first_tx_idx = INVALID_INDEX;
            last_tx_idx = INVALID_INDEX;
            ownership = TX_DESC_AVAILABLE;
        }
        else
        {
            /* Move on to next transmit descriptor. */
            first_tx_idx = increment_tx_index(first_tx_idx);
            
            /* Check if we reached a descriptor still pending tx. */
            ownership = g_mac.tx_descriptors[first_tx_idx].descriptor_0 & TDES0_OWN;
        }
    }
    while ((DESC_OWNED_BY_DRIVER == ownership) && (first_tx_idx != INVALID_INDEX));
    
    g_mac.last_tx_index = last_tx_idx;
    g_mac.first_tx_index = first_tx_idx;
}

/***************************************************************************//**
 * get_rx_frame_errors().
 * Retrieve the error codes from the RDES0 receive descriptor field for the
 * received packet.
 */
static uint32_t
get_rx_frame_errors
(
    uint32_t descriptor0
)
{
    uint32_t frame_errors;
    uint32_t full_duplex_link;
    uint32_t last_descriptor;
    
    /*
     * Make sure this was the last descriptor.
     */
    last_descriptor =  descriptor0 & RDES0_LS;
    if (RDES0_LS == last_descriptor)
    {
        /*
         * Ensure frame was received without errors. Ignore bits not relevant to
         * the current duplex mode.
         */
        full_duplex_link = ((MAC->CSR6 & CSR6_FD_MASK) >> CSR6_FD);
        if (1u == full_duplex_link)
        {
            frame_errors =  descriptor0 & (RDES0_TL | RDES0_RF | RDES0_DE | RDES0_CE);
        }
        else
        {
            frame_errors =  descriptor0 & (RDES0_TL | RDES0_CS | RDES0_RF | RDES0_DE | RDES0_CE);
        }
    }
    else
    {
        frame_errors = RDES0_LS;
    }
#ifdef MSCC_ETHERNET_MAC_TEST_HARNESS_FORCE_RX_ERRORS
    /*
     * Signal an error on the received packet if requested by the driver test
     * harness.
     */
    if (g_mac_test_harness_force_rx_error > 0U)
    {
        --g_mac_test_harness_force_rx_error;
        frame_errors = RDES0_CE;
    }
#endif
    return frame_errors;
}

/***************************************************************************//**
 * Retrieve the received packet length from the RDES0 receive descriptor field.
 */
static uint32_t
get_rx_packet_length
(
    uint32_t descriptor0
)
{
    uint32_t pckt_length;
    
    if ((descriptor0 & RDES0_OWN) != 0u)
    {
        /* Current descriptor is empty */
        pckt_length = 0u;
    }
    else
    {
        pckt_length = (descriptor0 >> RDES0_FL_OFFSET ) & RDES0_FL_MASK;
        /* strip crc */
        if(pckt_length > 4u)
        {
            pckt_length -= 4u;
        }
        else
        {
            pckt_length = 0u;
        }
    }
    
    return pckt_length;
}

/***************************************************************************//**
 * rxpkt_handler().
 */
static void
rxpkt_handler
(
    void
)
{
    int16_t first_rx_descr_idx;
    int16_t next_free_rx_desc_idx;
    uint32_t ownership;
    
    first_rx_descr_idx = g_mac.first_rx_desc_index;
    next_free_rx_desc_idx = g_mac.next_free_rx_desc_index;
    
    do
    {
        uint32_t frame_errors;
        void * caller_info;
        
        volatile mss_mac_rx_descriptor_t * cdesc = &g_mac.rx_descriptors[first_rx_descr_idx];
        caller_info = cdesc->caller_info;
        
        update_rx_statistics(first_rx_descr_idx);
        
        frame_errors = get_rx_frame_errors(cdesc->descriptor_0);
        if (0u == frame_errors)
        {
            uint32_t pckt_length;
            
            pckt_length = get_rx_packet_length(cdesc->descriptor_0);
            
            if ((pckt_length > 0u) && (g_mac.pckt_rx_callback != NULL_POINTER))
            {
                g_mac.pckt_rx_callback(cdesc->buffer_1, 
                                           pckt_length,
                                           caller_info);
            }
        }
        else
        {
            g_mac.rx_descriptors[next_free_rx_desc_idx].buffer_1 = cdesc->buffer_1;
            g_mac.rx_descriptors[next_free_rx_desc_idx].caller_info = caller_info;
            g_mac.rx_descriptors[next_free_rx_desc_idx].descriptor_1 = cdesc->descriptor_1;
            g_mac.rx_descriptors[next_free_rx_desc_idx].descriptor_0 = RDES0_OWN;
            next_free_rx_desc_idx = increment_rx_index(next_free_rx_desc_idx);
            g_mac.next_free_rx_desc_index = next_free_rx_desc_idx;
            /* Start receive */
            start_receiving();
        }
        
        first_rx_descr_idx = increment_rx_index(first_rx_descr_idx);
        ownership = g_mac.rx_descriptors[first_rx_descr_idx].descriptor_0 & RDES0_OWN;
    }
    while ((DESC_OWNED_BY_DRIVER == ownership) &&
           (first_rx_descr_idx != next_free_rx_desc_idx));
    
    g_mac.first_rx_desc_index = first_rx_descr_idx;
}

/***************************************************************************//**
 * enable_tx_interrupt().
 */
static void enable_tx_interrupt(void)
{
    MAC->CSR7 |= (CSR7_TIE_MASK | CSR7_TUE_MASK);  
}

/***************************************************************************//**
 * disable_tx_interrupt().
 */
static void disable_tx_interrupt(void)
{
    MAC->CSR7 &= ~(CSR7_TIE_MASK | CSR7_TUE_MASK);
}

/***************************************************************************//**
 * enable_rx_interrupt().
 */
static void enable_rx_interrupt(void)
{
    MAC->CSR7 |= CSR7_RIE_MASK ;
}

/***************************************************************************//**
 * disable_rx_interrupt().
 */
static void disable_rx_interrupt(void)
{
    MAC->CSR7 &= ~(CSR7_RIE_MASK) ;
}

/***************************************************************************//**
 * disable_rx_interrupt().
 */
static void
update_tx_statistics
(
    int16_t tx_desc_index
)
{
    uint32_t desc;
    /* update counters */
    desc = g_mac.tx_descriptors[tx_desc_index].descriptor_0;
    
    if ((desc & TDES0_LO) != 0u)
    {
        g_mac.statistics.tx_loss_of_carrier++;
    }
    if ((desc & TDES0_NC) != 0u)
    {
        g_mac.statistics.tx_no_carrier++;
    }
    if ((desc & TDES0_LC) != 0u)
    {
        g_mac.statistics.tx_late_collision++;
    }
    if ((desc & TDES0_EC) != 0u)
    {
        g_mac.statistics.tx_excessive_collision++;
    }
    if ((desc & TDES0_UF) != 0u)
    {
        g_mac.statistics.tx_underflow_error++;
        /* On underflow, the Transmit process goes into suspended state.
           To bring it out of Suspend, issue a stop command*/
        stop_transmission();
    }
    
    g_mac.statistics.tx_collision_count +=
        (desc >> TDES0_CC_OFFSET) & TDES0_CC_MASK;
}

/***************************************************************************//**
 * disable_rx_interrupt().
 */
static void update_rx_statistics
(
    int16_t rx_desc_index
)
{
    uint32_t desc;

    /* update counters */
    desc = g_mac.rx_descriptors[rx_desc_index].descriptor_0;

    if ((desc & RDES0_FF) != 0u)
    {
        g_mac.statistics.rx_filtering_fail++;
    }

    if ((desc & RDES0_DE) != 0u)
    {
        g_mac.statistics.rx_descriptor_error++;
    }

    if ((desc & RDES0_RF) != 0u)
    {
        g_mac.statistics.rx_runt_frame++;
    }

    if ((desc & RDES0_FS) == 0u)
    {
        g_mac.statistics.rx_not_first++;
    }

    if ((desc & RDES0_LS) == 0u)
    {
        g_mac.statistics.rx_not_last++;
    }

    if ((desc & RDES0_TL) != 0u)
    {
        g_mac.statistics.rx_frame_too_long++;
    }

    if ((desc & RDES0_CS) != 0u)
    {
        g_mac.statistics.rx_collision_seen++;
    }

    if ((desc & RDES0_CE) != 0u)
    {
        g_mac.statistics.rx_crc_error++;
    }

    desc = MAC->CSR8;

    g_mac.statistics.rx_fifo_overflow +=
        (desc & (CSR8_OCO_MASK | CSR8_FOC_MASK)) >> CSR8_FOC;

    g_mac.statistics.rx_missed_frame +=
        (desc & (CSR8_MFO_MASK | (uint32_t)CSR8_MFC_MASK));
}

/***************************************************************************//**
 * Stops transmission.
 * Function will wait until transmit operation enters stop state.
 */
static void stop_transmission(void)
{
    uint32_t tx_process_state;
    
    do
    {
        MAC->CSR6 &= ~(CSR6_ST_MASK);
        tx_process_state = ((MAC->CSR5 & CSR5_TS_MASK) >> CSR5_TS);
    }
    while (tx_process_state != CSR5_TS_STOPPED);
}

/***************************************************************************//**
 * Starts transmission.
 */
static void start_transmission(void)
{
    MAC->CSR6 |= CSR6_ST_MASK;
}

/***************************************************************************//**
 * Stops transmission.
 * Function will wait until transmit operation enters stop state.
 */
static void stop_receiving(void)
{
    uint32_t rx_process_state;
    
    do
    {
        MAC->CSR6 &= ~(CSR6_SR_MASK);
        rx_process_state = ((MAC->CSR5 & CSR5_RS_MASK) >> CSR5_RS);
    }
    while (rx_process_state != CSR5_RS_STOPPED);
}

/***************************************************************************//**
 * Starts transmission.
 */
static void start_receiving(void)
{
    MAC->CSR6 |= CSR6_SR_MASK;
}

/***************************************************************************//**
 *
 */
static void config_mac_hw
(
    const mss_mac_cfg_t * cfg
)
{
    
    stop_transmission();
    stop_receiving();
    
    if(cfg->receive_all != 0u)
    {
        MAC->CSR6 |= CSR6_RA_MASK;
    }
    else
    {
        MAC->CSR6 &= ~CSR6_RA_MASK;
    }
 
    if(cfg->tx_threshold_mode != 0u)
    {
        MAC->CSR6 |= CSR6_TTM_MASK;
    }
    else
    {
        MAC->CSR6 &= ~CSR6_TTM_MASK;
    }
    
    if(cfg->store_and_forward  != 0u)
    {
        MAC->CSR6 |= CSR6_SF_MASK;
    }
    else
    {
        MAC->CSR6 &= ~CSR6_SF_MASK;
    }

    MAC->CSR6 |= (uint32_t)(cfg->threshold_control << 14u);
    
    if(cfg->pass_all_multicast != 0u)
    {
        MAC->CSR6 |= CSR6_PM_MASK;
    }
    else
    {
        MAC->CSR6 &= ~CSR6_PM_MASK;
    }
    
    if(cfg->promiscous_mode != 0u)
    {
        MAC->CSR6 |= CSR6_PR_MASK;
    }
    else
    {
        MAC->CSR6 &= ~CSR6_PR_MASK;
    }
    
    if(cfg->pass_badframe  != 0u)
    {
        MAC->CSR6 |= CSR6_PB_MASK;
    }
    else
    {
        MAC->CSR6 &= ~CSR6_PB_MASK;
    }
}

/***************************************************************************//**
 * Auto-detect the PHY's address by attempting to read the PHY identification
 * register containing the PHY manufacturer's identifier.
 * Attempting to read a PHY register using an incorrect PHY address will result
 * in a value with all bits set to one on the MDIO bus. Reading any other value
 * means that a PHY responded to the read request, therefore we have found the
 * PHY's address.
 * This function returns the detected PHY's address or 32 (PHY_ADDRESS_MAX + 1)
 * if no PHY is responding.
 */
static uint8_t probe_phy(void)
{
    uint8_t phy_address = PHY_ADDRESS_MIN;
    const uint16_t ALL_BITS_HIGH = 0xffffu;
    uint8_t PHYREG_PHYID1R = 0x02u;   /* PHY Identifier 1 register address. */
    uint32_t found;
    
    do
    {
        uint16_t reg;
        
        reg = MSS_MAC_mdio_read(phy_address, PHYREG_PHYID1R);
        if (reg != ALL_BITS_HIGH)
        {
            found = 1u;
        }
        else
        {
            found = 0u;
            ++phy_address;
        }
    }
    while ((phy_address <= PHY_ADDRESS_MAX) && (0u == found));    
    
    return phy_address;
}
/**************************************************************************//**
 * 
 */
static void rx_desc_ring_init (void)
{        
    int32_t ring_idx;
    for (ring_idx = 0; ring_idx < MSS_MAC_RX_RING_SIZE; ring_idx++)
    {
        g_mac.rx_descriptors[ring_idx].descriptor_0 = 0u;
        g_mac.rx_descriptors[ring_idx].descriptor_1 = RDES1_RCH;
        if ((MSS_MAC_RX_RING_SIZE - 1) == ring_idx)
        {
            g_mac.rx_descriptors[ring_idx].buffer_2 = (uint32_t)&g_mac.rx_descriptors[0];
        }
        else
        {
            g_mac.rx_descriptors[ring_idx].buffer_2 = (uint32_t)&g_mac.rx_descriptors[ring_idx + 1];
        }
    }
}        
static void tx_desc_ring_init(void)
{
    int32_t ring_idx;
    
    for (ring_idx = 0; ring_idx < MSS_MAC_TX_RING_SIZE; ++ring_idx)
    {
        g_mac.tx_descriptors[ring_idx].descriptor_0 = 0u;
        g_mac.tx_descriptors[ring_idx].descriptor_1 = TDES1_TCH;
        if ((MSS_MAC_TX_RING_SIZE - 1) == ring_idx)
        {
            g_mac.tx_descriptors[ring_idx].buffer_2 = (uint32_t)&g_mac.tx_descriptors[0];
        }
        else
        {
            g_mac.tx_descriptors[ring_idx].buffer_2 = (uint32_t)&g_mac.tx_descriptors[ring_idx + 1];
        }
    }
}
/******************************** END OF FILE *********************************/

#ifdef __cplusplus
}
#endif
