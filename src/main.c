//
//  main.c
//  ObjcClassEnum
//
//  Created by Cătălin Stan on 11/09/2020.
//  Copyright © 2020 Cătălin Stan. All rights reserved.
//

#include <stdio.h>

__attribute__((constructor))
static void initializer(void) {}

__attribute__((destructor))
static void finalizer(void) {}
 
