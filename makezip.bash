#!/bin/bash

read -p "filename for zip (without extension): " filename
zip "$filename".zip   *.c *.h SD_CARD/*.c SD_CARD/*.h SD_CARD/*.txt logo/* Makefile *.aps *.aws
