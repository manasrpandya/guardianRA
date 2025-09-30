#!/usr/bin/env bash
set -euo pipefail
if [ $# -lt 1 ]; then
  echo "Usage: $0 /absolute/path/to/firmware.hex [JLINK_SPEED]" >&2
  exit 1
fi
HEX="$1"
SPEED="${2:-4000}"
if ! command -v JLinkExe >/dev/null 2>&1; then
  echo "[flash_hex] Error: JLinkExe not found on PATH" >&2
  exit 2
fi
if [ ! -f "$HEX" ]; then
  echo "[flash_hex] Error: HEX not found: $HEX" >&2
  exit 3
fi
TMP_SCRIPT="$(mktemp)"
cat > "$TMP_SCRIPT" <<EOF
device R7FA8D1BH
if SWD
speed $SPEED
r
h
loadfile $HEX
r
g
q
EOF
trap 'rm -f "$TMP_SCRIPT"' EXIT
JLinkExe -CommandFile "$TMP_SCRIPT"
