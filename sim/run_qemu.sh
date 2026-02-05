#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <test.s> [--gdb] [--gdb-port <port>]" >&2
  exit 1
fi

INPUT="$1"
USE_GDB=false
GDB_PORT=1234
shift

while [[ $# -gt 0 ]]; do
  case "$1" in
    --gdb)
      USE_GDB=true
      shift
      ;;
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
if [[ ! -f "$INPUT" ]]; then
  echo "Input file not found: $INPUT" >&2
  exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
mkdir -p "$BUILD_DIR"

BASE_NAME="$(basename "$INPUT")"
NAME="${BASE_NAME%.*}"
ELF_OUT="$BUILD_DIR/${NAME}.elf"

if command -v riscv64-unknown-elf-gcc >/dev/null 2>&1; then
  CC="riscv64-unknown-elf-gcc"
  TARGET_FLAGS="-march=rv64gcv -mabi=lp64d"
elif command -v riscv64-linux-gnu-gcc >/dev/null 2>&1; then
  CC="riscv64-linux-gnu-gcc"
  TARGET_FLAGS="-march=rv64gcv -mabi=lp64d"
elif command -v clang >/dev/null 2>&1; then
  CC="clang"
  TARGET_FLAGS="--target=riscv64-unknown-elf -march=rv64gcv -mabi=lp64d -fuse-ld=lld"
else
  echo "No RISC-V toolchain found. Please install riscv64-unknown-elf-gcc, riscv64-linux-gnu-gcc, or clang." >&2
  exit 1
fi

if ! command -v qemu-system-riscv64 >/dev/null 2>&1; then
  echo "qemu-system-riscv64 not found. Please install qemu-system-riscv64." >&2
  exit 1
fi

$CC $TARGET_FLAGS \
  -nostdlib -nostartfiles -ffreestanding \
  $([[ "$USE_GDB" == true ]] && echo "-g") \
  -T "$SCRIPT_DIR/riscv.ld" \
  "$SCRIPT_DIR/start.S" \
  "$INPUT" \
  -o "$ELF_OUT"

echo "$ELF_OUT" > "$BUILD_DIR/.last_elf"

QEMU_ARGS=(
  -machine virt
  -cpu rv64,v=true,vlen=128,elen=64
  -m 128M
  -nographic
  -bios none
  -kernel "$ELF_OUT"
  -semihosting
)

if [[ "$USE_GDB" == true ]]; then
  if ! command -v gdb-multiarch >/dev/null 2>&1; then
    echo "gdb-multiarch not found. Please install gdb-multiarch." >&2
    exit 1
  fi
  echo "GDB waiting on port $GDB_PORT..."
  echo "In another terminal, run: $SCRIPT_DIR/gdb_attach.sh $ELF_OUT --gdb-port $GDB_PORT"
  QEMU_ARGS+=(-S -gdb "tcp::$GDB_PORT")
fi

qemu-system-riscv64 \
  "${QEMU_ARGS[@]}"
