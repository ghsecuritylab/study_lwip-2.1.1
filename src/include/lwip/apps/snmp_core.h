/**
 * @file
 * SNMP core API for implementing MIBs
 */

/*
 * Copyright (c) 2006 Axon Digital Design B.V., The Netherlands.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Christiaan Simons <christiaan.simons@axon.tv>
 *         Martin Hentschel <info@cl-soft.de>
 */

#ifndef LWIP_HDR_APPS_SNMP_CORE_H
#define LWIP_HDR_APPS_SNMP_CORE_H

#include "lwip/apps/snmp_opts.h"

#if LWIP_SNMP /* don't build if not configured for use in lwipopts.h */

#include "lwip/ip_addr.h"
#include "lwip/err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* basic ASN1 defines */
#define SNMP_ASN1_CLASS_UNIVERSAL   0x00
#define SNMP_ASN1_CLASS_APPLICATION 0x40
#define SNMP_ASN1_CLASS_CONTEXT     0x80
#define SNMP_ASN1_CLASS_PRIVATE     0xC0

#define SNMP_ASN1_CONTENTTYPE_PRIMITIVE   0x00
#define SNMP_ASN1_CONTENTTYPE_CONSTRUCTED 0x20

/* universal tags (from ASN.1 spec.) */
#define SNMP_ASN1_UNIVERSAL_END_OF_CONTENT  0
#define SNMP_ASN1_UNIVERSAL_INTEGER         2
#define SNMP_ASN1_UNIVERSAL_OCTET_STRING    4
#define SNMP_ASN1_UNIVERSAL_NULL            5
#define SNMP_ASN1_UNIVERSAL_OBJECT_ID       6
#define SNMP_ASN1_UNIVERSAL_SEQUENCE_OF    16

/* application specific (SNMP) tags (from SNMPv2-SMI) */
#define SNMP_ASN1_APPLICATION_IPADDR    0  /* [APPLICATION 0] IMPLICIT OCTET STRING (SIZE (4)) */
#define SNMP_ASN1_APPLICATION_COUNTER   1  /* [APPLICATION 1] IMPLICIT INTEGER (0..4294967295) => u32_t */
#define SNMP_ASN1_APPLICATION_GAUGE     2  /* [APPLICATION 2] IMPLICIT INTEGER (0..4294967295) => u32_t */
#define SNMP_ASN1_APPLICATION_TIMETICKS 3  /* [APPLICATION 3] IMPLICIT INTEGER (0..4294967295) => u32_t */
#define SNMP_ASN1_APPLICATION_OPAQUE    4  /* [APPLICATION 4] IMPLICIT OCTET STRING */
#define SNMP_ASN1_APPLICATION_COUNTER64 6  /* [APPLICATION 6] IMPLICIT INTEGER (0..18446744073709551615) */

/* context specific (SNMP) tags (from RFC 1905) */
#define SNMP_ASN1_CONTEXT_VARBIND_NO_SUCH_INSTANCE 1

