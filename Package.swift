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
    targets: [
        .target(
            name: "ObjcClassEnum",
            path: "./",
            publicHeadersPath: "include",
        ),
    ]
)
