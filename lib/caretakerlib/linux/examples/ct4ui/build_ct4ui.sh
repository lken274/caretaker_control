#!/bin/bash
src_dir=ct4ui
dst_dir=build-ct4ui-output
if [ -d $dst_dir ] ; then rm -rf $dst_dir; fi
cp -r $src_dir $dst_dir
cd $dst_dir
qmake -makefile *
make
