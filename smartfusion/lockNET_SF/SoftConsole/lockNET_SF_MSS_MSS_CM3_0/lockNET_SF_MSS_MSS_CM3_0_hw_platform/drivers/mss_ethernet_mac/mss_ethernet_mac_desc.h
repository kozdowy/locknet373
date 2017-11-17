/***************************************************************************//**
 * (c) Copyright 2012-2015 Microsemi SoC Products Group.  All rights reserved.
 *
 * Header file for SmartFusion MSS Ethernet MAC driver.
 * SmartFusion MSS Ethernet MAC driver internal definitions.
 *
 *
 * SVN $Revision: 7436 $
 * SVN $Date: 2015-05-25 15:32:30 +0530 (Mon, 25 May 2015) $
 *
 *
 ******************************************************************************/
#ifndef MSS_ETHERNET_MAC_DESC_H
#define MSS_ETHERNET_MAC_DESC_H    1

/*******************************************************************************
 * Receive descriptor bits
 */

/***************************************************************************//**
 * Ownership bit.
 * 1 - Ethernet MAC owns the descriptor. <br>
 * 0 - The host owns the descriptor. <br>
 * Ethernet MAC will clear this bit when it completes a current frame reception
 * or when the data buffers associated with a given descriptor are already full.
 */
#define RDES0_OWN   ((uint32_t)0x80000000u)

/***************************************************************************//**
 * Filtering fail.
 * When set, indicates that a received frame did not pass the address recognition 
 * process. 
 * This bit is valid only for the last descriptor of the frame (RDES0.8 set), 
 * when the CSR6.30 (receive all) bit is set and the frame is at
 * least 64 bytes long.
 */
#define RDES0_FF    ((uint32_t)0x40000000u)

/***************************************************************************//**
 * Frame length.
 * Indicates the length, in bytes, of the data transferred into a host memory 
 * for a given frame. 
 * This bit is valid only when RDES0.8 (last descriptor) is set and RDES0.14 
 * (descriptor error) is cleared.
 */
#define RDES0_FL_MASK       ((uint32_t)0x00003FFFu)
#define RDES0_FL_OFFSET     16u

/***************************************************************************//**
 * Error summary.
 * This bit is a logical OR of the following bits:
 * RDES0.1 - CRC error
 * RDES0.6 - Collision seen
 * RDES0.7 - Frame too long
 * RDES0.11 - Runt frame
 * RDES0.14 - Descriptor error
 * This bit is valid only when RDES0.8 (last descriptor) is set.
 */
#define RDES0_ES    ((uint32_t)0x00008000u)

/***************************************************************************//**
 * Descriptor error.
 * Set by Ethernet MAC when no receive buffer was available when trying to store
 * the received data. 
 * This bit is valid only when RDES0.8 (last descriptor) is set.
 */
#define RDES0_DE    ((uint32_t)0x00004000u)

/***************************************************************************//**
 * Runt frame.
 * When set, indicates that the frame is damaged by a collision or by a premature
 * termination before the end of a collision window.
 * This bit is valid only when RDES0.8 (last descriptor) is set.
 */
#define RDES0_RF    ((uint32_t)0x00000800u)

/***************************************************************************//**
 * Multicast frame.
 * When set, indicates that the frame has a multicast address.
 * This bit is valid only when RDES0.8 (last descriptor) is set.
 */
#define RDES0_MF    ((uint32_t)0x00000400u)

/***************************************************************************//**
 * First descriptor.
 * When set, indicates that this is the first descriptor of a frame.
 */
#define RDES0_FS    ((uint32_t)0x00000200u)

/***************************************************************************//**
 * Last descriptor.
 * When set, indicates that this is the last descriptor of a frame.
 */
#define RDES0_LS    ((uint32_t)0x00000100u)

/***************************************************************************//**
 * Frame too long.
 * When set, indicates that a current frame is longer than maximum size of 1,518
 * bytes, as specified by 802.3. TL (frame too long) in the receive descriptor 
 * has been set when the received frame is longer than 1,518 bytes. This flag 
 * is valid in all receive descriptors when multiple descriptors are used for
 * one frame.
 */
#define RDES0_TL    ((uint32_t)0x00000080u)

/***************************************************************************//**
 * Collision seen.
 * When set, indicates that a late collision was seen (collision after 64 bytes
 * following SFD).
 * This bit is valid only when RDES0.8 (last descriptor) is set.
 */
#define RDES0_CS    ((uint32_t)0x00000040u)

