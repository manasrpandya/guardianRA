#!/usr/bin/env bash
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
JLINK_SCRIPT="$REPO_ROOT/firmware/jlink/flash_hex.jlink"

if ! command -v JLinkExe >/dev/null 2>&1; then
  echo "[flash] Error: JLinkExe not found in PATH." >&2
  echo "Install SEGGER J-Link and ensure 'JLinkExe' is on PATH." >&2
  exit 1
fi

if [ ! -f "$JLINK_SCRIPT" ]; then
  echo "[flash] Error: J-Link script not found: $JLINK_SCRIPT" >&2
  exit 1
fi

echo "[flash] Using J-Link script: $JLINK_SCRIPT"
JLinkExe -CommandFile "$JLINK_SCRIPT"
