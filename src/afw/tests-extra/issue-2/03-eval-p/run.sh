#!/bin/sh
# Finite eval-p probes. Prints wall time plus the script's in_use/rss line.
set -e
root=$(CDPATH= cd -- "$(dirname "$0")" && pwd)
workloads="$root/_workloads"
afw_bin=${AFW:-afw}

probes="for_let_int for_let_concat for_let_script compile_once_for_let while_eval for_let_compile for_let_eval for_let_eval_steps mini_bmp"

echo "eval-p probes using $afw_bin"
echo "cwd=$root"
echo "----"

for name in $probes; do
    src="$workloads/${name}.as"
    echo "=== $name ==="
    start=$(date +%s.%N)
    "$afw_bin" -s script "$src"
    end=$(date +%s.%N)
    python3 -c "print('wall_sec=%.3f' % ($end - $start))"
    echo
done
