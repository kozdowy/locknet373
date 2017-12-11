/*
 * keys.h
 *
 *  Created on: Dec 11, 2017
 *      Author: eliubakk
 */

#ifndef KEYS_H_
#define KEYS_H_

#define RSA_PUBLIC_KEY 0x10001
#define RSA_SERVER_MODULUS_U 0xeda515ef
#define RSA_SERVER_MODULUS_L 0x24029417
#define RSA_SERVER_RESIDUE_U 0x859cfcfb
#define RSA_SERVER_RESIDUE_L 0x5a1f75d5

#ifdef LOCK_0
#define RSA_PRIVATE_KEY_U 0x4984c047
#define RSA_PRIVATE_KEY_L 0x4cbc17f1
#define RSA_MODULUS_U 0xd8c0938e
#define RSA_MODULUS_L 0x985082eb
#define RSA_RESIDUE_U 0x60296e8c
#define RSA_RESIDUE_L 0x02ecfdea
#endif

#ifdef LOCK_1
#define RSA_PRIVATE_KEY_U 0xa57d4dbe
#define RSA_PRIVATE_KEY_L 0xdacdd1e1
#define RSA_MODULUS_U 0xbc8686e2
#define RSA_MODULUS_L 0x83f127ff
#define RSA_RESIDUE_U 0x41ce4fcd
#define RSA_RESIDUE_L 0x33b22431
#endif

#endif /* KEYS_H_ */