/* full ASN1 type defines */
/* 定义了当前系统内支持的所有 asn1 对象数据类型 */
#define SNMP_ASN1_TYPE_END_OF_CONTENT (SNMP_ASN1_CLASS_UNIVERSAL | SNMP_ASN1_CONTENTTYPE_PRIMITIVE | SNMP_ASN1_UNIVERSAL_END_OF_CONTENT)
#define SNMP_ASN1_TYPE_INTEGER        (SNMP_ASN1_CLASS_UNIVERSAL | SNMP_ASN1_CONTENTTYPE_PRIMITIVE | SNMP_ASN1_UNIVERSAL_INTEGER)
#define SNMP_ASN1_TYPE_OCTET_STRING   (SNMP_ASN1_CLASS_UNIVERSAL | SNMP_ASN1_CONTENTTYPE_PRIMITIVE | SNMP_ASN1_UNIVERSAL_OCTET_STRING)
#define SNMP_ASN1_TYPE_NULL           (SNMP_ASN1_CLASS_UNIVERSAL | SNMP_ASN1_CONTENTTYPE_PRIMITIVE | SNMP_ASN1_UNIVERSAL_NULL)
#define SNMP_ASN1_TYPE_OBJECT_ID      (SNMP_ASN1_CLASS_UNIVERSAL | SNMP_ASN1_CONTENTTYPE_PRIMITIVE | SNMP_ASN1_UNIVERSAL_OBJECT_ID)
#define SNMP_ASN1_TYPE_SEQUENCE       (SNMP_ASN1_CLASS_UNIVERSAL | SNMP_ASN1_CONTENTTYPE_CONSTRUCTED | SNMP_ASN1_UNIVERSAL_SEQUENCE_OF)
#define SNMP_ASN1_TYPE_IPADDR         (SNMP_ASN1_CLASS_APPLICATION | SNMP_ASN1_CONTENTTYPE_PRIMITIVE | SNMP_ASN1_APPLICATION_IPADDR)
#define SNMP_ASN1_TYPE_IPADDRESS      SNMP_ASN1_TYPE_IPADDR
#define SNMP_ASN1_TYPE_COUNTER        (SNMP_ASN1_CLASS_APPLICATION | SNMP_ASN1_CONTENTTYPE_PRIMITIVE | SNMP_ASN1_APPLICATION_COUNTER)
#define SNMP_ASN1_TYPE_COUNTER32      SNMP_ASN1_TYPE_COUNTER
#define SNMP_ASN1_TYPE_GAUGE          (SNMP_ASN1_CLASS_APPLICATION | SNMP_ASN1_CONTENTTYPE_PRIMITIVE | SNMP_ASN1_APPLICATION_GAUGE)
#define SNMP_ASN1_TYPE_GAUGE32        SNMP_ASN1_TYPE_GAUGE
#define SNMP_ASN1_TYPE_UNSIGNED32     SNMP_ASN1_TYPE_GAUGE
#define SNMP_ASN1_TYPE_TIMETICKS      (SNMP_ASN1_CLASS_APPLICATION | SNMP_ASN1_CONTENTTYPE_PRIMITIVE | SNMP_ASN1_APPLICATION_TIMETICKS)
#define SNMP_ASN1_TYPE_OPAQUE         (SNMP_ASN1_CLASS_APPLICATION | SNMP_ASN1_CONTENTTYPE_PRIMITIVE | SNMP_ASN1_APPLICATION_OPAQUE)
#if LWIP_HAVE_INT64
#define SNMP_ASN1_TYPE_COUNTER64      (SNMP_ASN1_CLASS_APPLICATION | SNMP_ASN1_CONTENTTYPE_PRIMITIVE | SNMP_ASN1_APPLICATION_COUNTER64)
#endif

#define SNMP_VARBIND_EXCEPTION_OFFSET 0xF0
#define SNMP_VARBIND_EXCEPTION_MASK   0x0F

/** error codes predefined by SNMP prot. */
typedef enum {
  SNMP_ERR_NOERROR             = 0,
/* 
outdated v1 error codes. do not use anmore!
#define SNMP_ERR_NOSUCHNAME 2  use SNMP_ERR_NOSUCHINSTANCE instead
#define SNMP_ERR_BADVALUE   3  use SNMP_ERR_WRONGTYPE,SNMP_ERR_WRONGLENGTH,SNMP_ERR_WRONGENCODING or SNMP_ERR_WRONGVALUE instead
#define SNMP_ERR_READONLY   4  use SNMP_ERR_NOTWRITABLE instead
*/
  SNMP_ERR_GENERROR            = 5,
  SNMP_ERR_NOACCESS            = 6,
  SNMP_ERR_WRONGTYPE           = 7,
  SNMP_ERR_WRONGLENGTH         = 8,
  SNMP_ERR_WRONGENCODING       = 9,
  SNMP_ERR_WRONGVALUE          = 10,
  SNMP_ERR_NOCREATION          = 11,
  SNMP_ERR_INCONSISTENTVALUE   = 12,
  SNMP_ERR_RESOURCEUNAVAILABLE = 13,
  SNMP_ERR_COMMITFAILED        = 14,
  SNMP_ERR_UNDOFAILED          = 15,
  SNMP_ERR_NOTWRITABLE         = 17,
  SNMP_ERR_INCONSISTENTNAME    = 18,

  SNMP_ERR_NOSUCHINSTANCE      = SNMP_VARBIND_EXCEPTION_OFFSET + SNMP_ASN1_CONTEXT_VARBIND_NO_SUCH_INSTANCE
} snmp_err_t;

/** internal object identifier representation */
/* snmp 中用来表示 OID 变量的数据结构 */
struct snmp_obj_id
{
  u8_t len;                      /* 表示 OID 变量中有效数据字长度 */
  u32_t id[SNMP_MAX_OBJ_ID_LEN]; /* 表示 OID 变量实际数据 */
};

/* snmp 中用来表示常量引用的 OID 变量的数据结构 */
struct snmp_obj_id_const_ref
{
  u8_t len;        /* 表示当前 OID 结构体的 id 字段中有效数据字节数 */
  const u32_t* id; /* 表示当前 OID 结构体变量实际数据 */
};

