// swift-tools-version:5.2
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "ObjcClassEnum",
    platforms: [
        .macOS(.v10_11),
    ],
    products: [
        .library(name: "ObjcClassEnum", targets: ["ObjcClassEnum"]),
    ],
    dependencies: [],
    targets: [
        .target(
            name: "ObjcClassEnum",
            dependencies: [],
            path: "./",
            publicHeadersPath: "include",
            cSettings: []
        ),
    ]
)
