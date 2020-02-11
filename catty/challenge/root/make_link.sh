#!/bin/bash

function makeit() {
    cd /home/bsides/
    ln -s ../dist/launch_nc home/launch_nc.elf
}
makeit