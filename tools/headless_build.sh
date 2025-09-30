#!/usr/bin/env bash
set -euo pipefail
# Headless build helper for e² studio.
# Usage:
#   E2STUDIO=/path/to/e2studio bash tools/headless_build.sh
# Produces a Debug HEX for firmware/guardianRA/ and prints its path.

if [ -z "${E2STUDIO:-}" ]; then
  echo "[build] Error: set E2STUDIO to your e² studio executable path." >&2
  echo "Example: E2STUDIO=\"/home/you/renesas/ra/e2studio/eclipse/e2studio\" bash tools/headless_build.sh" >&2
  exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PROJ_DIR="$REPO_ROOT/firmware/guardianRA"
WS_DIR="$REPO_ROOT/.e2s_ws"

mkdir -p "$WS_DIR"

# Import and build Debug
"$E2STUDIO" \
  -nosplash \
  -application org.eclipse.cdt.managedbuilder.core.headlessbuild \
  -data "$WS_DIR" \
  -import "$PROJ_DIR" \
  -build guardianRA/Debug \
  -consoleLog || true

# Try to locate a HEX under the project directory
HEX_PATH=$(find "$PROJ_DIR" -type f -name "*.hex" | head -n 1 || true)
if [ -z "$HEX_PATH" ]; then
  echo "[build] Warning: No .hex found under $PROJ_DIR. Check e² studio logs for configuration issues." >&2
  exit 2
fi

echo "[build] HEX: $HEX_PATH"