extern const struct snmp_obj_id_const_ref snmp_zero_dot_zero; /* administrative identifier from SNMPv2-SMI */

/** SNMP variant value, used as reference in struct snmp_node_instance and table implementation */
union snmp_variant_value
{
  void* ptr;
  const void* const_ptr;
  u32_t u32;
  s32_t s32;
#if LWIP_HAVE_INT64
  u64_t u64;
#endif
};


/**
SNMP MIB node types
 tree node is the only node the stack can process in order to walk the tree,
 all other nodes are assumed to be leaf nodes.
 This cannot be an enum because users may want to define their own node types.
*/
/* 定义了当前系统内 snmp 协议中支持的节点类型，我们可以根据需要添加自定义节点类型 */
#define SNMP_NODE_TREE         0x00  /* 表示是一个包含子节点的树形节点 */
/* predefined leaf node types */
#define SNMP_NODE_SCALAR       0x01  /* 表示是一个标量叶子节点 */
#define SNMP_NODE_SCALAR_ARRAY 0x02  /* 表示子节点都是标量节点的标量数组叶子节点 */
#define SNMP_NODE_TABLE        0x03  /* 表示是一个表格叶子节点 */
#define SNMP_NODE_THREADSYNC   0x04  /* 表示是一个线程同步代理叶子节点 */

/** node "base class" layout, the mandatory fields for a node  */
/* 表示 snmp 树形结构中的一个节点成员基本信息 */
struct snmp_node
{
  /** one out of SNMP_NODE_TREE or any leaf node type (like SNMP_NODE_SCALAR) */
  u8_t node_type;
  /** the number assigned to this node which used as part of the full OID */
  u32_t oid;
};

/** SNMP node instance access types */
/* 表示 snmp 树形结构中的叶子节点实例的数据访问权限类型 */
typedef enum {
  SNMP_NODE_INSTANCE_ACCESS_READ    = 1,
  SNMP_NODE_INSTANCE_ACCESS_WRITE   = 2,
  SNMP_NODE_INSTANCE_READ_ONLY      = SNMP_NODE_INSTANCE_ACCESS_READ,
  SNMP_NODE_INSTANCE_READ_WRITE     = (SNMP_NODE_INSTANCE_ACCESS_READ | SNMP_NODE_INSTANCE_ACCESS_WRITE),
  SNMP_NODE_INSTANCE_WRITE_ONLY     = SNMP_NODE_INSTANCE_ACCESS_WRITE,
  SNMP_NODE_INSTANCE_NOT_ACCESSIBLE = 0
} snmp_access_t;

struct snmp_node_instance;

typedef s16_t (*node_instance_get_value_method)(struct snmp_node_instance*, void*);
typedef snmp_err_t (*node_instance_set_test_method)(struct snmp_node_instance*, u16_t, void*);
typedef snmp_err_t (*node_instance_set_value_method)(struct snmp_node_instance*, u16_t, void*);
typedef void (*node_instance_release_method)(struct snmp_node_instance*);

#define SNMP_GET_VALUE_RAW_DATA 0x4000  /* do not use 0x8000 because return value of node_instance_get_value_method is signed16 and 0x8000 would be the signed bit */

/** SNMP node instance */
/* 表示 snmp 树形结构中的叶子节点实例 */
struct snmp_node_instance
{
  /** prefilled with the node, get_instance() is called on; may be changed by user to any value to pass an arbitrary node between calls to get_instance() and get_value/test_value/set_value */
  /* 在调用 get_instance 方法前可以被设置成任何类型数据结构指针用来传递节点数据结构指针 */
  const struct snmp_node* node;
  
  /** prefilled with the instance id requested; for get_instance() this is the exact oid requested; for get_next_instance() this is the relative starting point, stack expects relative oid of next node here */
  /* 在调用 get_instance 方法前可以被设置成 exact oid 用来传递 exact oid 参数 
     在调用 get_next_instance 方法前可以被设置成 starting base oid 用来传递 starting base oid 参数 */
  struct snmp_obj_id instance_oid;

  /** ASN type for this object (see snmp_asn1.h for definitions) */
  /* 表示当前 snmp 实例对象数据类型，例如 SNMP_ASN1_TYPE_INTEGER */
  u8_t asn1_type;
  
  /** one out of instance access types defined above (SNMP_NODE_INSTANCE_READ_ONLY,...) */  
  /* 表示当前 snmp 实例对象数据的数据访问权限类型，例如 SNMP_NODE_INSTANCE_READ_ONLY */
  snmp_access_t access;

