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

@end

int main(int argc, const char * argv[]) {
    if(0 != class_createEnum(Color.class)) {
        perror("class_createEnum: ");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
