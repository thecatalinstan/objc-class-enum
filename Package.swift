// swift-tools-version:5.2
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "ObjcClassEnum",
    platforms: [
        .macOS(.v10_11),
        .iOS(.v9),
        .tvOS(.v9),
        .watchOS(.v2)
    ],
    products: [
        .library(name: "ObjcClassEnum", targets: ["ObjcClassEnum"]),
    ],
    targets: [
        .target(
            name: "ObjcClassEnum",
            path: "./",
            exclude: ["./Framework"],
            publicHeadersPath: "include"
        ),
    ]
)
