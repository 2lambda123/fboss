#!/bin/bash

if [[ "$BUCK_DEFAULT_RUNTIME_RESOURCES" == "" ]]; then
    echo "Error: must be run via buck target //fboss/fsdb/if/oss:sync_model_thriftpath"
    exit 1
fi

TARGET_PATH_REL="fboss/fsdb/if/oss/fsdb_model_thriftpath.h"
# shellcheck disable=SC2001
FBSOURCE_ROOT=$(echo "$BUCK_DEFAULT_RUNTIME_RESOURCES" | sed 's:\(.*fbsource\)/.*:\1:')
FBCODE_ROOT="$FBSOURCE_ROOT/fbcode"
SRC="$BUCK_DEFAULT_RUNTIME_RESOURCES/$TARGET_PATH_REL"
DST="$FBCODE_ROOT/$TARGET_PATH_REL"

if [[ "$1" == "verify" ]]; then
    echo "Diff $SRC against $DST"
    /bin/diff "$SRC" "$DST"
    ret="$?"
    if [ "$ret" != 0 ]; then
        echo "##################################################################################"
        echo "Error: Generated thriftpath does not match oss synced file at fboss/fsdb/if/oss/fsdb_model_thriftpath.h"
        echo "Error: If you changed FSDB thrift model, trigger a resync using buck2 run //fboss/fsdb/if/oss:sync_model_thriftpath"
        echo "##################################################################################"
    fi
    exit $ret
else
    echo "Copying $SRC to $DST"
    cp "$SRC" "$DST"
fi