#!/usr/bin/env bash
set -euo pipefail
# End-to-end helper: offline export -> headless build -> flash
# Usage:
#   E2STUDIO=/path/to/e2studio bash tools/build_and_flash.sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# 1) Offline export (produces scaler.h / ae_model.h)
pushd "$REPO_ROOT/offline" >/dev/null
bash run_demo.sh
popd >/dev/null

# 2) Headless build
if [ -z "${E2STUDIO:-}" ]; then
  echo "[build_and_flash] Error: set E2STUDIO to the e² studio executable path." >&2
  echo "Example: E2STUDIO=\"/home/you/renesas/ra/e2studio/eclipse/e2studio\" bash tools/build_and_flash.sh" >&2
  exit 1
fi
set +e
HEX_LINE=$("$SCRIPT_DIR/headless_build.sh" 2>/dev/null | tail -n 1)
RC=$?
set -e
if [ $RC -ne 0 ]; then
  echo "[build_and_flash] Headless build did not report a HEX. See e² studio logs above." >&2
  exit 2
fi
HEX_PATH="${HEX_LINE#*[build] HEX: }"
if [ ! -f "$HEX_PATH" ]; then
  echo "[build_and_flash] Could not locate HEX: $HEX_PATH" >&2
  exit 3
fi

# 3) Flash
bash "$SCRIPT_DIR/flash_hex.sh" "$HEX_PATH"