  /** returns object value for the given object identifier. Return values <0 to indicate an error */
  node_instance_get_value_method get_value;
  /** tests length and/or range BEFORE setting */
  node_instance_set_test_method set_test;
  /** sets object value, only called when set_test() was successful */
  node_instance_set_value_method set_value;
  /** called in any case when the instance is not required anymore by stack (useful for freeing memory allocated in get_instance/get_next_instance methods) */
  node_instance_release_method release_instance;

  /** reference to pass arbitrary value between calls to get_instance() and get_value/test_value/set_value */
  /* 在调用 get_instance 函数后和调用 get_value/test_value/set_value 函数前用来传递需要的参数 */
  union snmp_variant_value reference;
  
  /** see reference (if reference is a pointer, the length of underlying data may be stored here or anything else) */
  /* 如果当前实例的 reference 成员表示的时一个指针参数，则当前字段表示这个指针参数中携带的有效数据字节数 */
  u32_t reference_len;
};


/** SNMP tree node */
/* 表示 snmp 树形结构中的一个树形节点，包含了当前节点以及当前节点的子节点信息 */
struct snmp_tree_node
{
  /** inherited "base class" members */
  struct snmp_node node;                   /* 表示当前节点类型和 OID 值 */
  u16_t subnode_count;                     /* 表示当前节点下包含的子节点个数 */
  const struct snmp_node* const *subnodes; /* 表示当前节点下包含的子节点数组指针 */
};

/* 根据指定的参数创建一个 snmp 树形结构中“包含”子节点的树节点 */
#define SNMP_CREATE_TREE_NODE(oid, subnodes) \
  {{ SNMP_NODE_TREE, (oid) }, \
  (u16_t)LWIP_ARRAYSIZE(subnodes), (subnodes) }

/* 根据指定的参数创建一个 snmp 树形结构中“不包含”子节点的树节点 */
#define SNMP_CREATE_EMPTY_TREE_NODE(oid) \
  {{ SNMP_NODE_TREE, (oid) }, \
  0, NULL }

/** SNMP leaf node */
/* snmp 树形结构中用来表示叶子节点的数据结构 */
struct snmp_leaf_node
{
  /** inherited "base class" members */
  struct snmp_node node;
  snmp_err_t (*get_instance)(const u32_t *root_oid, u8_t root_oid_len, struct snmp_node_instance* instance);
  snmp_err_t (*get_next_instance)(const u32_t *root_oid, u8_t root_oid_len, struct snmp_node_instance* instance);
};

/** represents a single mib with its base oid and root node */
/* 表示 snmp 树形结构中的 mib 节点 */
struct snmp_mib
{
  const u32_t *base_oid;              /* 当前 mib 节点的 base OID 值 */
  u8_t base_oid_len;                  /* 当前 mib 节点的 base OID 值的“字”长度 */
  const struct snmp_node *root_node;  /* 当前 mib 节点下的根节点指针 */
};

/* 根据指定的参数创建一个 snmp 树形结构中的 mib 层级的根节点 */
#define SNMP_MIB_CREATE(oid_list, root_node) { (oid_list), (u8_t)LWIP_ARRAYSIZE(oid_list), root_node }

/** OID range structure */
struct snmp_oid_range
{
  u32_t min;
  u32_t max;
};

/** checks if incoming OID length and values are in allowed ranges */
u8_t snmp_oid_in_range(const u32_t *oid_in, u8_t oid_len, const struct snmp_oid_range *oid_ranges, u8_t oid_ranges_len);

typedef enum {
  SNMP_NEXT_OID_STATUS_SUCCESS,     /* 表示在 snmp_next_oid_check 中指定的 snmp oid 在之前的 next oid state 表示范围之内 */
  SNMP_NEXT_OID_STATUS_NO_MATCH,
  SNMP_NEXT_OID_STATUS_BUF_TO_SMALL /* 表示指定的 snmp oid 数据长度超过了指定的 next oid state 的 next_oid_max_len 字段值 */
} snmp_next_oid_status_t;

/** state for next_oid_init / next_oid_check functions */
struct snmp_next_oid_state
{
  const u32_t* start_oid;
  u8_t start_oid_len;

  u32_t* next_oid;
  u8_t next_oid_len;
  u8_t next_oid_max_len;

  snmp_next_oid_status_t status;
  void* reference;
};

void snmp_next_oid_init(struct snmp_next_oid_state *state,
  const u32_t *start_oid, u8_t start_oid_len,
  u32_t *next_oid_buf, u8_t next_oid_max_len);
