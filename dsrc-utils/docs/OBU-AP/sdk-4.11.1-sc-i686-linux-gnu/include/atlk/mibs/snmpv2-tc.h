/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_MIBS_SNMPV2_TC_H
#define _ATLK_MIBS_SNMPV2_TC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Mapping of types in SNMPv2-TC
*/

/** Status of a MIB table conceptual row */
typedef enum  {
  /**
     Indicates that the conceptual row with all columns is available
     for use by the managed device.
  */
  MIB_RowStatus_active = 1,

  /**
     Indicates that the conceptual row exists in the agent, but is
     unavailable for use by the managed device.
  */
  MIB_RowStatus_notInService = 2,

  /**
     Indicates that the conceptual row exists in the agent, one or
     more required columns in the row are not instantiated.
  */
  MIB_RowStatus_notReady = 3,

  /**
     Supplied by a manager wishing to create a new instance of a
     conceptual row and make it available for use.
  */
  MIB_RowStatus_createAndGo = 4,

  /**
     Supplied by a manager wishing to create a new instance of a
     conceptual row but not making it available for use.
  */
  MIB_RowStatus_createAndWait = 5,

  /**
     Supplied by a manager wishing to delete all of the instances
     associated with an existing conceptual row.
  */
  MIB_RowStatus_destroy = 6

} mib_RowStatus_t;

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_SNMPV2_TC_H */
