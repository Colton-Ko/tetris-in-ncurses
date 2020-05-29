#!/bin/bash
echo "Detecting current operating system and CPU architecture..."
if [ $(uname) == "Darwin" ] && [ $(uname -p) == "arm64" ]; then
        cp make/Makefile.ios Makefile
        chmod +x getiOSSDK.sh
        ./getiOSSDK.sh
        echo "Switched to Makefiles for jailbroken iOS."
        echo "Run make to start compiling."
else
        cp make/Makefile.other Makefile
        echo "Switched to Makefiles for non-jailbroken iOS systems."
        echo "Run make to start compiling."
fi