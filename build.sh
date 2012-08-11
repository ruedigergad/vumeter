#!/bin/sh

find . -name .svn -exec rm -rf {} \;
find . -name .git -exec rm -rf {} \;
rm -rf debian
mv qtc_packaging/debian_harmattan debian
#mv debian/sb_rules debian/rules
rm -rf qtc_packaging/
fakeroot dpkg-buildpackage -sa
