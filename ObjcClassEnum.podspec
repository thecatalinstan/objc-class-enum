Pod::Spec.new do |s|
  s.name                    = "ObjcClassEnum"
  s.version                 = "1.0.0"
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

  s.osx.deployment_target   = "10.9"
  s.osx.library             = "Objc"
end
