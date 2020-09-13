//
//  classenum.c
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
#include <ctype.h>

typedef struct {
    objc_property_t _Nonnull property;
    objc_AssociationPolicy policy;
    const void * _Nonnull key;
    BOOL readonly;
} enum_member_t;

static const void *enum_members_key = (void *)&enum_members_key;
static const void *enum_members_count_key = (void *)&enum_members_count_key;

static inline objc_property_t _Nonnull * _Nullable
class_copyEnumPropertyList_Meta(Class _Nullable cls, unsigned int * _Nullable outCount);

static inline enum_member_t * _Nullable
enum_getMember(Class _Nonnull cls, const char * _Nonnull name);

static inline objc_AssociationPolicy
property_getAssociationPolicy(objc_property_t _Nonnull property);

static inline enum_member_t * _Nullable
class_getEnumLayout(Class _Nonnull cls, unsigned int * _Nullable outCount);

static inline void
class_setEnumLayout(Class _Nonnull cls, enum_member_t *members, unsigned int count);

static id _Nullable
enum_get(id _Nonnull self, SEL _Nonnull _cmd);

static void
enum_set(id _Nonnull self, SEL _Nonnull _cmd, id _Nullable value);

BOOL
class_createEnum(Class cls) {
    if (!class_isMetaClass(cls) && !(cls = objc_getMetaClass(class_getName(cls)))) {
        return NO;
    }
    
    unsigned int count = 0;
    objc_property_t *properties;
    if(!(properties = class_copyEnumPropertyList_Meta(cls, &count))) {
        return NO;
    }
    
    enum_member_t *members = calloc(count, sizeof(enum_member_t));
    
    for (unsigned int i = 0; i < count; i++) {
        objc_property_t property = properties[i];
        members[i].property = property;
        members[i].policy = property_getAssociationPolicy(property);
        members[i].readonly = !!property_copyAttributeValue(property, "R");
        members[i].key = (void *)&(members[i]);
        
        const char *getter = property_getName(members[i].property);
        if(!class_addMethod(cls, sel_registerName(getter), (IMP)enum_get, "@@:")) {
            continue;
        }
        
        if (!members[i].readonly) {
            const size_t size = strlen(getter) + 5;
            char setter[size];
            snprintf(setter, size,  "set%c%s:", toupper(getter[0]), getter + 1);
            if(!class_addMethod(cls, sel_registerName(setter), (IMP)enum_set, "v@:@")) {
                continue;
            }
        }
    }
    
    class_setEnumLayout(cls, members, count);
    
    free(properties);
    return YES;
}

objc_property_t *
class_copyEnumPropertyList(Class cls, unsigned int *outCount) {
    if (!class_isMetaClass(cls) && !(cls = objc_getMetaClass(class_getName(cls)))) {
        if (outCount) {
            *outCount = 0;
        }
        return NULL;
    }
    return class_copyEnumPropertyList_Meta(cls, outCount);
}

objc_property_t *
class_copyEnumPropertyList_Meta(Class cls, unsigned int *outCount) {
    unsigned int property_count = 0, enum_count = 0;
    objc_property_t *properties = NULL, *enum_properties = NULL;
    char *cls_type = NULL;
            
    if (!(properties = class_copyPropertyList(cls, &property_count))) {
        goto done;
    }
    
    if (!(enum_properties = calloc(property_count, sizeof(objc_property_t)))) {
        goto done;
    }
            
    if(-1 == asprintf(&cls_type, "@\"%s\"", class_getName(cls))) {
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

id
class_getEnumValue(Class cls, objc_property_t property) {
    id instance;
    if (!(instance = class_createInstance(cls, 0))) {
        return NULL;
    }
    
    if (!class_isMetaClass(cls) && !(cls = objc_getMetaClass(class_getName(cls)))) {
        return NULL;
    }
    
    Method getter;
    SEL getter_sel = sel_registerName(property_getName(property));
    if (!(getter = class_getClassMethod(cls, getter_sel))) {
       return NULL;
    }

    typeof(instance)(*getter_implementation)(id, SEL);
    if (!(getter_implementation = (typeof(getter_implementation))method_getImplementation(getter))) {
        return NULL;
    }
    
    return getter_implementation((id)cls, getter_sel);
}

#pragma mark - Internals

objc_AssociationPolicy
property_getAssociationPolicy(objc_property_t property) {
    objc_AssociationPolicy policy;
    if (property_copyAttributeValue(property, "&")) {
        if (property_copyAttributeValue(property, "N")) {
            policy = OBJC_ASSOCIATION_RETAIN_NONATOMIC;
        } else {
            policy = OBJC_ASSOCIATION_RETAIN;
        }
    } else if (property_copyAttributeValue(property, "C")) {
        if (property_copyAttributeValue(property, "N")) {
            policy = OBJC_ASSOCIATION_COPY_NONATOMIC;
        } else {
            policy = OBJC_ASSOCIATION_COPY;
        }
    } else {
        policy = OBJC_ASSOCIATION_ASSIGN;
    }
    return policy;
}

enum_member_t *
enum_getMember(Class cls, const char *name) {
    unsigned int count;
    enum_member_t *members;
    if (!(members = class_getEnumLayout(cls, &count))) {
        return NULL;
    }
        
    enum_member_t *member = NULL;
    for (unsigned int i = 0; i < count; i++) {
        const char *prop_name = property_getName(members[i].property);
        if (!strcmp(prop_name, name)) {
            member = (members + i);
            break;
        }
    }
    return member;
}

enum_member_t *
class_getEnumLayout(Class cls, unsigned int *outCount) {
    if (outCount) {
        int *members_count;
        if (!(members_count = (int *)objc_getAssociatedObject((id)cls, enum_members_count_key))) {
            *outCount = 0;
        } else {
            *outCount = *members_count;
        }
    }
    return (enum_member_t *)objc_getAssociatedObject((id)cls, enum_members_key);
}

void
class_setEnumLayout(Class cls, enum_member_t *members, unsigned int count) {
    int *members_count;
    if (!(members_count = (int *)objc_getAssociatedObject((id)cls, enum_members_count_key))) {
        members_count = calloc(1, sizeof(unsigned int));
        objc_setAssociatedObject((id)cls, enum_members_count_key, (id)members_count, OBJC_ASSOCIATION_ASSIGN);
    }
    *members_count = count;
    objc_setAssociatedObject((id)cls, enum_members_key, (id)members, OBJC_ASSOCIATION_ASSIGN);
}

#pragma mark - Getter/Setter

id
enum_get(id self, SEL _cmd) {
    Class cls = (Class)self;
    if (!class_isMetaClass(cls) && !(cls = objc_getMetaClass(class_getName(cls)))) {
        return NULL;
    }
    const char *getter = sel_getName(_cmd);
    enum_member_t *member;
    if (!(member = enum_getMember(cls, getter))) {
        return NULL;
    }
    return objc_getAssociatedObject((id)cls, member->key);
}

void
enum_set(id self, SEL _cmd, id value) {
    Class cls = (Class)self;
    if (!class_isMetaClass(cls) && !(cls = objc_getMetaClass(class_getName(cls)))) {
        return;
    }
    
    const char *setter = sel_getName(_cmd);
    const size_t size = strlen(setter) - 3;
    char getter[size];
    snprintf(getter, size, "%c%s", tolower((setter + 3)[0]), setter + 4);
    enum_member_t *member = NULL;
    if (!(member = enum_getMember(cls, getter))) {
        return;
    }
        
    objc_setAssociatedObject((id)cls, member->key, value, member->policy);
}

