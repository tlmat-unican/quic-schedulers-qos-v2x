/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _ATLK_MIBS_INET_ADDRESS_MIB_H
#define _ATLK_MIBS_INET_ADDRESS_MIB_H

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Inet address MIB API
*/

/** Internet address type */
typedef enum {
  /** An unknown address type */
  MIB_InetAddressType_unknown = 0,
  
  /** An IPv4 address as defined by the InetAddressIPv4 textual convention */
  MIB_InetAddressType_ipv4 = 1,

  /** An IPv6 address as defined by the InetAddressIPv6 textual convention */
  MIB_InetAddressType_ipv6 = 2,

  /** 
     A non-global IPv4 address including a zone index as defined by the
     InetAddressIPv4z textual convention.
  */
  MIB_InetAddressType_ipv4z = 3,

  /** 
     A non-global IPv6 address including a zone index as defined by the
     InetAddressIPv6z textual convention.
  */
  MIB_InetAddressType_ipv6z = 4,

  /* A DNS domain name as defined by the InetAddressDNS textual convention */
  MIB_InetAddressType_dns = 16

} mib_InetAddressType_t;

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_INET_ADDRESS_MIB_H */
