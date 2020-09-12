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

#if defined(__cplusplus)
#define OBJC_CLASS_ENUM_EXPORT extern "C"
#else
#define OBJC_CLASS_ENUM_EXPORT extern
#endif

OBJC_CLASS_ENUM_EXPORT int class_createEnum(Class _Nonnull cls); 

#endif /* enum_h */
