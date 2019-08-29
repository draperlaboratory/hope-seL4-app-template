#!/bin/bash

clone_or_pull () {
	URL=$1
	FOLDER=$2
	BRANCH=$3

	if [ ! -d "$FOLDER" ] ; then
		git clone --single-branch --branch $BRANCH $URL $FOLDER
	else
		pushd "$FOLDER"
		git pull
		popd
	fi
}

TEMPLATE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

clone_or_pull https://github.com/draperlaboratory/hope-seL4.git $TEMPLATE_DIR/kernel isp

clone_or_pull https://github.com/draperlaboratory/hope-seL4_tools $TEMPLATE_DIR/tools/seL4 isp

clone_or_pull https://github.com/draperlaboratory/hope-riscv-pk.git $TEMPLATE_DIR/tools/riscv-pk isp 

mkdir -p projects

clone_or_pull https://github.com/draperlaboratory/hope-seL4_libs.git $TEMPLATE_DIR/projects/seL4_libs isp 

clone_or_pull https://github.com/draperlaboratory/hope-musllibc.git $TEMPLATE_DIR/projects/musllibc isp 

clone_or_pull https://github.com/draperlaboratory/hope-util_libs.git $TEMPLATE_DIR/projects/util_libs isp 

clone_or_pull https://github.com/draperlaboratory/hope-sel4runtime.git $TEMPLATE_DIR/projects/sel4runtime isp

clone_or_pull https://github.com/draperlaboratory/hope-sel4_dev_scripts.git $TEMPLATE_DIR/tools/dev-scripts isp

ln -sf $TEMPLATE_DIR/tools/dev-scripts/make-riscv-build.sh $TEMPLATE_DIR/make-riscv-build.sh
ln -sf $TEMPLATE_DIR/tools/seL4/cmake-tool/init-build.sh $TEMPLATE_DIR/init-build.sh
