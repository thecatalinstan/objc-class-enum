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
@property (class, readonly, weak) Color *green;
@property (class, readonly, nonatomic) Color *blue;
@property (class, readonly, copy) Color *yellow;
@property (class, readonly, copy, nonatomic) Color *purple;
@property (class, readonly) NSHashTable<Color *> *allValues;

@property (readonly) NSString *rawValue;

@end

@interface Color ()

@property (class, readwrite) Color *red;
@property (class, readwrite, weak) Color *green;
@property (class, readwrite, nonatomic) Color *blue;

@end

static Color *greenColor;

@implementation Color

@dynamic red, green, blue, yellow, purple, allValues;

+ (void)initialize {
    assert(class_createEnum(self));
    self.red = [Color new];
    self.green = (greenColor = [Color new]);
    self.blue = [Color new];
}

@end

int main(int argc, const char * argv[]) {
    Class cls = Color.class;
    NSMapTable<NSString *, Color *> *map = [NSMapTable weakToWeakObjectsMapTable];
    
    unsigned int count;
    objc_property_t *properties;
    if(!(properties = class_copyEnumPropertyList(cls, &count))) {
        return EXIT_FAILURE;
    }
    
    for (unsigned int i = 0; i < count; i++) {
        objc_property_t property = properties[i];
        [map setObject:class_getEnumValue(cls, property) ?: NSNull.null
                forKey:@(property_getName(property))];
    }
    
    NSLog(@"%@", map);
    NSLog(@"%@", Color.allValues);

    return EXIT_SUCCESS;
}
