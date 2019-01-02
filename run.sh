#!/bin/bash

clear
./kompilator $1 $2
./maszyna_rejestrowa/maszyna-rejestrowa-cln $2
