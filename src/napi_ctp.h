/*
 * napi_ctp.h
 *
 * Copyright (c) 2022, 2023 Xiongfei Shi
 *
 * Author: Xiongfei Shi <xiongfei.shi(a)icloud.com>
 * License: Apache-2.0
 *
 * https://github.com/shixiongfei/napi-ctp
 */

#ifndef __NAPI_CTP_H__
#define __NAPI_CTP_H__

#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"
#include <assert.h>
#include <node_api.h>

#if defined(_MSC_VER)
#include <malloc.h>
#define dynarray(type, name, size)                                             \
  type *name = (type *)_alloca((size) * sizeof(type))
#else
#define dynarray(type, name, size) type name[size]
#endif

typedef struct Constructors {
  napi_ref marketData;
  napi_ref trader;
} Constructors;

#define arraysize(a) ((int)(sizeof(a) / sizeof(*a)))

Constructors *getConstructors(napi_env env);

napi_status defineClass(napi_env env, const char *name,
                        napi_callback constructor, size_t propertyCount,
                        const napi_property_descriptor *properties,
                        napi_ref *result);

napi_value createInstance(napi_env env, napi_callback_info info,
                          napi_ref constructor, size_t argc);

#endif /* __NAPI_CTP_H__ */
