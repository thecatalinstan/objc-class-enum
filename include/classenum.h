//
//  classenum.h
//  ObjcClassEnum
//
//  Created by Cătălin Stan on 11/09/2020.
//  Copyright © 2020 Cătălin Stan. All rights reserved.
//

#ifndef classenum_h
#define classenum_h

#include <objc/runtime.h>

OBJC_EXPORT BOOL
class_createEnum(Class _Nullable cls);

OBJC_EXPORT objc_property_t _Nonnull * _Nullable
class_copyEnumPropertyList(Class _Nullable cls, unsigned int * _Nullable outCount);

OBJC_EXPORT id _Nullable
class_getEnumValue(Class _Nullable cls, objc_property_t _Nullable property);

#endif /* enum_h */
