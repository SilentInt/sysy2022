#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
GDB_PORT=1234

ELF=""
if [[ $# -gt 0 ]]; then
  case "$1" in
    *.s)
      BASE_NAME="$(basename "$1")"
      NAME="${BASE_NAME%.*}"
      ELF="$BUILD_DIR/${NAME}.elf"
      shift
      ;;
    *.elf)
      ELF="$1"
      shift
      ;;
  esac
fi

while [[ $# -gt 0 ]]; do
  case "$1" in
    --gdb-port)
      if [[ $# -lt 2 ]]; then
        echo "Missing value for --gdb-port" >&2
        exit 1
      fi
      GDB_PORT="$2"
      shift 2
      ;;
    *)
      echo "Unknown option: $1" >&2
      exit 1
      ;;
  esac
done

if [[ -z "$ELF" ]]; then
  if [[ -f "$BUILD_DIR/.last_elf" ]]; then
    ELF="$(cat "$BUILD_DIR/.last_elf")"
  else
    echo "Usage: $0 <elf|test.s> [--gdb-port <port>]" >&2
    echo "Tip: run ./sim/run_qemu.sh <test.s> --gdb first to record last ELF." >&2
    exit 1
  fi
fi

if [[ ! -f "$ELF" ]]; then
  echo "ELF file not found: $ELF" >&2
  exit 1
fi

if ! command -v gdb-multiarch >/dev/null 2>&1; then
  echo "gdb-multiarch not found. Please install gdb-multiarch." >&2
  exit 1
fi

gdb-multiarch -q "$ELF" \
  -ex "target remote :$GDB_PORT"
