#!/bin/bash
cd /home/coder
ulimit -c 0 -n 10 -t 10
exec ./service
