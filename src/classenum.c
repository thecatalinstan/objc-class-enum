//
//  enum.c
//  ObjcClassEnum
//
//  Created by Cătălin Stan on 11/09/2020.
//  Copyright © 2020 Cătălin Stan. All rights reserved.
//

#include "classenum.h"

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

#define YESNO "yes":"no"

BOOL
class_createEnum(Class cls) {
    unsigned int count = 0;
    objc_property_t *properties;
    if(!(properties = class_copyEnumPropertyList(cls, &count))) {
        return NO;
    }
    
    for (unsigned int i = 0; i < count; i++) {
        objc_property_t property = properties[i];
        printf("%s\n", property_getName(property));
    }
    
    free(properties);
    return YES;
}

objc_property_t *
class_copyEnumPropertyList(Class cls, unsigned int *outCount) {
    unsigned int property_count = 0, enum_count = 0;
    objc_property_t *properties = NULL, *enum_properties = NULL;
    char *cls_type = NULL;
    Class meta;
    
    if (!(meta = objc_getMetaClass(class_getName(cls)))) {
        goto done;
    }
            
    if (!(properties = class_copyPropertyList(meta, &property_count))) {
        goto done;
    }
    
    if (!(enum_properties = calloc(property_count, sizeof(objc_property_t)))) {
        goto done;
    }
            
    if(-1 == asprintf(&cls_type, "@\"%s\"", class_getName(meta))) {
        goto done;
    }

    for (unsigned int i = 0; i < property_count; i++) {
        objc_property_t property = properties[i];
        
        // only allow dynamic properties
        if(!property_copyAttributeValue(property, "D")) {
            continue;
        }
        
        // check the type
        const char *type;
        if(!(type = property_copyAttributeValue(property, "T")) || strcmp(type, cls_type)) {
            continue;
        }
        
        enum_properties[enum_count] = property;
        enum_count++;
        continue;
        
        printf(" - raw attributes:\n");
        unsigned int attribute_count = 0, j = 0;
        objc_property_attribute_t *attributes = property_copyAttributeList(property, &attribute_count);
        for(j = 0; j < attribute_count; j++) {
            objc_property_attribute_t attr = attributes[j];
            printf("    *** %s: %s\n", attr.name, attr.value);
        }
        free(attributes);
    }
    
done:
    
    if (cls_type) {
        free(cls_type);
        cls_type = NULL;
    }
    
    if (properties) {
        free(properties);
        properties = NULL;
    }
            
    if(enum_count && enum_properties && enum_count < property_count && !(enum_properties = realloc(enum_properties, enum_count * sizeof(objc_property_t)))) {
        enum_count = 0;
    }
    
    if (enum_count == 0) {
        if (enum_properties) {
            free(enum_properties);
        }
        enum_properties = NULL;
    }
       
    if (outCount) {
        *outCount = enum_count;
    }
    
    return enum_properties;
}
