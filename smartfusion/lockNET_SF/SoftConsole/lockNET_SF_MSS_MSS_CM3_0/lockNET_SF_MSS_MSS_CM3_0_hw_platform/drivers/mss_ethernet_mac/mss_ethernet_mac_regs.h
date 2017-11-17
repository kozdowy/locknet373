/***************************************************************************//**
 * (c) Copyright 2012-2015 Microsemi SoC Products Group.  All rights reserved.
 *
 * Register bit offsets and masks definitions for SmartFusion Ethernet MAC.
 *
 * SVN $Revision: 7436 $
 * SVN $Date: 2015-05-25 15:32:30 +0530 (Mon, 25 May 2015) $
 *
 *******************************************************************************/
#ifndef MSS_ETHERNET_MAC_REGISTERS_H_
#define MSS_ETHERNET_MAC_REGISTERS_H_


/*******************************************************************************
 * CSR0 register:
 *------------------------------------------------------------------------------
 * CSR0 - Bus Mode Register
 */

#define CSR0_SPD            21u
#define CSR0_DBO            20u
#define CSR0_TAP            17u 
#define CSR0_PBL            8u
#define CSR0_BLE            7u
#define CSR0_DSL            2u
#define CSR0_BAR            1u
#define CSR0_SWR            0u

#define CSR0_SPD_MASK       ((uint32_t)1u << CSR0_SPD)
#define CSR0_DBO_MASK       ((uint32_t)1u << CSR0_DBO)
#define CSR0_TAP_MASK       ((uint32_t)7u << CSR0_TAP)
#define CSR0_PBL_MASK       ((uint32_t)64u << CSR0_PBL)
#define CSR0_BLE_MASK       ((uint32_t)1u << CSR0_BLE)
#define CSR0_DSL_MASK       ((uint32_t)32u << CSR0_DSL)
#define CSR0_BAR_MASK       ((uint32_t)1u << CSR0_BAR)
#define CSR0_SWR_MASK       ((uint32_t)1u << CSR0_SWR)

/*
 * Allowed values for CSR0_DBO:
 *------------------------------------------------------------------------------
 * LITTLEENDIAN:   Little endian mode used for data descriptors
 * BIGENDIAN:      Big endian mode used for data descriptors
 */
#define LITTLEENDIAN    0u
#define BIGENDIAN       1u

/*
 * Allowed values for CSR0_TAP:
 *------------------------------------------------------------------------------
 * TAP_DISABLED:   TAP disabled
 * TAP_819US:      TAP 819/81.9us
 * TAP_2450US:     TAP 2450/245us
 * TAP_5730US:     TAP 5730/573us
 * TAP_51_2US:     TAP 51.2/5.12us
 * TAP_102_4US:    TAP 102.4/10.24us
 * TAP_153_6US:    TAP 156.6/15.26us
 * TAP_358_4US:    TAP 358.4/35.84us
 */
#define TAP_DISABLED    0x0
#define TAP_819US       0x1
#define TAP_2450US      0x2
#define TAP_5730US      0x3
#define TAP_51_2US      0x4
#define TAP_102_4US     0x5
#define TAP_153_6US     0x6
#define TAP_358_4US     0x7
/*******************************************************************************
 * CSR1 register:
 *------------------------------------------------------------------------------
 * CSR1 - Transmit Poll Demand Register
 */
 
#define CSR1_TPD3           24u
#define CSR1_TPD2           16u
#define CSR1_TPD1           8u
#define CSR1_TPD0           0u

#define CSR1_TPD3_MASK      ((uint32_t)1u << CSR1_TPD3)
#define CSR1_TPD2_MASK      ((uint32_t)1u << CSR1_TPD2)
#define CSR1_TPD1_MASK      ((uint32_t)1u << CSR1_TPD1)
#define CSR1_TPD0_MASK      ((uint32_t)1u << CSR1_TPD0)
 
/*******************************************************************************
 * CSR2 register:
 *------------------------------------------------------------------------------
 * CSR2 - Receive Poll Demand Register
 */

