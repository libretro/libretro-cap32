#!/bin/sh

# put here the version you want to use
VERSION=cmocka-1.1.5

echo "*** cmocka git update ($VERSION)"

# get the latest revision from repository.
git clone https://git.cryptomilk.org/projects/cmocka.git cmocka.git
if [ -d cmocka.git ]
then
    rm -rf cmocka-lib
    cd cmocka.git
    git checkout $VERSION
    mkdir ../cmocka-lib
    cp -r include src AUTHORS COPYING README.md ../cmocka-lib/
    cd ..
    rm -rf cmocka.git

    echo "cmocka: ready."
else
    echo "Did you install Git?"
fi
