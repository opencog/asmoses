#! /usr/bin/env python3

__author__ = 'Cosmo Harrigan'

import sys
import unittest

for p in sys.path:
	print ('Package search path: ' + p)

from nose.tools import *
from opencog.pyasmoses import moses, MosesException, MosesCandidate


class TestMOSES(unittest.TestCase):
    def setUp(self):
        self.moses = moses()

    def tearDown(self):
        del self.moses

    def test_run_xor_python(self):
        input_data = [[0, 0, 0], [1, 1, 0], [1, 0, 1], [0, 1, 1]]
        output = self.moses.run(input=input_data, python=True)
        assert isinstance(output[0], MosesCandidate)
        print ("In run_xor_python, the program is:\n", output[0].program)
        assert output[0].score == 0
        model = output[0].eval
        assert not model([0, 0])
        assert not model([1, 1])
        assert model([0, 1])
        assert model([1, 0])

    def test_run_xor_combo(self):
        input_data = [[0, 0, 0], [1, 1, 0], [1, 0, 1], [0, 1, 1]]
        output = self.moses.run(input=input_data, args="-c 1")
        assert len(output) == 1
        assert isinstance(output[0], MosesCandidate)
        assert output[0].score == 0
        print ("In run_xor_combo, the program is:" + str(output[0].program))
        # Either one of the two below are equally likely, both are correct.
        assert output[0].program == b"and(or(!$1 !$2) or($1 $2)) "
        # assert output[0].program == b"or(and(!$1 $2) and($1 !$2)) "

    def test_run_majority_python(self):
        output = self.moses.run(args="-H maj -c 2 -z 100", python=True)
        assert isinstance(output[0], MosesCandidate)
        print ("In run_majority_python, the program is:\n", output[0].program)
        assert output[0].score == 0
        model = output[0].eval
        assert not model([0, 1, 0, 1, 0])
        assert model([1, 1, 0, 1, 0])

    @raises(MosesException)
    def test_run_raise(self):
        assert raises(MosesException, self.moses.run(args="-c"))

    @raises(MosesException)
    def test_run_manually_raise(self):
        assert raises(MosesException, self.moses.run_manually(args="-c"))

    def test_float_score(self):
        input_data = [[0, 0, 0],
              [0.2, 0.2, 0.4],
              [1, 1, 2],
              [1, 0, 1],
              [2., 1, 3]]
        output = self.moses.run(input=input_data, python=True, args='--max-time=60 --balance=1')
        self.assertTrue(any([isinstance(o.score, float) for o in output]),
                "Test only valid when there is a float score")
        model = output[0].eval
        print(model([0, 1]))
        print(model([1, 1]))