#define CSR2_RPD3           24u
#define CSR2_RPD2           16u
#define CSR2_RPD1           8u
#define CSR2_RPD0           0u

#define CSR2_RPD3_MASK      ((uint32_t)1u << CSR2_RPD3)
#define CSR2_RPD2_MASK      ((uint32_t)1u << CSR2_RPD2)
#define CSR2_RPD1_MASK      ((uint32_t)1u << CSR2_RPD1)
#define CSR2_RPD0_MASK      ((uint32_t)1u << CSR2_RPD0)


/*******************************************************************************
 * CSR3 register:
 *------------------------------------------------------------------------------
 * CSR3 - Receive Descriptor List Base Address Register
 */
 
#define CSR3_RLA3           24u
#define CSR3_RLA2           16u
#define CSR3_RLA1           8u
#define CSR3_RLA0           0u
#define CSR3_RLA3_MASK      ((uint32_t)1u << CSR3_RLA3)
#define CSR3_RLA2_MASK      ((uint32_t)1u << CSR3_RLA2)
#define CSR3_RLA1_MASK      ((uint32_t)1u << CSR3_RLA1)
#define CSR3_RLA0_MASK      ((uint32_t)1u << CSR3_RLA0)

/*******************************************************************************
 * CSR4 register:
 *------------------------------------------------------------------------------
 * CSR4 - Transmit Descriptor List Base Address Register
 */
#define CSR4_TLA3           24u
#define CSR4_TLA2           16u
#define CSR4_TLA1           8u
#define CSR4_TLA0           0u
#define CSR4_TLA3_MASK      ((uint32_t)1u << CSR4_TLA3)
#define CSR4_TLA2_MASK      ((uint32_t)1u << CSR4_TLA2)
#define CSR4_TLA1_MASK      ((uint32_t)1u << CSR4_TLA1)
#define CSR4_TLA0_MASK      ((uint32_t)1u << CSR4_TLA0)


/*******************************************************************************
 * CSR5 register:
 *------------------------------------------------------------------------------
 * CSR5 - Status Register
 */
 
#define CSR5_TS             20u
#define CSR5_RS             17u
#define CSR5_NIS            16u
#define CSR5_AIS            15u
#define CSR5_ERI            14u
#define CSR5_GTE            11u
#define CSR5_ETI            10u
#define CSR5_RPS            8u
#define CSR5_RU             7u
#define CSR5_RI             6u
#define CSR5_UNF            5u
#define CSR5_TU             2u
#define CSR5_TPS            1u
#define CSR5_TI             0u

#define CSR5_TS_MASK        ((uint32_t)7u << CSR5_TS)
#define CSR5_RS_MASK        ((uint32_t)7u << CSR5_RS)
#define CSR5_NIS_MASK       ((uint32_t)1u << CSR5_NIS)
#define CSR5_AIS_MASK       ((uint32_t)1u << CSR5_AIS)
#define CSR5_ERI_MASK       ((uint32_t)1u << CSR5_ERI)
#define CSR5_GTE_MASK       ((uint32_t)1u << CSR5_GTE)
#define CSR5_ETI_MASK       ((uint32_t)1u << CSR5_ETI)
#define CSR5_RPS_MASK       ((uint32_t)1u << CSR5_RPS)
#define CSR5_RU_MASK        ((uint32_t)1u << CSR5_RU)
#define CSR5_RI_MASK        ((uint32_t)1u << CSR5_RI)
#define CSR5_UNF_MASK       ((uint32_t)1u << CSR5_UNF)
#define CSR5_TU_MASK        ((uint32_t)1u << CSR5_TU)
#define CSR5_TPS_MASK       ((uint32_t)1u << CSR5_TPS)
#define CSR5_TI_MASK        ((uint32_t)1u << CSR5_TI)

#define CSR5_INT_BITS       (CSR5_NIS_MASK | CSR5_AIS_MASK | CSR5_ERI_MASK | \
                            CSR5_GTE_MASK | CSR5_ETI_MASK | CSR5_RPS_MASK | \
                            CSR5_RU_MASK | CSR5_RI_MASK | CSR5_UNF_MASK | \
                            CSR5_TU_MASK | CSR5_TPS_MASK | CSR5_TI_MASK)

