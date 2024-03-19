# LUV_OBC
Primary repository for code and resources pertaining to ERAU capstone project L.U.V.'s onboard computer.
This repository is intended to be used for developing software to run on L.U.V., so while abstracting the code to work on other systems is technically possible, it may be tricky.
Ideally once this project is done I might run through, clean things up, and make an "official release" version as a resource, but that'll be a future task.

## Contains:
- Source code for the software running on the OBC
- Any build / autogen / compilation scripts required to convert from source code to software on the OBC (just Makefile for now)
  - To build the code, cd into the src directory and run "make luvbeaglebone" or "make luvlaptop" for the beaglebone or laptop, respectively
- Guides and documentation (will be added in the future, doxygen maybe????? not sure yet)
