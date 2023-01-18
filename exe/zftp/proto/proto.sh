#!/bin/bash
protoc --cpp_out=. zlk_message.proto
mv zlk_message.pb.cc ../src/common
mv zlk_message.pb.h ../include

