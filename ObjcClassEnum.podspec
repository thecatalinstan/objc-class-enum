Pod::Spec.new do |s|
  s.name                    = "ObjcClassEnum"
  s.version                 = "1.0.3"
  s.license                 = "MIT"
  s.summary                 = "Nicer enums for objective-c with class properties."
  s.homepage                = "https://github.com/thecatalinstan/objc-class-enum"
  s.author                  = { "Cătălin Stan" => "catalin.stan@me.com" }

  s.source                  = { 
                                :git => "https://github.com/thecatalinstan/objc-class-enum.git",
                                :tag => s.version 
                              }
  s.module_name             = "ObjcClassEnum"
  s.requires_arc            = true  
  s.source_files            = "Framework/ObjcClassEnum.h", "include/classenum.h", "src/classenum.c"
  s.public_header_files     = "Framework/ObjcClassEnum.h", "include/classenum.h"

  s.osx.deployment_target   = "10.11"
  s.ios.deployment_target   = "9.0"
  s.tvos.deployment_target   = "9.0"
  s.watchos.deployment_target   = "2.0"
  
  s.osx.library             = "Objc"
  s.ios.library             = "Objc"
  s.tvos.library            = "Objc"
  s.watchos.library         = "Objc"
end