/***************************************************************************//**
 * Frame type.
 * When set, indicates that the frame has a length field larger than 1,500 
 * (Ethernet-type frame). When cleared, indicates an 802.3-type frame.
 * This bit is valid only when RDES0.8 (last descriptor) is set.
 * Additionally, FT is invalid for runt frames shorter than 14 bytes.
 */
#define RDES0_FT    ((uint32_t)0x00000020u)

/***************************************************************************//**
 * Report on MII error.
 * When set, indicates that an error has been detected by a physical layer chip
 * connected through the MII interface.
 * This bit is valid only when RDES0.8 (last descriptor) is set.
 */
#define RDES0_RE    ((uint32_t)0x00000008u)

/***************************************************************************//**
 * Dribbling bit.
 * When set, indicates that the frame was not byte-aligned.
 * This bit is valid only when RDES0.8 (last descriptor) is set.
 */
#define RDES0_DB    ((uint32_t)0x00000004u)

/***************************************************************************//**
 * CRC error.
 * When set, indicates that a CRC error has occurred in the received frame.
 * This bit is valid only when RDES0.8 (last descriptor) is set.
 * Additionally, CE is not valid when the received frame is a runt frame.
 */
#define RDES0_CE    ((uint32_t)0x00000002u)

/***************************************************************************//**
 * This bit is reset for frames with a legal length.
 */
#define RDES0_ZERO  ((uint32_t)0x00000001u)

/***************************************************************************//**
 * Receive end of ring.
 * When set, indicates that this is the last descriptor in the receive 
 * descriptor ring. Ethernet MAC returns to the first descriptor in the ring,
 * as specified by CSR3 (start of receive list address).
 */
#define RDES1_RER   0x02000000u

/***************************************************************************//**
 * Second address chained.
 * When set, indicates that the second buffer's address points to the next 
 * descriptor and not to the data buffer. Note that RER takes precedence over RCH.
 */
#define RDES1_RCH   0x01000000u

/***************************************************************************//**
 * Buffer 2 size.
 * Indicates the size, in bytes, of memory space used by the second data buffer.
 * This number must be a multiple of four. If it is 0, Ethernet MAC ignores the
 * second data buffer and fetches the next data descriptor.
 * This number is valid only when RDES1.24 (second address chained) is cleared.
 */
#define RDES1_RBS2_MASK         0x7FFu
#define RDES1_RBS2_OFFSET       11u

/***************************************************************************//**
 * Buffer 1 size
 * Indicates the size, in bytes, of memory space used by the first data buffer.
 * This number must be a multiple of four. If it is 0, Ethernet MAC ignores 
 * the first data buffer and uses the second data buffer.
 */
#define RDES1_RBS1_MASK         0x7FFu
#define RDES1_RBS1_OFFSET       0u


/*******************************************************************************
 * Transmit descriptor bits
 */

/***************************************************************************//**
 * Ownership bit.
 * 1 - Ethernet MAC owns the descriptor.
 * 0 - The host owns the descriptor.
 * Ethernet MAC will clear this bit when it completes a current frame transmission
 * or when the data buffers associated with a given descriptor are empty.
 */
#define TDES0_OWN     ((uint32_t)0x80000000u)

/***************************************************************************//**
 * Error summary.
 * This bit is a logical OR of the following bits:
 * TDES0.1 - Underflow error
 * TDES0.8 - Excessive collision error
 * TDES0.9 - Late collision
 * TDES0.10 - No carrier
 * TDES0.11 - Loss of carrier
 * This bit is valid only when TDES1.30 (last descriptor) is set.
 */
#define TDES0_ES      (1u << 15u)

/***************************************************************************//**
 * Loss of carrier.
 * When set, indicates a loss of the carrier during a transmission.
 * This bit is valid only when TDES1.30 (last descriptor) is set.
 */
#define TDES0_LO      (1u << 11u)

/***************************************************************************//**
 * No carrier.
 * When set, indicates that the carrier was not asserted by an external transceiver
 * during the transmission.
 * This bit is valid only when TDES1.30 (last descriptor) is set.
 */
#define TDES0_NC      (1u << 10u)

/***************************************************************************//**
 * Late collision.
 * When set, indicates that a collision was detected after transmitting 64 bytes.
 * This bit is not valid when TDES0.1 (underflow error) is set.
 * This bit is valid only when TDES1.30 (last descriptor) is set.
 */
#define TDES0_LC      (1u << 9u)

