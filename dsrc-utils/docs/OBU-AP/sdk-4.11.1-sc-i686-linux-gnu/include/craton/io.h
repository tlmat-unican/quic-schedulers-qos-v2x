/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_IO_H
#define _CRATON_IO_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Memory-mapped I/O API
*/

/* Optional tracing helpers */
#ifdef __CRATON_IO_TRACE
void ioread8_trace(uint8_t value, const void *addr);
void ioread16_trace(uint16_t value, const void *addr);
void ioread32_trace(uint32_t value, const void *addr);
void iowrite8_trace(uint8_t value, const void *addr);
void iowrite16_trace(uint16_t value, const void *addr);
void iowrite32_trace(uint32_t value, const void *addr);
#else /* __CRATON_IO_TRACE */
atlk_inline void ioread8_trace(uint8_t value, const void *addr) {}
atlk_inline void ioread16_trace(uint16_t value, const void *addr) {}
atlk_inline void ioread32_trace(uint32_t value, const void *addr) {}
atlk_inline void iowrite8_trace(uint8_t value, const void *addr) {}
atlk_inline void iowrite16_trace(uint16_t value, const void *addr) {}
atlk_inline void iowrite32_trace(uint32_t value, const void *addr) {}
#endif /* __CRATON_IO_TRACE */

#ifdef __arm__

atlk_inline uint8_t
ioread8_async(const void *addr)
{
  return *(volatile uint8_t *)addr;
}

atlk_inline uint16_t
ioread16_async(const void *addr)
{
  return *(volatile uint16_t *)addr;
}

atlk_inline uint32_t
ioread32_async(const void *addr)
{
  return *(volatile uint32_t *)addr;
}

atlk_inline void
iowrite8_async(uint8_t value, void *addr)
{
  *(volatile uint8_t *)addr = value;
}

atlk_inline void
iowrite16_async(uint16_t value, void *addr)
{
  *(volatile uint16_t *)addr = value;
}

atlk_inline void
iowrite32_async(uint32_t value, void *addr)
{
  *(volatile uint32_t *)addr = value;
}

atlk_inline void
dmb(void)
{
  __asm__ __volatile__("dmb": : : "memory");
}

atlk_inline void
dsb(void)
{
  __asm__ __volatile__("dsb": : : "memory");
}

#endif /* __arm__ */

#ifdef __arc__

atlk_inline uint8_t
ioread8_async(const void *addr)
{
  uint8_t rv;
  __asm__ __volatile__("ldb.di %0,[%1]" : "=r" (rv) : "r" (addr));
  return rv;
}

atlk_inline uint16_t
ioread16_async(const void *addr)
{
  uint16_t rv;
  __asm__ __volatile__("ldw.x.di %0,[%1]" : "=r" (rv) : "r" (addr));
  return rv;
}

atlk_inline uint32_t
ioread32_async(const void *addr)
{
  uint32_t rv;
  __asm__ __volatile__("ld.di %0,[%1]" : "=r" (rv) : "r" (addr));
  return rv;
}

atlk_inline void
iowrite8_async(uint8_t value, void *addr)
{
  __asm__ __volatile__("stb.di %0,[%1]" : : "r" (value), "r" (addr));
}

atlk_inline void
iowrite16_async(uint16_t value, void *addr)
{
  __asm__ __volatile__("stw.di %0,[%1]" : : "r" (value), "r" (addr));
}

atlk_inline void
iowrite32_async(uint32_t value, void *addr)
{
  __asm__ __volatile__("st.di %0,[%1]" : : "r" (value), "r" (addr));
}

atlk_inline void
dmb(void)
{
  __asm__ __volatile__("sync": : : "memory");
}

atlk_inline void
dsb(void)
{
  __asm__ __volatile__("sync": : : "memory");
}

#endif /* __arc__ */

/**
   Read a memory-mapped byte.

   @param[in] ptr Pointer to read from

   @return Read byte
*/
atlk_inline uint8_t
ioread8(const void *ptr)
{
  uint8_t rv = ioread8_async(ptr);
  dmb();
  ioread8_trace(rv, ptr);
  return rv;
}

/**
   Read a memory-mapped word.

   @param[in] ptr Pointer to read from

   @return Read word
*/
atlk_inline uint16_t
ioread16(const void *ptr)
{
  uint16_t rv = ioread16_async(ptr);
  dmb();
  ioread16_trace(rv, ptr);
  return rv;
}

/**
   Read a memory-mapped double word.

   @param[in] ptr Pointer to read from

   @return Read double word
*/
atlk_inline uint32_t
ioread32(const void *ptr)
{
  uint32_t rv = ioread32_async(ptr);
  dmb();
  ioread32_trace(rv, ptr);
  return rv;
}

/**
   Write a memory-mapped byte.

   @param[in] value Value to write
   @param[in] ptr Pointer to write to
*/
atlk_inline void
iowrite8(uint8_t value, void *ptr)
{
  dsb();
  iowrite8_async(value, ptr);
}

/**
   Write a memory-mapped word.

   @param[in] value Value to write
   @param[in] ptr Pointer to write to
*/
atlk_inline void
iowrite16(uint16_t value, void *ptr)
{
  dsb();
  iowrite16_async(value, ptr);
  iowrite16_trace(value, ptr);
}

/**
   Write a memory-mapped double word.

   @param[in] value Value to write
   @param[in] ptr Pointer to write to
*/
atlk_inline void
iowrite32(uint32_t value, void *ptr)
{
  dsb();
  iowrite32_async(value, ptr);
  iowrite32_trace(value, ptr);
}

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_IO_H */