u8_t snmp_next_oid_precheck(struct snmp_next_oid_state *state, const u32_t *oid, u8_t oid_len);
u8_t snmp_next_oid_check(struct snmp_next_oid_state *state, const u32_t *oid, u8_t oid_len, void* reference);

void snmp_oid_assign(struct snmp_obj_id* target, const u32_t *oid, u8_t oid_len);
void snmp_oid_combine(struct snmp_obj_id* target, const u32_t *oid1, u8_t oid1_len, const u32_t *oid2, u8_t oid2_len);
void snmp_oid_prefix(struct snmp_obj_id* target, const u32_t *oid, u8_t oid_len);
void snmp_oid_append(struct snmp_obj_id* target, const u32_t *oid, u8_t oid_len);
u8_t snmp_oid_equal(const u32_t *oid1, u8_t oid1_len, const u32_t *oid2, u8_t oid2_len);
s8_t snmp_oid_compare(const u32_t *oid1, u8_t oid1_len, const u32_t *oid2, u8_t oid2_len);

#if LWIP_IPV4
u8_t snmp_oid_to_ip4(const u32_t *oid, ip4_addr_t *ip);
void snmp_ip4_to_oid(const ip4_addr_t *ip, u32_t *oid);
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
u8_t snmp_oid_to_ip6(const u32_t *oid, ip6_addr_t *ip);
void snmp_ip6_to_oid(const ip6_addr_t *ip, u32_t *oid);
#endif /* LWIP_IPV6 */
#if LWIP_IPV4 || LWIP_IPV6
u8_t snmp_ip_to_oid(const ip_addr_t *ip, u32_t *oid);
u8_t snmp_ip_port_to_oid(const ip_addr_t *ip, u16_t port, u32_t *oid);

u8_t snmp_oid_to_ip(const u32_t *oid, u8_t oid_len, ip_addr_t *ip);
u8_t snmp_oid_to_ip_port(const u32_t *oid, u8_t oid_len, ip_addr_t *ip, u16_t *port);
#endif /* LWIP_IPV4 || LWIP_IPV6 */

struct netif;
u8_t netif_to_num(const struct netif *netif);

snmp_err_t snmp_set_test_ok(struct snmp_node_instance* instance, u16_t value_len, void* value); /* generic function which can be used if test is always successful */

err_t snmp_decode_bits(const u8_t *buf, u32_t buf_len, u32_t *bit_value);
err_t snmp_decode_truthvalue(const s32_t *asn1_value, u8_t *bool_value);
u8_t  snmp_encode_bits(u8_t *buf, u32_t buf_len, u32_t bit_value, u8_t bit_count);
u8_t  snmp_encode_truthvalue(s32_t *asn1_value, u32_t bool_value);

/* 定义了当前 snmp 系统统计的数据类型 */
struct snmp_statistics
{
  u32_t inpkts;              /* 表示接收到的 snmp 数据包数 */
  u32_t outpkts;             /* 表示发送出的 snmp 数据包数 */
  u32_t inbadversions;       /* 表示接收到的 snmp 版本不匹配的数据包数 */
  u32_t inbadcommunitynames; /* 表示接收到的 community 名不匹配的数据包数 */
  u32_t inbadcommunityuses;  /* 表示接收到的 community 用户不匹配的数据包数 */
  u32_t inasnparseerrs;
  u32_t intoobigs;
  u32_t innosuchnames;
  u32_t inbadvalues;
  u32_t inreadonlys;
  u32_t ingenerrs;
  u32_t intotalreqvars;
  u32_t intotalsetvars;
  u32_t ingetrequests;
  u32_t ingetnexts;
  u32_t insetrequests;
  u32_t ingetresponses;
  u32_t intraps;
  u32_t outtoobigs;
  u32_t outnosuchnames;
  u32_t outbadvalues;
  u32_t outgenerrs;
  u32_t outgetrequests;
  u32_t outgetnexts;
  u32_t outsetrequests;
  u32_t outgetresponses;
  u32_t outtraps;
#if LWIP_SNMP_V3
  u32_t unsupportedseclevels;
  u32_t notintimewindows;
  u32_t unknownusernames;
  u32_t unknownengineids;
  u32_t wrongdigests;
  u32_t decryptionerrors;
#endif
};

extern struct snmp_statistics snmp_stats;

#ifdef __cplusplus
}
#endif

#endif /* LWIP_SNMP */

#endif /* LWIP_HDR_APPS_SNMP_CORE_H */
