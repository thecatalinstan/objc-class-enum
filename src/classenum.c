//
//  enum.c
//  ObjcClassEnum
//
//  Created by Cătălin Stan on 11/09/2020.
//  Copyright © 2020 Cătălin Stan. All rights reserved.
//

#include "classenum.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

#define EXPORT __attribute__((visibility("default")))
#define YESNO "yes":"no"

EXPORT int class_createEnum(Class cls) {
    const char *cls_name = class_getName(cls);
    Class meta;
    if (!(meta = objc_getMetaClass(cls_name))) {
        errno = EINVAL;
        return EXIT_FAILURE;
    }
    
    char *cls_type = NULL;
    asprintf(&cls_type, "@\"%s\"", cls_name);
    
    unsigned int property_count = 0, i = 0;
    objc_property_t *properties = class_copyPropertyList(meta, &property_count);
    for (i = 0; i < property_count; i++) {
        objc_property_t property = properties[i];
        printf("%s\n", property_getName(property));

        // check dynamic
        const char *dynamic = property_copyAttributeValue(property, "D");
        printf(" - dynamic: %s\n", dynamic ? YESNO);

        // check the type
        const char *type = property_copyAttributeValue(property, "T");
        printf(" - class_type: %s\n", !strcmp(type, cls_type) ? YESNO);        
                
        printf(" - raw attributes:\n");
        unsigned int attribute_count = 0, j = 0;
        objc_property_attribute_t *attributes = property_copyAttributeList(property, &attribute_count);
        for(j = 0; j < attribute_count; j++) {
            objc_property_attribute_t attr = attributes[j];
            printf("    *** %s: %s\n", attr.name, attr.value);
        }
        free(attributes);
    }
        
    free(cls_type);
    free(properties);
        
    return EXIT_SUCCESS;
}
