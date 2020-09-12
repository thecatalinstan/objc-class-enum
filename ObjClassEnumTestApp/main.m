//
//  main.m
//  ObjClassEnumTestApp
//
//  Created by Cătălin Stan on 11/09/2020.
//  Copyright © 2020 Cătălin Stan. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "classenum.h"

@interface Color : NSObject

@property (class, readonly) Color *red;
@property (class, readonly, strong) Color *green;
@property (class, readonly, nonatomic) Color *blue;
@property (class, readonly) NSHashTable<Color *> *allValues;

@property (readonly) NSString *rawValue;

@end

@interface Color ()
@property (strong, class) Color *red;
@end

@implementation Color

@dynamic red, green, blue;

+ (void)initialize {
    assert(class_createEnum(self));
}

@end

int main(int argc, const char * argv[]) {
    Class cls = Color.class;
    NSMapTable<NSString *, Color *> *map = [NSMapTable strongToWeakObjectsMapTable];
    
    unsigned int count;
    objc_property_t *properties;
    if(!(properties = class_copyEnumPropertyList(cls, &count))) {
        return EXIT_FAILURE;
    }
    
    for (unsigned int i = 0; i < count; i++) {
        objc_property_t property = properties[i];
        const char *name = property_getName(property);
        Color *value = class_getEnumValue(cls, property);
        [map setObject:value forKey:@(name)];
    }
    
    NSLog(@"%@", map);
    
    
    
        
    
    return EXIT_SUCCESS;
}
