# objc-class-enum

Nicer enums for objective-c with class properties. 

A common way of creating more expressive _enum-like_ structures in languages that don't support such constructs is to use _static_ or class properties to encapsulate the enum logic. 

```objc
@interface Color : NSObject
@property (class, readonly) Color *red;
@property (class, readonly, weak) Color *green;
@end
```

The hurdle with this approach in objective-c is that there is a lot of boilerplate code to setup the class properties and to provide additional functionality, such as iterating through the enum members.

This library aims to get rid of that boilerplaye. In order to accomplish that, we use the Objective-C runtime to analyze the declared properties and set up getter and setter implementations, as well as an _all members_ property that we can use to iterate over the enum mebers.

## Setting up a Class Enum

The `class_createEnum` is the only thig you'll need to use.

```c
BOOL class_createEnum(Class _Nullable cls);
```

```objc
+ (void)initialize {
    assert(class_createEnum(self));
}
```

`class_createEnum` will setup the properties that satisft the following conditions as members of the enum:

1. is a `class` property
2. property is `@dynamic`
3. property type an instance of the class

```objc
@interface Color : NSObject
@property (class, readonly) Color *red;
@end

@implementation Color
@dynamic red;
@end
```

## Iterating Memebers

Additionally, `class_createEnum` will setup an _all members_ property if it encounters a property that satisfies the following conditions:

1. is a `class` property
2. property is `@dynamic`
3. property type an instance of the `NSHashTable`
4. property is called one of the following: `all`, `allMembers`, `allValues`, `all[ClassName]s`

```objc
@interface Color : NSObject
@property (class, readonly) NSHashTable<Color *> *allValues;
@end

@implementation Color
@dynamic allValues;
@end
```

## Debugging

Two additional functions are offered for debugging purposes:

```c
objc_property_t  * class_copyEnumPropertyList(Class cls, unsigned int *outCount)
```

This is similar to the runtime's `class_copyPropertyList` except that it returns the properties included in the enum. It requires a prior call to `class_createEnum`. You must free the pointer returned using `free()`.


```c
id class_getEnumValue(Class cls, objc_property_t property)
```

Will get the value of the enum member of the class `cls`, described by `property`.

Here's an example of debugging using the functions described above.

```objc
unsigned int count;
objc_property_t *properties;
if((properties = class_copyEnumPropertyList(cls, &count))) {
    for (unsigned int i = 0; i < count; i++) {
        objc_property_t property = properties[i];
        NSLog(@" *** %s: %@", property_getName(property), class_getEnumValue(cls, property));
    }
    free(properties);
}
```

## Example

The code in the [_TestApp_](https://github.com/thecatalinstan/objc-class-enum/blob/master/TestApp/main.m) is a good example of how to set up such a class, and how to debug it's properties. Check it out here: [](https://github.com/thecatalinstan/objc-class-enum/blob/master/TestApp/main.m).

A minimal example that uses the `Color` class to structure more pieces of information is below

```objc
@interface Color : NSObject

@property (class, nonatomic, strong) Color *red;
@property (class, nonatomic, strong) Color *green;

// Example payload
@property NSColor *rawValue;
@property BOOL allowed;

@end

@implementation Color

@dynamic red, green;

+ (void)initialize {
    assert(class_createEnum(self));
    self.red = [Color initWithColor:NSColor.redColor allowed:NO];
    self.green = [Color initWithColor:NSColor.greenColor allowed:YES];
}

@end
```



