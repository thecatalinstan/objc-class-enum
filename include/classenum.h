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
class_createEnum(Class _Nullable cls)
    OBJC_AVAILABLE(10.5, 2.0, 9.0, 1.0, 2.0);

OBJC_EXPORT objc_property_t _Nonnull * _Nullable
class_copyEnumPropertyList(Class _Nullable cls, unsigned int * _Nullable outCount)
    OBJC_AVAILABLE(10.5, 2.0, 9.0, 1.0, 2.0);

#endif /* enum_h */
