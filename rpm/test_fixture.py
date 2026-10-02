#!/usr/bin/python3
# Copyright (C) 2026 Qore Technologies, s.r.o.
# SPDX-License-Identifier: MIT
"""Reject missing or ambiguous native package artifacts."""
import importlib.util
from pathlib import Path
import tempfile
import unittest

loader = importlib.util.spec_from_file_location('fixture', Path(__file__).with_name('run-tests.py'))
fixture = importlib.util.module_from_spec(loader)
loader.loader.exec_module(fixture)


class ArtifactTests(unittest.TestCase):
    def test_missing_native_module_fails(self):
        with tempfile.TemporaryDirectory() as directory:
            with self.assertRaisesRegex(RuntimeError, 'exactly one'):
                fixture.native_module([Path(directory)])

    def test_selects_exact_native_module_among_other_modules(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            expected = root / 'xmlsec-api-2.0.qmod'
            expected.touch()
            (root / 'xml-api-2.0.qmod').touch()
            self.assertEqual(expected, fixture.native_module([root]))

    def test_duplicate_abi_artifacts_fail(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for name in ('xmlsec-api-2.0.qmod', 'xmlsec-api-1.5.qmod'):
                (root / name).touch()
            with self.assertRaisesRegex(RuntimeError, 'exactly one'):
                fixture.native_module([root])


if __name__ == '__main__':
    unittest.main()