/***************************************************************************//**
 * Excessive collisions.
 * When set, indicates that the transmission was aborted after 16 retries.
 * This bit is valid only when TDES1.30 (last descriptor) is set.
 */
#define TDES0_EC      (1u << 8u)

/***************************************************************************//**
 * Collision count.
 * This field indicates the number of collisions that occurred before the end
 * of a frame transmission.
 * This value is not valid when TDES0.8 (excessive collisions bit) is set.
 * This bit is valid only when TDES1.30 (last descriptor) is set.
 */
#define TDES0_CC_MASK       0xFu
#define TDES0_CC_OFFSET     3u

/***************************************************************************//**
 * Underflow error.
 * When set, indicates that the FIFO was empty during the frame transmission.
 * This bit is valid only when TDES1.30 (last descriptor) is set.
 */
#define TDES0_UF      (1u << 1u)

/***************************************************************************//**
 * Deferred.
 * When set, indicates that the frame was deferred before transmission. Deferring
 * occurs if the carrier is detected when the transmission is ready to start.
 * This bit is valid only when TDES1.30 (last descriptor) is set.
 */
#define TDES0_DE      (1u)

/***************************************************************************//**
 * Interrupt on completion.
 * Setting this flag instructs Ethernet MAC to set CSR5.0 (transmit interrupt) 
 * immediately after processing a current frame.
 * This bit is valid when TDES1.30 (last descriptor) is set or for a setup packet.
 */
#define TDES1_IC      (1u << 31u)

/***************************************************************************//**
 * Last descriptor.
 * When set, indicates the last descriptor of the frame.
 */
#define TDES1_LS      (1u << 30u)

/***************************************************************************//**
 * First descriptor.
 * When set, indicates the first descriptor of the frame.
 */
#define TDES1_FS      (1u << 29u)

/***************************************************************************//**
 * Filtering type.
 * This bit, together with TDES0.22 (FT0), controls a current filtering mode.
 * This bit is valid only for the setup frames.
 */
#define TDES1_FT1     (1u << 28u)

/***************************************************************************//**
 * Setup packet.
 * When set, indicates that this is a setup frame descriptor.
 */
#define TDES1_SET     (1u << 27u)

/***************************************************************************//**
 * Add CRC disable.
 * When set, Ethernet MAC does not append the CRC value at the end of the frame. 
 * The exception is when the frame is shorter than 64 bytes and automatic byte 
 * padding is enabled. In that case, the CRC field is added, despite the state
 * of the AC flag.
 */
#define TDES1_AC      (1u << 26u)

/***************************************************************************//**
 * Transmit end of ring.
 * When set, indicates the last descriptor in the descriptor ring.
 */
#define TDES1_TER     (1u << 25u)

/***************************************************************************//**
 * Second address chained.
 * When set, indicates that the second descriptor's address points to the next
 * descriptor and not to the data buffer.
 * This bit is valid only when TDES1.25 (transmit end of ring) is reset.
 */
#define TDES1_TCH     (1u << 24u)

/***************************************************************************//**
 * Disabled padding.
 * When set, automatic byte padding is disabled. Ethernet MAC normally appends 
 * the PAD field after the INFO field when the size of an actual frame is less
 * than 64 bytes. After padding bytes, the CRC field is also inserted, despite
 * the state of the AC flag. When DPD is set, no padding bytes are appended.
 */
#define TDES1_DPD     (1u << 23u)

/***************************************************************************//**
 * Filtering type.
 * This bit, together with TDES0.28 (FT1), controls the current filtering mode.
 * This bit is valid only when the TDES1.27 (SET) bit is set.
 */
#define TDES1_FT0     (1u << 22u)

/***************************************************************************//**
 * Buffer 2 size.
 * Indicates the size, in bytes, of memory space used by the second data buffer.
 * If it is zero, Ethernet MAC ignores the second data buffer and fetches the 
 * next data descriptor.
 * This bit is valid only when TDES1.24 (second address chained) is cleared.
 */
#define TDES1_TBS2_MASK         0x7FFu
#define TDES1_TBS2_OFFSET       11u

/***************************************************************************//**
 * Buffer 1 size.
 * Indicates the size, in bytes, of memory space used by the first data buffer.
 * If it is 0, Ethernet MAC ignores the first data buffer and uses the second
 * data buffer.
 */
#define TDES1_TBS1_MASK        0x7FFu
#define TDES1_TBS1_OFFSET    0u


#endif    /* MSS_ETHERNET_MAC_DESC_H */

