RTest
=====

![RTest-Screenshot](./RTest-Screenshot.png)

Installation
------------

For Debian-based systems, use the provided deb from
[the latest release](https://github.com/HARPTech/RTest/releases/latest).

Other systems either need to unpack the .deb or compile their own version.

Example of Usage with Regulation Kernel
---------------------------------------

The RTest application can be used to run any RSupport-based regulation kernel
application. Simply give the start command into the text input field in the lower
right corner of the application and hit "Run Wrapped Regulation Kernel".

An example for this would be the following statement:

    python3 $HOME/Downloads/RVerify/example-rk/kernel_setup.py

Assignment Parser
-----------------

The Assignment Parser tab in the right hand side provides an automated way
to set all rover variables to the provided values. By copying the output of
[RVerify](https://github.com/HARPTech/RVerify) (when it finds a corner case)
into the provided window and pressing the button below, the situation the problem
was found in is setup in the visualisation on the left side.

Automated Test / Benchmark
--------------------------

The automated tests goes through all possible assignments of values for the steering
direction and the forward velocity in steps of 500 units. Each new assignment happens
as soon as the result for the last assignment arrived at RTest. This feature exemplifies
the huge problem domain two `int16` inputs give.

Contributing and Mode of Development
------------------------------------

This repository is hosted at [phabricator.harptech.eu](https://phabricator.harptech.eu).
A public mirror is provided
at [github.com/HARPTech/RTest](https://github.com/HARPTech/RTest). Contributions
are welcome and will be merged into the repository after they have been reviewed
in the internal system.