/*------------------------------------------------------------------------------
 * CSR5_TS:
 *   TS field of register CSR5.
 *------------------------------------------------------------------------------
 * Transmit process state
 */

/** 000 - Stopped; RESET or STOP TRANSMIT command issued.             */
#define CSR5_TS_STOPPED         0u
/** 001 - Running, fetching the transmit descriptor.                  */
#define CSR5_TS_RUNNING_FD      1u
/** 010 - Running, waiting for end of transmission.                   */
#define CSR5_TS_RUNNING_WT      2u
/** 011 - Running, transferring data buffer from host memory to FIFO. */
#define CSR5_TS_RUNNING_TD      3u
/** 101 - Running, setup packet.                                      */
#define CSR5_TS_RUNNING_SP      5u
/** 110 - Suspended; FIFO underflow or unavailable descriptor.        */
#define CSR5_TS_SUSPENDED       6u
/** 111 - Running, closing transmit descriptor.                       */
#define CSR5_TS_RUNNING_CD      7u

/*------------------------------------------------------------------------------
 * CSR5_RS:
 *   RS field of register CSR5.
 *------------------------------------------------------------------------------
 * Receive process state
 */

/** 000 - Stopped; RESET or STOP RECEIVE command issued.                      */
#define CSR5_RS_STOPPED         0u
/** 001 - Running, fetching the receive descriptor.                           */
#define CSR5_RS_RUNNING_FD      1u
/** 010 - Running, waiting for the end-of-receive packet before prefetch of the
 *next descriptor. */
#define CSR5_RS_RUNNING_WR      2u
/** 011 - Running, waiting for the receive packet.                            */
#define CSR5_RS_RUNNING_RB      3u
/** 100 - Suspended, unavailable receive buffer.                              */
#define CSR5_RS_SUSPENDED       4u
/** 101 - Running, closing the receive descriptor.                            */
#define CSR5_RS_RUNNING_CD      5u
/** 111 - Running, transferring data from FIFO to host memory.                */
#define CSR5_RS_RUNNING_TD      7u

/*******************************************************************************
 * CSR6 register:
 *------------------------------------------------------------------------------
 * CSR6 - Operation Mode Register
 */
 
#define CSR6_RA             30u
#define CSR6_TTM            22u
#define CSR6_SF             21u
#define CSR6_TR             14u
#define CSR6_ST             13u
#define CSR6_FD             9u
#define CSR6_PM             7u
#define CSR6_PR             6u
#define CSR6_IF             4u
#define CSR6_PB             3u
#define CSR6_HO             2u
#define CSR6_SR             1u
#define CSR6_HP             0u
 
#define CSR6_RA_MASK        ((uint32_t)1u << CSR6_RA)
#define CSR6_TTM_MASK       ((uint32_t)1u << CSR6_TTM)
#define CSR6_SF_MASK        ((uint32_t)1u << CSR6_SF)
#define CSR6_TR_MASK        ((uint32_t)1u << CSR6_TR)
#define CSR6_ST_MASK        ((uint32_t)1u << CSR6_ST)
#define CSR6_FD_MASK        ((uint32_t)1u << CSR6_FD)
#define CSR6_PM_MASK        ((uint32_t)1u << CSR6_PM)
#define CSR6_PR_MASK        ((uint32_t)1u << CSR6_PR)
#define CSR6_IF_MASK        ((uint32_t)1u << CSR6_IF)
#define CSR6_PB_MASK        ((uint32_t)1u << CSR6_PB)
#define CSR6_HO_MASK        ((uint32_t)1u << CSR6_HO)
#define CSR6_SR_MASK        ((uint32_t)1u << CSR6_SR)
#define CSR6_HP_MASK        ((uint32_t)1u << CSR6_HP)

/*******************************************************************************
 * CSR7 register:
 *------------------------------------------------------------------------------
 * CSR7 - Interrupt Enable Register
 */
 
