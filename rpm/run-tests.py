#!/usr/bin/python3
# Copyright (C) 2026 Qore Technologies, s.r.o.
# SPDX-License-Identifier: MIT
"""Run XML Security tests against exactly one native module and packaged XML."""
import argparse
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile


def native_module(directories):
    modules = [file for directory in directories for file in directory.glob('xmlsec-api-*.qmod')]
    if len(modules) != 1:
        raise RuntimeError('Expected exactly one XML Security native module: ' + repr(modules))
    return modules[0]


def run(build=None, compiler=False):
    source = Path(__file__).resolve().parents[1]
    env = os.environ.copy()
    for key in ('QORE_MODULE_DIR', 'QORE_MODULE_DIR_ONLY', 'QORE_INCLUDE_DIR', 'LD_LIBRARY_PATH', 'LD_PRELOAD'):
        env.pop(key, None)
    env.update(LC_ALL='C.UTF-8', TZ='UTC')
    paths = subprocess.check_output(['/usr/bin/qore', '--module-path'], env=env, text=True).strip().split(':')
    module = native_module([build.resolve()] if build else [Path(path) for path in paths])
    env.update(QORE_MODULE_DIR=':'.join(dict.fromkeys([str(module.parent), *paths])), QORE_MODULE_DIR_ONLY='1')
    qore = ['/usr/bin/qore', '-b', '--enable-debug', '-l', 'xml', '-l', str(module)]
    # Mandatory preload rejects a missing integration dependency before the suite can skip it.
    subprocess.run([*qore, '-e', 'exit(0);'], env=env, check=True, timeout=30)
    with tempfile.TemporaryDirectory(prefix='qore-xmlsec-rpm-') as directory:
        root = Path(directory)
        suite = (source / 'test/xmlsec.qtest').read_text()
        (root / 'xmlsec.qtest').write_text(re.sub(r'^%prepend-module-path .*\n', '', suite, flags=re.M))
        for name in ('test-cert.pem', 'test-key.pem'):
            shutil.copyfile(source / 'test' / name, root / name)
        subprocess.run([*qore, str(root / 'xmlsec.qtest'), '-v'], env=env, cwd=root, check=True, timeout=180)
        if compiler:
            code = (source / 'debian/tests/compiler').read_text().split("<<'EOF'\n", 1)[1].split('\nEOF', 1)[0]
            (root / 'xmlsec-smoke.q').write_text(code + '\n')
            subprocess.run(['/usr/bin/qcc', '-o', str(root / 'xmlsec-smoke'), str(root / 'xmlsec-smoke.q')],
                           env=env, cwd=root, check=True, timeout=120)
            subprocess.run([str(root / 'xmlsec-smoke')], env=env, cwd=root, check=True, timeout=30)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument('--build-dir', type=Path)
    mode.add_argument('--installed', action='store_true')
    parser.add_argument('--compiler', action='store_true')
    args = parser.parse_args()
    run(args.build_dir, args.compiler)
