#!/usr/bin/env bash
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Create venv if missing
if [ ! -d "$SCRIPT_DIR/.venv" ]; then
  python3 -m venv "$SCRIPT_DIR/.venv"
fi
source "$SCRIPT_DIR/.venv/bin/activate"
python3 -m pip install --upgrade pip >/dev/null
python3 -m pip install -r "$SCRIPT_DIR/requirements.txt" >/dev/null

# Run offline pipeline
python3 "$SCRIPT_DIR/main.py" \
  --export-dir "$SCRIPT_DIR/model_export" \
  --firmware-include "$REPO_ROOT/firmware/guardianRA/include"

echo "[µSentinel] Offline run complete. Exported headers:"
echo "  - $SCRIPT_DIR/model_export/"
echo "  - $REPO_ROOT/firmware/guardianRA/include/"