#define CSR7_NIE            16u
#define CSR7_AIE            15u
#define CSR7_ERE            14u
#define CSR7_GTE            11u
#define CSR7_ETE            10u
#define CSR7_RSE            8u
#define CSR7_RUE            7u
#define CSR7_RIE            6u
#define CSR7_UNE            5u
#define CSR7_TUE            2u
#define CSR7_TSE            1u
#define CSR7_TIE            0u
#define CSR7_NIE_MASK       ((uint32_t)1u << CSR7_NIE)
#define CSR7_AIE_MASK       ((uint32_t)1u << CSR7_AIE)
#define CSR7_ERE_MASK       ((uint32_t)1u << CSR7_ERE)
#define CSR7_GTE_MASK       ((uint32_t)1u << CSR7_GTE)
#define CSR7_ETE_MASK       ((uint32_t)1u << CSR7_ETE)
#define CSR7_RSE_MASK       ((uint32_t)1u << CSR7_RSE)
#define CSR7_RUE_MASK       ((uint32_t)1u << CSR7_RUE)
#define CSR7_RIE_MASK       ((uint32_t)1u << CSR7_RIE)
#define CSR7_UNE_MASK       ((uint32_t)1u << CSR7_UNE)
#define CSR7_TUE_MASK       ((uint32_t)1u << CSR7_TUE)
#define CSR7_TSE_MASK       ((uint32_t)1u << CSR7_TSE)
#define CSR7_TIE_MASK       ((uint32_t)1u << CSR7_TIE)

/*******************************************************************************
 * CSR8 register:
 *------------------------------------------------------------------------------
 * CSR8 - Missed Frames and Overflow Counter Register
 */
 
#define CSR8_OCO            28u
#define CSR8_FOC            17u
#define CSR8_MFO            16u
#define CSR8_MFC            0u
#define CSR8_OCO_MASK       0x10000000u  
#define CSR8_FOC_MASK       0x0FFE0000u
#define CSR8_MFO_MASK       0x00010000u
#define CSR8_MFC_MASK       0x0000FFFFu


/*******************************************************************************
 * CSR9 register:
 *------------------------------------------------------------------------------
 * CSR9 - MII Management and Serial ROM Interface Register
 */
 
#define CSR9_MDI            19u 
#define CSR9_MII            18u
#define CSR9_MDO            17u
#define CSR9_MDC            16u
#define CSR9_MDI_MASK       ((uint32_t)1u << CSR9_MDI)
#define CSR9_MII_MASK       ((uint32_t)1u << CSR9_MII)
#define CSR9_MDO_MASK       ((uint32_t)1u << CSR9_MDO)
#define CSR9_MDC_MASK       ((uint32_t)1u << CSR9_MDC)

/*******************************************************************************
 * CSR11 register:
 *------------------------------------------------------------------------------
 * CSR11 - General-Purpose Timer and Interrupt Mitigation Control Register
 */
 
#define CSR11_CS            31u
#define CSR11_TT            27u
#define CSR11_NTP           24u
#define CSR11_RT            20u
#define CSR11_NRP           17u
#define CSR11_CON           16u
#define CSR11_TIM           0u
#define CSR11_CS_MASK       ((uint32_t)1u << CSR11_CS)
#define CSR11_TT_MASK       ((uint32_t)1u << CSR11_TT)
#define CSR11_NTP_MASK      ((uint32_t)1u << CSR11_NTP)
#define CSR11_RT_MASK       ((uint32_t)1u << CSR11_RT)
#define CSR11_NRP_MASK      ((uint32_t)1u << CSR11_NRP)
#define CSR11_CON_MASK      ((uint32_t)1u << CSR11_CON)
#define CSR11_TIM_MASK      ((uint32_t)0xFFFFu << CSR11_TIM)

#define CSR11_TIM_MAX_VAL      ((uint32_t)1u << CSR11_TIM)

#endif /* MSS_ETHERNET_MAC_REGISTERS_H_*/
