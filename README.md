# objc-class-enum

Nicer enums for objective-c with class properties. Or singletons if you will.

A common way of creating more expressive _enum-like_ structures in languages that don't support such constructs is to use _static_ or class properties to encapsulate the enum logic. The idea is to expose specific instances of the object encapsulating the data as singletons. The approach has some merit as it gets rid of the need to have arrays and dictionaries of associated values or to have massive `switch` statements, since all the data is encapsulated in the perticular instance. Additionally it can make the code that uses such _class enums_ more expressive and easier to read. None of this is particular to Objective-C.

```objc
@interface Color : NSObject
@property (class, readonly) Color *red;
@property (class, readonly, weak) Color *green;
@end
```

The hurdle with this approach, in Objective-C, is that there is a lot of boilerplate code to setup the class properties and to provide additional functionality, such as iterating through the enum members. This can be manageable if there are only a few singletons to setup, but there are frequent cases where the members of such enums number in the tens, and sometimes even in the hundreds. It can get cumbersome to write >50 getters/setters that all look the same and to setup arrays or dictionaries to keep track of mapping various other values to enum members.

This little library aims to get rid of that boilerplate. I wanted to be able to have _one_ function call that would take care of setting up the boilerplate. Luckily this is Objective-C, so we can use the runtime to accomplish just that. 

## Setting up a Class Enum

```objc
+ (void)initialize {
    assert(class_createEnum(self));
}
```

The `class_createEnum` function is the only thig you'll need to use. It will dothe following:
- setup getter implementations
- setup setter implementations if the property is `readwrite`
- setup an _all members_ `NSHashTable` property that we can use to iterate over the _enum mebers_.

```c
BOOL class_createEnum(Class cls);
```

`class_createEnum` will consider properties that satisfy the following conditions as members of the enum:

1. property is a `class` property
2. property is `@dynamic`
3. property type is an instance of the class

```objc
@interface Color : NSObject
@property (class) Color *red;
@end

@implementation Color
@dynamic red;
@end
```

## Iterating Memebers

```objc
@property (class, readonly) NSHashTable<Color *> *allColors;
```

Additionally, `class_createEnum` will setup an _all members_ property if it encounters a property that satisfies the following conditions:

1. property is a `class` property
2. property is `readonly`
2. property is `@dynamic`
3. property type is `NSHashTable *`
4. property is named one of the following: `all`, `allMembers`, `allValues`, `all[ClassName]s` (ex. `allColors` for the `Color` class)

```objc
@interface Color : NSObject
@property (class, readonly) NSHashTable<Color *> *allColors;
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
        NSLog(@"%s: %@", property_getName(property), class_getEnumValue(cls, property));
    }
    free(properties);
}
```

## Example

The code in the [TestApp](https://github.com/thecatalinstan/objc-class-enum/blob/master/TestApp/main.m) is a good example of how to set up such a class, and how to debug it's properties. Check it out at [https://github.com/thecatalinstan/objc-class-enum/blob/master/TestApp/main.m](https://github.com/thecatalinstan/objc-class-enum/blob/master/TestApp/main.m).

Here's a minimal example that uses the `Color` class to structure more pieces of information associated with a _color_:

```objc
@interface Color : NSObject

// Class enum (singletons)
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



