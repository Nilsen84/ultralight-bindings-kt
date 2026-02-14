#!/usr/bin/env python3
"""
Cross-platform library renaming script for Ultralight SDK.

Renames Ultralight shared libraries (UltralightCore, WebCore, Ultralight, AppCore)
to obfuscated names (Sph0-Sph3) and patches inter-library references so they
resolve the renamed copies at runtime. This allows using these bindings in an
environment where a different version of Ultralight may already be loaded.

Usage:
    python rename_libs.py --platform <win-x64|linux-x64|mac-x64|mac-arm64> \
                          --input-dir <sdk-bin-dir> \
                          --output-dir <renamed-lib-dir>
"""

import argparse
import os
import platform
import re
import subprocess
import sys
import tempfile
from pathlib import Path
from shutil import copyfile, copy


LIB_MAPPING = {
    'UltralightCore': 'Sph0',
    'WebCore': 'Sph1',
    'Ultralight': 'Sph2',
    'AppCore': 'Sph3',
}


def get_platform_lib_names(plat: str) -> dict[str, str]:
    """Get the full filename mapping for a given platform."""
    if plat == 'win-x64':
        return {f'{k}.dll': f'{v}.dll' for k, v in LIB_MAPPING.items()}
    elif plat.startswith('linux'):
        return {f'lib{k}.so': f'lib{v}.so' for k, v in LIB_MAPPING.items()}
    elif plat.startswith('mac'):
        return {f'lib{k}.dylib': f'lib{v}.dylib' for k, v in LIB_MAPPING.items()}
    else:
        raise ValueError(f'Unsupported platform: {plat}')


def rename_windows(dll_mapping, input_dir: Path, output_dir: Path):
    """Rename DLLs on Windows using dumpbin + lib to regenerate import libraries."""

    for input_name, output_name in dll_mapping.items():
        input_path = input_dir / input_name
        output_path = output_dir / output_name

        with tempfile.TemporaryDirectory() as tmp_dir:
            # Extract exports using dumpbin
            dump = subprocess.run(
                ['dumpbin', '/EXPORTS', str(input_path.resolve())],
                capture_output=True, check=True
            )
            lines = dump.stdout.splitlines(keepends=False)
            pattern = r'^\s*(\d+)\s+[A-Z0-9]+\s+[A-Z0-9]{8}\s+([^ ]+)'

            library_output = 'EXPORTS \n'
            for line in lines:
                matches = re.search(pattern, line.decode('ascii'))
                if matches is not None:
                    function_name = matches.group(2)
                    library_output += function_name + '\n'

            # Write .def file
            deffile_name = os.path.join(tmp_dir, Path(output_name).stem + '.def')
            with open(deffile_name, 'w') as f:
                f.write(library_output)

            # Generate import library
            subprocess.run(
                ['lib', '/MACHINE:X64', '/DEF:' + os.path.basename(deffile_name)],
                cwd=tmp_dir, check=True
            )

            # Copy DLL with new name
            copyfile(input_path, output_path)
            # Copy generated .lib
            copy(
                (Path(tmp_dir) / output_name).with_suffix('.lib'),
                output_dir
            )

    # Patch all DLL references within the renamed DLLs
    for output_name in dll_mapping.values():
        output_path = output_dir / output_name
        if output_path.exists():
            patch_binary(output_path, dll_mapping)


def rename_linux(so_mapping, input_dir: Path, output_dir: Path):
    """Rename shared libraries on Linux using patchelf."""

    for input_name, output_name in so_mapping.items():
        input_path = input_dir / input_name
        output_path = output_dir / output_name

        copyfile(input_path, output_path)
        os.chmod(output_path, 0o755)

        subprocess.run(
            ['patchelf', '--set-soname', output_name, str(output_path)],
            check=True
        )

        for old_name, new_name in so_mapping.items():
            subprocess.run(
                ['patchelf', '--replace-needed', old_name, new_name, str(output_path)],
                check=True
            )


    print(f'Renamed {len(so_mapping)} libraries in {output_dir}')


def rename_macos(dylib_mapping, input_dir: Path, output_dir: Path):
    """Rename shared libraries on macOS using install_name_tool."""

    for input_name, output_name in dylib_mapping.items():
        input_path = input_dir / input_name
        output_path = output_dir / output_name

        copyfile(input_path, output_path)
        os.chmod(output_path, 0o755)

        # Change the install name (ID) to just the filename
        subprocess.run(
            ['install_name_tool', '-id', f'@rpath/{output_name}', str(output_path)],
            check=True
        )

        # Replace references to old library names
        for old_name, new_name in dylib_mapping.items():
            if old_name == input_name:
                continue
            # Try both common install name patterns
            for old_ref in [f'@rpath/{old_name}', f'@loader_path/{old_name}', old_name]:
                subprocess.run(
                    ['install_name_tool', '-change', old_ref, f'@rpath/{new_name}', str(output_path)],
                    check=True
                )

    for output_name in dylib_mapping.values():
        output_path = output_dir / output_name
        if output_path.exists():
            subprocess.run(
                ['codesign', '--force', '--sign', '-', str(output_path)],
                check=True
            )

    print(f'Renamed {len(dylib_mapping)} libraries in {output_dir}')


def patch_binary(path: Path, mapping: dict[str, str]):
    """Binary search-and-replace of library name references within a file."""
    byte_map = {k.encode('utf-8').lower(): v.encode('utf-8') for k, v in mapping.items()}
    sorted_keys = sorted(byte_map.keys(), key=len, reverse=True)
    pattern = re.compile(b"|".join(re.escape(k) for k in sorted_keys), re.IGNORECASE)

    with open(path, 'rb') as file:
        data = file.read()

    def substitute(match):
        key = match.group(0).lower()
        value = byte_map.get(key)
        replacement = value.ljust(len(key), b'\x00')
        print(f"[{path.name}] {key} -> {value}")
        return replacement

    new_data = pattern.sub(substitute, data)
    with open(path, 'wb') as file:
        file.write(new_data)


def main():
    parser = argparse.ArgumentParser(description='Rename Ultralight SDK libraries')
    parser.add_argument('--platform', required=True,
                        choices=['win-x64', 'linux-x64', 'mac-x64', 'mac-arm64'],
                        help='Target platform')
    parser.add_argument('--input-dir', required=True, type=Path,
                        help='SDK bin directory containing original libraries')
    parser.add_argument('--output-dir', required=True, type=Path,
                        help='Output directory for renamed libraries')
    args = parser.parse_args()

    mappings = get_platform_lib_names(args.platform)
    args.output_dir.mkdir(parents=True, exist_ok=True)

    if args.platform == 'win-x64':
        rename_windows(mappings, args.input_dir, args.output_dir)
    elif args.platform == 'linux-x64':
        rename_linux(mappings, args.input_dir, args.output_dir)
    elif args.platform.startswith('mac'):
        rename_macos(mappings, args.input_dir, args.output_dir)
    else:
        print(f'Unsupported platform: {args.platform}', file=sys.stderr)
        sys.exit(1)


if __name__ == '__main__':
    main()
